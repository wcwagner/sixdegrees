#include <algorithm>
#include <string>
#include <unordered_map>
#include <iostream>
#include <string>

struct edge{
    std::string actor;
    std::string movie;
};

class Graph{

private:
    std::unordered_map<std::string, std::vector<edge>> G;

public:
    Graph() : G() {}

    void addEdge(const std::string& actor1, const std::string& actor2, const std::string& movie );
    friend std::ostream& operator<<(std::ostream& os, const Graph& Gr);
};

void Graph::addEdge(const std::string& actor1, const std::string& actor2, const std::string& movie ){
        // undirected graph so mark both directions in adj list
        if( G.find(actor1) != G.end() )
            G[actor1].push_back( {actor2, movie} );
        else
            G[actor1] = { {actor2, movie} };

        if( G.find(actor2) != G.end() )
            G[actor2].push_back( {actor1, movie} );
        else
            G[actor2] = { {actor1, movie} };
}

std::ostream& operator<<(std::ostream& os, const Graph& Gr){
    for(auto it1 = Gr.G.cbegin(); it1 != Gr.G.cend(); it1++){
        for( auto it2 = it1->second.cbegin(); it2 != it1->second.cend(); it2++){
            os << it1->first << "---" << it2->movie << "---" << it2->actor << std::endl;
        }
    }
    return os;
};

int main(){
    Graph G;
    G.addEdge("Brad Pitt", "Angelina Jolie", "Mr. And Ms. Smith");
    std::cout << G;
    return 0;
}
