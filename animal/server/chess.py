# -*- coding: utf-8 -*-
"""
Created on Fri Oct 03 13:06:01 2014

@author: juda
"""

class chess:
    def __init__(self,log):
        #initiation
        self.log=log
        self.map=[[-1 for i in range(7)] for i in range(9)]
        
        
        #elephant>lion>tiger>leopard>dog>wolf>cat>mouse form 7 to 0
        #grass:-1  river:-2  trap:-3  cave:-4
        self.map[0][0]=(5,0)
        self.map[8][6]=(5,1)
        self.map[0][6]=(6,0)
        self.map[8][0]=(6,1)
        self.map[1][1]=(1,0)
        self.map[7][5]=(1,1)
        self.map[1][5]=(3,0)
        self.map[7][1]=(3,1)
        self.map[2][0]=(7,0)
        self.map[6][6]=(7,1)
        self.map[2][2]=(2,0)
        self.map[6][4]=(2,1)
        self.map[2][4]=(4,0)
        self.map[6][2]=(4,1)
        self.map[2][6]=(0,0)
        self.map[6][0]=(0,1)
        
        self.map[0][2]=self.map[0][4]=self.map[1][3]=(-3,0)
        self.map[7][3]=self.map[8][2]=self.map[8][4]=(-3,1)
        self.map[0][3]=(-4,0)
        self.map[8][3]=(-4,1)
        
        for i in range(3,6):
            for j in range(7):
                if j!=0 and j!=3 and j!=6:
                    self.map[i][j]=-2
    
    def __isDiretion__(self,char):
        return char=='U' or char=='D' or char=='L' or char=='R'
                
    def __oneStep__(self,a,b,char):
        if char=='U':
            a+=1
        elif char=='D':
            a-=1
        elif char=='L':
            b-=1
        elif char=='R':
            b+=1
        return (a,b)
        
    def __isBoundary__(self,x,y):
        return x>=0 and x<9 and y>=0 and y<7

    def __gone__(self,animal,x,y,char):
        if animal==5 or animal==6:
            if char=='U':
                if (x,y) in ((2,1),(2,2),(2,4),(2,5)):
                    if self.map[x+1][y]==-2 and self.map[x+2][y]==-2 and self.map[x+3][y]==-2:
                        return (x+4,y)
                    else:
                        return (-1,-1)
                else:
                    return (x+1,y)
            elif char=='D':
                if (x,y) in ((6,1),(6,2),(6,4),(6,5)):
                    if self.map[x-1][y]==-2 and self.map[x-2][y]==-2 and self.map[x-3][y]==-2:
                        return (x-4,y)
                    else:
                        return (-1,-1)
                else:
                    return (x-1,y)
            elif char=='L':
                if x>=3 and x<=5:
                    if self.map[x][y-1]==-2 and self.map[x][y-2]==-2:
                        return (x,y-3)
                    else:
                        return (-1,-1)
                else:
                    return (x,y-1)
            elif char=='R':
                if x>=3 and x<=5:
                    if self.map[x][y+1]==-2 and self.map[x][y+2]==-2:
                        return (x,y+3)
                    else:
                        return (-1,-1)
                else:
                    return (x,y+1)
        elif animal==0:
            return self.__oneStep__(x,y,char)
        else:
            if char=='U':
                if (x,y) in ((2,1),(2,2),(2,4),(2,5)):
                    return (-1,-1)
                else:
                    return (x+1,y)
            elif char=='D':
                if (x,y) in ((6,1),(6,2),(6,4),(6,5)):
                    return (-1,-1)
                else:
                    return (x-1,y)
            elif char=='L':
                if x>=3 and x<=5:
                    return (-1,-1)
                else:
                    return (x,y-1)
            elif char=='R':
                if x>=3 and x<=5:
                    return (-1,-1)
                else:
                    return (x,y+1)
            
    def __isAnimal__(self,a,b):
        return isinstance(self.map[a][b],tuple) and self.map[a][b][0]>=0
        
    def __eat__(self,animal,x,y,a,b):
        if (a,b) in ((0,3) or (8,3)):
            if self.map[x][y][1]==self.map[a][b][1]:
                return False
        if animal==0:
            if self.__isAnimal__(a,b):
                if self.__recover__(x,y)==-2 and self.__recover__(a,b)==-1:
                    return False
                if self.__recover__(x,y)==-1 and self.__recover__(a,b)==-2:
                    return False
                if self.map[a][b][1]!=self.map[x][y][1]:
                    return self.map[a][b][0]==0 or self.map[a][b][0]==7 or self.__recover__(a,b)==(-3,self.map[x][y][1])
                else:
                    return False
            else:
                return True
        elif animal==7:
            if self.__isAnimal__(a,b):
                if self.map[a][b][1]!=self.map[x][y][1]:
                    return ((self.map[a][b][0]!=0 and self.map[a][b][0]<=animal) or self.__recover__(a,b)==(-3,self.map[x][y][1])) 
                else:
                    return False
            else:
                return True
        else:
            if self.__isAnimal__(a,b):
                if self.map[a][b][1]!=self.map[x][y][1]:
                    return self.map[a][b][0]<=animal or self.__recover__(a,b)==(-3,self.map[x][y][1])
                else:
                    return False
            else:
                return True
    
    def __recover__(self,x,y):
        if (x,y) in ((0,2),(0,4),(1,3)):
            return (-3,0)
        elif (x,y) in ((8,2),(8,4),(7,3)):
            return (-3,1)
        elif (x,y)==(0,3):
            return (-4,0)
        elif (x,y)==(8,3):
            return (-4,1)
        elif x>=3 and x<=5 and y!=0 and y!=3 and y!=6:
            return -2
        else:
            return -1
    
    def check(self,player,message):
        try:
            info=message.split()
            x=int(info[0])
            y=int(info[1])
            diretion=info[2]
            if self.__isBoundary__(x,y) and self.__isDiretion__(diretion):
                if self.__isAnimal__(x,y) and self.map[x][y][1]==player:
                    animal_type=self.map[x][y][0]
                    nextx,nexty=self.__gone__(animal_type,x,y,diretion)
                    if self.__isBoundary__(nextx,nexty):
                        if self.__eat__(animal_type,x,y,nextx,nexty):
                            self.map[nextx][nexty]=self.map[x][y]
                            self.map[x][y]=self.__recover__(x,y)
                            self.log.logging("    Correct move!")
                            return (x,y,nextx,nexty)
                        else:
                            self.log.logging("    cannot eat or move")
                            self.showmap()
                            return False
                    else:
                        self.log.logging("    uncorrect goes")
                        return False
                else:
                    self.log.logging("    try to use uncorrect role")
                    return False
            else:
                self.log.logging("    uncorrect infomation")
                return False
        except:
            self.log.logging("    unknown error")
            return False

    def transMessage(self,player,message):
        try:
            if player==0:
                return message
            else:
                info=message.split()
                info[0]=str(8-int(info[0]))
                info[1]=str(6-int(info[1]))
                if info[2]=='U':
                    info[2]='D'
                elif info[2]=='D':
                    info[2]='U'
                elif info[2]=='L':
                    info[2]='R'
                else:
                    info[2]='L'
                return ' '.join(info)
        except:
            self.log.logging('    transMessage fail. Player: ' + str(player) + ', message: ' + str(message))
            return False

