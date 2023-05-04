// 生成顺序数据集

#include<iostream>
#include<chrono>
#include<fstream>
#include<string>
using namespace std;

int main(){
    // 按照规模修改N
    int N = 10000;
    int* arr = new int[N];

    for (int i = 0;i < N;i++){
		// [a,b] rand() % (b - a + 1) + a
		// [-100,100]
		arr[i] = rand() % (100 + 100 + 1) - 100;
		// cout << arr[i] << " ";
	}

    fstream ofs;
    ofs.open("../data/rand_10000_test.txt", ios::out);

    for (int i = 0; i < N; i++){
        ofs << arr[i] << ' ';
    }
}