//
//  chessbase.cpp
//  Beast Chess for Test
//
//  Created by Runzhe Yang on 12/4/14.
//  Copyright (c) 2014 Runzhe Yang. All rights reserved.
//

#include "chessbase.h"
#include <iostream>
#include <tuple>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

namespace CHESSBASE
{
    void Initialize(MAP (*map)[7], MY_ANIMAL cc[], ENEMY dd[])
    {
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 4; ++j){
                cc[i].value_of_step[j] = 0;}
            cc[i].isAlive = true;}
        
        for (int i= 0; i < 8; ++i)
            dd[i].isAlive = true;
        
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 7; ++j)
                if (map[i][j].atype > 0){
                    cc[map[i][j].atype - 1].x = i;
                    cc[map[i][j].atype - 1].y = j;}
        
        for (int i = 6; i < 9; ++i)
            for (int j = 0; j < 7; ++j)
                if (map[i][j].atype > 0){
                    dd[map[i][j].atype - 1].x = i;
                    dd[map[i][j].atype - 1].y = j;}
    }
    
    void Refresh(MAP (*map)[7], MY_ANIMAL cc[], ENEMY dd[], const animal_type chesscode, const int to_x, const int to_y, const int who, const int my_idd)
    {
        if (who == my_idd)
        {
            map[cc[chesscode - 1].x][cc[chesscode - 1].y].atype = nanan;
            
            if (map[to_x][to_y].atype != nanan){
                if (dd[map[to_x][to_y].atype - 1].x == to_x && dd[map[to_x][to_y].atype - 1].y == to_y)
                        dd[map[to_x][to_y].atype - 1].isAlive = false;}
            map[to_x][to_y].atype = chesscode;
            
            cc[chesscode - 1].x = to_x;
            cc[chesscode - 1].y = to_y;
        }
        else
        {
            map[dd[chesscode - 1].x][dd[chesscode - 1].y].atype = nanan;
            
            if (map[to_x][to_y].atype != nanan)
            {   if (cc[map[to_x][to_y].atype - 1].x == to_x && cc[map[to_x][to_y].atype - 1].y == to_y)
                    cc[map[to_x][to_y].atype - 1].isAlive = false;
                    std::cout<<"!^@#%$&"<<std::endl;}
            
            map[to_x][to_y].atype = chesscode;
            
            dd[chesscode - 1].x = to_x;
            dd[chesscode - 1].y = to_y;
        }
        
        for (int i = 0; i < 8; ++i){
            std::cout << i << " on " << "(" << cc[i].x << ", " << cc[i].y << ") " << cc[i].isAlive << std::endl;}
        std::cout << std::endl;
        for (int i = 0; i < 8; ++i){
            std::cout << "e " << i << " on " << "(" << dd[i].x << ", " << dd[i].y << ") " << dd[i].isAlive << std::endl;}
    }
    
    
    //To give the move a value..........
    
    int Try_Move(const animal_type & chesscode, const step_type & direction, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[], bool & start_chain, const int & repeat_ch, bool & can_change)
    {
        LOCATION to;
        to = What_Is_Next(chesscode, cc[chesscode - 1].x, cc[chesscode - 1].y, direction, map);
        
        if (!Is_Step_Valid(chesscode, map, to, direction, cc))
            return 0;
        else
        {
            int big = 0;
            int half = Half_Understanding(cc, dd, repeat_ch);
            int disv = Vdis(to);
            bool to_defence = false;
            
            can_change = true;
            
            if (disv > 2000) can_change = false;
            
            if (half == 3 && !Enemy_Weak(to, map, dd))
                to_defence = true;
            
            //--------------------To Win-----------------
            if (to.x > 5 && Enemy_Weak(to, map, dd) && (to.x - cc[chesscode - 1].x > 0 || abs(to.y - 3) < abs(cc[chesscode - 1].y - 3)))
            {
                disv += 20;
                if (map[to.x][to.y].mtype == trap)
                {disv += 500; can_change = false;}
            }
            //--------------------------------------------
            
            
            //-----------------Move Big Chess First--------------
            if (chesscode >= tiger && chesscode <= lion) big = 7;
            else if (chesscode == elephant) big = 1;
            
            
            if (dd[tiger - 1].isAlive && dd[tiger - 1].x > 5 && chesscode == lion) big = 1;
            
            if (cc[chesscode - 1].x > 2 && big != 0) big = 1;
            // adjust for defence ?? Should I keep it?
            //---------------------------------------------------
            
            
            
            //----------------------Situation Move----------------------------
            if (to_defence && chesscode != mouse)
                disv = Vdis_New(to, chesscode, map, cc, dd);
            //---------------------(Back for Defence)-------------------------
            
            
            if (half == 1 && chesscode == lion)
                big += 10;
            //-----------------------------------------------------------------
            
            
            //------------------Leaopard Wait for Mouse-----------
            if (chesscode == leopard && dd[mouse - 1].isAlive) disv = 2;
            if (Advantage(cc, dd) && chesscode == leopard) {big += 20; disv = Vdis(to);}
            //-----------------------------------------------------
            
            
            //-----------------------Attack--------------------
            if (half == 4 && In_Danger_Simple(animal_type(repeat_ch + 1), map, cc, dd) == 0 && chesscode != animal_type(repeat_ch + 1) && (chesscode == lion || chesscode == tiger||chesscode == mouse||chesscode == leopard))
            {
                if (chesscode == lion && cc[chesscode - 1].x < 5) disv += 15;
                if (chesscode == tiger && cc[chesscode - 1].x < 5) disv += 15;
                if (chesscode == leopard && cc[chesscode - 1].x < 5 && (cc[tiger - 1].x > 5 || !cc[tiger - 1].isAlive) && (cc[lion - 1].x > 5 || !cc[lion - 1].isAlive)) disv += 15 + (rand() + 1) % 20;
                if (chesscode == mouse && cc[chesscode - 1].x < 5 && (cc[tiger - 1].x > 5 || !cc[tiger - 1].isAlive) && (cc[lion - 1].x > 5 || !cc[lion - 1].isAlive)) disv += 15 + (rand() + 1) % 25;
            }
            //--------------------------------------------------
            
            
            //----------Cross Jump On River && Rush forward-------------
            if (chesscode >= tiger && chesscode <= lion && to.x <= 6 && to.x > 2 && to.y != 3)
            {
                if (to.x > cc[chesscode - 1].x && !to_defence)
                    disv += 3;
                if (to.x < cc[chesscode - 1].x && to_defence)
                    disv += 3;
            }
            
            int jumpbonus = 0;
            if ((chesscode == tiger || chesscode == lion) && (direction == left || direction == right))
                if (abs(to.y - cc[chesscode - 1].y) > 1 && to.y != 3) {
                    jumpbonus = 2;
                    animal_type tea = map[cc[chesscode - 1].x - 1][cc[chesscode - 1].y].atype;
                    if (tea == lion) {
                        if (cc[tea - 1].isAlive == true && cc[tea - 1].x == 2 && cc[tea - 1].y == 3) jumpbonus += 10;}}
            
            int rush_bonus = 0;
            if ((chesscode == tiger || chesscode == lion) && (direction == up && !to_defence))
                if (cc[chesscode - 1].x == 5)
                    rush_bonus = 5;
            //-------------------------------------------------------------
            
            //---------------------Defence in Trap-------------------------
            int adj = 0;
            if (to.x < 3 && abs(cc[chesscode - 1].y - 3) < 3 && In_Danger_Simple(chesscode, map, cc, dd) != 0)
            {
                if (abs(to.x - 0) < abs(cc[chesscode - 1].x - 0))
                    adj = 4;
                if (abs(to.y - 3) < abs(cc[chesscode - 1].y - 3))
                    adj = 1;
                if (abs(to.x - 0) > abs(cc[chesscode - 1].x - 0) || abs(to.y - 3) > abs(cc[chesscode - 1].y - 3))
                    adj = -1;
            }
            else if (to.x < 3 && abs(cc[chesscode - 1].y - 3) == 3 && In_Danger_Simple(chesscode, map, cc, dd) != 0)
            {
                if (abs(to.x - 0) < abs(cc[chesscode - 1].x - 0))
                    adj = 1;
                if (abs(to.y - 3) < abs(cc[chesscode - 1].y - 3))
                    adj = 2;
                if (abs(to.x - 0) > abs(cc[chesscode - 1].x - 0))
                    adj = -2;
                if (abs(to.y - 3) > abs(cc[chesscode - 1].y - 3))
                    adj = -1;
            }
            //-------------------------------------------------------------
            
            
            //------------------All STOPs & Can't STOPs-------------------------
            if (To_Danger(to, chesscode, map, cc, dd) && Vdis(to) < 900 && !(chesscode == mouse && Yammy(to, chesscode, map, cc, dd) > 7) && Make_Chance(to, chesscode, map, cc, dd) == 0) {start_chain = false; return 1;}
            
            if (To_Danger_Specail(to, chesscode, map, cc, dd) && Vdis(to) < 900)
                return 1;
            
            if (In_Danger_Simple(chesscode, map, cc, dd) != 0 || In_Danger_Special(chesscode, map, cc, dd) != 0)
                {/*start_chain = false;*/ if (int(chesscode) - 1 == repeat_ch) can_change = false;}
            
            if (Stop_For_Defense(chesscode, map, cc, dd) && Yammy(to, chesscode, map, cc, dd) < 100 && !(chesscode == elephant && map[to.x][to.y].mtype == trap)) {start_chain = false; if (int(chesscode) - 1 == repeat_ch) can_change = false; return 1;}
            
            if (In_Assistant(chesscode, map, cc, dd) && Yammy(to, chesscode, map, cc, dd) < 100) {start_chain = false; if (int(chesscode) - 1 == repeat_ch) can_change = false; return 1;}
            
            if (In_Gentle_Help(chesscode, map, cc, dd) && To_Help_Other_Specail(to, chesscode, map, cc, dd) == 0 && !(To_Yammy(to, chesscode, map, cc, dd) > 0 && To_Help_False_And_Yammy(to, map) >  0 && map[cc[chesscode - 1].x][cc[chesscode - 1].y].mtype != trap) && Yammy(to, chesscode, map, cc, dd) < 100) {start_chain = false; if (int(chesscode) - 1 == repeat_ch) can_change = false; return 2;}
            //----------------------------------------

            
            disv = std::max(disv, Vdis_Special(to, chesscode, map, cc, dd));
            
            
            //------Special dealing to Elephant-------------
            int eleToY = 0;
            if (to_defence && chesscode == elephant)
            {
                eleToY = -To_Yammy(to, chesscode, map, cc, dd);
                disv = Vdis_Special(to, chesscode, map, cc, dd);
                if (cc[chesscode - 1].x == 1 && cc[chesscode - 1].y == 3)
                    disv = 0;
            }
            if (chesscode == elephant && cc[chesscode - 1].x == 1 && cc[chesscode - 1].y == 3)
                disv = 0;
            if (chesscode == elephant)
                eleToY = -To_Yammy(to, chesscode, map, cc, dd);
            //----------------------------------------------
            
            
            int make_chance = Make_Chance(to, chesscode, map, cc, dd);
            
            if (cc[chesscode - 1].x < 3) make_chance = 0;
            
            return disv - In_Danger_Simple(chesscode, map, cc, dd) - In_Danger_Special(chesscode, map, cc, dd) + Yammy(to, chesscode, map, cc, dd) + big + To_Yammy(to, chesscode, map, cc, dd) + To_Help_Other_Specail(to, chesscode, map, cc, dd) + jumpbonus + To_Help(to, chesscode, map, cc, dd) + rush_bonus + make_chance + adj + eleToY;
        }
    }
    
    LOCATION What_Is_Next(const animal_type & chesscode, int current_x, int current_y, const step_type & direction, const MAP (*map)[7])
    {
        LOCATION to;
        
        switch (direction) {
            case up:
                to.x = current_x + 1;
                to.y = current_y;
                break;
                
            case left:
                to.x = current_x;
                to.y = current_y - 1;
                break;
                
            case down:
                to.x = current_x - 1;
                to.y = current_y;
                break;
                
            case right:
                to.x = current_x;
                to.y = current_y + 1;
                break;
                
            default:
                to.x = current_x;
                to.y = current_y;
                break;
        }
        if (chesscode == tiger || chesscode == lion)
            while(map[to.x][to.y].mtype == water)
            {
                switch (direction) {
                    case up:
                        ++to.x;
                        break;
                        
                    case left:
                        --to.y;
                        break;
                        
                    case down:
                        --to.x;
                        break;
                        
                    case right:
                        ++to.y;
                        break;
                        
                    default:
                        break;
                }
            }
        
        std::cout << "a testing move to (" << to.x << ", " << to.y << ")" << std::endl;
        
        return to;
    }
    
    bool Can_Eat(int prey, int predation, const MAP (*map)[7], const MY_ANIMAL cc[], int xx, int yy)
    {
        if (map[xx][yy].mtype == trap && xx < 2)
            return true;
        if (prey > predation && (prey != 8 || predation != 1))
            return false;
        else if (predation == 1 && map[cc[predation - 1].x][cc[predation - 1].y].mtype != map[xx][yy].mtype && (map[cc[predation - 1].x][cc[predation - 1].y].mtype == water || map[xx][yy].mtype == water))
            return false;
        else if (prey == 1 && predation != 1 && map[xx][yy].mtype == water)
            return false;
        else if (prey == 1 && predation == 8)
            return false;
        else
            return true;
    }
    
    bool Will_Be_Eat(int prey, int predation, const MAP (*map)[7], const ENEMY dd[], int xx, int yy)
    {
        if (map[xx][yy].mtype == trap && xx > 6)
            return true;
        if (prey > predation && (prey != 8 || predation != 1))
            return false;
        else if (predation == 1 && map[dd[predation - 1].x][dd[predation - 1].y].mtype != map[xx][yy].mtype && (map[dd[predation - 1].x][dd[predation - 1].y].mtype == water || map[xx][yy].mtype == water))
            return false;
        else if (prey == 1 && predation != 1 && map[xx][yy].mtype == water)
            return false;
        else if (prey == 1 && predation == 8)
            return false;
        else
            return true;
    }
    
    bool Is_Step_Valid(const animal_type & chesscode, const MAP (*map)[7], LOCATION to, const step_type & direction, const MY_ANIMAL cc[])
    {
        if (to.x < 0 || to.x > 8 || to.y < 0 || to.y > 6 || (to.x == 0 && to.y == 3))
            return false;
        
        if (map[to.x][to.y].atype != nanan && cc[int(map[to.x][to.y].atype) - 1].x == to.x && cc[int(map[to.x][to.y].atype) - 1].y == to.y)
            return false;
        
        if (map[to.x][to.y].atype != nanan && !Can_Eat(map[to.x][to.y].atype, chesscode, map, cc, to.x, to.y))
            return false;
        
        if (chesscode != mouse && map[to.x][to.y].mtype == water)
            return false;
        
        if ((chesscode == tiger || chesscode == lion)){
            if (direction == up && map[to.x - 1][to.y].mtype == water){
                for (int i = 3; i < 6; ++i)
                    if (map[i][to.y].atype == mouse) return false;}
            else if (direction == down && map[to.x + 1][to.y].mtype == water){
                for (int i = 3; i < 6; ++i)
                    if (map[i][to.y].atype == mouse) return false;}
            else if (direction == left && map[to.x][to.y + 1].mtype == water){
                for (int i = 1; i < 3; ++i)
                    if (map[to.x][to.y + i].atype == mouse) return false;}
            else if (direction == right && map[to.x][to.y - 1].mtype == water){
                for (int i = 1; i < 3; ++i)
                    if (map[to.x][to.y - i].atype == mouse) return false;}
        }
        return true;
    }
    
    int Vdis(LOCATION to)
    {
        int disans = abs(8 - to.x) + abs(3 - to.y);
        if (disans == 0) return 100000;
        else return 16 - disans;
    }
    
    int Vdis_Special(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[])
    {
        if (chesscode == mouse)
        {
            animal_type tea = elephant;
            int disans = 16;
            if (dd[tea - 1].isAlive)
            {
                disans = abs(dd[tea - 1].x - to.x) + abs(dd[tea - 1].y - to.y);
                if (map[to.x][to.y].mtype == water) disans += 5;
            }
            return 16 - disans;
        }
        if (chesscode == elephant)
        {
            int disans = 22;
            if (dd[lion - 1].isAlive || dd[tiger - 1].isAlive)
                disans = abs(1 - to.x) + abs(3 - to.y);
            return 22 - disans;
        }
        /*if (chesscode == lion)
        {
            if (dd[tiger - 1].x < 5)
            {
                animal_type tea = tiger;
                int disans = 16;
                if (dd[tea - 1].isAlive)
                    disans = abs(dd[tea - 1].x - to.x) + abs(dd[tea - 1].y - to.y);
                return 18 - disans;
            }
        }*/
        /*if (dd[mouse - 1].isAlive && map[dd[mouse - 1].x][dd[mouse - 1].y].mtype != water && chesscode != tiger && chesscode != lion && chesscode != elephant)
        {
            int disans = 16, eans = 0;
            disans = abs(dd[mouse - 1].x - to.x) + abs(dd[mouse - 1].y - to.y);
            if (dd[lion - 1].isAlive)
                eans = abs(dd[lion - 1].x - to.x) + abs(dd[lion - 1].y - to.y);
            return 16 - disans + eans;
        }*/
        return 0;
    }
    
    int Vdis_New(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[])
    {
        if (chesscode == elephant){
            int disans = abs(1 - to.x) + abs(3 - to.y);
            return 9 - disans;}
        
        if (chesscode == lion || chesscode == tiger){
            if (cc[chesscode - 1].x > 2 && cc[chesscode - 1].isAlive)
            {
                int disans = abs(0 - to.x) + abs(3 - to.y);
                return 30 - disans;
            }
            else
            {
                if (dd[mouse - 1].isAlive && dd[mouse - 1].x < 3)
                {
                    int disans = abs(dd[mouse - 1].x - to.x) + abs(dd[mouse - 1].y - to.y);
                    int curdis = abs(dd[mouse - 1].x - cc[chesscode - 1].x) + abs(dd[mouse - 1].y - cc[chesscode - 1].y);
                    if (curdis == 2 && disans > 2) return 2;
                    else return 13 - disans;
                }
                if (dd[leopard - 1].isAlive && dd[leopard - 1].x < 3)
                {
                    int disans = abs(dd[leopard - 1].x - to.x) + abs(dd[leopard - 1].y - to.y);
                    int curdis = abs(dd[leopard - 1].x - cc[chesscode - 1].x) + abs(dd[leopard - 1].y - cc[chesscode - 1].y);
                    if (curdis == 2 && disans > 2) return 2;
                    else return 13 - disans;
                }
                if (dd[dog - 1].isAlive && dd[dog - 1].x < 3)
                {
                    int disans = abs(dd[dog - 1].x - to.x) + abs(dd[dog - 1].y - to.y);
                    int curdis = abs(dd[dog - 1].x - cc[chesscode - 1].x) + abs(dd[dog - 1].y - cc[chesscode - 1].y);
                    if (curdis == 2 && disans > 2) return 2;
                    else return 13 - disans;
                }
                if (dd[wolf - 1].isAlive && dd[wolf - 1].x < 3)
                {
                    int disans = abs(dd[wolf - 1].x - to.x) + abs(dd[wolf - 1].y - to.y);
                    int curdis = abs(dd[wolf - 1].x - cc[chesscode - 1].x) + abs(dd[wolf - 1].y - cc[chesscode - 1].y);
                    if (curdis == 2 && disans > 2) return 2;
                    else return 13 - disans;
                }
                if (dd[cat - 1].isAlive && dd[cat - 1].x < 3)
                {
                    int disans = abs(dd[cat - 1].x - to.x) + abs(dd[cat - 1].y - to.y);
                    int curdis = abs(dd[cat - 1].x - cc[chesscode - 1].x) + abs(dd[cat - 1].y - cc[chesscode - 1].y);
                    if (curdis == 2 && disans > 2) return 2;
                    else return 13 - disans;
                }
                if (dd[tiger - 1].isAlive && dd[tiger - 1].x < 3)
                {
                    int disans = abs(dd[tiger - 1].x - to.x) + abs(dd[tiger - 1].y - to.y);
                    int curdis = abs(dd[tiger - 1].x - cc[chesscode - 1].x) + abs(dd[tiger - 1].y - cc[chesscode - 1].y);
                    if (curdis == 2 && disans > 2) return 2;
                    else return 13 - disans;
                }
                if (dd[lion - 1].isAlive && dd[lion - 1].x < 3 && chesscode == lion)
                {
                    int disans = abs(dd[lion - 1].x - to.x) + abs(dd[lion - 1].y - to.y);
                    int curdis = abs(dd[lion - 1].x - cc[chesscode - 1].x) + abs(dd[lion - 1].y - cc[chesscode - 1].y);
                    if (curdis == 2 && disans > 2) return 2;
                    else return 13 - disans;
                }
            }
        }
        
        int disans = abs(0 - to.x) + abs(3 - to.y);
        return 16 - disans;
    }
    
    animal_type Danger_From(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[])
    {
        animal_type tottea = nanan;
        if (cc[chesscode - 1].x - 1 >= 0)
        {
            animal_type tea = map[cc[chesscode - 1].x - 1][cc[chesscode - 1].y].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == cc[chesscode - 1].x - 1 && dd[tea - 1].y == cc[chesscode - 1].y && dd[tea - 1].isAlive)
                    if (Will_Be_Eat(chesscode, tea, map, dd, cc[chesscode - 1].x, cc[chesscode - 1].y))
                        tottea = (tottea > tea ? tottea : tea);
            }
        }
        if (cc[chesscode - 1].x + 1 <= 8)
        {
            animal_type tea = map[cc[chesscode - 1].x + 1][cc[chesscode - 1].y].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == cc[chesscode - 1].x + 1 && dd[tea - 1].y == cc[chesscode - 1].y && dd[tea - 1].isAlive)
                    if (Will_Be_Eat(chesscode, tea, map, dd, cc[chesscode - 1].x, cc[chesscode - 1].y))
                        tottea = (tottea > tea ? tottea : tea);
            }
        }
        if (cc[chesscode - 1].y - 1 >= 0)
        {
            animal_type tea = map[cc[chesscode - 1].x][cc[chesscode - 1].y - 1].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == cc[chesscode - 1].x && dd[tea - 1].y == cc[chesscode - 1].y - 1 && dd[tea - 1].isAlive)
                    if (Will_Be_Eat(chesscode, tea, map, dd, cc[chesscode - 1].x, cc[chesscode - 1].y))
                        tottea = (tottea > tea ? tottea : tea);
            }
        }
        if (cc[chesscode - 1].y + 1 <= 6)
        {
            animal_type tea = map[cc[chesscode - 1].x][cc[chesscode - 1].y + 1].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == cc[chesscode - 1].x && dd[tea - 1].y == cc[chesscode - 1].y + 1 && dd[tea - 1].isAlive)
                    if (Will_Be_Eat(chesscode, tea, map, dd, cc[chesscode - 1].x, cc[chesscode - 1].y))
                        tottea = (tottea > tea ? tottea : tea);
            }
        }
        return tottea;
    }
    
    int In_Danger_Simple(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[])
    {
        int more_weight = 0;
        if (chesscode >= tiger) more_weight = 10;
        if (chesscode == elephant) more_weight += 20;
        if (cc[chesscode - 1].x <= 2) more_weight += 20;
        if (cc[chesscode - 1].x <= 2 && map[cc[chesscode - 1].x][cc[chesscode - 1].y].mtype == trap) more_weight += 35;
        
        
        if (cc[chesscode - 1].x - 1 >= 0)
        {
            animal_type tea = map[cc[chesscode - 1].x - 1][cc[chesscode - 1].y].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == cc[chesscode - 1].x - 1 && dd[tea - 1].y == cc[chesscode - 1].y && dd[tea - 1].isAlive)
                    if (Will_Be_Eat(chesscode, tea, map, dd, cc[chesscode - 1].x, cc[chesscode - 1].y))
                        return -5 - more_weight;
            }
        }
        if (cc[chesscode - 1].x + 1 <= 8)
        {
            animal_type tea = map[cc[chesscode - 1].x + 1][cc[chesscode - 1].y].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == cc[chesscode - 1].x + 1 && dd[tea - 1].y == cc[chesscode - 1].y && dd[tea - 1].isAlive)
                    if (Will_Be_Eat(chesscode, tea, map, dd, cc[chesscode - 1].x, cc[chesscode - 1].y))
                        return -5 - more_weight;
            }
        }
        if (cc[chesscode - 1].y - 1 >= 0)
        {
            animal_type tea = map[cc[chesscode - 1].x][cc[chesscode - 1].y - 1].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == cc[chesscode - 1].x && dd[tea - 1].y == cc[chesscode - 1].y - 1 && dd[tea - 1].isAlive)
                    if (Will_Be_Eat(chesscode, tea, map, dd, cc[chesscode - 1].x, cc[chesscode - 1].y))
                        return -5 - more_weight;
            }
        }
        if (cc[chesscode - 1].y + 1 <= 6)
        {
            animal_type tea = map[cc[chesscode - 1].x][cc[chesscode - 1].y + 1].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == cc[chesscode - 1].x && dd[tea - 1].y == cc[chesscode - 1].y + 1 && dd[tea - 1].isAlive)
                    if (Will_Be_Eat(chesscode, tea, map, dd, cc[chesscode - 1].x, cc[chesscode - 1].y))
                        return -5 - more_weight;
            }
        }
        return 0;
    }
    
    int In_Danger_Special(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[])
    {
        int more_weight = 0;
        if (chesscode >= tiger) more_weight = 10;
        if (chesscode == elephant) more_weight += 20;
        if (cc[chesscode - 1].x <= 2) more_weight += 20;
        if (cc[chesscode - 1].x <= 2 && map[cc[chesscode - 1].x][cc[chesscode - 1].y].mtype == trap) more_weight += 35;
        
        if (cc[chesscode - 1].x - 1 >= 0)
        {
            map_type mea = map[cc[chesscode - 1].x - 1][cc[chesscode - 1].y].mtype;
            if (mea == water)
            {
                bool lisafe = false;
                for (int i = 1; i <= 3; ++i)
                    if (map[cc[chesscode - 1].x - i][cc[chesscode - 1].y].atype == mouse) lisafe = true;
                if (!lisafe){
                    animal_type tea = map[cc[chesscode - 1].x - 4][cc[chesscode - 1].y].atype;
                    if ((tea == lion || tea == tiger) && chesscode != elephant)
                        if (dd[tea - 1].x == cc[chesscode - 1].x - 4 && dd[tea - 1].y == cc[chesscode - 1].y)
                            return -5 - more_weight;}
            }

        }
        if (cc[chesscode - 1].x + 1 <= 8)
        {
            map_type mea = map[cc[chesscode - 1].x + 1][cc[chesscode - 1].y].mtype;
            if (mea == water)
            {
                bool lisafe = false;
                for (int i = 1; i <= 3; ++i)
                    if (map[cc[chesscode - 1].x + i][cc[chesscode - 1].y].atype == mouse) lisafe = true;
                if (!lisafe){
                    animal_type tea = map[cc[chesscode - 1].x + 4][cc[chesscode - 1].y].atype;
                    if ((tea == lion || tea == tiger) && chesscode != elephant)
                        if (dd[tea - 1].x == cc[chesscode - 1].x + 4 && dd[tea - 1].y == cc[chesscode - 1].y)
                            return -5 - more_weight;}
            }
            
        }
        if (cc[chesscode - 1].y - 1 >= 0)
        {
            map_type mea = map[cc[chesscode - 1].x][cc[chesscode - 1].y - 1].mtype;
            if (mea == water)
            {
                bool lisafe = false;
                for (int i = 1; i <= 2; ++i)
                    if (map[cc[chesscode - 1].x][cc[chesscode - 1].y - i].atype == mouse) lisafe = true;
                if (!lisafe){
                    animal_type tea = map[cc[chesscode - 1].x][cc[chesscode - 1].y - 3].atype;
                    if ((tea == lion || tea == tiger) && chesscode != elephant)
                        if (dd[tea - 1].x == cc[chesscode - 1].x && dd[tea - 1].y == cc[chesscode - 1].y - 3)
                            return -5 - more_weight;}
            }
            
        }
        if (cc[chesscode - 1].y + 1 >= 0)
        {
            map_type mea = map[cc[chesscode - 1].x][cc[chesscode - 1].y + 1].mtype;
            if (mea == water)
            {
                bool lisafe = false;
                for (int i = 1; i <= 2; ++i)
                    if (map[cc[chesscode - 1].x][cc[chesscode - 1].y + i].atype == mouse) lisafe = true;
                if (!lisafe){
                    animal_type tea = map[cc[chesscode - 1].x][cc[chesscode - 1].y + 3].atype;
                    if ((tea == lion || tea == tiger) && chesscode != elephant)
                        if (dd[tea - 1].x == cc[chesscode - 1].x && dd[tea - 1].y == cc[chesscode - 1].y + 3)
                            return -5 - more_weight;}
            }
            
        }
        return 0;
    }
    
    bool In_Gentle_Help(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[])
    {
        LOCATION to;
        to.x = cc[chesscode - 1].x;
        to.y = cc[chesscode - 1].y;
        
        if (to.x - 1 >= 0)
        {
            animal_type tea = map[to.x - 1][to.y].atype;
            if (tea != nanan && cc[tea - 1].x == to.x - 1 && cc[tea - 1].y == to.y){
                if (In_Danger_Simple(tea, map, cc, dd) < 0)
                    if (tea != elephant && Can_Eat(Danger_From(tea, map, cc, dd), chesscode, map, cc, to.x - 1, to.y)) // little problem
                        return true;
            }
        }
        if (to.x + 1 <= 8)
        {
            animal_type tea = map[to.x + 1][to.y].atype;
            if (tea != nanan && cc[tea - 1].x == to.x + 1 && cc[tea - 1].y == to.y){
                if (In_Danger_Simple(tea, map, cc, dd) < 0)
                    if (tea != elephant && Can_Eat(Danger_From(tea, map, cc, dd), chesscode, map, cc, to.x + 1, to.y)) // little problem
                        return true;
            }
        }
        if (to.y - 1 >= 0)
        {
            animal_type tea = map[to.x][to.y - 1].atype;
            if (tea != nanan && cc[tea - 1].x == to.x && cc[tea - 1].y == to.y - 1){
                if (In_Danger_Simple(tea, map, cc, dd) < 0)
                    if (tea != elephant && Can_Eat(Danger_From(tea, map, cc, dd), chesscode, map, cc, to.x, to.y - 1)) // little problem
                        return true;
            }
        }
        if (to.y + 1 <= 6)
        {
            animal_type tea = map[to.x][to.y + 1].atype;
            if (tea != nanan && cc[tea - 1].x == to.x && cc[tea - 1].y == to.y + 1){
                if (In_Danger_Simple(tea, map, cc, dd) < 0)
                    if (tea != elephant && Can_Eat(Danger_From(tea, map, cc, dd), chesscode, map, cc, to.x, to.y + 1)) // little problem
                        return true;
            }
        }
        return false;
    }
    
    bool In_Assistant(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[])
    {
        LOCATION to;
        to.x = cc[chesscode - 1].x;
        to.y = cc[chesscode - 1].y;
        if (chesscode == elephant){
            if (to.x - 1 >= 0)
            {
                animal_type tea = map[to.x - 1][to.y].atype;
                if (tea != nanan && cc[tea - 1].x == to.x - 1 && cc[tea - 1].y == to.y){
                    if (In_Danger_Special(tea, map, cc, dd) == -5)
                        return true;
                }
            }
            if (to.x + 1 <= 8)
            {
                animal_type tea = map[to.x + 1][to.y].atype;
                if (tea != nanan && cc[tea - 1].x == to.x + 1 && cc[tea - 1].y == to.y){
                    if (In_Danger_Special(tea, map, cc, dd) == -5)
                        return true;
                }
            }
            if (to.y - 1 >= 0)
            {
                animal_type tea = map[to.x][to.y - 1].atype;
                if (tea != nanan && cc[tea - 1].x == to.x && cc[tea - 1].y == to.y - 1){
                    if (In_Danger_Special(tea, map, cc, dd) == -5)
                        return true;
                }
            }
            if (to.y + 1 <= 6)
            {
                animal_type tea = map[to.x][to.y + 1].atype;
                if (tea != nanan && cc[tea - 1].x == to.x && cc[tea - 1].y == to.y + 1){
                    if (In_Danger_Special(tea, map, cc, dd) == -5)
                        return true;
                }
            }
        }
        if (to.x - 1 >= 0)
        {
            map_type mea = map[to.x - 1][to.y].mtype;
            if (mea == trap && to.x < 4){
                if (Hole_In_Danger(chesscode, map, cc, dd, to.x - 1, to.y))
                    return true;
            }
        }
        if (to.x + 1 <= 8)
        {
            map_type mea = map[to.x + 1][to.y].mtype;
            if (mea == trap && to.x < 4){
                if (Hole_In_Danger(chesscode, map, cc, dd, to.x + 1, to.y))
                    return true;
            }
        }
        if (to.y - 1 >= 0)
        {
            map_type mea = map[to.x][to.y - 1].mtype;
            if (mea == trap && to.x < 4){
                if (Hole_In_Danger(chesscode, map, cc, dd, to.x, to.y - 1))
                    return true;
            }
        }
        if (to.y + 1 <= 6)
        {
            map_type mea = map[to.x][to.y - 1].mtype;
            if (mea == trap && to.x < 4){
                if (Hole_In_Danger(chesscode, map, cc, dd, to.x, to.y - 1))
                    return true;
            }
        }
        
        return false;
    }
    
    bool Stop_For_Defense(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[])
    {
        if (cc[chesscode - 1].x < 4)
        {
            if (cc[chesscode - 1].x - 1 >= 0)
            {
                map_type mea = map[cc[chesscode - 1].x - 1][cc[chesscode - 1].y].mtype;
                if (mea == trap)
                {
                    if (Hole_In_Danger(chesscode, map, cc, dd, cc[chesscode - 1].x - 1, cc[chesscode - 1].y))
                            return true;
                }
            }
            if (cc[chesscode - 1].x + 1 <= 8)
            {
                map_type mea = map[cc[chesscode - 1].x + 1][cc[chesscode - 1].y].mtype;
                if (mea == trap)
                {
                    if (Hole_In_Danger(chesscode, map, cc, dd, cc[chesscode - 1].x + 1, cc[chesscode - 1].y))
                        return true;
                }
            }
            if (cc[chesscode - 1].y - 1 >= 0)
            {
                map_type mea = map[cc[chesscode - 1].x][cc[chesscode - 1].y - 1].mtype;
                if (mea == trap)
                {
                    if (Hole_In_Danger(chesscode, map, cc, dd, cc[chesscode - 1].x, cc[chesscode - 1].y - 1))
                        return true;
                }
            }
            if (cc[chesscode - 1].y + 1 <= 6)
            {
                map_type mea = map[cc[chesscode - 1].x][cc[chesscode - 1].y + 1].mtype;
                if (mea == trap)
                {
                    if (Hole_In_Danger(chesscode, map, cc, dd, cc[chesscode - 1].x, cc[chesscode - 1].y + 1))
                        return true;
                }
            }
        }
        return false;
    }
    
    int To_Help(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[])
    {
        int help_value = 0;
        int more = To_Help_False_And_Yammy(to, map);
        
        if (to.x - 1 >= 0)
        {
            animal_type tea = map[to.x - 1][to.y].atype;
            if (tea != nanan && cc[tea - 1].x == to.x - 1 && cc[tea - 1].y == to.y){
                if (In_Danger_Simple(tea, map, cc, dd) == -5)
                    if (tea != elephant && Can_Eat(Danger_From(tea, map, cc, dd), chesscode, map, cc, to.x - 1, to.y)) // little problem
                    {if (tea == lion || tea == tiger) help_value +=5;
                        help_value += 5;}
            }
        }
        if (to.x + 1 <= 8)
        {
            animal_type tea = map[to.x + 1][to.y].atype;
            if (tea != nanan && cc[tea - 1].x == to.x + 1 && cc[tea - 1].y == to.y){
                if (In_Danger_Simple(tea, map, cc, dd) == -5)
                    if (tea != elephant && Can_Eat(Danger_From(tea, map, cc, dd), chesscode, map, cc, to.x + 1, to.y)) // little problem
                    {if (tea == lion || tea == tiger) help_value +=5;
                        help_value += 5;}
            }
        }
        if (to.y - 1 >= 0)
        {
            animal_type tea = map[to.x][to.y - 1].atype;
            if (tea != nanan && cc[tea - 1].x == to.x && cc[tea - 1].y == to.y - 1){
                if (In_Danger_Simple(tea, map, cc, dd) == -5)
                    if (tea != elephant && Can_Eat(Danger_From(tea, map, cc, dd), chesscode, map, cc, to.x, to.y - 1)) // little problem
                    {if (tea == lion || tea == tiger) help_value +=5;
                        help_value += 5;}
            }
        }
        if (to.y + 1 <= 6)
        {
            animal_type tea = map[to.x][to.y + 1].atype;
            if (tea != nanan && cc[tea - 1].x == to.x && cc[tea - 1].y == to.y + 1){
                if (In_Danger_Simple(tea, map, cc, dd) == -5)
                    if (tea != elephant && Can_Eat(Danger_From(tea, map, cc, dd), chesscode, map, cc, to.x, to.y + 1)) // little problem
                    {if (tea == lion || tea == tiger) help_value +=5;
                        help_value += 5;}
            }
        }
        return (help_value > 0) ? help_value + more : help_value;
    }
    
    int To_Help_Other_Specail(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[])
    {
        if (chesscode == elephant){
            if (to.x - 1 >= 0)
            {
                map_type mea = map[to.x - 1][to.y].mtype;
                if (mea == trap && to.x < 4){
                    if (Hole_In_Danger(chesscode, map, cc, dd, to.x - 1, to.y))
                        return 50;
                }
                animal_type tea = map[to.x - 1][to.y].atype;
                if (tea != nanan && cc[tea - 1].x == to.x - 1 && cc[tea - 1].y == to.y){
                    if (In_Danger_Special(tea, map, cc, dd) == -5)
                        return 10;
                }
            }
            if (to.x + 1 <= 8)
            {
                map_type mea = map[to.x + 1][to.y].mtype;
                if (mea == trap && to.x < 4){
                    if (Hole_In_Danger(chesscode, map, cc, dd, to.x + 1, to.y))
                        return 50;
                }
                animal_type tea = map[to.x + 1][to.y].atype;
                if (tea != nanan && cc[tea - 1].x == to.x + 1 && cc[tea - 1].y == to.y){
                    if (In_Danger_Special(tea, map, cc, dd) == -5)
                        return 10;
                }
            }
            if (to.y - 1 >= 0)
            {
                map_type mea = map[to.x][to.y - 1].mtype;
                if (mea == trap && to.x < 4){
                    if (Hole_In_Danger(chesscode, map, cc, dd, to.x, to.y - 1))
                        return 50;
                }
                animal_type tea = map[to.x][to.y - 1].atype;
                if (tea != nanan && cc[tea - 1].x == to.x && cc[tea - 1].y == to.y - 1){
                    if (In_Danger_Special(tea, map, cc, dd) == -5)
                        return 10;
                }
            }
            if (to.y + 1 <= 6)
            {
                map_type mea = map[to.x][to.y - 1].mtype;
                if (mea == trap && to.x < 4){
                    if (Hole_In_Danger(chesscode, map, cc, dd, to.x, to.y - 1))
                        return 50;
                }
                animal_type tea = map[to.x][to.y + 1].atype;
                if (tea != nanan && cc[tea - 1].x == to.x && cc[tea - 1].y == to.y + 1){
                    if (In_Danger_Special(tea, map, cc, dd) == -5)
                        return 10;
                }
            }
        }
        
        if (to.x - 1 >= 0)
        {
            map_type mea = map[to.x - 1][to.y].mtype;
            if (mea == trap && to.x < 4){
                if (Hole_In_Danger(chesscode, map, cc, dd, to.x - 1, to.y))
                    return 35;
            }
        }
        if (to.x + 1 <= 8)
        {
            map_type mea = map[to.x + 1][to.y].mtype;
            if (mea == trap && to.x < 4){
                if (Hole_In_Danger(chesscode, map, cc, dd, to.x + 1, to.y))
                    return 35;
            }
        }
        if (to.y - 1 >= 0)
        {
            map_type mea = map[to.x][to.y - 1].mtype;
            if (mea == trap && to.x < 4){
                if (Hole_In_Danger(chesscode, map, cc, dd, to.x, to.y - 1))
                    return 35;
            }
        }
        if (to.y + 1 <= 6)
        {
            map_type mea = map[to.x][to.y - 1].mtype;
            if (mea == trap && to.x < 4){
                if (Hole_In_Danger(chesscode, map, cc, dd, to.x, to.y - 1))
                    return 35;
            }
        }
        
        return 0;
    }
    
    int To_Help_False_And_Yammy(const LOCATION to, const MAP (*map)[7])
    {
        if (to.x < 4)
        {
            if (to.x - 1 >= 0)
            {
                map_type mea = map[to.x - 1][to.y].mtype;
                if (mea == trap && to.x < 4)
                    return 5;
            }
            if (to.x + 1 <= 8)
            {
                map_type mea = map[to.x + 1][to.y].mtype;
                if (mea == trap && to.x < 4)
                    return 5;
            }
            if (to.y - 1 >= 0)
            {
                map_type mea = map[to.x][to.y - 1].mtype;
                if (mea == trap && to.x < 4)
                    return 5;
            }
            if (to.y + 1 <= 6)
            {
                map_type mea = map[to.x][to.y + 1].mtype;
                if (mea == trap && to.x < 4)
                    return 5;
            }

        }
        return 0;
    }

    bool Hole_In_Danger(const animal_type chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[], int xx,int yy)
    {
        int hazard = 0;
        
        int x = cc[chesscode - 1].x, y = cc[chesscode - 1].y;
        
        if (map[xx][yy - 1].atype != nanan)
        {
            if (dd[map[xx][yy - 1].atype - 1].x == xx && dd[map[xx][yy - 1].atype - 1].y == yy - 1) ++hazard;
        }
        if (map[xx][yy + 1].atype != nanan)
        {
            if (dd[map[xx][yy + 1].atype - 1].x == xx && dd[map[xx][yy + 1].atype - 1].y == yy + 1) ++hazard;
        }
        if (map[xx + 1][yy].atype != nanan)
        {
            if (dd[map[xx + 1][yy].atype - 1].x == xx + 1 && dd[map[xx + 1][yy].atype - 1].y == yy) ++hazard;
        }
        
        
        if (map[xx][yy - 1].atype != nanan)
        {
            if (y != yy - 1 && cc[map[xx][yy - 1].atype - 1].x == xx && cc[map[xx][yy - 1].atype - 1].y == yy - 1)
                --hazard;
        }
        
        if (map[xx][yy + 1].atype != nanan)
        {
            if (y != yy + 1 && cc[map[xx][yy + 1].atype - 1].x == xx && cc[map[xx][yy + 1].atype - 1].y == yy + 1)
                --hazard;
        }
        if (map[xx + 1][yy].atype != nanan)
        {
            if (x != xx + 1 && cc[map[xx + 1][yy].atype - 1].x == xx + 1 && cc[map[xx + 1][yy].atype - 1].y == yy)
                --hazard;
        }
        
        
        if (hazard > 0) return true;
        else return false;
    }
    
    bool To_Danger(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[])
    {
        if (to.x - 1 >= 0)
        {
            animal_type tea = map[to.x - 1][to.y].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == to.x - 1 && dd[tea - 1].y == to.y && dd[tea - 1].isAlive){
                    if (map[to.x][to.y].mtype == trap) return true;
                    if (Will_Be_Eat(chesscode, tea, map, dd, to.x, to.y))
                        return true;}
            }
        }
        if (to.x + 1 <= 8)
        {
            animal_type tea = map[to.x + 1][to.y].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == to.x + 1 && dd[tea - 1].y == to.y && dd[tea - 1].isAlive){
                    if (map[to.x][to.y].mtype == trap) return true;
                    if (Will_Be_Eat(chesscode, tea, map, dd, to.x, to.y))
                        return true;}
            }
        }
        if (to.y - 1 >= 0)
        {
            animal_type tea = map[to.x][to.y - 1].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == to.x && dd[tea - 1].y == to.y - 1 && dd[tea - 1].isAlive){
                    if (map[to.x][to.y].mtype == trap) return true;
                    if (Will_Be_Eat(chesscode, tea, map, dd, to.x, to.y))
                        return true;}
            }
        }
        if (cc[chesscode - 1].y + 1 <= 6)
        {
            animal_type tea = map[to.x][to.y + 1].atype;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == to.x && dd[tea - 1].y == to.y + 1 && dd[tea - 1].isAlive){
                    if (map[to.x][to.y].mtype == trap) return true;
                    if (Will_Be_Eat(chesscode, tea, map, dd, to.x, to.y))
                        return true;}
            }
        }
        return false;
    }
    
    int To_Yammy(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[])
    {
        int more = To_Help_False_And_Yammy(to, map);
        
        int adj= 0;
        
        if (to.x < 3)
        {
            if ((to.x - 0) < (cc[chesscode - 1].x - 0) || (to.y - 3) < (cc[chesscode - 1].y - 3))
                adj = 1;
            if ((to.x - 0) > (cc[chesscode - 1].x - 0) || (to.y - 3) > (cc[chesscode - 1].y - 3))
                adj = -1;
        }
        
        if (chesscode == elephant)
        {
            if (adj != 0 && chesscode == elephant && to.x < 2)
                adj += 3;
        }
        
        if (to.x - 1 >= 0)
        {
            animal_type tea = map[to.x - 1][to.y].atype;
            map_type mea = map[to.x - 1][to.y].mtype;
            if (mea == trap) more += 7;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == to.x - 1 && dd[tea - 1].y == to.y && dd[tea - 1].isAlive){
                    if (map[to.x][to.y].mtype == trap) return 0;
                    if (Can_Eat(tea, chesscode, map, cc, dd[tea - 1].x, dd[tea - 1].y) && chesscode != mouse)
                        return 7 - chesscode + tea + more + adj;}
            }
        }
        if (to.x + 1 <= 8)
        {
            animal_type tea = map[to.x + 1][to.y].atype;
            map_type mea = map[to.x - 1][to.y].mtype;
            if (mea == trap) more += 7;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == to.x + 1 && dd[tea - 1].y == to.y && dd[tea - 1].isAlive){
                    if (map[to.x][to.y].mtype == trap) return 0;
                    if (Can_Eat(tea, chesscode, map, cc, dd[tea - 1].x, dd[tea - 1].y) && chesscode != mouse)
                        return 7 - chesscode + tea + more + adj;}
            }
        }
        if (to.y - 1 >= 0)
        {
            animal_type tea = map[to.x][to.y - 1].atype;
            map_type mea = map[to.x - 1][to.y].mtype;
            if (mea == trap) more += 7;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == to.x && dd[tea - 1].y == to.y - 1 && dd[tea - 1].isAlive){
                    if (map[to.x][to.y].mtype == trap) return 0;
                    if (Can_Eat(tea, chesscode, map, cc, dd[tea - 1].x, dd[tea - 1].y) && chesscode != mouse)
                        return 7 - chesscode + tea + more + adj;}
            }
        }
        if (cc[chesscode - 1].y + 1 <= 6)
        {
            animal_type tea = map[to.x][to.y + 1].atype;
            map_type mea = map[to.x - 1][to.y].mtype;
            if (mea == trap) more += 7;
            if (tea != nanan)
            {
                if (dd[tea - 1].x == to.x && dd[tea - 1].y == to.y + 1 && dd[tea - 1].isAlive){
                    if (map[to.x][to.y].mtype == trap) return 0;
                    if (Can_Eat(tea, chesscode, map, cc, dd[tea - 1].x, dd[tea - 1].y) && chesscode != mouse)
                        return 7 - chesscode + tea + more + adj;}
            }
        }
        return 0;
    }
    
    bool To_Danger_Specail(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[])
    {
        if (to.x - 1 >= 0)
        {
            map_type mea = map[to.x - 1][to.y].mtype;
            if (mea == water)
            {
                bool lisafe = false;
                for (int i = 1; i <= 3; ++i)
                    if (map[to.x - i][to.y].atype == mouse) lisafe = true;
                if (!lisafe){
                    animal_type tea = map[to.x - 4][to.y].atype;
                    if ((tea == lion || tea == tiger) && chesscode != elephant)
                        if (dd[tea - 1].x == to.x - 4 && dd[tea - 1].y == to.y)
                            return true;}
            }
            
        }
        if (to.x + 1 <= 8)
        {
            map_type mea = map[to.x + 1][to.y].mtype;
            if (mea == water)
            {
                bool lisafe = false;
                for (int i = 1; i <= 3; ++i)
                    if (map[to.x + i][to.y].atype == mouse) lisafe = true;
                if (!lisafe){
                    animal_type tea = map[to.x + 4][to.y].atype;
                    if ((tea == lion || tea == tiger) && chesscode != elephant)
                        if (dd[tea - 1].x == to.x + 4 && dd[tea - 1].y == to.y)
                            return true;}
            }
            
        }
        if (to.y - 1 >= 0)
        {
            map_type mea = map[to.x][to.y - 1].mtype;
            if (mea == water)
            {
                bool lisafe = false;
                for (int i = 1; i <= 2; ++i)
                    if (map[to.x][to.y - i].atype == mouse) lisafe = true;
                if (!lisafe){
                    animal_type tea = map[to.x][to.y - 3].atype;
                    if ((tea == lion || tea == tiger) && chesscode != elephant)
                        if (dd[tea - 1].x == to.x && dd[tea - 1].y == to.y - 3)
                            return true;}
            }
            
        }
        if (to.y + 1 <= 6)
        {
            map_type mea = map[to.x][to.y + 1].mtype;
            if (mea == water)
            {
                bool lisafe = false;
                for (int i = 1; i <= 2; ++i)
                    if (map[to.x][to.y + i].atype == mouse) lisafe = true;
                if (!lisafe){
                    animal_type tea = map[to.x][to.y + 3].atype;
                    if ((tea == lion || tea == tiger) && chesscode != elephant)
                        if (dd[tea - 1].x == to.x && dd[tea - 1].y == to.y + 3)
                            return true;}
            }
            
        }
        return false;
    }
    
    int Yammy(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[])
    {
        animal_type prey = map[to.x][to.y].atype;
        if (prey != nanan)
        {
            int morew = 0;
            if (map[to.x][to.y].mtype == trap && to.x < 4) morew = 900;
            if (prey >= tiger) morew += 50;
            if (chesscode == elephant) morew += 7;
            if (Can_Eat(prey, chesscode, map, cc, to.x, to.y))
                return 7 - (int(chesscode) - int(prey)) + morew;
        }
        return 0;
    }
    
    bool Enemy_Weak(const LOCATION to, const MAP (*map)[7], const ENEMY dd[])
    {
        int n = 0;
        n = ((abs(to.x - 8) + abs(to.y - 2)) < (abs(to.x - 7) + abs(to.y - 3)) ? 0 : 1);
        if (n == 0)
        {
            n = ((abs(to.x - 8) + abs(to.y - 2)) < (abs(to.x - 8) + abs(to.y - 4)) ? 0 : 2);
        }
        else
        {
            n = ((abs(to.x - 7) + abs(to.y - 3)) <= (abs(to.x - 8) + abs(to.y - 4)) ? 1 : 2);
        }
        
        bool kk = true;
        switch (n)
        {
            case 0:
                if (map[8][1].atype != nanan && dd[map[8][1].atype - 1].isAlive && dd[map[8][1].atype - 1].x == 8 && dd[map[8][1].atype - 1].y == 1)
                {kk = false;}
                if (map[7][2].atype != nanan && dd[map[7][2].atype - 1].isAlive && dd[map[7][2].atype - 1].x == 7 && dd[map[7][2].atype - 1].y == 2)
                {kk = false;}
                if (kk) return true;
                break;
                
            case 1:
                if (map[6][3].atype != nanan && dd[map[6][3].atype - 1].isAlive && dd[map[6][3].atype - 1].x == 6 && dd[map[6][3].atype - 1].y == 3)
                {kk = false;}
                if (map[7][2].atype != nanan && dd[map[7][2].atype - 1].isAlive && dd[map[7][2].atype - 1].x == 7 && dd[map[7][2].atype - 1].y == 2)
                {kk = false;}
                if (map[7][4].atype != nanan && dd[map[7][4].atype - 1].isAlive && dd[map[7][4].atype - 1].x == 7 && dd[map[7][4].atype - 1].y == 4)
                {kk = false;}
                if (kk) return true;
                break;
                
            case 2:
                if (map[8][5].atype != nanan && dd[map[8][5].atype - 1].isAlive && dd[map[8][5].atype - 1].x == 8 && dd[map[8][5].atype - 1].y == 5)
                {kk = false;}
                if (map[7][4].atype != nanan && dd[map[7][4].atype - 1].isAlive && dd[map[7][4].atype - 1].x == 7 && dd[map[7][4].atype - 1].y == 4)
                {kk = false;}
                if (kk) return true;
                break;
        }
        return false;
    }
    
    int Make_Chance(const LOCATION to,const animal_type chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[])
    {
        if ((To_Danger(to, chesscode, map, cc, dd) || To_Danger_Specail(to, chesscode, map, cc, dd)) && chesscode != elephant)
        {
            int more = To_Help_False_And_Yammy(to, map);
            
            int adj= 0;
            
            if (to.x < 3)
            {
                if ((to.x - 0) < (cc[chesscode - 1].x - 0) || (to.y - 3) < (cc[chesscode - 1].y - 3))
                    adj = 1;
                if ((to.x - 0) > (cc[chesscode - 1].x - 0) || (to.y - 3) > (cc[chesscode - 1].y - 3))
                    adj = -1;
            }
            
            
            //-------try to find the greatest danger---------
            animal_type tottea = nanan;
            bool mouse_in = false;
            
            if (to.x - 1 >= 0)
            {
                animal_type tea = map[to.x - 1][to.y].atype;
                if (tea != nanan)
                {
                    if (dd[tea - 1].x == to.x - 1 && dd[tea - 1].y == to.y && dd[tea - 1].isAlive)
                    {
                        tottea = (tottea > tea ? tottea : tea);
                        if (tea == mouse) mouse_in = true;
                    }
                }
            }
            if (to.x + 1 <= 8)
            {
                animal_type tea = map[to.x + 1][to.y].atype;
                if (tea != nanan)
                {
                    if (dd[tea - 1].x == to.x + 1 && dd[tea - 1].y == to.y && dd[tea - 1].isAlive)
                    {
                        tottea = (tottea > tea ? tottea : tea);
                        if (tea == mouse) mouse_in = true;
                    }
                }
            }
            if (to.y - 1 >= 0)
            {
                animal_type tea = map[to.x][to.y - 1].atype;
                if (tea != nanan)
                {
                    if (dd[tea - 1].x == to.x && dd[tea - 1].y == to.y - 1 && dd[tea - 1].isAlive)
                    {
                        tottea = (tottea > tea ? tottea : tea);
                        if (tea == mouse) mouse_in = true;
                    }
                }
            }
            if (to.y + 1 <= 6)
            {
                animal_type tea = map[to.x][to.y + 1].atype;
                if (tea != nanan)
                {
                    if (dd[tea - 1].x == to.x && dd[tea - 1].y == to.y + 1 && dd[tea - 1].isAlive)
                    {
                        tottea = (tottea > tea ? tottea : tea);
                        if (tea == mouse) mouse_in = true;
                    }
                }
            }
            //-------------------------------------------------
            
            std::cout << tottea << std::endl;
            
            int x = cc[chesscode - 1].x, y = cc[chesscode - 1].y;
            
            if (tottea != nanan)
            {
            
                if (to.x - 1 >= 0 && x != to.x - 1)
                {
                    animal_type tea = map[to.x - 1][to.y].atype;
                    if (tea != nanan)
                    {
                        if (cc[tea - 1].x == to.x - 1 && cc[tea - 1].y == to.y && cc[tea - 1].isAlive)
                            if (Can_Eat(tottea, tea, map, cc, to.x, to.y) && !Stop_For_Defense(tea, map, cc, dd) && !In_Assistant(tea, map, cc, dd) && !In_Gentle_Help(tea, map, cc, dd) && !(tea == elephant && mouse_in))
                                return 7 - tea + tottea + more + adj;
                    }
                }
                if (to.x + 1 <= 8 && x != to.x + 1)
                {
                    animal_type tea = map[to.x + 1][to.y].atype;
                    if (tea != nanan)
                    {
                        if (cc[tea - 1].x == to.x + 1 && cc[tea - 1].y == to.y && cc[tea - 1].isAlive)
                            if (Can_Eat(tottea, tea, map, cc, to.x, to.y) && !Stop_For_Defense(tea, map, cc, dd) && !In_Assistant(tea, map, cc, dd) && !In_Gentle_Help(tea, map, cc, dd) && !(tea == elephant && mouse_in))
                                return 7 - tea + tottea + more + adj;
                    }
                }
                if (to.y - 1 >= 0 && y != to.y - 1)
                {
                    animal_type tea = map[to.x][to.y - 1].atype;
                    if (tea != nanan)
                    {
                        if (cc[tea - 1].x == to.x && cc[tea - 1].y == to.y - 1 && cc[tea - 1].isAlive)
                            if (Can_Eat(tottea, tea, map, cc, to.x, to.y) && !Stop_For_Defense(tea, map, cc, dd) && !In_Assistant(tea, map, cc, dd) && !In_Gentle_Help(tea, map, cc, dd) && !(tea == elephant && mouse_in))
                                return 7 - tea + tottea + more + adj;
                    }
                }
                if (to.y + 1 <= 6 && y != to.y + 1)
                {
                    animal_type tea = map[to.x][to.y + 1].atype;
                    if (tea != nanan)
                    {
                        if (cc[tea - 1].x == to.x && cc[tea - 1].y == to.y + 1 && cc[tea - 1].isAlive)
                            if (Can_Eat(tottea, tea, map, cc, to.x, to.y) && !Stop_For_Defense(tea, map, cc, dd) && !In_Assistant(tea, map, cc, dd) && !In_Gentle_Help(tea, map, cc, dd) && !(tea == elephant && mouse_in))
                                return 7 - tea + tottea + more + adj;
                    }
                }
                
                
            }
        }
        return 0;
    }
    
    int Half_Understanding(const MY_ANIMAL cc[], const ENEMY dd[], const int repeat_ch)
    {
        int dangerCount = 0;
        for (int i = 0; i < 8; ++i)
        {
            if (dd[i].isAlive && dd[i].x < 3)
                ++dangerCount;
        }
        if (dangerCount >= 2) return 3;
        
        
        if ((dd[elephant - 1].isAlive && dd[elephant - 1].x > 6 && dd[elephant - 1].y < 3) || !dd[elephant - 1].isAlive) return 1;
        
        if ((dd[lion - 1].isAlive && dd[lion - 1].x < 3)|| (dd[tiger - 1].isAlive && dd[tiger - 1].x < 3)) return 2;
        
        if (repeat_ch == 5 || repeat_ch == 6 || ((cc[lion - 1].x >= 6 || !cc[lion - 1].isAlive)&& (cc[tiger - 1].x >= 6 || !cc[tiger - 1].isAlive)))
            if (cc[repeat_ch].isAlive && cc[repeat_ch].x >= 6) return 4;
        
        return 0;
    }
    
    
    bool Advantage(const MY_ANIMAL cc[], const ENEMY dd[])
    {
        int c = 0, d = 0;
        for (int i = 5; i < 9; ++i)
        {
            if (cc[animal_type(i)].isAlive) ++c;
            if (dd[animal_type(i)].isAlive) ++d;
        }
        
        if (c >
            d + 1) return true;
        else return false;
    }
}
