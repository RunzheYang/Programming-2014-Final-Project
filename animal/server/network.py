# -*- coding: utf-8 -*-
"""
Created on Thu Oct 02 16:19:21 2014

@author: Juda
"""

import os
import sys
import socket
import record
import random
import thread
import time
import select

class server:
    '''
    network communication by using socket
    '''
    def __init__(self,log):
        self.log=log
        self.log.logging('Initialization:', 'SHOWALL')
        if len(sys.argv) >= 2 and sys.argv[1] == 'p2dv':
            self.p2dv = True
            self.log.logging('    p2dv.in mode', 'SHOWALL')
        else:
            self.p2dv = False

        if self.p2dv:
            host = 'localhost'
        else:
            host=socket.gethostbyname(socket.gethostname())

        # Find a unused port
        while True:
            try:
                if self.p2dv:
                    port = random.randint(1024,65535)
                else:
                    port = 12345
                self.spy=socket.socket()
                self.spy.bind((str(host),port))
                self.spy.listen(2)
                self.port = port
                if self.p2dv:
                    self.spy.settimeout(1)
                break
            except:
                self.log.logging('    Port %d is used. Trying another.' % (port), 'SHOWALL')
                if not self.p2dv:
                    os.sleep(0.5)
        
        self.log.logging("    Waiting to connect ...",'SHOWALL')
        self.log.logging("    The PC's host is %s, the port is %d"%(host,port),'SHOWALL')
        
        # Determine which player is first player  
        first = 0 if random.random()<0.5 else 1
        self.first_player = first

        # Wait for AIs to connect
        self.AI=[None,None]
        self.AIname=['[Unknown]','[Unknown]']


    def acceptAI(self, clientID, first):
        try:
            self.AI[clientID],addr=self.spy.accept()
            self.AI[clientID].settimeout(2)
            self.send(clientID,str(first))
            self.AIname[clientID]=self.receive(clientID, '[Unknown]')
            self.log.logging("    AI%d: %s from %s connected"%(clientID,self.AIname[clientID],addr[0]),'SHOWALL')
        except:
            if not self.p2dv:
                self.log.logging('    Accept AI%d failed.'%clientID, 'SHOWALL')
                exit(1)
            self.log.logging('    AI%d timeout'%clientID, 'SHOWALL')
            self.AI[clientID] = None

    def prepare(self):
        self.log.addJsonUser(self.AIname[0],self.AIname[1])

        # Check if AIs timeout
        if self.p2dv:
            if not self.AI[0] and not self.AI[1]:
                self.log.logging('    Both AI timeout. Tie!', 'SHOWALL')
                self.log.addJsonNumber('result', 2)
                raise Exception('    Both AI timeout. Tie!')
            elif self.AI[0] and not self.AI[1]:
                self.log.logging('    AI1 timeout. AI0 wins.', 'SHOWALL')
                self.log.addJsonNumber('result', 0)
                raise Exception('    AI1 timeout. AI0 wins.')
            elif self.AI[1] and not self.AI[0]:
                self.log.logging('    AI0 timeout. AI1 wins.', 'SHOWALL')
                self.log.addJsonNumber('result', 1)
                raise Exception('    AI0 timeout. AI1 wins.')

        self.log.logging("    %s is the first player"%self.AIname[self.first_player],'SHOWALL')
        self.log.logging("    %s is the second player"%self.AIname[1-self.first_player],'SHOWALL')

        
    def send(self,clientID,message):
        try:
            self.AI[clientID].send('%s\n'%message)
        except:
            self.log.logging('    Send message to AI%d timeout. AI%d wins.'%(clientID, 1-clientID), 'SHOWALL')
            self.log.addJsonNumber('result', 1-clientID)
            raise Exception('    Send message to AI%d timeout. AI%d wins.'%(clientID, 1-clientID))
        
    def receive(self,clientID,fbvalue=None):
        try:
            res = self.AI[clientID].recv(128)
            return res.strip()
        except:
            if not fbvalue:
                self.log.logging('    Receive message from AI%d timeout. AI%d wins.'%(clientID, 1-clientID), 'SHOWALL')
                self.log.addJsonNumber('result', 1-clientID)
                raise Exception('    Receive message from AI%d timeout. AI%d wins.'%(clientID, 1-clientID))
            else:
                return fbvalue
        
    def __del__(self):
        if self.AI[0]:
        	self.AI[0].close()
        if self.AI[1]:
        	self.AI[1].close()
        if self.spy:
        	self.spy.close()

