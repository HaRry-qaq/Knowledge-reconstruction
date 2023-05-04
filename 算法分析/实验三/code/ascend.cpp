// 生成顺序数据集

#include<iostream>
#include<chrono>
#include<fstream>
#include<string>
using namespace std;

int main(){
    // 按照规模修改N
    int N = 1000;
    int* arr = new int[N];
    for (int i = 0; i < N; i++){
        arr[i] = i;
    }

    fstream ofs;
    ofs.open("../data/ascend_1000.txt", ios::out);

    for (int i = 0; i < N; i++){
        ofs << arr[i] << ' ';
    }

}