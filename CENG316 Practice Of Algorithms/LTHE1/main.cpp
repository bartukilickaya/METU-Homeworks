#include <iostream>
#include <vector>
#include<algorithm>
#include <cmath>
using namespace std;

int chain_size = 0;
int min_anchor = 0;
int max_anchor = 0;
vector<vector<int> > result_pairs = { };
vector<vector<int> >res = { };
int heuristic( vector<vector<int> >& all_chains){

    vector<int> vector = all_chains.back(); // get searching vector
    int max_switch = 0;
    if( (vector[0] - min_anchor != 1) && (min_anchor - vector[0] != 1) ){
        max_switch++;
    }
    if( (vector[chain_size-1] - max_anchor != 1) && (max_anchor - vector[chain_size-1] != 1) ){
        max_switch++;
    }
    for( int i = 0 ; i < chain_size-1; i++ ){
        if( (vector[i] - vector[i+1] != 1) && (vector[i+1] - vector[i] != 1) ){
            max_switch++;
        }
    }
    return ceil(max_switch/2.);

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
    //print_chain(chain);
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
            chain = all_chains.back();
            //print_chain(chain);
            reverse(chain.begin() + i,chain.begin() + j);
            all_chains.push_back(chain);
            if( dfs(all_chains,limit,depth + 1) ){
                vector<int> reverse = {i +1 , j}; // one of the reverse moves which should be in result list
                res.push_back(reverse); // append to result list
                return true;
            }
            all_chains.pop_back(); // remove
        }
    }
    return false;
}

void findChain(vector<vector<int> >&all_chains){

    for (int limit = 1; dfs(all_chains, limit,1) == false; limit++) {
    }


}

int main(void){

    vector<int> chain = {};
    int number;
    while( scanf("%d",&number) != EOF){
        chain.push_back(number);
    }
    //chain = {1,3,2};
    chain_size = chain.size();
    vector<vector<int>>all_chains = {};
    all_chains.push_back(chain);
    min_anchor = *min_element(chain.begin(),chain.end()) - 1;
    max_anchor = *max_element(chain.begin(),chain.end()) + 1;

    findChain(all_chains);

    int result_size = res.size();

    cout << result_size << endl;
    for(int i = result_size -1 ; i >= 0; i--){
        cout << res[i][0] << " " << res[i][1] << endl;
    }

    return 0;
}