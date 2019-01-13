#include "gameengine.h"
#include <memory>
#include <cmath>
#include <cstdlib>
#include <QDebug>
#include <map>
#include <string>
#include <iostream>

//GameEngine::tacticIdentifier[11] = {energy, motivation, shooting, stealing, sprint, rebound, passing, handling, block, jump, strength}
int GameEngine::FullCourtPress[11] = {-15,2,-1,5,2,3,0,0,5,0,4};
int GameEngine::FastBreak[11] = {-10,3,5,3,3,2,-2,+2,0,0,-2};
int GameEngine::ZoneDefence[11] = {-5,0,2,3,2,5,-3,0,-2,4,-2};
int GameEngine::FullTimeAttack[11] = {-10,5,4,0,-1,1,3,2,0,0,-1};


std::map< std::string, int(*)[11] > GameEngine::tactics {
    {"FullCourtPress", &FullCourtPress},
    {"FastBreak", &FastBreak},
    {"ZoneDefence", &ZoneDefence},
    {"FullTimeAttack", &FullTimeAttack}
};


GameEngine::GameEngine()
{
    qDebug()<<"Game Engine ready";

}

void GameEngine::simulateThisWeeksGames(League& league) const{
    const std::vector< std::pair< Team, Team > > thisWeeksGames = league.getThisWeeksGames(); //pair of references already
    for (std::vector< std::pair< Team, Team > >::const_iterator i = thisWeeksGames.begin(); i != thisWeeksGames.end(); i++) {
        simulateAutomatedGame(league, i->first, i->second);
    }
}

void GameEngine::simulateAutomatedGame(League& league, Team team1, Team team2) const{
    std::pair< int, int > score = getAutomaticWinner(team1, team2);
    std::pair<Team, Team> match(team1,team2);
    //league.ThisWeeksScores.insert(std::pair< std::pair<Team, Team>, std::pair<int, int>>(match, score));
    league.ThisWeeksScores.push_back(score);
    updateTeamsOverall(league, team1, team2, score);
}

void GameEngine::playThisWeeksGame(User& manager, League& league, Team& opponentsTeam)const {

}

std::pair< int, int > GameEngine::getAutomaticWinner(const Team team1, const Team team2) const{
    const int NUMBER_OF_DRAWS = 40;

    const double total_1 = team1.overallgeneral;      //an integer between 0 and 100
    const double total_2 = team2.overallgeneral;

    const double p1 = total_1 / (total_1+total_2);
                                                                               //A number between 0 and 1
    // const double p2 = total_2 / (total_1+total_2);

    int c1 = 0;      //A counter to count the points that will make a team win
    int c2 = 0;

    double r;

    for (int i =0; i<NUMBER_OF_DRAWS; i++){
        r = double(rand()) / (RAND_MAX);//A random number between 0 and 1
        //team 1 attacks
        if (r <= p1) {
            if (0.3 < r){
                c1 += 3;
            }
            else {
                c1 += 2;}
        }
        //team2 attacks
        r = double(rand()) / (RAND_MAX);//A random number between 0 and 1
        if (r > p1){
            if (0.3 < r){
                c2 += 3;
            }
            else {
                c2 += 2;}
        }
    }

    std::pair< int, int > t(c1, c2);
    return t;
}

double KFactor(double overall) {
    return 0.3*(100 - overall);
}                      //K factor (to be improved), teams with higher scores gets smaller modifications


void GameEngine::updateTeamsOverall(League& league, Team& team1, Team& team2, std::pair< int, int > score) const{

    double ov1 = team1.overallgeneral;
    double ov2 = team2.overallgeneral;
    double ovMean = (ov1 + ov2)/2;
    double Q1 = std::pow(10.0,ov1/30);
    double Q2 = std::pow(10.0,ov2/30);
    double Exp = Q1/(Q1+Q2); //expectance for team1, between 0 and 1



    double ov1New;
    double ov2New;
    int mc1;
    int mc2;

    if (score.second > score.first) {
        ov1New = ov1 + (1-Exp)*KFactor(ov1);
        ov2New = ov2 - (1-Exp)*KFactor(ovMean);
        mc1 = -10;
        mc2 = 10; //to edit
    }

    else{
        ov1New = ov1 - Exp*KFactor(ovMean);
        ov2New = ov2 + Exp*KFactor(ov2);
        mc1 = 10;
        mc2 = -10;
    }

    if (ov1New < 10){          //set the minimum as 10 to prevent negative values
        ov1New = 10;
    }
    if (ov2New < 10){
        ov2New = 10;
    }


    if (ov1New > 100){          //set the max as 100
        ov1New = 100;
    }
    if (ov2New > 100){
        ov2New = 100;
    }

    setAfterMatchOverall(league, team1, int(ov1New - ov1), mc1);
    setAfterMatchOverall(league, team2, int(ov2New - ov2), mc2);
}

void GameEngine::setAfterMatchOverall(League& league, Team& team, const int change, const int motivationChange) const {
    for (std::vector< Player >::iterator player = team.players.begin(); player != team.players.end(); player++){
        player->afterMatchUpdate(change, motivationChange);
        team.update_overall();
    }
}

void GameEngine::applyTactic(Team& team,const  std::string tacticName) const{
    // modifiers of attributes [sprint;rebound;passing;handling;shooting;

    // decrese energy of the first five players

    // update the attributes of the first five players

    for (std::vector<Player>::iterator player = team.players.begin(); player != team.players.begin()+5; player++) {
        player->energy += (*tactics[tacticName])[0];
        player->motivation += (*tactics[tacticName])[1];
        player->shooting += (*tactics[tacticName])[2];
        player->stealing += (*tactics[tacticName])[3];
        player->sprint += (*tactics[tacticName])[4];
        player->rebound += (*tactics[tacticName])[5];
        player->passing += (*tactics[tacticName])[6];
        player->handling += (*tactics[tacticName])[7];
        player->block += (*tactics[tacticName])[8];
        player->jump += (*tactics[tacticName])[9];
        player->strength += (*tactics[tacticName])[10];
    }
     team.update_overall();
}

void GameEngine::getBacktoDefaultTactic(Team& playingTeam, Team& initTeam) const {
    // copy init tean attributes to playing team attributes except from energy
    // do not decrease energy
    //assuming the players are in the same order

    for (std::vector<Player>::iterator playingPlayer = playingTeam.players.begin(), initPlayer = initTeam.players.begin();
         playingPlayer != playingTeam.players.end() && initPlayer != initTeam.players.end();
         playingPlayer++, initPlayer++){

        playingPlayer->stealing = initPlayer->stealing;
        playingPlayer->block = initPlayer->block;
        playingPlayer->sprint = initPlayer->sprint;
        playingPlayer->rebound = initPlayer->rebound;
        playingPlayer->strength = initPlayer->strength;
        playingPlayer->shooting = initPlayer->shooting;
        playingPlayer->handling = initPlayer->handling;
        playingPlayer->passing = initPlayer->passing;
        playingPlayer->jump = initPlayer->jump;
        playingPlayer->motivation = initPlayer->motivation;

        //motivation and energy are not copied
    }
    playingTeam.update_overall();

}

Team GameEngine::copyTeam(Team team) const{
    return team;
}

int GameEngine::getAttackResult(Team& managersTeam, Team& oppentsTeam, bool isManagerAttacking) const {
    return 1; // testing

    //get the players playing in managers team (first 5 in the list)
    //get the average attributes

    //same for opponents team but take all the players, (already strored in the team)

    //do some Heuristics (include energy?) depending on whether it's attack or defence

    //decrease the energy of the players playing (5first) for manager by some value
    // decrease energy of all players of opponents team
    // same total decrease

    //update both teams overall

    // return 0, 2, 3 respectively if attack failed, scored 2 points, scored 3points
    // optionally create a message " {playerName} scored a 3point! wow!! ... "

    //@ongoing kevin
}

void GameEngine::endOfQuarterRest(User* manager, Team& managersTeam, Team& oppentsTeam) const{
    //default tactic
    getBacktoDefaultTactic(managersTeam, manager->team);

    // add energy to all players of both teams (careful take min(energy+ ,  100))
    //@Joan

    //update managers'team
    manager->team = copyTeam(managersTeam);
}
