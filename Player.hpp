//
//  Player.hpp
//  CSE Project
//
//  Created by Edison Reshketa on 19/11/2018.
//  Copyright © 2018 Edison Reshketa. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp
#include <stdio.h>
#pragma once
#include <string>
#include <list>
#include <cmath>
class Player{
public:
    //constuctors
    Player();
    Player(std::string pos);

    //player attributes
    std::string name;
    std::string surname;
    std::string position; // PG=pointguard SG=shootingguard F=forward PF=powerforward C=center
    int division; 
    int age;
    int height;
    int weight;
    int sprint;
    int rebound;
    int passing;
    int handling;
    int shooting;
    int stealing;
    int block;
    int jump;
    int strength;
    int motivation;
    int energy;
    double attack;
    double defence;
    double overallgeneral;
    double marketvalue;
    //player methods
    void update_overall();

};

#endif /* Player_hpp */

