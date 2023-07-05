#include <iostream>
#include <vector>
using namespace std;
#define MODULO 8191

long long total_count = 0;
int width = 0;
int height = 0;
long long ***dp = nullptr;


long long nChoosek( int n, int k ){

    long long result = 1;

    for (int i = 1; i <= k; ++i) {
        result *= n - k + i;
        result /= i;
        //result %= MODULO;
    }
    result %= MODULO;
    return result;
}

long long solver_helper(long long ***dp,int width_index,int width,int height,int k,int l){
    long long count = 0;
    if( width - width_index == 1 ){
        return 1;
    }
    else if( width - width_index == 2 ){
        if( l > 0 ){
            for(int i = 0 ; i <= height -k -l; i++){
                if( dp[width_index+1][i+l][0] == -1 ){
                    dp[width_index+1][i+l][0] = 1;
                }
                count %= MODULO;
                count += nChoosek(height -k -l,i) * dp[width_index+1][i+l][0] % MODULO;

            }
        }
        else{
            for(int i = 1 ; i <= height -k -l; i++){
                if( dp[width_index+1][i+l][0] == -1 ){
                    dp[width_index+1][i+l][0] = 1;
                }
                count %= MODULO;
                count += nChoosek(height -k -l,i) * dp[width_index+1][i+l][0] % MODULO;

            }
        }

    }

    else if( width - width_index == 3 ){
        if(l > 0){
            for(int i = 0; i <= height -k -l;i++){
                for(int j = 0; j <= height -k -l;j++){
                    if( i+j > height-k-l){
                        continue;
                    }
                    if( dp[width_index+1][i+l][j] == -1){
                        dp[width_index+1][i+l][j] = solver_helper(dp,width_index+1,width,height,i+l,j) % MODULO;
                    }
                    count %= MODULO;
                    count += nChoosek(height -k-l,i) * nChoosek(height -k -l-i,j) % MODULO * dp[width_index+1][i+l][j] % MODULO;
                }
            }
        }
        else{
            for(int i = 0; i <= height -k -l;i++){
                for(int j = 0; j <= height -k -l;j++){
                    if( i+j == 0 || i+j > height-k-l){
                        continue;
                    }
                    else{
                        if( dp[width_index+1][i+l][j] == -1){
                            dp[width_index+1][i+l][j] = solver_helper(dp,width_index+1,width,height,i+l,j) % MODULO;
                        }
                        count %= MODULO;
                        count += nChoosek(height -k-l,i) * nChoosek(height -k -l-i,j) % MODULO * dp[width_index+1][i+l][j] % MODULO;

                    }
                }
            }
        }
    }
    else{
        if(l > 0){
            for(int i = 0; i <= height -k -l;i++){
                for(int j = 0; j <= height -k -l;j++){
                    if( i+j > height-k-l ){
                        continue;
                    }
                    if( dp[width_index+1][i+l][j] == -1){
                        dp[width_index+1][i+l][j] = solver_helper(dp,width_index+1,width,height,i+l,j) % MODULO;
                    }
                    count %= MODULO;
                    count += nChoosek(height -k-l,i) * nChoosek(height -k -l-i,j) % MODULO * dp[width_index+1][i+l][j] % MODULO;
                }
            }
        }
        else{
            for(int i = 0; i <= height -k -l;i++){
                for(int j = 0; j <= height -k -l;j++){
                    if( i+j == 0 || i+j > height-k-l){
                        continue;
                    }
                    else{
                        if( dp[width_index+1][i+l][j] == -1){
                            dp[width_index+1][i+l][j] = solver_helper(dp,width_index+1,width,height,i+l,j) % MODULO;
                        }
                        count %= MODULO;
                        count += nChoosek(height -k-l,i) * nChoosek(height -k -l-i,j) % MODULO * dp[width_index+1][i+l][j] % MODULO;
                    }
                }

            }
        }
    }
    return count % MODULO;
}

long long solver(int width,int height){

    if( width == 1 ){
        return 1;
    }
    else if( width == 2 ){
        for(int i = 1 ; i <= height; i++) {
            total_count += nChoosek(height,i);
        }
    }
    else if( width == 3 ){
        dp = new long long**[width];
        for (int i = 0; i < width; ++i) {
            dp[i] = new long long*[height+1];
            for (int j = 0; j < height+1; ++j)
                dp[i][j] = new long long[height+1];
        }
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height+1; j++){
                for(int t = 0; t < height+1; t++){
                    dp[i][j][t] = -1; // set dp
                }
            }
        }

        for(int i = 0; i <= height -1;i++){
            for(int j = 0; j <= height; j++){
                if( i+j == 0 || i+j > height){
                    continue;
                }
                else{
                    if( dp[1][i][j] == -1 ){
                        dp[1][i][j] = solver_helper(dp,1,width,height,i,j) % MODULO;
                    }
                    total_count %= MODULO;
                    total_count += nChoosek(height,i) * nChoosek(height -i,j) % MODULO * dp[1][i][j] % MODULO;

                }
            }
        }
    }
    else{
        dp = new long long**[width];
        for (int i = 0; i < width; ++i) {
            dp[i] = new long long*[height+1];
            for (int j = 0; j < height+1; ++j)
                dp[i][j] = new long long[height+1];
        }
        for(int i = 0; i < width; i++){
            for(int j = 0; j < height+1; j++){
                for(int t = 0; t < height+1; t++){
                    dp[i][j][t] = -1; // set dp
                }
            }
        }

        for(int i = 0; i <= height -1;i++){
            for(int j = 0; j <= height -1; j++){
                if( i+j == 0 || i+j > height){
                    continue;
                }
                else{
                    if( dp[1][i][j] == -1 ){
                        dp[1][i][j] = solver_helper(dp,1,width,height,i,j) % MODULO;
                    }
                    total_count %= MODULO;
                    total_count += nChoosek(height,i) * nChoosek(height -i,j) % MODULO * dp[1][i][j] % MODULO;
                }
            }
        }
    }
    return total_count % MODULO;
}

int main() {

    scanf("%d %d",&width,&height);
    //width = 23;
    //height = 32;

    long long ans = solver(width,height);
    cout << ans;
    return 0;
}
