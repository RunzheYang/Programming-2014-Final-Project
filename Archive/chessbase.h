//
//  chessbase.h
//  Beast Chess for Test
//
//  Created by Runzhe Yang on 12/4/14.
//  Copyright (c) 2014 Runzhe Yang. All rights reserved.
//

#ifndef __Beast_Chess_for_Test__chessbase__
#define __Beast_Chess_for_Test__chessbase__

#include <stdio.h>
#include <tuple>
#include <vector>
#include <cstdlib>
#include <ctime>

namespace CHESSBASE
{
    enum map_type {ground, water, trap, hole};
    enum animal_type {nanan, mouse, cat, wolf, dog, leopard, tiger, lion,elephant};
    enum step_type {stay, up, left, down, right};
    
    struct MAP
    {
        const map_type mtype;
        animal_type atype;
    };
    
    struct MY_ANIMAL
    {
        int x;
        int y;
        bool isAlive;
        
        int value_of_step[4];
    };
    
    struct ENEMY
    {
        int x;
        int y;
        bool isAlive;
    };
    
    struct LOCATION
    {
        int x;
        int y;
    };
    
    void Initialize(MAP (*map)[7], MY_ANIMAL cc[], ENEMY dd[]);
    void Refresh(MAP (*map)[7], MY_ANIMAL cc[], ENEMY dd[], const animal_type chesscode, const int to_x, const int to_y, const int who, const int my_idd);
    
    
    int Try_Move(const animal_type & chesscode, const step_type & direction, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[], bool & start_chain, const int & repeat_ch, bool & can_change);
    
    LOCATION What_Is_Next(const animal_type & chesscode, int current_x, int current_y, const step_type & direction, const MAP (*map)[7]);
    bool Can_Eat(int prey, int predation, const MAP (*map)[7], const MY_ANIMAL cc[], int xx, int yy); //yes it's int and no need to -1!!
    bool Will_Be_Eat(int prey, int predation, const MAP (*map)[7], const ENEMY dd[], int xx, int yy);//ditto
    bool Is_Step_Valid(const animal_type & chesscode, const MAP (*map)[7], LOCATION to, const step_type & direction, const MY_ANIMAL cc[]);
    
    int Vdis(LOCATION to);
    
    int Vdis_Special(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[]);
    
    int Vdis_New(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[]);
    
    animal_type Danger_From(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[]);
    
    int In_Danger_Simple(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[]);
    
    int In_Danger_Special(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[]);
    
    bool In_Gentle_Help(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[]);
    
    bool In_Assistant(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[]);
    
    bool Stop_For_Defense(const animal_type & chesscode, const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[]);
    
    int To_Help(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[]);
    
    int To_Help_Other_Specail(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[]);
    
    int To_Help_False_And_Yammy(const LOCATION to, const MAP (*map)[7]);
    
    bool Hole_In_Danger(const MAP (*map)[7], const ENEMY dd[], int xx, int yy);
    
    bool To_Danger(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[]);
    
    bool To_Danger_Specail(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[]);
    
    int To_Yammy(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[]);
    
    int Yammy(const LOCATION to, const animal_type & chesscode, const MAP (*map)[7],const MY_ANIMAL cc[], const ENEMY dd[]);
    
    bool Enemy_Weak(const LOCATION to, const MAP (*map)[7], const ENEMY dd[]);
    
    int Make_Chance(const LOCATION to,const animal_type chesscode,  const MAP (*map)[7], const MY_ANIMAL cc[], const ENEMY dd[]);
    
    int Half_Understanding(const MY_ANIMAL cc[], const ENEMY dd[], const int repeat_ch);
}

#endif /* defined(__Beast_Chess_for_Test__chessbase__) */
