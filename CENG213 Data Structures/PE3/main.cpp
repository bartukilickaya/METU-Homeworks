#include "Graph.cpp"
#include <iostream>
/*You can add additional libraries here to read and process the dataset*/
#include <fstream> // file read/write

using namespace std;

// tester
int main() {

    // read input file
    ifstream inputF;

    // Metal trade Graph
    Graph graph;

    fstream file("world_trade.txt");

    string vertices;
    getline(file, vertices, '\t'); // skip header
    getline(file, vertices, '\n'); // number of vertices

    int numvertices = atoi( vertices.c_str() );
    HashTable<int, Node> nodeTable;
    // Build the graph from the txt input file
    for (int i = 0; i < numvertices; i++) {

        string vid, country, continent, gdp;
        getline(file, vid, '\t');
        getline(file, country, '\t');
        getline(file, continent, '\t');
        getline(file, gdp, '\n');

        Node node(atoi(vid.c_str()), country, continent, atol(gdp.c_str()));
        nodeTable.Insert(node.getVid(), node);

        graph.addNode(node);
    }
    getline(file, vertices, '\n'); // skip header

    // create connections
    string line;
    while (getline(file, line)) {
        int pos = line.find("\t");
        int vid1 = atoi(line.substr(0, pos).c_str());

        line = line.substr(pos+1);
        pos = line.find("\t");
        int vid2 = atoi(line.substr(0, pos).c_str());
        line = line.substr(pos+1);
        long import = atol(line.c_str());

        graph.addConnection(nodeTable.Get(vid1), nodeTable.Get(vid2), import);
    }
    file.close() ;
    int *p = new int[80];
    nodeTable.getKeys(p);
    Node first,dsc;
    for(int i = 0 ; i < 80; i++){
        if(nodeTable.Get(p[i]).getCountry() == "Honduras"){
            first.setCountry(nodeTable.Get(p[i]).getCountry()) ;
            first.setContinent(nodeTable.Get(p[i]).getContinent());
            first.setGdp(nodeTable.Get(p[i]).getGdp());
            first.setVid(nodeTable.Get(p[i]).getVid());
        }
    }
    for(int i = 0 ; i < 80; i++){
        if(nodeTable.Get(p[i]).getCountry() == "Guatemala"){
            dsc.setCountry(nodeTable.Get(p[i]).getCountry()) ;
            dsc.setContinent(nodeTable.Get(p[i]).getContinent());
            dsc.setGdp(nodeTable.Get(p[i]).getGdp());
            dsc.setVid(nodeTable.Get(p[i]).getVid());
        }
    }
    Graph graph2 = graph;
    list<string> path;
    list<string>::iterator it;
    path = graph2.findLeastCostPath(first,dsc);
    for(it = path.begin(); it != path.end(); ++it){
        cout << *it << ":::" ;
    }


	// YOU CAN WRITE YOUR TEST CASES BELOW



    return 0;
}

