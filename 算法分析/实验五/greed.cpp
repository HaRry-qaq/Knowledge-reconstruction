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
// 保存贪婪结果
int greed_res = 0;

// 设置矿石数组
void set_up_pyramid(){
    // 是否设置随机种子
    srand(int(time(0)));
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= i; j++){
            a[i][j] = rand() % 10;
            cout << a[i][j] << '\t';
        }
        cout << endl;
    }
}

void greedy_max_gold(int line_index, int column_index){
    // 如果在顶部
    if (line_index == 1) {
        greed_res += a[1][1];
    }
    // 如果达到底部
    else if (line_index == n) {
        return ;
    }

    // 比较下面两个矿石 走向更大的一边
    if (a[line_index + 1][column_index] >= a[line_index + 1][column_index + 1]){
        greed_res += a[line_index + 1][column_index];
        cout << greed_res <<endl;
        greedy_max_gold(line_index + 1, column_index);
    }
    else {
        greed_res += a[line_index + 1][column_index + 1];
        cout << greed_res <<endl;
        greedy_max_gold(line_index + 1, column_index + 1);
    }
}



int main(){
    set_up_pyramid();
    system_clock::time_point start2 = system_clock::now();
    greedy_max_gold(1, 1);
    system_clock::time_point end2 = system_clock::now();
    cout << "max_gold:" << greed_res << endl;
    cout << "running time:" << double(duration_cast<microseconds>(end2 - start2).count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;
}