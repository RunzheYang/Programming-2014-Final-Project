#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "network.h"
#include "player.h"

using namespace std;
using network::IO;

#define DEBUG_SINGAL 0
#define DEBUG(x) if(DEBUG_SINGAL)cout<<#x<<" : "<<x<<endl;

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

int main(int argc,char **argv)
{
	if(argc!=3 && argc!=2){
		cerr<<"<usage>: client ip [port]"<<endl;
		return 1;
	}
	string port = argc == 3 ? argv[2] : "12345";
	IO client(argv[1],port.c_str());
	string message;
	int x_space,y_space,id,new_x,new_y;
	
	client.receive(message);
	my_id=message[0]-'0';
	player=new Player(my_id,&step);
	client.send(string(player->name()+'\n'));
	while(true){
		client.receive(message);
		DEBUG(message);
		if(message=="game end"){
			break;
		}else{
			if(message=="action"){
				string decision="";
				makeDecision(decision);
				DEBUG(decision);
				if(decision==""){
					client.send("None\n");
				}else{
					client.send(decision);
				}
			}else{
				if(message=="None")
					continue;
				id=message[0]-'0';
				x_space=message[2]-'0';
				y_space=message[4]-'0';
				new_x=message[6]-'0';
				new_y=message[8]-'0';
				step.push_back(make_pair(id,make_pair(make_pair(x_space,y_space),make_pair(new_x,new_y))));
				player->addOperators(id,x_space,y_space,new_x,new_y);
			}
		}
	}
	delete player;
}
