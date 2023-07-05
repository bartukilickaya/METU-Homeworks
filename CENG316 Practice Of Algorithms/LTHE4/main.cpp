#include <limits>
#include "iostream"
#include <bits/stdc++.h>
#include <stdio.h>
#include <vector>
using namespace std;

double curr;
pair<int, int> minPair = {-1, -1};
double minDistance = numeric_limits<double>::max();

struct Cross {
    double x;
    int index;
    int pos;  // start and end points, 1 for end point of the line
};

struct Line {
    double x1;
    double x2;
    double y1;
    double y2;
    string word;

    double cross(double x) {
        if (x1 == x2){
            return y1;
        }
        else{
            return (y2 - y1)*(x - x1)/(x2 - x1) + y1;
        }
    }
};

vector<Line> lines;

set<int, function<bool(int, int)>> checkPosition([](int x1, int x2) { // lambda expression for checking lower or upper

    double ay = lines[x1].cross(curr);
    double by = lines[x2].cross(curr);
    if( ay != by ){
        return ay < by;
    }
    else{
        if( x1 < x2 ){
            return true;
        }
        else{
            return false;
        }
    }
});

void calculateDistance(int a, int b) {
    double ay = lines[a].cross(curr);
    double by = lines[b].cross(curr);
    if (minDistance > by - ay) {
        minDistance = by - ay;
        minPair = {a, b};
    }
}

void sweepLines(vector<Line> &linesNew) {
    vector<Cross> crosses;
    lines = linesNew;
    for(int i = 0; i < lines.size(); i++) {
        crosses.push_back({lines[i].x1,i, 0});
        crosses.push_back({lines[i].x2,i, 1});
    }

    sort(crosses.begin(), crosses.end(), [](Cross &a, Cross &b) {
        return make_pair(a.x, -a.pos) < make_pair(b.x, -b.pos);
    });
    vector<Cross>::iterator cross;
    for(cross = crosses.begin(); cross != crosses.end(); cross++) {
        curr = (*cross).x;
        int i = (*cross).index;
        int pos = (*cross).pos;
        if( !pos ) { // start position of the line

            auto it = checkPosition.insert(i).first;
            auto above = next(it);
            auto below = it;
            if (it != checkPosition.begin()){
                below--;
            }
            if (above != checkPosition.end()){
                calculateDistance(i, *above);
            }

            if (it != checkPosition.begin()){
                calculateDistance(*below, i);
            }

        }
        else { // end position of the line

            auto it = checkPosition.find(i);
            auto above = next(it);
            auto below = it;
            if (it != checkPosition.begin()){
                below--;
            }
            if (it != checkPosition.begin() && above != checkPosition.end()){
                calculateDistance(*below, *above);
            }
            if (above != checkPosition.end()){
                calculateDistance(i, *above);
            }
            if (it != checkPosition.begin()){
                calculateDistance(*below, i);
            }
            checkPosition.erase(it); // remove if line passed completely
        }
    }

    cout << lines[minPair.second].word << " " << lines[minPair.first].word << endl;
    return;
}

int main() {

    int n;
    scanf("%d",&n);
    vector<Line> lines(n);

    for(int i = 0; i < n; i++)
        cin >> lines[i].word >> lines[i].x1 >> lines[i].y1 >> lines[i].x2 >> lines[i].y2;

    sweepLines(lines);

    return 0;
}
