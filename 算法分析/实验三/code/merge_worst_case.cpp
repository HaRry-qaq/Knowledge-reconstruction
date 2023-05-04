// 生成归并排序最差情况的数据集

#include<iostream>
#include<chrono>
#include<fstream>
#include<string>
using namespace std;


int N = 1000;
int* res = new int [N];
int res_p = 0;

void setUpRes(int* arr, int len){
    if (len == 1){
        res[res_p] = arr[0];
        res_p++;
        return;
    }


    int left_count = 0;
    int right_count = 0;
    for (int i = 0; i < len; i++){
        if (i % 2 == 0){
            left_count ++;
        }
        else{
            right_count ++;
        }
    }
    // cout << left_count <<endl;
    // cout << right_count <<endl;
    int* leftarr = new int[left_count];
    int* rightarr = new int[right_count];
    int left_p = 0;
    int right_p = 0;
    for (int i = 0; i < len; i++){
        if (i % 2 == 0){
            // cout << arr[i] << endl;
            leftarr[left_p] = arr[i];
            left_p ++;
        }
        else{
            rightarr[right_p] = arr[i];
            right_p ++;
        }
    }
    // for (int i = 0; i < left_count; i ++){
    //     cout << leftarr[i] << ' ';
    // }

    setUpRes(leftarr, left_count);
    setUpRes(rightarr, right_count);
}


int main(){


    // 先生成长度为N的顺序数组
    int* arr = new int[N];
    for (int i = 1; i <= N; i++){
        arr[i-1] = i;
    }

    // for (int i = 0; i < N; i++){
    //     cout << arr[i] << ' ';
    // }
    // cout << endl;


    // 构造数组
    setUpRes(arr, N);

    // for (int i = 0; i < N; i++){
    //     cout << res[i] << ' ';
    // }

    fstream ofs;
    ofs.open("../data/merge_worst_1000.txt", ios::out);

    for (int i = 0; i < N; i++){
        ofs << res[i] << ' ';
    }


}