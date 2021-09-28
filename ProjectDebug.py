# -*- coding: utf-8 -*-
# @Time : 2021/9/15 10:01
# @FileName: UartDebug.py
# @Company : ZheJiang Evotrue Net Technology Co., Ltd.

__author__ = 'Zhigen.li'

import os
import sys
import socket
import serial
import serial.tools.list_ports
import time
import threading
from threading import RLock
import struct
import random

from PyQt5.QtWidgets import QApplication
from PyQt5 import QtCore
from PyQt5.QtWidgets import QDialog, QGridLayout, QPushButton, QTextEdit, QFileDialog, QLineEdit

targetList = []
dataPath = os.getcwd() + os.sep + 'DemoData/BoardV.csv'
with open(dataPath, 'r') as fp:
    csvData = fp.read()

csvLines = csvData.split('\n')

for i, csvLine in enumerate(csvLines):
    if i != 500000:
        cols = csvLine.split(',')
        targetList.append(float(cols[0]))
        targetList.append(float(cols[0]))
        # targetList.append(8.23445)

# for _ in range(500000):tmpVal
#     b = random.uniform(1.1, 9.9)
#     targetList.append(b)

str1 = b'raw data[unit:V]['
str2 = b'][DONE]\r\n'
rawData = struct.pack('17s1000000f9s', str1, *targetList, str2)
rawData = rawData.replace(b'\x00\x00\x00', b'')

class UartDebug(object):
    def __init__(self, port='COM1', bps=115200, timeout=5):
        self.ser = None
        self.recvStr = ''
        self.lock = RLock()
        self.exit = False

        # port_list = list(serial.tools.list_ports.comports())
        # if len(port_list) == 0:
        #     print('无可用串口')
        # else:
        #     for i in range(0, len(port_list)):
        #         print(port_list[i])
        self.port = port
        self.connect(port, bps, timeout)

    def connect(self, port='COM1', bps=115200, timeout=5):
        self.ser = serial.Serial(port, bps, timeout=timeout)
        self.thread = threading.Thread(target=self.recvThread)
        self.thread.start()

    def send(self, cmd):
        result = self.ser.write(cmd.encode("gbk"))

    def recvThread(self):
        while not self.exit:
            self.lock.acquire()
            self.recvStr += self.ser.read_all().decode('utf-8')
            if '\r\n' in self.recvStr:
                print('Uart {} recv: {}'.format(self.port, self.recvStr))
                self.send(self.recvStr + '[DONE]')
                print('Uart {} post: {}'.format(self.port, self.recvStr + '[DONE]'))
                self.recvStr = ''

            time.sleep(0.01)
            self.lock.release()

    def recv(self, timeout=3.0):
        time_delay = 0.0
        while time_delay < timeout:
            if '\n' in self.recvStr:
                res = self.recvStr
                self.lock.acquire()
                self.recvStr = ''
                self.lock.release()
                return res
            time.sleep(0.01)
            time_delay += 0.01

        return 'timeout'

    def close(self):
        self.exit = True
        if self.thread.isAlive:
            self.thread.join(2)
        if self.ser:
            self.ser.close()

    def __del__(self):
        self.close()


class SockDebug(object):
    def __init__(self, port):
        self.exit = False
        self.port = port
        self.s = None
        self.conn = None

    def socket_service(self):
        self.exit = False
        try:
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            self.s.bind(('127.0.0.1', self.port))
            self.s.listen(10)

        except socket.error as msg:
            print(msg)
            sys.exit(1)

        print('Waiting connection...')
        while not self.exit:
            try:
                conn, addr = self.s.accept()
                self.conn = conn
                self.t = threading.Thread(target=self.deal_data, args=(conn, addr))
                self.t.start()
            except:
                return

    def deal_data(self, conn, addr):
        print('Accept new connection from {0}'.format(addr))
        try:
            while not self.exit:
                data = conn.recv(1024)
                recv = data.decode('utf-8')
                if len(recv) == 0:
                    break

                if self.port == 7600:
                    if 'set' in recv:
                        if 'set_smu_output' in recv:
                            conn.send(bytes(recv, encoding='utf-8'))
                            conn.send(bytes(' OK\r\n', encoding='utf-8'))
                            continue
                        cmd = datetime.datetime.now().strftime('%H-%M-%S.%f') + recv + ' OK\r\n'
                        conn.send(bytes(cmd, encoding='utf-8'))
                elif self.port == 6050:
                    pass
                elif self.port == 6052 or self.port == 6053 or self.port == 6054:
                    print('Unit {} recv: {}'.format(self.port, recv))
                    if 'Register' in recv:
                        conn.send(bytes('Register#1$', encoding='utf-8'))
                elif self.port == 8801 or self.port == 8802 or self.port == 8803:
                    print('Unit {} recv: {}'.format(self.port, recv))
                    if 'ADCsample' in recv:
                        recv = recv.replace('ADCsample', 'sample')
                    if 'SET_pcb' in recv:
                        conn.send(bytes(recv + '[DONE]', encoding='utf-8'))
                    elif 'GET_pcb' in recv:
                        time.sleep(1.5)
                        conn.send(rawData)

                if recv == 'exit':
                    print('{0} connection close'.format(addr))
                    conn.send(bytes('Connection closed!', encoding='utf-8'))
                    break
        except:
            pass

        conn.close()

    def send(self, cmd):
        if self.conn:
            self.conn.send(bytes(cmd, encoding='utf-8'))

    def close(self):
        self.exit = True
        if self.t.isAlive:
            self.t.join(2)
        if self.s:
            self.s.close()


class Ui_MainDialog(QDialog):
    def __init__(self):
        super(Ui_MainDialog, self).__init__()
        self.setObjectName("MainWindow")
        self.initUI()
        self.handlerSocket = SockDebug(6050)
        self.unit1Socket = SockDebug(6052)
        self.unit2Socket = SockDebug(6053)
        self.unit3Socket = SockDebug(6054)

        self.unit1DataSocket = SockDebug(8801)
        self.unit2DataSocket = SockDebug(8802)
        self.unit3DataSocket = SockDebug(8803)
        _thread1 = threading.Thread(target=self.unit1DataSocket.socket_service)
        _thread1.start()
        _thread2 = threading.Thread(target=self.unit2DataSocket.socket_service)
        _thread2.start()
        _thread3 = threading.Thread(target=self.unit3DataSocket.socket_service)
        _thread3.start()

        self.unit1Uart1 = UartDebug('COM2')
        self.unit1Uart2 = UartDebug('COM4')
        self.unit2Uart1 = UartDebug('COM6')
        self.unit2Uart2 = UartDebug('COM8')
        self.unit3Uart1 = UartDebug('COM10')
        self.unit3Uart2 = UartDebug('COM12')

        self.unit1SocketBtn.click()
        self.unit2SocketBtn.click()
        self.unit3SocketBtn.click()
        self.unit1SocketBtn.setEnabled(False)
        self.unit2SocketBtn.setEnabled(False)
        self.unit3SocketBtn.setEnabled(False)

    def initUI(self):
        self.resize(800, 500)

        layout = QGridLayout(self)
        # self.openHandlerBtn = QPushButton('open handler', self)
        # self.unit1StartBtn = QPushButton('unit-1 start', self)
        # self.unit2StartBtn = QPushButton('unit-2 start', self)
        # self.unit3StartBtn = QPushButton('unit-3 start', self)
        # layout.addWidget(self.openHandlerBtn, 0, 0, 1, 1)
        # layout.addWidget(self.unit1StartBtn, 0, 1, 1, 1)
        # layout.addWidget(self.unit2StartBtn, 0, 2, 1, 1)
        # layout.addWidget(self.unit3StartBtn, 0, 3, 1, 1)
        # self.openHandlerBtn.clicked.connect(self.openHandler)
        # self.unit1StartBtn.clicked.connect(self.unit1Start)
        # self.unit2StartBtn.clicked.connect(self.unit2Start)
        # self.unit3StartBtn.clicked.connect(self.unit3Start)

        self.unit1SocketBtn = QPushButton('open unit-1 Socket', self)
        self.unit2SocketBtn = QPushButton('open unit-2 Socket', self)
        self.unit3SocketBtn = QPushButton('open unit-3 Socket', self)
        layout.addWidget(self.unit1SocketBtn, 1, 0, 1, 2)
        layout.addWidget(self.unit2SocketBtn, 1, 2, 1, 2)
        layout.addWidget(self.unit3SocketBtn, 1, 4, 1, 2)
        self.unit1SocketBtn.clicked.connect(self.openUnit1Socket)
        self.unit2SocketBtn.clicked.connect(self.openUnit2Socket)
        self.unit3SocketBtn.clicked.connect(self.openUnit3Socket)

        self.unit1StatusBtn = QPushButton('unit-1 Status', self)
        self.unit1GripOpenBtn = QPushButton('unit-1 Grip Open', self)
        self.unit1GripCloseBtn = QPushButton('unit-1 Grip Close', self)
        self.unit1StartTestBtn = QPushButton('unit-1 Start Test', self)
        self.unit1ResultBtn = QPushButton('unit-1 Result', self)
        self.unit1ErrorCodeBtn = QPushButton('unit-1 ErrorCode', self)
        layout.addWidget(self.unit1StatusBtn, 2, 0, 1, 1)
        layout.addWidget(self.unit1GripOpenBtn, 2, 1, 1, 1)
        layout.addWidget(self.unit1GripCloseBtn, 2, 2, 1, 1)
        layout.addWidget(self.unit1StartTestBtn, 2, 3, 1, 1)
        layout.addWidget(self.unit1ResultBtn, 2, 4, 1, 1)
        layout.addWidget(self.unit1ErrorCodeBtn, 2, 5, 1, 1)
        self.unit1StatusBtn.clicked.connect(self.unit1Status)
        self.unit1GripOpenBtn.clicked.connect(self.unit1GripOpen)
        self.unit1GripCloseBtn.clicked.connect(self.unit1GripClose)
        self.unit1StartTestBtn.clicked.connect(self.unit1StartTest)
        self.unit1ResultBtn.clicked.connect(self.unit1Result)
        self.unit1ErrorCodeBtn.clicked.connect(self.unit1ErrorCode)

        self.unit2StatusBtn = QPushButton('unit-2 Status', self)
        self.unit2GripOpenBtn = QPushButton('unit-2 Grip Open', self)
        self.unit2GripCloseBtn = QPushButton('unit-2 Grip Close', self)
        self.unit2StartTestBtn = QPushButton('unit-2 Start Test', self)
        self.unit2ResultBtn = QPushButton('unit-2 Result', self)
        self.unit2ErrorCodeBtn = QPushButton('unit-2 ErrorCode', self)
        layout.addWidget(self.unit2StatusBtn, 3, 0, 1, 1)
        layout.addWidget(self.unit2GripOpenBtn, 3, 1, 1, 1)
        layout.addWidget(self.unit2GripCloseBtn, 3, 2, 1, 1)
        layout.addWidget(self.unit2StartTestBtn, 3, 3, 1, 1)
        layout.addWidget(self.unit2ResultBtn, 3, 4, 1, 1)
        layout.addWidget(self.unit2ErrorCodeBtn, 3, 5, 1, 1)
        self.unit2StatusBtn.clicked.connect(self.unit2Status)
        self.unit2GripOpenBtn.clicked.connect(self.unit2GripOpen)
        self.unit2GripCloseBtn.clicked.connect(self.unit2GripClose)
        self.unit2StartTestBtn.clicked.connect(self.unit2StartTest)
        self.unit2ResultBtn.clicked.connect(self.unit2Result)
        self.unit2ErrorCodeBtn.clicked.connect(self.unit2ErrorCode)

        self.unit3StatusBtn = QPushButton('unit-3 Status', self)
        self.unit3GripOpenBtn = QPushButton('unit-3 Grip Open', self)
        self.unit3GripCloseBtn = QPushButton('unit-3 Grip Close', self)
        self.unit3StartTestBtn = QPushButton('unit-3 Start Test', self)
        self.unit3ResultBtn = QPushButton('unit-3 Result', self)
        self.unit3ErrorCodeBtn = QPushButton('unit-3 ErrorCode', self)
        layout.addWidget(self.unit3StatusBtn, 4, 0, 1, 1)
        layout.addWidget(self.unit3GripOpenBtn, 4, 1, 1, 1)
        layout.addWidget(self.unit3GripCloseBtn, 4, 2, 1, 1)
        layout.addWidget(self.unit3StartTestBtn, 4, 3, 1, 1)
        layout.addWidget(self.unit3ResultBtn, 4, 4, 1, 1)
        layout.addWidget(self.unit3ErrorCodeBtn, 4, 5, 1, 1)
        self.unit3StatusBtn.clicked.connect(self.unit3Status)
        self.unit3GripOpenBtn.clicked.connect(self.unit3GripOpen)
        self.unit3GripCloseBtn.clicked.connect(self.unit3GripClose)
        self.unit3StartTestBtn.clicked.connect(self.unit3StartTest)
        self.unit3ResultBtn.clicked.connect(self.unit3Result)
        self.unit3ErrorCodeBtn.clicked.connect(self.unit3ErrorCode)

        self.setLayout(layout)

    def unit1Status(self):
        self.unit1Socket.send('Status$')

    def unit1GripOpen(self):
        self.unit1Socket.send('Grip#0$')

    def unit1GripClose(self):
        self.unit1Socket.send('Grip#1$')

    def unit1StartTest(self):
        self.unit1Socket.send('StartTest#SNASDFJY365GDTRE1#Op123#Lot001#MP#RS#P1#Config01$')

    def unit1Result(self):
        self.unit1Socket.send('Result$')

    def unit1ErrorCode(self):
        self.unit1Socket.send('ErrorCode$')

    def unit2Status(self):
        self.unit2Socket.send('Status$')

    def unit2GripOpen(self):
        self.unit2Socket.send('Grip#0$')

    def unit2GripClose(self):
        self.unit2Socket.send('Grip#1$')

    def unit2StartTest(self):
        self.unit2Socket.send('StartTest#SNASDFJY365GDTRE2#Op123#Lot001#OffLine#RS#P1#Config01$')

    def unit2Result(self):
        self.unit2Socket.send('Result$')

    def unit2ErrorCode(self):
        self.unit2Socket.send('ErrorCode$')

    def unit3Status(self):
        self.unit3Socket.send('Status$')

    def unit3GripOpen(self):
        self.unit3Socket.send('Grip#0$')

    def unit3GripClose(self):
        self.unit3Socket.send('Grip#1$')

    def unit3StartTest(self):
        self.unit3Socket.send('StartTest#SNASDFJY365GDTRE3#Op123#Lot001#MP#RS#P1#Config01$')

    def unit3Result(self):
        self.unit3Socket.send('Result$')

    def unit3ErrorCode(self):
        self.unit3Socket.send('ErrorCode$')

    def openHandler(self):
        self.handlerSocketThread = threading.Thread(target=self.handlerSocket.socket_service)
        self.handlerSocketThread.start()

    def openUnit1Socket(self):
        self.unit1SocketThread = threading.Thread(target=self.unit1Socket.socket_service)
        self.unit1SocketThread.start()

    def openUnit2Socket(self):
        self.unit2SocketThread = threading.Thread(target=self.unit2Socket.socket_service)
        self.unit2SocketThread.start()

    def openUnit3Socket(self):
        self.unit3SocketThread = threading.Thread(target=self.unit3Socket.socket_service)
        self.unit3SocketThread.start()

    def unit1Start(self):
        self.handlerSocket.send('#1#SNASDFJY365GDTRE1')

    def unit2Start(self):
        self.handlerSocket.send('#2#SNASDFJY365GDTRE2')

    def unit3Start(self):
        self.handlerSocket.send('#3#SNASDFJY365GDTRE3')


if __name__ == '__main__':
    a = QApplication(sys.argv)
    dia = Ui_MainDialog()
    sys.exit(dia.exec())


