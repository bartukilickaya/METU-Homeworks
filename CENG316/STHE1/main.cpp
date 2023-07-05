#include <iostream>
#include <vector>
#include "algorithm"
#include  "cmath"
using namespace std;

int chain_size = 0;
int min_anchor = 0;
int max_anchor = 0;
vector<vector<int> >res = { };

int heuristic( vector<vector<int> >& all_chains){

    vector<int> vector = all_chains.back(); // get searching vector
    int max_switch = 0;
    if( (vector[0] - min_anchor != 1)  ){
        max_switch++;
    }
    if( (max_anchor - vector[chain_size-1] != 1) ){
        max_switch++;
    }
    for( int i = 0 ; i < chain_size-1; i++ ){
        if( (vector[i+1] - vector[i] != 1) ){
            max_switch++;
        }
    }
    return ceil(max_switch/3.);


}
void print_chain(vector<int> &chain){ // for debug
    for(int i = 0 ; i < chain_size; i++){
        cout << chain[i] << " ";
    }
    cout << endl;
}

bool check_sorted(vector<int> &chain){

    for(int i = 0; i < chain_size-1; i++){
        if( chain[i] > chain[i+1]){
            return false;
        }
    }
    return true;

}

bool dfs(vector<vector<int> > &all_chains,int limit,int depth){

    int needed_depth = heuristic(all_chains) + depth;
    vector<int> chain = all_chains.back();

    if ( needed_depth > limit ){ // prune early
        return false;
    }
    if( check_sorted(chain) ){
        return true;
    }
    for( int i = 0; i < chain_size; i++ ){
        for( int j = i + 2 ; j <= chain_size; j++){
            for(int k = 1 ; k < j - i ; k++){
                chain = all_chains.back();
                //print_chain(chain);
                rotate(chain.begin() + i, chain.begin() + i + k, chain.begin() + j);
                all_chains.push_back(chain);
                if( dfs(all_chains,limit,depth + 1) ){
                    vector<int> rotate = {i +1 , j, k}; // one of the rotate moves which should be in result list
                    res.push_back(rotate); // append to result list
                    return true;
                }
                all_chains.pop_back(); // remove
            }

        }
    }
    return false;
}

void findChain(vector<vector<int> >&all_chains){

    for (int limit = 1; dfs(all_chains, limit,1) == false; limit++) {
    }


}

int main() {
    vector<int> chain = {};
    int number;
    /*while( scanf("%d",&number) != EOF){
        chain.push_back(number);
    }*/
    //chain = {4 ,1 ,5, 3, 2};
    //chain = {8, 1, 7, 5, 6, 4, 2, 9, 3};
    chain = {6,5,4,3,2,1};
    chain_size = chain.size();
    vector<vector<int>>all_chains = {};
    all_chains.push_back(chain);
    min_anchor = 0;
    max_anchor = chain_size + 1;
    findChain(all_chains);

    int result_size = res.size();

    cout << result_size << endl;
    for(int i = result_size -1 ; i >= 0; i--){
        cout << res[i][0] << " " << res[i][1] << " " << res[i][2] << endl;
    }

    return 0;
}
