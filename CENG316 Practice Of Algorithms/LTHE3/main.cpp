#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

struct object {
    int label;
    int alpha;
    int beta;
    int gamma;
};

class FenwickTree {
private:

    vector<int> tree;
    int n;

public:
    FenwickTree(int n) : tree(n + 1, numeric_limits<int>::min()), n(n) {}

    static int lsb(int i){
        return i & (i ^ (i - 1));
    }

    int size() const{
        return tree.size() - 1;
    }

    int query(int k)const {
        int maximum = numeric_limits<int>::min();
        for (int i = k; i >= 0;i = (i & (i + 1)) - 1 ){
            if( tree[i] > maximum){
                maximum = tree[i];
            }
        }
        return maximum;
    }

    void update(int k, int value){
        for (int i = k; i < size(); i = i | (i + 1)){
            if( value > tree[i] ){
                tree[i] = value;
            }
        }
    }
};

bool compareAlpha(object& x, object& y) {
    if (x.alpha == y.alpha) {
        if (x.beta == y.beta) {
            return x.gamma >= y.gamma;
        }
        return x.beta < y.beta;
    }
    return x.alpha >= y.alpha;
}


bool compareBeta(object& x, object& y) {
    return x.beta >= y.beta;
}

int main() {
    int sz;
    scanf("%d",&sz);
    vector<object> objects(sz + 1);

    for(int i = 1; i <= sz; i++ ){
        objects[i].label = i;
        scanf("%d %d %d",&objects[i].alpha,&objects[i].beta,&objects[i].gamma);
    }

    // RSR for beta
    vector<object> betas = objects;
    sort(betas.begin() + 1, betas.end(), compareBeta);
    for (int i = 1, rank = 1; i <= sz; i++) {
        if (i == 1 || betas[i].beta != betas[i - 1].beta){
            rank++;
        }
        objects[betas[i].label].beta = rank;
    }

    // Sort by alpha
    sort(objects.begin() + 1, objects.end(), compareAlpha);

    FenwickTree tree(sz + 1); // construct tree with 1 size more for dummy node
    vector<int> results;

    for (int i = 1; i <= sz; i++) {
        //cout << tree.query(objects[i].beta ) << " : " <<  objects[i].alpha << " : " << objects[i].beta << " : " << objects[i].gamma << endl;
        int gamma = tree.query(objects[i].beta);
        if( i != 1 && objects[i-1].alpha == objects[i].alpha && objects[i -1].beta == objects[i].beta && gamma <= objects[i].gamma){ // check if alpha and beta are equal
            results.push_back(objects[i].label);
            tree.update(objects[i].beta, objects[i].gamma);
        }
        else if( i != 1 && objects[i-1].alpha == objects[i].alpha && objects[i -1].beta < objects[i].beta && gamma >= objects[i].gamma ){ // check if same alpha, lower rank and gamma
            continue;
        }
        else if ( gamma < objects[i].gamma) {
            results.push_back(objects[i].label);
            tree.update(objects[i].beta, objects[i].gamma);
        }
    }


    sort(results.begin(), results.end());

    vector<int>::iterator it;
    for(it = results.begin(); it != results.end(); it++){
        cout << *it << endl;
    }

    return 0;
}
