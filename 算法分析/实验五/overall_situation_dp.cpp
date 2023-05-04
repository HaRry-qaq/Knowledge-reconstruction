#include<iostream>
#include<ctime>
#include <cstdlib>
#include<chrono>
using namespace std;
using namespace std::chrono;


// 定义深度
const int n = 10;
// 定义探测器的深度 (x < n)   只有蒙图版需要
// const int x = 3;


// 定义矿石数组
int a[n + 1][n + 1];
// 定义从第i层第j块出发到出口能获得的最大钻石价值
int A[n + 1][n + 1];
// 定义路径数组
int r[n + 1][n + 1];

// 设置矿石数组
void set_up_pyramid(){
    // 是否设置随机种子
    // srand(int(time(0)));
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= i; j++){
            a[i][j] = rand() % 10;
            cout << a[i][j] << '\t';
        }
        cout << endl;
    }
}

// dp
int dp_max_gold(){
    // 从下向上计算
    for (int i = n; i >= 1; i--){
        // 从左到右计算
        for (int j = 1; j <= i ; j++){
            // 初始化最后一行
            if (i == n){
                // 赋值
                A[i][j] = a[i][j];
                r[i][j] = 0;
            }

            // 确定往左下走或往右下走
            else {
                // 往左下走
                if (A[i + 1][j] >= A[i + 1][j + 1]){
                    A[i][j] = a[i][j] + A[i + 1][j];
                    r[i][j] = j;
                }

                // 往右下走
                else if (A[i + 1][j] < A[i + 1][j + 1]){
                    A[i][j] = a[i][j] + A[i + 1][j + 1];
                    r[i][j] = j + 1;
                }
            }
        }
    }
    return A[1][1];
}

// 输出最佳路径
void get_best_way(){
    cout << "best way: " << a[1][1];
    for (int i = 1, j = 1; r[i][j] != 0; i++){
        j = r[i][j];
        cout << "-" << a[i + 1][j];
    }
    cout << endl;
}


int main(){
    set_up_pyramid();
    system_clock::time_point start2 = system_clock::now();
    int res = dp_max_gold();
    system_clock::time_point end2 = system_clock::now();
    get_best_way();
    cout << "max_gold:" << res << endl;
    cout << "running time:" << double(duration_cast<microseconds>(end2 - start2).count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;
}
