//
//  player.cpp
//  Beast Chess for Test
//
//  Created by Runzhe Yang on 12/4/14.
//  Copyright (c) 2014 Runzhe Yang. All rights reserved.
//

#include "player.h"
#include "chessbase.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <cstdlib>
#include <ctime>

string Player::name() const {
    return "和大家相处的这学期很愉快";
}

pair<pair<int, int>, char> Player::makeDecision() {
    
    CHESSBASE::animal_type chesscode = mouse;
    
    int local_max[33] = {0}, chess_to_move[33] = {0}, dir[33] = {0}, tempn = 1;
    
    can_change = true;
    
    for (int i = 0; i < 8; ++i){
        if (mchess[i].isAlive)
            for (int j = 0; j < 4; ++j){
                mchess[i].value_of_step[j] = CHESSBASE::Try_Move(CHESSBASE::animal_type(i + 1), CHESSBASE::step_type(j + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change);
                
                if (prevent > 21 && can_change)
                {
                    if (repeat_ch == i)
                    {
                        std::cout << "Yes" << std::endl << std::endl;
                        int kk = 0;
                        for (int k = 0; k < 4; ++k)
                        {
                            int maxt = 0;
                            int temp = CHESSBASE::Try_Move(CHESSBASE::animal_type(i + 1), CHESSBASE::step_type(k + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change);
                            if (temp > maxt) kk = k;
                        }
                     mchess[i].value_of_step[kk] -= prevent - 21; //4*
                    }
                }
                
                std::cout << i << " is alive, trying " << j << " result is " << mchess[i].value_of_step[j] <<std::endl;
                
                if (local_max[0] < mchess[i].value_of_step[j]){
                    tempn = 1;
                    local_max[0] = mchess[i].value_of_step[j];
                    chess_to_move[0] = i;
                    dir[0] = j;
                }else if (local_max[0] == mchess[i].value_of_step[j])
                {
                    local_max[tempn] = mchess[i].value_of_step[j];
                    chess_to_move[tempn] = i;
                    dir[tempn] = j;
                    ++tempn;
                }
            }
    }
    
    bool youqu = true;
    if (myactionn == 0 && CHESSBASE::Try_Move(CHESSBASE::animal_type(1 + 1), CHESSBASE::step_type(2 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(1 + 1), CHESSBASE::step_type(2 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
            {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 1;
            dir[0] = 2;
            tempn = 0;
        }
    }
    else if (myactionn == 1 && CHESSBASE::Try_Move(CHESSBASE::animal_type(2 + 1), CHESSBASE::step_type(3 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(2 + 1), CHESSBASE::step_type(3 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 2;
            dir[0] = 3;
            tempn = 0;
        }
    }
    else if (myactionn == 2 && CHESSBASE::Try_Move(CHESSBASE::animal_type(3 + 1), CHESSBASE::step_type(2 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(3 + 1), CHESSBASE::step_type(2 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 3;
            dir[0] = 2;
            tempn = 0;
        }
    }
    else if (myactionn == 3 && CHESSBASE::Try_Move(CHESSBASE::animal_type(7 + 1), CHESSBASE::step_type(3 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(7 + 1), CHESSBASE::step_type(3 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 7;
            dir[0] = 3;
            tempn = 0;
        }
    }
    else if (myactionn == 4 && CHESSBASE::Try_Move(CHESSBASE::animal_type(7 + 1), CHESSBASE::step_type(3 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(7 + 1), CHESSBASE::step_type(3 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 7;
            dir[0] = 3;
            tempn = 0;
        }
    }
    else if (myactionn == 5 && CHESSBASE::Try_Move(CHESSBASE::animal_type(7 + 1), CHESSBASE::step_type(2 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(7 + 1), CHESSBASE::step_type(2 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 7;
            dir[0] = 2;
            tempn = 0;
        }
    }
    else if (myactionn == 6 && CHESSBASE::Try_Move(CHESSBASE::animal_type(7 + 1), CHESSBASE::step_type(3 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(7 + 1), CHESSBASE::step_type(3 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 7;
            dir[0] = 3;
            tempn = 0;
        }
    }
    else if (myactionn == 7 && CHESSBASE::Try_Move(CHESSBASE::animal_type(4 + 1), CHESSBASE::step_type(3 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(4 + 1), CHESSBASE::step_type(3 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 4;
            dir[0] = 2;
            tempn = 0;
        }
    }
    else if (myactionn == 8 && CHESSBASE::Try_Move(CHESSBASE::animal_type(6 + 1), CHESSBASE::step_type(0 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(6 + 1), CHESSBASE::step_type(0 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 6;
            dir[0] = 0;
            tempn = 0;
        }
    }
    else if (myactionn == 9 && CHESSBASE::Try_Move(CHESSBASE::animal_type(5 + 1), CHESSBASE::step_type(0 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(5 + 1), CHESSBASE::step_type(0 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 5;
            dir[0] = 0;
            tempn = 0;
        }
    }
    else if (myactionn == 10 && CHESSBASE::Try_Move(CHESSBASE::animal_type(0 + 1), CHESSBASE::step_type(0 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(0 + 1), CHESSBASE::step_type(0 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 0;
            dir[0] = 0;
            tempn = 0;
        }
    }
    else if (myactionn == 11 && CHESSBASE::Try_Move(CHESSBASE::animal_type(0 + 1), CHESSBASE::step_type(1 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(0 + 1), CHESSBASE::step_type(1 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 0;
            dir[0] = 1;
            tempn = 0;
        }
    }
    else if (myactionn == 12 && CHESSBASE::Try_Move(CHESSBASE::animal_type(6 + 1), CHESSBASE::step_type(0 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) != 0)
    {
        if (CHESSBASE::Try_Move(CHESSBASE::animal_type(6 + 1), CHESSBASE::step_type(0 + 1), chessboard, mchess, echess, start_chain, repeat_ch, can_change) == 1)
        {start_chain = false;}
        if (start_chain)
        {
            youqu = false;
            chess_to_move[0] = 6;
            dir[0] = 1;
            tempn = 0;
        }
    }
    
    if (youqu)
    {
        tempn = (rand() + 1) % tempn;
        start_chain = false;
    }
    
    ++myactionn;

    if (chess_to_move[tempn] == now_ch)
    {
        ++prevent;
        if (prevent > 21) repeat_ch = now_ch;
    }
    else
    {
        now_ch = chess_to_move[tempn];
        prevent = 0;
        repeat_ch = 8;
    }
    
    
    chesscode = CHESSBASE::animal_type(chess_to_move[tempn] + 1);
    CHESSBASE::LOCATION to;
    
    to = CHESSBASE::What_Is_Next(chesscode, mchess[chess_to_move[tempn]].x, mchess[chess_to_move[tempn]].y, CHESSBASE::step_type(dir[tempn] + 1), chessboard);
    
    pair<int, int> ans_local;
    ans_local = make_pair(mchess[chess_to_move[tempn]].x, mchess[chess_to_move[tempn]].y);
    pair<pair<int, int>, char> deci;
    deci = make_pair(ans_local, DirChar(dir[tempn]));
    
    CHESSBASE::Refresh(chessboard, mchess, echess, chesscode, to.x, to.y, my_id_p, my_id_p);
    
    return deci;
}

void Player::addOperators(int e1,int e2,int e3,int e4,int e5){
    operators_tuple.push_back(make_tuple(e1,e2,e3,e4,e5));
    
    int otsize = int(operators_tuple.size());
    int x_space = 0, y_space = 0, new_x = 0, new_y = 0, operation_id = 0;
    CHESSBASE::animal_type chesscode = mouse;
    
    operation_id = get<0>(operators_tuple[operators_tuple.size() - 1]);
    x_space = get<1>(operators_tuple[operators_tuple.size() - 1]);
    y_space = get<2>(operators_tuple[operators_tuple.size() - 1]);
    new_x = get<3>(operators_tuple[operators_tuple.size() - 1]);
    new_y = get<4>(operators_tuple[operators_tuple.size() - 1]);
    
    chesscode = chessboard[x_space][y_space].atype;
    
    if (myactionn && !adjust){
        if (operation_id != my_id_p && otsize != lotsize){
            my_id_p = 1;
        }
        adjust = true;
    }
    else if (!adjust){
        if (operation_id == my_id_p && otsize != lotsize){
            my_id_p = 0;
        }
        adjust = true;
    }
    
    if (operation_id != my_id_p && otsize != lotsize){
        CHESSBASE::Refresh(chessboard, mchess, echess, chesscode, new_x, new_y, operation_id, my_id_p);
    }
    if (lotsize != otsize) lotsize = otsize;
}

