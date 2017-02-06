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

    //open file for parsing
    std::ifstream inFile( filename.c_str() );

    if(! inFile){
        std::cerr << "unable to open input file: "
             << filename << " --exiting...\n";
        return -1;
    }

    std::string line;
    std::vector<role> roles;

    while(std::getline(inFile, line)){
        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens; // will hold [actorName, Movie]
        while(std::getline(iss, token, '\t')){
            std::transform(token.begin(), token.end(), token.begin(), ::tolower);
            tokens.push_back(token);
        }
        role thisRole = {tokens[1], tokens[0]};
        roles.push_back(thisRole);
    }
    std::unordered_map<std::string, std::vector<std::string>> movieToActors;
    for(auto & role : roles){
         if( movieToActors.find(role.movie) != movieToActors.end() )
            movieToActors[role.movie].push_back(role.actor);
         else
            movieToActors[role.movie] = {role.actor};
    }
    for(auto it: movieToActors){
        std::cout << it.first << " has: ";
        for(auto actor : it.second){
            std::cout << actor << " ";
        }
        std::cout << "---------------------------------------\n" << std::endl;
    }
    return 0;
 }
