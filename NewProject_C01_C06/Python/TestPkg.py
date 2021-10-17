# -*- coding: utf-8 -*-
# @Time : 2021/10/11 14:55
# @FileName: TestPkg.py
# @Function: 打包测试文件

__author__ = 'Zhigen.li'

import time
import argparse
import os
import sys
import tarfile
import shutil
import zipfile
import py7zr
from datetime import datetime


def creatPakage(path: str, isRemove=False, mode=2):
    '''
    打包测试文件夹
    :param path: 文件夹路径
    :param isRemove: 打包后是否删除源文件夹
    :param mode: 保存为zip或tar.gz或7z(0,1,2)
    :return:
    '''
    if not os.path.exists(path):
        return 'Sorry, Path {} does not exist.'.format(path)

    if os.path.isfile(path):
        return'Sorry, This function can only compress folders.'

    _t = datetime.now().strftime('_%Y%m%d%H%M%S')
    os.rename(path, path + _t)
    path += _t

    if mode == 0:
        p_pakage = path + '.zip'
        if os.path.exists(p_pakage):
            os.remove(p_pakage)
        zf = zipfile.ZipFile(p_pakage, 'w')
        recursion(zf, path, mode=mode)
        zf.close()
    elif mode == 1:
        p_pakage = path + '.tar.gz'
        if os.path.exists(p_pakage):
            os.remove(p_pakage)
        tar = tarfile.open(p_pakage, "w:gz")
        tar.add(path)
        tar.close()
    elif mode == 2:
        p_pakage = path + '.7z'
        if os.path.exists(p_pakage):
            os.remove(p_pakage)
        archive = py7zr.SevenZipFile(p_pakage, 'w')
        recursion(archive, path, mode=mode)
        archive.close()
    else:
        raise Exception('Mode code error.')

    if isRemove:
        shutil.rmtree(path)


def recursion(fp, folderPath, currParh=None, mode=2):
    if currParh is None:
        currParh = folderPath
    files = os.listdir(currParh)
    for file in files:
        filePath = os.path.join(currParh, file)
        if os.path.isfile(filePath):
            if mode == 0:
                fp.write(filePath, arcname=os.path.join(filePath.replace(folderPath, '')), compress_type=zipfile.ZIP_DEFLATED)
            elif mode == 2:
                fp.write(filePath, arcname=os.path.join(filePath.replace(folderPath, '')))
        elif os.path.isdir(filePath):
            recursion(fp, folderPath, filePath, mode)


def deleteFiles(folderPath):
    '''
    监控文件夹，删除创建时间至今大于某个天数的文件
    :param path: 需要检测的文件夹
    :return:
    '''

    dateInterval = 9999 #最大时间，需要更改这个时间
    # dateInterval = 30 # 大于30天的文件全部删除

    if dateInterval >= 366:
        return

    if not os.path.exists(folderPath):
        return

    if os.path.isdir(folderPath):
        getFileCreateTime(folderPath, dateInterval)


def getFileCreateTime(folderPath, dateInterval):
    files = os.listdir(folderPath)
    if len(files) < 1:
        return

    nowTime = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    timeArray = time.strptime(nowTime, "%Y-%m-%d %H:%M:%S")
    _ct = time.mktime(timeArray)

    for file in files:
        filePath = os.path.join(folderPath, file)
        if os.path.isfile(filePath):
            _mt = os.path.getctime(filePath)
            if _ct - _mt > dateInterval * 24 * 60 * 60:
                os.remove(filePath)
                if len(os.listdir(folderPath)) == 0:
                    os.removedirs(folderPath)
        else:
            getFileCreateTime(filePath, dateInterval)

if __name__ == '__main__':

    parser = argparse.ArgumentParser(prog="package")
    parser.add_argument('-z', '--zipPath', help="需要压缩的文件夹路径", required=True, type=str)
    parser.add_argument('-r', '--remove', help="压缩后是否删除原文件夹", type=bool, default=True, choices=[True, False])
    parser.add_argument('-m', '--mode', help="压缩文件的格式", type=int, default=0, choices=[0, 1, 2])
    parser.add_argument('-d', '--deletePath', help="需要监控删除的文件夹", default='', type=str)
    args = parser.parse_args()

    print(args)

    creatPakage(args.zipPath, args.remove, args.mode)
    deleteFiles(args.deletePath)
    sys.exit(0)