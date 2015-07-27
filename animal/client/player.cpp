#include "player.h"
#include "tuple"
#include "vector"

string Player::name() const {
	//decide your name
}

pair<pair<int, int>, char> Player::makeDecision() {
	//your code
}

void Player::addOperators(int e1,int e2,int e3,int e4,int e5){
	operators_tuple.push_back(make_tuple(e1,e2,e3,e4,e5));
}