#include "Graph.h"
using namespace std;
/*Do not add new libraries or files*/


Graph::Graph() {
    // TODO: IMPLEMENT THIS FUNCTION.
    //cout << adjList.Capacity() << "::" << adjList.Size() << endl;
}

Graph::Graph(const Graph& rhs) {
    // TODO: IMPLEMENT THIS FUNCTION.
    adjList = rhs.adjList;

}

Graph& Graph::operator=(const Graph& rhs) {
    // TODO: IMPLEMENT THIS FUNCTION.
    adjList = rhs.adjList;
    //cout << adjList.Capacity() << "::" << adjList.Size() << endl;
    return *this;

}

Graph::~Graph() {
// TODO: IMPLEMENT THIS FUNCTION.
}


void Graph::addNode(const Node &node) {
    // TODO: IMPLEMENT THIS FUNCTION.
    string s = to_string(node.getVid());
    list<Edge> l;
    adjList.Insert(s,l);
}

void Graph::addConnection(const Node& headNode, const Node& tailNode, int import) {
    // TODO: IMPLEMENT THIS FUNCTION.
    string s = to_string(headNode.getVid());
    Edge newedge;
    newedge.setImport(import);
    newedge.setTailNode(tailNode);
    adjList.Get(s).push_back(newedge);
}

list<Node> Graph::getAdjacentNodes(const Node& node) {
    // TODO: IMPLEMENT THIS FUNCTION.
    string s = to_string(node.getVid());
    list<Edge> adjedges;
    list<Node> adjnodes;
    list<Edge>::iterator it;
    adjedges = adjList.Get(s);
    for(it = adjedges.begin() ; it != adjedges.end(); ++it){
        adjnodes.push_back(it->getTailNode());
    }
    //cout << adjedges.back().getTailNode().getCountry();
    return adjnodes;
}

long Graph::getTotalImports(const Node& node) {
    // TODO: IMPLEMENT THIS FUNCTION.
    string s = to_string(node.getVid());
    long imp = 0;
    list<Edge>::iterator it;
    list<Edge> adjedges;
    adjedges = adjList.Get(s);
    for(it = adjedges.begin() ; it != adjedges.end(); ++it){
        imp += it->getImport();
    }
    return imp;
}

void Graph::deleteNode(const Node& node) {
    // TODO: IMPLEMENT THIS FUNCTION.
    string s = to_string(node.getVid());
    adjList.Get(s).clear();
    adjList.Delete(s);
    string *kys = new string[adjList.Size()];
    list<Edge>::iterator it;
    adjList.getKeys(kys);
    int i = 0;
        while(i != adjList.Size()){
            for(it = adjList.Get(kys[i]).begin(); it != adjList.Get(kys[i]).end(); ++it){
                if(node.getVid() == it->getTailNode().getVid()){
                    adjList.Get(kys[i]).erase(it);
                    break;
                }
            }
            i++;
        }
        delete []kys;
}
int Graph::minDistance(int dist[],bool sptSet[]){

    int min = INT_MAX, min_index;

    for (int v = 0; v < adjList.Size(); v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

void Graph::getPath(int parent[], int j,list<int> &indexs){

    if (parent[j] == - 1)
        return;

    getPath(parent, parent[j],indexs);
    indexs.push_back(j);
}

list<string> Graph::findLeastCostPath(const Node& srcNode, const Node& destNode) {
    string *keys = new string[adjList.Size()];
    adjList.getKeys(keys);
    list<string> *p = new list<string>[adjList.Size()];
    list<int> *ints = new list<int>[adjList.Size()];
    list<int> indexs;
    list<Edge>::iterator itt;
    list<string>::iterator it;
    list<int>::iterator q;
    list<string> ret;
    string s;
    int source,dest;
    int graph[adjList.Size()][adjList.Size()];
    for(int i = 0; i < adjList.Size(); i++){
        for(int j = 0; j < adjList.Size(); j++){
            graph[i][j] = 0;
        }
    }
    for(int i = 0 ; i < adjList.Size(); i++){
        for(itt = adjList.Get(keys[i]).begin(); itt != adjList.Get(keys[i]).end(); ++itt){
            s = to_string((*itt).getTailNode().getVid());
            p[i].push_back(s);
        }
    }
    for(int i = 0 ; i < adjList.Size(); i++){
        for(it = p[i].begin(); it != p[i].end(); ++it){
            for(int k = 0; k < adjList.Size(); k++){
                if(keys[k] == (*it)){
                    ints[i].push_back(k);
                    break;
                }
            }
        }
    }
    for(int i = 0; i < adjList.Size();i++){
        for(q = ints[i].begin(),itt = adjList.Get(keys[i]).begin(); q != ints[i].end(); ++q,++itt){
            graph[i][*q] = (*itt).getImport();
        }
    }

    int dist[adjList.Size()];
    bool sptSet[adjList.Size()];
    int parent[adjList.Size()];
    for (int i = 0; i < adjList.Size(); i++){
        parent[i] = -1;
        sptSet[i] = false;
        dist[i] = INT_MAX;
    }
    for(int i = 0; i < adjList.Size(); i++){
        if(to_string(srcNode.getVid()) == keys[i]){
            source = i;
            break;
        }
    }
    for(int i = 0; i < adjList.Size(); i++){
        if(to_string(destNode.getVid()) == keys[i]){
            dest = i;
            break;
        }
    }
    dist[source] = 0;
    for (int count = 0; count < adjList.Size() - 1; count++){
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;
        for (int v = 0; v < adjList.Size(); v++) {
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v]){
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
    indexs.push_back(source);
    getPath(parent,dest,indexs);
    int flag;
    for(q = indexs.begin(); q != indexs.end(); ++q){
        flag = 0;
        for(int j = 0; j < adjList.Size(); j++){
            for(itt = adjList.Get(keys[j]).begin() ; itt != adjList.Get(keys[j]).end(); ++itt){
                if( to_string((*itt).getTailNode().getVid()) == keys[*q]){
                    ret.push_back( (*itt).getTailNode().getCountry());
                    flag = 1;
                    break;
                }
            }
            if(flag){
                break;
            }
        }
    }
    delete [] p;
    delete []keys;
    delete []ints;
    return ret;
}

bool Graph::isCyclicUtil(int v, bool visited[], bool *recStack, list<int> *p)
{
    if(visited[v] == false)
    {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;

        // Recur for all the vertices adjacent to this vertex
        list<int>::iterator it;
        for(it = p[v].begin(); it != p[v].end(); ++it){
            if ( !visited[(*it)] && isCyclicUtil(*it, visited, recStack,p) ){

                return true;
            }
            else if (recStack[*it]){
                return true;
            }
        }

    }
    recStack[v] = false;  // remove the vertex from recursion stack
    return false;
}

bool Graph::isCyclic() {
    // TODO: IMPLEMENT THIS FUNCTION.
    string *keys = new string[adjList.Size()];
    adjList.getKeys(keys);
    list<string> *p = new list<string>[adjList.Size()];
    list<int> *ints = new list<int>[adjList.Size()];
    list<Edge>::iterator itt;
    list<string>::iterator it;
    string s;
    for(int i = 0 ; i < adjList.Size(); i++){
        for(itt = adjList.Get(keys[i]).begin(); itt != adjList.Get(keys[i]).end(); ++itt){
            s = to_string((*itt).getTailNode().getVid());
            p[i].push_back(s);
        }
    }
    for(int i = 0 ; i < adjList.Size(); i++){
        for(it = p[i].begin(); it != p[i].end(); ++it){
            for(int k = 0; k < adjList.Size(); k++){
                if(keys[k] == (*it)){
                    ints[i].push_back(k);
                    break;
                }
            }
        }
    }
    bool *visited = new bool[adjList.Size()];
    bool *recStack = new bool[adjList.Size()];
    for(int i = 0; i < adjList.Size(); i++){
        visited[i] = false;
        recStack[i] = false;
    }
    for(int i = 0; i < adjList.Size(); i++){
        if (isCyclicUtil(i, visited, recStack,ints)){
            delete []visited;
            delete []recStack;
            delete []p;
            delete []keys;
            delete []ints;
            return true;
        }
    }
    delete [] visited;
    delete [] recStack;
    delete [] p;
    delete [] keys;
    delete []ints;
    return false;
}


list<string> Graph::getBFSPath(const Node& srcNode, const Node& destNode) {
    // TODO: IMPLEMENT THIS FUNCTION.
    list<Edge>::iterator it;
    list<Edge> edges;
    string *keys = new string[adjList.Size()];
    list<string> rt;
    list<string> tmplist;
    list<string>::iterator itt;
    int flag = 0,flagg;
    adjList.getKeys(keys);
    string src = to_string(srcNode.getVid()),dest = to_string(destNode.getVid());
    tmplist.push_back(src);
    while(!tmplist.empty()){
        edges = adjList.Get(tmplist.front());
        for(it = edges.begin(); it != edges.end(); ++it){
            if(dest  != to_string((*it).getTailNode().getVid()) ){
                flagg = 0;
                for(itt = tmplist.begin(); itt != tmplist.end(); ++itt){
                    if( (*itt) == to_string((*it).getTailNode().getVid())){
                        flagg = 1;
                        break;
                    }
                }
                if(!flagg){
                    tmplist.push_back( to_string((*it).getTailNode().getVid()) );
                }
            }
            else{
                flag = 1;
                break;
            }
        }
        if(flag){
            break;
        }
        else{
            rt.push_back(tmplist.front());
            tmplist.pop_front();
        }
    }
    tmplist.push_back(dest);
    while(!tmplist.empty()){
        rt.push_back(tmplist.front());
        tmplist.pop_front();
    }
    for(itt = rt.begin(); itt != rt.end(); ++itt){
       for(int i = 0; i < adjList.Size(); i++){
           flag = 0;
           for(it = adjList.Get(keys[i]).begin(); it != adjList.Get((keys[i])).end(); ++it){
               if( to_string((*it).getTailNode().getVid()) == (*itt) ){
                   flag = 1;
                   tmplist.push_back( (*it).getTailNode().getCountry() );
                   break;
               }
           }
           if(flag){
               break;
           }
       }
    }
    delete []keys;
    return tmplist;
}





