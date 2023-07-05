#include <list>
#include <stack>
#include <queue>
#include <vector>

#include "HashTable.h"
#include "Edge.h"
/* Do not add new libraries or files */

using namespace std;

/** An adjacency list representation of a directed weighted graph. */
class Graph {

public:

    // TODO: IMPLEMENT THESE FUNCTIONS.
    /** CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR */
    Graph();
    Graph(const Graph& rhs);
    Graph& operator=(const Graph& rhs);
    ~Graph();

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Adds the given node to the graph with vid or country as key,
     * and an empty edge list as value */
    void addNode(const Node& node);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Adds a new edge to the edge list of headNode using tailNode and import data */
    void addConnection(const Node& headNode, const Node& tailNode, int import);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Given a country, returns all adjacent
     * countries of the given country as a list of Node
     * Throw ItemNotFoundException, if the given node does not exist */
    list<Node> getAdjacentNodes(const Node& node);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** For the given node, returns the sum of imports */
    long getTotalImports(const Node& node);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Deletes the given country from the graph
     * together with its incident connections(edges) */
    void deleteNode(const Node& node);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Given two countries , returns the least cost path/route between
     * them using import values as the edge weight */
    list<string> findLeastCostPath(const Node& srcNode, const Node& destNode);

    // TODO: IMPLEMENT THIS FUNCTION.
    /** Detect whether the graph contains any cycle */
    bool isCyclic();

    // TODO: IMPLEMENT THIS FUNCTION.
    /** returns the BFS path from srcNode to destNode as a list of country names */
    list<string> getBFSPath(const Node& srcNode, const Node& destNode);

private:
    /** Adjacency list representation of the graph; 
    You can change the signature of this variable*/
    HashTable<string, list<Edge>> adjList;
    // == DEFINE HELPER METHODS & VARIABLES BELOW ==
    bool isCyclicUtil(int v, bool *visited, bool *recStack, list<int> *p);

    void BFSHelper(int u, int d, bool *visited, int *path, int &ind, list<int> *ints, string *keys,list<int> &rt);

    int minDistance(int *dist, bool *sptSet);

    void getPath(int *parent, int j, list<int> &indexs);
};
// End of header file
