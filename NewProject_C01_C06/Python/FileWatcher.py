# -*- coding: utf-8 -*-
# @Time : 2021/11/5 16:22
# @FileName: Main.py
# @Company : xx xx xx

__author__ = 'Zhigen.li'
'''
监视某个文件是否被修改，记录修改内容
'''

import sys
import os
import difflib
import threading
import time
from datetime import datetime
import shutil
import zmq
import json
import argparse

class DiffFile:
    """文件比较类"""

    @classmethod
    def _read_file(cls, file):
        """
        读取文件内容，以列表形式返回
        :return:
        """
        try:
            with open(file, "rb") as fp:
                # 二进制方式读取文件内容，并转换为str类型
                lines = fp.read().decode('utf-8')
                # 按行进行分割
                text = lines.splitlines()
                # print text
                return text
        except Exception as e:
            print("ERROR: %s" % str(e))
            return str(e)

    @classmethod
    def compare_file(cls, file1, file2, out_file):
        """
        比较文件，生成html格式
        """
        file1_content = cls._read_file(file1)
        file2_content = cls._read_file(file2)
        return DiffFile.compare_text_to_file(file1_content, file2_content, out_file)

    @classmethod
    def compare_text(cls, src_text, target_text):
        """
        比较给定的2个字符串
        """
        d = difflib.Differ()
        return "".join(list(d.compare(src_text, target_text)))

    @classmethod
    def compare_text_to_file(cls, src_text, target_text, out_file):
        """
        比较给定的2个字符串，生成html格式
        """
        compare = difflib.HtmlDiff()
        compare_result = compare.make_file(src_text, target_text)
        isChanged = DiffFile.isChanged(compare_result)
        if isChanged:
            with open(out_file, 'w') as fp:
                fp.writelines(compare_result)
        return isChanged

    @classmethod
    def isChanged(cls, htmlStr):
        num = len(htmlStr) - len(htmlStr.replace('diff_add', ''))
        num += len(htmlStr) - len(htmlStr.replace('diff_chg', ''))
        num += len(htmlStr) - len(htmlStr.replace('diff_sub', ''))
        num = num / len('diff_sub')
        if num == 6:
            return False
        return True


class Watcher(object):
    def __init__(self, path):
        self.path = path
        self.zmqReq = ZmqReqProtocol()

        self.changeLogPath = os.path.dirname(__file__) + os.sep + 'ChangeLog'
        if not os.path.exists(self.changeLogPath):
            os.makedirs(self.changeLogPath)
        logCsvPath = self.changeLogPath + os.sep + 'ChangeLog.csv'
        self.csvHandel = open(logCsvPath, 'a')
        if self.csvHandel.tell() == 0:
            self.csvHandel.write('Time,ChangeLog\n')
            self.csvHandel.flush()

        while True:
            try:
                self.makeTranscript()
            except Exception as e:
                self.zmqReq.sendString([], "File Watcher Err: {}, Advised to restart the test software.".format(str(e)))
            time.sleep(20)

    def makeTranscript(self):
        '''
        制作副本文件
        :param path: 文件或文件夹地址
        :return:
        '''
        if not os.path.exists(self.path):
            return

        transcriptPath = 'C:\\Transcript'
        if not os.path.exists(transcriptPath):
            os.makedirs(transcriptPath)

        dstFileList = os.listdir(transcriptPath)
        if os.path.isfile(self.path):
            wfileNameWithSuffix = os.path.split(self.path)[-1]
            if wfileNameWithSuffix not in dstFileList:
                shutil.copy(self.path, transcriptPath + os.sep + wfileNameWithSuffix)
                self.csvHandel.write('{},Add new Watcher {}\n'.format(datetime.now().strftime('%Y-%m-%d %H:%M:%S'), self.path))
                self.csvHandel.flush()
            else:
                currT = datetime.now().strftime('%Y-%m-%d-%H-%M-%S')
                htmlPath = self.changeLogPath + os.sep + currT + '.html'
                isChanged = DiffFile.compare_file(self.path, transcriptPath + os.sep + wfileNameWithSuffix, htmlPath)
                if isChanged:
                    res = self.zmqReq.sendString([self.path], "")
                    if not res['ErrorStr'] == '':
                        isAllowChange = False
                    elif res['Result'] == True:
                        isAllowChange = True
                    elif res['Result'] == False:
                        isAllowChange = False
                    else:
                        isAllowChange = False

                    if isAllowChange:
                        features = (transcriptPath + os.sep + wfileNameWithSuffix).split('.')
                        backupName = features[0] + '_' + currT + '.' + 'backup'
                        os.rename(transcriptPath + os.sep + wfileNameWithSuffix, backupName)
                        shutil.copy(self.path, transcriptPath + os.sep + wfileNameWithSuffix)
                        self.csvHandel.write('{},It is detected that {} has been modified. Modification is allowed.\n'.format(currT, self.path))
                        self.csvHandel.flush()
                    else:
                        os.remove(htmlPath)
                        shutil.copy(transcriptPath + os.sep + wfileNameWithSuffix, self.path)
                        self.csvHandel.write(
                            '{},It is detected that {} has been modified. Modification is not allowed. Restoring.\n'.format(currT, self.path))
                        self.csvHandel.flush()
        else:
            if self.path.endswith('\\'):
                self.path = self.path[:-1]
            elif self.path.endswith('/'):
                self.path = self.path[:-1]

            if os.path.isdir(self.path):
                folderName = os.path.split(self.path)[-1]
                dstFolderPath = transcriptPath + os.sep + folderName
                if not os.path.exists(dstFolderPath):
                    os.makedirs(dstFolderPath)

                dstFileList = os.listdir(dstFolderPath)
                srcFileList = os.listdir(self.path)
                tempDic = {}
                htmlPath = ''
                hList = []
                for file in srcFileList:
                    if file not in dstFileList:
                        _src = self.path + os.sep + file
                        _dst = dstFolderPath + os.sep + file
                        shutil.copy(_src, _dst)
                        self.csvHandel.write('{},Add new Watcher file {}\n'.format(datetime.now().strftime('%Y-%m-%d %H:%M:%S'), _src))
                        self.csvHandel.flush()
                    else:
                        _src = self.path + os.sep + file
                        _dst = dstFolderPath + os.sep + file

                        currT = datetime.now().strftime('%Y-%m-%d-%H-%M-%S')
                        htmlPath = self.changeLogPath + os.sep + currT + '.html'
                        isChanged = DiffFile.compare_file(_src, _dst, htmlPath)
                        if isChanged:
                            tempDic.setdefault(_src, _dst)
                            hList.append(htmlPath)

                if tempDic:
                    res = self.zmqReq.sendString(list(tempDic.keys()), "")
                    if not res['ErrorStr'] == '':
                        isAllowChange = False
                    elif res['Result'] == True:
                        isAllowChange = True
                    elif res['Result'] == False:
                        isAllowChange = False
                    else:
                        isAllowChange = False

                    for _src, _dst in tempDic.items():
                        if isAllowChange:
                            features = _dst.split('.')
                            backupName = features[0] + '_' + currT + '.' + 'backup'
                            os.rename(_dst, backupName)
                            shutil.copy(_src, _dst)
                            self.csvHandel.write(
                                '{},It is detected that {} has been modified. Modification is allowed.\n'.format(
                                    currT, _src))
                            self.csvHandel.flush()
                        else:
                            shutil.copy(_dst, _src)
                            self.csvHandel.write(
                                '{},It is detected that {} has been modified. Modification is not allowed. Restoring.\n'.format(
                                    currT, _src))
                            self.csvHandel.flush()

                    if not isAllowChange:
                        for p in hList:
                            os.remove(p)

    def __del__(self):
        self.csvHandel.close()


class ZmqReqProtocol(object):
    def __init__(self, ip='127.0.0.1', port=4399):
        super(ZmqReqProtocol, self).__init__()
        self.ip = ip
        self.port = port

        context = zmq.Context()
        self.socket = context.socket(zmq.REQ)
        handle = "tcp://{}:{}".format(self.ip, self.port)
        # socket.setsockopt( zmq.RCVTIMEO, 4000 )   #设置超时
        self.socket.connect(handle)

    def sendString(self, changeList, errStr):
        cmd = {"ChangeList": changeList, "ErrorStr": errStr}
        self.socket.send_string(json.dumps(cmd))
        data = self.socket.recv()
        ret = str(data, encoding="utf-8")
        dic = json.loads(ret)
        return dic

    def __del__(self):
        self.socket.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog="package")
    parser.add_argument('-p', '--path', help="需要监控的文件或文件夹", required=True, type=str)
    args = parser.parse_args()

    print(args)

    watcher = Watcher(args.path)
