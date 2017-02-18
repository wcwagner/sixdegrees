#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <string>

/*
TODO:
    Refactor into basic Graph class
    Keep track of paths between two actors
*/
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

    int sixDegreesOptimized(const std::string& actor1, const std::string& actor2) const;

    std::vector<std::string> successors(const std::vector<std::string>&,
                                        std::unordered_set<std::string>&) const;

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
    // basic BFS implementation to determine if two actors are withhin six degrees of sep.
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

template <typename T>
bool intersects(std::vector<T>& v1, std::vector<T>& v2){
    // Determines whether there's any common elements between two vectorss
    if(v1.empty() || v2.empty()) return false;
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    auto it1 = v1.begin(), it2 = v2.begin();

    while(it1 < v1.end() && it2 < v2.end()){
        if(*it1 < *it2)
            it1++;
        else if( *it2 < *it1)
            it2++;
        else
            return true;
    }
    return false;
}


std::vector<std::string> Graph::successors(const std::vector<std::string>& nodes,
                                           std::unordered_set<std::string>& visited) const{
    /*
    Returns a list of all ndoes adjacent to any of the nodes in the `nodes` vector
    passed in, if they have not been visited already.
    */
    std::vector<std::string> successors;
    for(auto it1 = nodes.cbegin(); it1 != nodes.cend(); it1++){
        auto adjEdges = getEdgeSet(*it1);
        for(auto it2 = adjEdges.cbegin(); it2 != adjEdges.cend(); it2++){
            if(visited.find(it2->actor) == visited.cend()){
                successors.push_back(it2->actor);
                visited.insert(it2->actor);
            }
        }
    }
    return successors;
}

int Graph::sixDegreesOptimized(const std::string& actor1, const std::string& actor2) const{
    /*
    For actor1 and actor2, calculate all other actors with at most 3 degrees
    of separation. Then take the intersection of those two sets. This is essentially
    a bi-directional BFS.
    */
    if(actor1 == actor2)
        return true;

    std::vector<std::string> nodes1 = {actor1}, nodes2 = {actor2};
    std::unordered_set<std::string> visited1 = {actor1}, visited2 = {actor2};
    int degrees = 1;
    while(degrees <= 3){

        nodes1 = successors(nodes1, visited1);
        if(intersects<std::string>(nodes1, nodes2))
            return degrees;

        degrees++;

        nodes2 = successors(nodes2, visited2);
        if(intersects(nodes1, nodes2)){
            return degrees;
        }
        degrees++;
    }
    // over six degrees of sep.
    return 0;
}


std::ostream& operator<<(std::ostream& os, const Graph& Gr){
    for(auto it1 = Gr.G.cbegin(); it1 != Gr.G.cend(); it1++){
        for( auto it2 = it1->second.cbegin(); it2 != it1->second.cend(); it2++){
            os << it1->first << "---" << it2->movie << "---" << it2->actor << std::endl;
        }
    }
    return os;
};
