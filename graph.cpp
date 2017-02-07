#include <algorithm>
#include <string>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <string>

struct Edge{
    std::string actor;
    std::string movie;
};

class Graph{

private:
    std::unordered_map<std::string, std::vector<Edge>> G;

public:
    Graph() : G() {}

    void addVertex(const std::string& actor);

    void addEdge(const std::string& actor1, const std::string& actor2, const std::string& movie );

    const std::vector<Edge>& getEdgeSet(const std::string& actor) const;

    bool containsVertex(const std::string& actor) const;

    bool containsEdge(const std::string& actor1, const std::string& actor2) const;

    bool sixDegreesBFS(const std::string& actor1, const std::string& actor2) const;

    std::unordered_map<std::string, std::vector<Edge>>& getAdjList();

    friend std::ostream& operator<<(std::ostream& os, const Graph& Gr);
};


void Graph::addVertex(const std::string& actor){
    if( G.find(actor) == G.end() )
        G[actor] = {};
}


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


const std::vector<Edge>& Graph::getEdgeSet(const std::string& actor) const{
    // will throw out_of_range if vertex not in graph
    return G.at(actor); // at for const ref, [] is non-const
}


bool Graph::containsVertex(const std::string& actor) const{
    return G.find(actor) != G.end();
}


bool Graph::containsEdge(const std::string& actor1, const std::string& actor2) const{
    if( containsVertex(actor1) ){
        for(auto it = G.at(actor1).cbegin(); it != G.at(actor1).cend(); it++){
            if( it->actor == actor2 )
                return true;
        }
    }
    return false;
}

bool Graph::sixDegreesBFS(const std::string& actor1, const std::string& actor2) const{
    // basic BFS implementation to determine if two actors are withhin six degrees of adj
    int degrees = 0;
    std::queue<std::string> Q({actor1});
    std::unordered_map<std::string, std::pair<std::string, std::string> > parent;

    while(!Q.empty() && degrees < 6){
        auto u = Q.front(); Q.pop();
        auto adjEdges = getEdgeSet(u);
        for(auto v = adjEdges.cbegin(); v != adjEdges.cend(); v++){
            parent[v->actor] = {u, v->movie};
            if( v->actor == actor2)
                return true;
            Q.push(v->actor);
            degrees++;
        }
    }
    return false;
}

std::unordered_map<std::string, std::vector<Edge>>& Graph::getAdjList(){
    return G;
}

std::ostream& operator<<(std::ostream& os, const Graph& Gr){
    for(auto it1 = Gr.G.cbegin(); it1 != Gr.G.cend(); it1++){
        for( auto it2 = it1->second.cbegin(); it2 != it1->second.cend(); it2++){
            os << it1->first << "---" << it2->movie << "---" << it2->actor << std::endl;
        }
    }
    return os;
};


/*

int main(){
    Graph G;
    G.addEdge("Brad Pitt", "Angelina Jolie", "Mr. And Ms. Smith");
    std::cout << G;
    auto E = G.getEdgeSet("Brad Pitt");
    for(auto it = E.cbegin(); it != E.cend(); it++){
        std::cout << it->actor << std::endl;
    }
    if(G.containsVertex("Leo Dicaprio")){
        std::cout << "Found Leo" << std::endl;
    }
    G.addVertex("Leo Dicaprio");
    if(G.containsVertex("Leo Dicaprio")){
        std::cout << "Inserted and Found Leo" << std::endl;
    }
    G.addVertex("Leo Dicaprio");
    E = G.getEdgeSet("Leo Dicaprio");
    return 0;
}
*/
