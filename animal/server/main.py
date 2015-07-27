#!/usr/bin/python2.7
# -*- coding: utf-8 -*-
"""
Created on Wed Oct 01 22:43:57 2014

@author: Juda
@email: judaplus@sjtu.edu.cn
"""

import os
import sys
import socket
import network
import record
import chess
import threading

def main():
    global running
    global log
    global server
    global board
    global steps
    try:
        server.prepare()
        now_player=1-server.first_player
        player_limit=[3,3]

        STEP_LIMIT = 5000
        while steps < STEP_LIMIT:
            # Send Message: action
            steps+=1
            log.logging('Step %d:'%steps, 'SHOWALL')
            now_player=1-now_player
            log.logging("    Send to player %d [name: %s] a signal: ACTION"%(now_player,server.AIname[now_player]))
            server.send(now_player,'action')

            # Receive Message
            message=server.receive(now_player)
            log.logging("    Receive message form player %d [name: %s]: %s"%(now_player,server.AIname[now_player],message))

            # Process Message
            feedback = False
            if not (message=="None"):
                message=board.transMessage(now_player,message)
                if message:
                    log.logging("    Transform message form [number: %d] [name: %s]: %s"%(now_player,server.AIname[now_player],message))
                    feedback=board.check(now_player,message)
                else:
                    feedback = False
                if feedback!=False:
                    log.logging("    Player %d [name %s] choose (%d,%d) move to (%d %d)"%(now_player,server.AIname[now_player],feedback[0],feedback[1],feedback[2],feedback[3]),"SHOWALL")
                    server.send(0,'%d %d %d %d %d'%(now_player,feedback[0],feedback[1],feedback[2],feedback[3]))
                    log.logging("    Send to player 0 [name: %s] infomation : (%d %d %d %d %d)"%(server.AIname[0],now_player,feedback[0],feedback[1],feedback[2],feedback[3]))
                    server.send(1,'%d %d %d %d %d'%(now_player,8-feedback[0],6-feedback[1],8-feedback[2],6-feedback[3]))
                    log.logging("    Send to player 1 [name: %s] infomation : (%d %d %d %d %d)"%(server.AIname[1],now_player,8-feedback[0],6-feedback[1],8-feedback[2],6-feedback[3]))
                    log.addJsonStep(player=now_player,valid=True,source=[feedback[0],feedback[1]],target=[feedback[2],feedback[3]])
                    if (feedback[2],feedback[3]) in ((0,3),(8,3)):
                        log.logging("    The Player %d (name %s) win the game"%(now_player,server.AIname[now_player]),'SHOWALL')
                        log.addJsonNumber('result',now_player)
                        break

            if not feedback:
                player_limit[now_player]-=1
                log.logging("    The limit of invalid action of player %d (name %s) is %d"%(now_player,server.AIname[now_player],player_limit[now_player]),'SHOWALL')
                log.addJsonStep(player=now_player,valid=False)
            if player_limit[now_player]==0:
                log.logging("    The Player %d (name %s) win the game"%(1-now_player,server.AIname[1-now_player]),'SHOWALL')
                log.addJsonNumber('result',1-now_player)
                break

        # Step Limit
        if steps == STEP_LIMIT:
            log.logging('    Draw!','SHOWALL')
            log.addJsonNumber('result',2)
        
        # Close connections
        log.logging('Closing connections:', 'SHOWALL')
        server.send(0,"game end")
        log.logging("    Player %d [name: %s] connection closed succueefully"%(0,server.AIname[0]), 'SHOWALL')
        server.send(1,"game end")
        log.logging("    Player %d [name: %s] connection closed succueefully"%(1,server.AIname[1]), 'SHOWALL')


    except Exception,e:
        log.logging(e, 'SHOWALL')
    finally:
        log.logging("Total Steps: %d"%steps,'SHOWALL')
        log.logging("End!",'SHOWALL')
        log.addJsonNumber('total',steps)
        log.writeFiles()
        running = False



# Global variables
running = True
log=record.record()
server=network.server(log)
board=chess.chess(log)
steps=0

if server.p2dv:
    # p2dv.in mode:
    while True:
        line = sys.stdin.readline()
        if not running:
            sys.stderr.write('finished\n')
            sys.stderr.flush()
            break

        if line == 'get steps\n':
            sys.stderr.write('%d\n'%steps)
        elif line == 'accept ai0\n':
            server.acceptAI(0,server.first_player)
            sys.stderr.write('%s\n'%server.AIname[0])
        elif line == 'accept ai1\n':
            server.acceptAI(1,1-server.first_player)
            sys.stderr.write('%s\n'%server.AIname[1])
        elif line == 'get port\n':
            sys.stderr.write('%d\n'%server.port)
        elif line == 'play\n':
            t = threading.Thread(target=main)
            t.start()
            sys.stderr.write('ready\n')

        sys.stderr.flush()

else:
    # User mode:
    server.acceptAI(0,server.first_player)
    server.acceptAI(1,1-server.first_player)
    main()

