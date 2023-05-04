#include<iostream>
#include<ctime>
#include <cstdlib>
#include<chrono>
using namespace std;
using namespace std::chrono;


// 定义深度
const int n = 10;
// 定义探测器的深度 (x < n)   只有蒙图版需要
const int x = 3;

// 定义矿石数组
int a[n + 1][n + 1];
// 定义路径数组
int r[n + 1][n + 1];
// 定义从第i层第j块出发到出口能获得的最大钻石价值
int A[n + 1][n + 1];

// 保存贪婪结果
int greed_res = 0;



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


void cover_graph_max_gold(int line_index, int column_index){
    if (line_index == 1){
        greed_res += a[1][1];
        cout << greed_res << endl;
    }
    // 如果达到底部
    else if (line_index >= n) {
        return ;
    }

    // 有可能剩下的深度小于x
    int size;
    if (line_index + x <= n) size = x;
    else size = n - line_index;


    for(int i = line_index + size; i >= line_index; i--){
        for(int j = column_index; j <= column_index + size; j++){
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
                    if(i != line_index)
                        A[i][j] = a[i][j] + A[i + 1][j];
                    else
                        A[i][j] = A[i + 1][j];
                    r[i][j] = j;
                }

                // 往右下走
                else if (A[i + 1][j] < A[i + 1][j + 1]){
                    if(i != line_index)
                        A[i][j] = a[i][j] + A[i + 1][j + 1];
                    else
                        A[i][j] = A[i + 1][j + 1];
                    r[i][j] = j + 1;
                }
            }
        }
    }





    cout << "best way: " << a[line_index][column_index];
    for (int i = line_index, j = column_index; i < line_index + size; i++){
        j = r[i][j];
        cout << "-" << a[i + 1][j];
    }
    cout << endl;






    greed_res += A[line_index][column_index];
    cout << A[line_index][column_index] <<endl;

    // 寻找next_column_index
    int next_column_index = column_index;
    for (int i = line_index; r[i][next_column_index] != 0; i++){
        next_column_index = r[i][next_column_index];
    }
    cout << "next_line_index:" << line_index + size <<endl;
    cout << "next_column_index:" << next_column_index <<endl;
    cover_graph_max_gold(line_index + size, next_column_index);
}







int main() {
    set_up_pyramid();
    system_clock::time_point start2 = system_clock::now();
    cover_graph_max_gold(1, 1);
    system_clock::time_point end2 = system_clock::now();
    // cout << endl;
    // for (int i = 1; i <= n; i++){
    //     for (int j = 1; j <= i; j++){
    //         cout << A[i][j] << '\t';
    //     }
    //     cout << endl;
    // }
    cout << "max_gold:" << greed_res << endl;
    cout << "running time:" << double(duration_cast<microseconds>(end2 - start2).count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;
}





