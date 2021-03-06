#include "League.h"
#include <utility>
#include <iostream>

//auxiliary function to find the intersection of two vectors

std::vector<int> intersection(const std::vector<int> vect1, const std::vector<int> vect2) {
    std::vector<int> intersect;
    for (std::vector<int>::const_iterator i = vect1.begin(); i != vect1.end(); i++) {
        for (std::vector<int>::const_iterator j = vect2.begin(); j != vect2.end(); j++) {
            if (*i == *j) {
                intersect.push_back(*i);
            }
        }
    }
    return intersect;
}

void League::updateranking(){
    ranking=teams;
    sort(ranking.begin(),ranking.end(),comparepoints);
}

bool League::comparepoints(Team a, Team b){
    return (a.points>b.points);
}


League::League(int divi, std::string seas){          //Takes arguments : division as an int and a season as a string
    division = divi ;
    season = seas;
    std::vector<std::string> teamNames = { "Lakers","Teuta","Lion","Barcelona","Madrid","CSKAMoscow","Worrioes","Vllaznia","ParisBasket","Spurs","Cavaliers","Raptors","Warriors","Thunders","Bers","Bulls","Wizards","Celtics","Pacers","Nets","Hawks" };
    for (int i = 0; i < 12; i++) {
        int n = rand()%(teamNames.size()) ;
        Team t= Team( teamNames[n] );
        teamNames.erase(teamNames.begin()+n);
        teams.push_back(t);
    }
    for (int i=0; i<30 ; i++){
        playermarket.push_back(Player());
    }
    Calendar = calendar();
    current_week = 1;
    ranking=teams;
    sort(ranking.begin(),ranking.end(),comparepoints);
    week=0;
    ThisWeeksGames = getThisWeeksGames();
}


League::League(){          //Creates a default league in 3rd division and in season 17/18
    division = 3 ;
    season = "1718";
    int n = rand()%12 ;
    std::vector<std::string> teamNames = { "Lakers","Vllaznia","ParisBasket","Spurs","Cavaliers","Raptors","Warriors","Thunders","Bers","Bulls","Wizards","Celtics","Pacers","Nets","Hawks" };

    for (int i = 0; i < 12; i++) {
        Team t= Team( teamNames[(n + i) % 12] );
        teams.push_back(t);
    }
    Calendar=calendar();
    current_week = 1;
    for (int i=0; i<30 ; i++){
        playermarket.push_back(Player());
    }
    ThisWeeksGames = getThisWeeksGames();  //uncomment when calendar is ready
    week=0;
}

std::map<int, std::vector< std::pair<int,int> > > League::calendar()
{
    std::map<int, std::vector< std::pair<int, int> > > calendar;

    std::vector<int> Teams1 = {1,2,3,4,5,6};
    std::vector<int> Teams2 = {7,8,9,10,11,12};

    for ( int w=1; w<23; w++){

        for (int t = 0; t< 6; t++){
                std::pair<int, int> match = std::make_pair(Teams1[t], Teams2[t]);
                calendar[w].push_back(match);

            }
        Teams1.insert( Teams1.begin()+1 , Teams2[0]);

        Teams2.push_back(Teams1[Teams1.size()-1]);
        Teams2.erase(Teams2.begin());
        Teams1.pop_back();
    }
    return calendar;
}



std::vector< int > League::getAllUserMatches() {
    std::vector< int > users_matches;
    for (int w = 1; w < 23; w++) {
        const std::vector< std::pair<int,int> > matches= calendar()[w];

        for (std::vector< std::pair<int,int> >::const_iterator match = matches.begin(); match != matches.end(); ++match) {
            int t1= std::get<0>(*match);
            int t2= std::get<1>(*match);
            if(t1==1){
                users_matches.push_back( t2 );
            }
            if(t2==1){
                users_matches.push_back( t1 );
            }
        }
    }
    return users_matches;

}


const std::vector<std::pair<Team, Team>> League::getThisWeeksGames(){
    std::vector<std::pair<int,int>> matches = this->Calendar[current_week];
    std::vector<std::pair<Team, Team>> matches_return;

    for(std::vector<std::pair<int,int>>::const_iterator match = matches.begin();match != matches.end();++match){
      int t1= std::get<0>(*match);
      int t2= std::get<1>(*match);

        if( t1 != 1 && t2 != 1){   //Checking if team 1 (user's team) is in the match
            std::pair< Team, Team > match_return;
            match_return = std::make_pair(this->teams[t1-1], this->teams[t2-1]);
            //The GameEngine comment:
            //Teams are passed by value in std::make_pair hence we will have to do naive search to modify the teams
            //attributes later on
            matches_return.push_back(match_return);
        }


    }
    return matches_return;
}

Team& League::getThisWeeksOpponentTeam(){
    std::cout << this->getAllUserMatches()[this->current_week] << std::endl;
    int team_int = this->getAllUserMatches()[this->current_week-1];
    return this->teams[team_int-1];
}





