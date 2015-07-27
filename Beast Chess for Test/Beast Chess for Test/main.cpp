//
//  main.cpp
//  Beast Chess for Test
//
//  Created by Runzhe Yang on 12/4/14.
//  Copyright (c) 2014 Runzhe Yang. All rights reserved.
//

#include <iostream>
#include <tuple>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "player.h"

using namespace std;

vector<pair<int, pair<pair<int ,int>, pair<int, int > > > > step;
int my_id;

Player *player;

void makeDecision(string& decision){
    pair<pair<int, int>, char> res=player->makeDecision();
    decision=to_string(res.first.first);
    decision+=' ';
    decision+=to_string(res.first.second);
    decision+=' ';
    decision+=res.second;
    decision+='\n';
}

int main(int argc, const char * argv[])
{
    srand((int)time(NULL));
    string message;
    int x_space,y_space,id,new_x,new_y;
    
    cout << "begin" << endl;
    cin >> message;
    my_id = message[0]-'0';
    
    player=new Player(my_id, &step);
    
    while(true){
        cin >> message;
        
        if (message == "gg")
        {
            break;
        }
        else
        {
            if (message == "action")
            {
                string decision = "";
                makeDecision(decision);
                
                if(decision==""){
                    cout << "None" << endl;
                }
                else
                {
                    cout << decision <<endl;
                }
            }
            else
            {
                if (message == "None")
                    continue;
                id = message[0] - '0';
                x_space = message[2] - '0';
                y_space=message[4] - '0';
                new_x=message[6] - '0';
                new_y=message[8] - '0';
                step.push_back(make_pair(id,make_pair(make_pair(x_space,y_space),make_pair(new_x,new_y))));
                player->addOperators(id,x_space,y_space,new_x,new_y);
            }
        }
    }
    delete player;

    
    
    return 0;
}
