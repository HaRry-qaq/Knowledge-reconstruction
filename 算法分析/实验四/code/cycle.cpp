#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;



void cycle(int n){
    // 平行线上的数
    int line_left;
    int line_right;

    // 多边形的边数
    int angle_num;
    // 奇数则不变，中间为虚拟对手（轮空）
    // 偶数则减1，将编号最大的选手放在多边形中心，与多边形最顶上的点的选手比赛
    if (n % 2 == 1){
        angle_num = n;
    }
    else{
        angle_num = n - 1;
    }
    // 对顶点进行轮转
    for (int i = 1;i <= angle_num; i++){
        cout << "day" << i << ":" << endl;
        // 如果选手个数为奇数，则必然每一天都会有人轮空
        // 我们选取最顶上的这个顶点轮空
        if (n % 2 == 1){
            cout << "competitor " << i << " have a bye" << endl;
        }
        // 如果选手个数为偶数，则最顶上的顶点和中间得选手比赛
        else{
            cout << i << " vs " << n << endl;
        }
        
        // 计算平行线左边和右边的选手(第一组)
        line_left = (i-1) % angle_num ? (i-1) % angle_num : angle_num;
        line_right = (i+1) % angle_num ? (i+1) % angle_num : angle_num;
        cout << line_left << " vs " << line_right << endl;
        
        // 遍历所有平行线
        for (int j = 1; j <= angle_num / 2 - 1; j++){
            line_left = (line_left-1) % angle_num ? (line_left-1) % angle_num : angle_num;
            line_right = (line_right+1) % angle_num ? (line_right+1) % angle_num : angle_num;
            cout << line_left << " vs " << line_right << endl;
        }

    }


}


int main()
{
    int num;
    cout << "input competitor num" << endl;
    cin >> num;
    cycle(num);
}