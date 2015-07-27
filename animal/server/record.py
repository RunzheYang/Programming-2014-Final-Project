# -*- coding: utf-8 -*-
"""
Created on Fri Oct 03 11:24:31 2014

@author: juda
"""

import time
import json
import os

class record:
    '''
    record all infomation during the match.
    '''
    def __init__(self):
        try:
            os.mkdir('./record')
        except Exception, e:
            pass
        self.file=open("./record/%s.log"%self.__nowTime__(),'w')
        self.json=open("json.log",'w')
        self.jsonContent={'user':None,'result':None,'step':[],'total':None}
    
    def __nowTime__(self):
        temp=time.asctime().split()
        return temp[1]+temp[2]+'_'+''.join(temp[3].split(':'))+'_'+temp[4]
    
    def logging(self,message,mode="FILE"):
        self.file.write("%s\n"%message)
        self.file.flush()
        if mode!="FILE":
            print message

    def addJsonUser(self,usr1,usr2):
        self.jsonContent['user']=[usr1,usr2]

    def addJsonStep(self,**kwargs):
        self.jsonContent['step'].append(kwargs)

    def addJsonNumber(self,name,content):
        self.jsonContent[name]=content

    def writeFiles(self):
        self.json.write(json.dumps(self.jsonContent))
        self.json.flush()
        self.json.close()
        self.file.close()

