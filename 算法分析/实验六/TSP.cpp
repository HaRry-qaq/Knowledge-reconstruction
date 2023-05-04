#include<iostream>
#include <unordered_map>
using namespace std;



int V[100]; //顶点集合
const int n = 4; //顶点个数




// int w[n][n] = {
//     __INT_MAX__,10,20,5,
//     10,__INT_MAX__,15,5,
//     20,15,__INT_MAX__,5,
//     5,5,5,__INT_MAX__
// };  //权重矩阵

int w[n][n] = {
    __INT_MAX__,20,10,5,
    20,__INT_MAX__,15,5,
    10,15,__INT_MAX__,5,
    5,5,5,__INT_MAX__
};  //权重矩阵



int x[n]; //路径序列
int x_best[n]; //最短路径序列
int cv = 0; //当前最短路径长度
int v_best= __INT_MAX__; //最短路径长度
unordered_map<int, int> used_map = {
    //初始化第一个城市
    {0,1},
    
    {1,0},
    {2,0},
    {3,0},
}; 

// 递归回溯
void TspDFS_1(int i){
    if (i == n) {
        cv = cv + w[x[n-1]][0];
        if(cv < v_best) {
            v_best = cv;
            for (int j = 0; j < n;j++){
                x_best[j] = x[j];
            }
        }
        cv = cv - w[x[n-1]][0];
    }

    for (int u = 0; u < n; u++){
        // 该城市还没经过
        if (used_map[u] == 0) {
            if (cv + w[x[i-1]][u] < v_best) {
                x[i] = u;

                cout << "x:" ;
                for (int j = 0; j <= i;j++){
                    cout << x[j] + 1 << ' ';
                }
                cout << endl;

                cv = cv + w[x[i-1]][u];
                
                used_map[u] = 1;

                // cout << "map:" ;
                // for (int j = 0; j <= 3;j++){
                //     cout << used_map[j] << ' ';
                // }
                // cout << endl;

                // cout << "cv:" ;
                // cout << cv << endl;

                TspDFS_1(i+1);

                // 回溯
                x[i] = 0;
                cv = cv - w[x[i-1]][u];
                used_map[u] = 0;

            }
        }
    }

}




int main() {
    //初始化第一个城市
    x[0] = 0;

    TspDFS_1(1);



    cout << "v_best:";
    cout << v_best << endl;
    cout << "x_best:";
    for (int i = 0; i < n; i++){
        cout << x_best[i] + 1 << '-';
    }
    cout << 1 << endl;
}