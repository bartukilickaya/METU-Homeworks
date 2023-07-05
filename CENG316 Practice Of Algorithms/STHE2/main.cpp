#include <iostream>
using namespace std;
#define MODULO 8191
#define NOTSET 8192
int width = 0;
int height = 0;
int D = 0;
uint16_t ***dp = nullptr;


long long solver(int D ){ // d for coming 2x1 blocks, D for remaining 2x1 blocks I have in total

    dp[height-1][width-1][0] = 1;

    for(int i = height-1 ; i >= 0; i--){
        for(int j = width-1; j >= 0; j--){
            if( i == height-1 && j == width-1 ){
                continue;
            }
            for(int d = 0; d <= D; d++){
                if( ((height - (i+1)) * width + (width-j)) / 2. < d) {
                    dp[i][j][d] = 0;
                    break;
                }
                else if( j == width -1 ){
                    dp[i][j][d] = dp[i+1][0][d];
                }
                else if( j == width -2){
                    if( i == height -1 ){
                        if( d == 0 ){
                            dp[i][j][0] = dp[i][j+1][d];
                        }
                        else{
                            dp[i][j][d] = dp[i][j+1][d] + 1;
                        }

                    }
                    else{
                        if( d == 0 ){
                            dp[i][j][d] = dp[i][j+1][d];
                        }
                        else{
                            dp[i][j][d] = (dp[i][j+1][d] + dp[i+1][0][d-1]) % MODULO;
                        }

                    }
                }
                else{
                    if( d == 0 ){
                        dp[i][j][0] = dp[i][j+1][0];
                    }
                    else{
                        dp[i][j][d] = (dp[i][j+1][d] + dp[i][j+2][d-1]) % MODULO;
                    }

                }
                //cout << dp[i][j][d] << endl;

            }
        }
    }
    /*for(int i = 0; i <= D; i++){
        cout << dp[0][0][i] << endl;
    }*/
    return dp[0][0][D] % MODULO;
}


int main() {

    //scanf("%d %d %d",&width,&height,&D);
    width = 20;
    height = 20;
    D = 100;

    dp = new uint16_t **[height];
    for (int i = 0; i < height; ++i) {
        dp[i] = new uint16_t*[width];
        for (int j = 0; j < width; ++j)
            dp[i][j] = new uint16_t[D+1];
    }
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int t = 0; t < D+1; t++){
                dp[i][j][t] = 0; // set dp
            }
        }
    }
    long long ans = solver(D) ;
    cout << ans;

    return 0;
}
