#pragma once
#include <array>
#include <string>
#include<limits>
#include<time.h>
#include<iostream>
#include<time.h>
#include"ncn.h"
#include"common.h"
using namespace std;

extern int time_limitation;

class AB_agent {
private:
    LBChess LB;
    const int player;
    int alphaBeta(int depth, int player, int alpha, int beta);
public:
    string runable(bool flag);
    AB_agent(const int p, array<array<int, 5>, 13> b) :player(p), LB(b) {}
};

