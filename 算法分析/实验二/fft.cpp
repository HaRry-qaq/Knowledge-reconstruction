#include<iostream>
#include<complex>
using namespace std;

// 最大位数
const int MAX = 1 << 20;
// pi
const double pi = acos(-1);

// 定义复数系数数组
complex<double> a[MAX], b[MAX];
// 定义a,b多项式长度，这里默认相同，为n（与分治法对齐）
int n;
int m;
// 注意到fft与ifft区别仅有一个符号，所以写在一个函数内,使用i进行判别：i=1时为fft，i=-1时为ifft
void fft_or_ifft(int len, complex<double> *a, int i){
    // 如果只有一项，则不需要再拆分，返回
    if (len <= 1){return ;}
    int mid = len >> 1;
    // 定义奇数、偶数子数组
    complex<double>* A1 = new complex<double>[mid + 1];
    complex<double>* A2 = new complex<double>[mid + 1];
    for (int i = 0; i <= len; i += 2){
        A1[i >> 1] = a[i];
        A2[i >> 1] = a[i + 1];
    }
    fft_or_ifft(mid, A1, i);
    fft_or_ifft(mid, A2, i);
    complex<double> w1(cos(pi / mid), i * sin(pi / mid));
    complex<double> w(1,0);
    complex<double> x;
    for (int i = 0; i < mid; i++){
        x = w * A2[i];
        a[i] = A1[i] + x;
        a[i + mid] = A1[i] - x;
        w = w * w1;
    }
}

int main(){
    // 输入最高位
    cin >> n;
    cin >> m;
    // 输入第一个式子
    for (int i = 0; i <= n; i++){
        double x;
        cin >> x;
        a[i].real(x);
    }
    // 输入第二个式子
    for (int i = 0; i <= m; i++){
        double x;
        cin >> x;
        b[i].real(x);
    }

    // fft需要2的整数幂次; 默认两条式子长度相同
    int len = 1 << int(fmax((int)ceil(log2(n + m)), 1));
    fft_or_ifft(len, a, 1);
    fft_or_ifft(len, b, 1);
    for (int i = 0; i <= len; i++){
        a[i] = a[i] * b[i];
    }
    cout << endl;
    fft_or_ifft(len, a, -1);


    int* res = new int[n + m + 1];
    res[0] = 0;
    // fft之后的结果复制到res
    for (int i = 0; i <=n + m; i++){
        cout << a[i].real() / len + 1e-6 <<" ";
        res[i + 1] = a[i].real() / len + 1e-6;
    }
    cout << endl;

    // 处理
    for (int i = n + m + 1; i > 0; i--){
        if (res[i] >= 10){
            int num = res[i] / 10;
            // 防止最高位还有进位
            res[i] = res[i] % 10;
            res[i - 1] += num;
        }
    }

    cout << endl;
    // for ( int i = 0; i <= n + m + 1;i++){
    //     cout << res[i] << ' ';
    // }

    for (int i = 0 ; i <= n + m + 1; i++){
        if (i == 0 && res[i] == 0){continue;}
        cout << res[i];
    }
}

// 1 2 4 5
// 1 2 3 4
//            4 8 16 20
//         3 6 12 15
//       2 4 8 10
//     1 2 4 5       
//     ------------------
//           1 5 3 6 3 3 0