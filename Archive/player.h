//
//  player.h
//  Beast Chess for Test
//
//  Created by Runzhe Yang on 12/4/14.
//  Copyright (c) 2014 Runzhe Yang. All rights reserved.
//

#ifndef __Beast_Chess_for_Test__player__
#define __Beast_Chess_for_Test__player__

#include <vector>
#include <string>
#include <tuple>
#include <ctime>
#include <iostream>
#include "chessbase.h"

using namespace std;




/**
 *@para my_id 0 present that your are the first player, while 1 present that your are the second player.
 *@para operators all operators that 2 players had done. each element is in the format of <id, < <x,y> ,<move to x, move to y> > >
 *@para operators_tuple the same as operators
 **/

using namespace CHESSBASE;

class Player{
private:
    int my_id_p;
    const vector<pair<int, pair<pair<int ,int>, pair<int, int> > > >  *operators;
    vector<tuple<int, int, int, int, int> > operators_tuple;
    
    MAP chessboard[9][7]=
    {{{ground,tiger},{ground,nanan},{trap,nanan},{hole,nanan},{trap,nanan},{ground,nanan},{ground,lion}},           //0
        {{ground,nanan},{ground,cat},{ground,nanan},{trap,nanan},{ground,nanan},{ground,dog},{ground,nanan}},           //1
        {{ground,elephant},{ground,nanan},{ground,wolf},{ground,nanan},{ground,leopard},{ground,nanan},{ground,mouse}}, //2
        {{ground,nanan},{water,nanan},{water,nanan},{ground,nanan},{water,nanan},{water,nanan},{ground,nanan}},         //3
        {{ground,nanan},{water,nanan},{water,nanan},{ground,nanan},{water,nanan},{water,nanan},{ground,nanan}},         //4
        {{ground,nanan},{water,nanan},{water,nanan},{ground,nanan},{water,nanan},{water,nanan},{ground,nanan}},         //5
        {{ground,mouse},{ground,nanan},{ground,leopard},{ground,nanan},{ground,wolf},{ground,nanan},{ground,elephant}}, //6
        {{ground,nanan},{ground,dog},{ground,nanan},{trap,nanan},{ground,nanan},{ground,cat},{ground,nanan}},           //7
        {{ground,lion},{ground,nanan},{trap,nanan},{hole,nanan},{trap,nanan},{ground,nanan},{ground,tiger}}};           //8
    //  0   1   2   3   4   5   6
    
    MY_ANIMAL mchess[8];
    ENEMY echess[8];
    int lotsize = 0;
    bool adjust = false;
    int myactionn = 0;
    int prevent = 0;
    int now_ch = 8;
    int repeat_ch = 8;
    bool start_chain = true;
    bool can_change = true;
    
public:
    Player(int idd, vector<pair<int, pair<pair<int ,int>, pair<int, int> > > >  *_operators){
        
        std::cout << "already" << std::endl;
        if (idd == 0)
            my_id_p = 0;
        else if(idd == 1)
            my_id_p = 1;
        else my_id_p = 1;
        operators = _operators;
        
        // I think...it's a strange problem...
        CHESSBASE::Initialize(chessboard, mchess, echess);
        
        for (int i = 0; i < 8; ++i){
            std::cout << i << " on " << "(" << mchess[i].x << ", " << mchess[i].y << ")" << std::endl;}
        std::cout << std::endl;
        for (int i = 0; i < 8; ++i){
            std::cout << "e " << i << " on " << "(" << echess[i].x << ", " << echess[i].y << ")" << std::endl;}
    }
    ~Player(){}
    
    /**
     *@return your AI name.
     **/
    string name() const;
    
    /**
     *make a decision which animal goes.
     *@return the Animal which is localed at pair<int, int> would goes. The char is one of U(up),D(down),L(left),R(right).
     **/
    pair<pair<int, int>, char> makeDecision();
    
    char DirChar(int dir){
        switch (dir) {
            case 0:
                return 'U';
                break;
            case 1:
                return 'L';
                break;
            case 2:
                return 'D';
                break;
            case 3:
                return 'R';
                break;
                
            default:
                return '?';
                break;
        }
    }
    
    void addOperators(int, int, int, int, int);
};

#endif
