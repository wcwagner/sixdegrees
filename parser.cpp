#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

struct role
{
    std::string movie;
    std::string actor;
};


int main(){
    std::cout << "Filename: ";
    std::string filename;
    std::cin >> filename;

    std::ifstream inFile( filename.c_str() );

    if(! inFile){
        std::cerr << "unable to open input file: "
             << filename << " --exiting...\n";
        return -1;
    }

    std::string line;
    std::vector<role> roles;

    // extract movie and actor name from each line of the file
    while(std::getline(inFile, line)){
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens; // will hold [actorName, Movie]
        while(std::getline(iss, token, '\t')){
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            tokens.push_back(token);
        }
        role thisRole;
        thisRole.actor = tokens[0];
        thisRole.movie = tokens[1];
        roles.push_back(thisRole);
    }

    // map each movie to a vector of it's corresponding actors
    std::unordered_map<std::string, std::vector<std::string>> movieToActors;
    for(auto & role : roles){
         if( movieToActors.find(role.movie) != movieToActors.end() )
            movieToActors[role.movie].push_back(role.actor);
         else
            movieToActors[role.movie] = {role.actor};
    }

    return 0;
 }
