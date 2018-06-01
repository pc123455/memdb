# !/usr/bin/python
# -*- coding: utf-8 -*-
"""
    @Create : 2018/6/1
    @Author : xwy
"""

import socket

HOST = '10.223.224.71'
PORT = 6379

class Client:

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.buffer_size = 1024
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self):
        self.sock.connect((self.host, self.port))

    def send(self, message):
        self.sock.sendall(message)

    def receive(self):
        data = str()
        while True:
            buff = self.sock.recv(self.buffer_size)
            data += buff
            if len(buff) < self.buffer_size:
                return data


class Parser:

    @classmethod
    def encode(cls, commond):
        args = commond.split(" ")
        param_list = []
        for arg in args:
            param_list.append(u''.join(["$", str(len(arg)), "\r\n", arg, "\r\n"]))

        param_str = ''.join(param_list)
        return ''.join(["*", str(len(param_list)), "\r\n", param_str])

    @classmethod
    def decode(cls, raw):
        first_char = raw[0]
        response = None
        if first_char == "+":
            # Simple Strings
            response = raw[1:].replace("\r\n", "")
        elif first_char == "-":
            # Errors
            response = raw[1:].replace("\r\n", "")
        elif first_char == ":":
            # Integers
            response = int(raw[1:].replace("\r\n", ""))
        elif first_char == "$":
            response = raw[1:].split("\r\n")
            if not len(response):
                raise Exception("response error:{}".raw)
            else:
                if int(response[0]) == -1:
                    response = None
                else:
                    response = response[1]
        elif first_char == "*":
            response = raw[1:].split("\r\n")
            if not len(response):
                raise Exception("response error:{}".raw)
            else:
                num = int(response[0])
                if num == -1:
                    response = None
                else:
                    response_list = []
                    for i in range(num):
                        if int(response[2 * i + 1][1:]) == -1:
                            response_list.append(None)
                        else:
                            response_list.append(response[2 * i + 2])

                    response = "\r\n".join(response_list)
        else:

            pass

        return response


if __name__ == "__main__":
    # s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # s.connect((HOST, PORT))
    client = Client(HOST, PORT)
    client.connect()
    while True:
        command = raw_input()
        client.send(Parser.encode(command))
        print Parser.decode(client.receive())
