#include<iostream>
#include<cmath>
using namespace std;

int sign(long long int num){
    if (num > 0){return 1;}
    else {return -1;}
}


long long int integersMultiplication(long long int x, long long int y, long long int len_x, long long int len_y){
    if (x == 0 || y == 0){
        return 0;
    }

    else if (len_x == 1 || len_y == 1){
        return x * y;
    }
    else{
        // 推导：
        // x = a * 10^(len_x / 2) + b        y = c * 10^(len_y / 2) + d
        // xy = ac * 10^(len_x / 2 + len_y / 2) + (ad * 10^(len_x / 2) + bc * 10^(len_y / 2)) + bd
        // F1 = ac  F2 = bd  F3 = (a * 10^(len_x / 2) - b) * (d - c * 10^(len_y / 2))
        // 可化简为
        // xy = F1 * 10^(len_x / 2 + len_y / 2) + (F3 + F1 * 10^(len_x / 2 + len_y / 2) + F2) +F2

        int len_b = len_x / 2;
        int len_a = len_x - len_b;
        int len_d = len_y / 2;
        int len_c = len_y -len_d;

        long long int a = (long long int)(x / pow(10, len_b));
        // 带符号，不用再多考虑符号问题
        long long int b = (long long int)(x % (long long int)pow(10, len_b));
        long long int c = (long long int)(y / pow(10, len_d));
        // 带符号，不用再多考虑符号问题
        long long int d = (long long int)(y % (long long int)pow(10, len_d));
        long long int F1 = integersMultiplication(a, c, len_a, len_c);
        long long int F2 = integersMultiplication(b, d, len_b, len_d);
        long long int F3 = integersMultiplication((long long int)(a * pow(10, len_b) - b), (long long int)(d - c * pow(10, len_d)), len_b, len_d);
    
        return (long long int)(F1 * pow(10, len_b + len_d) + (F3 + F1 * pow(10, len_b + len_d) + F2) + F2);
    
    }



}

int main(){
    // 输入数据
    long long int x = 98765;
    long long int y = -6800;
    cout << integersMultiplication(x, y, 5, 4) << endl;
    cout << "truth:" << x * y << endl;
}