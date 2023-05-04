#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

// 先随便定义一个最大容纳选手人数
const int max_num = 10000;
// 定义结果表，行为天数，列为选手
int res[max_num][max_num];




void deal(int n){
    // 设置参数

    // 确保前半部分长于后半部分
    int m = (int)ceil(n / 2.0);

    // 依据题意，n为偶数时循环赛进行n-1天,n为奇数时为m天
    int days;
    if (n % 2 == 0){
        days = n - 1;
    }
    else{
        days = n;
    }

    // 将天数分为两个部分 先算出前半部分再纵向构造:m为偶数时为m-1，m为奇数时为m
    int passed_days;
    if (m % 2 == 0){
        passed_days = m - 1;
    }
    else{
        passed_days = m;
    }
    
    // 横向构造
    for (int i = 1; i <= m; i++){
        for (int j = 1; j <= passed_days; j++){
            // 若本来有对手（不是虚拟对手，即上一个递归计算得到的子问题答案不为0）
            if (res[i][j] != 0){
                res[i + m][j] = res[i][j] + m;
            }

            // 否则让它们互为对手
            else{
                res[i][j] = i + m;
                res[i + m][j] = i;
            }
        }
    }

    // 注意，如果上述情况出现了虚拟对手的情况（即为奇数），则res[1][passed_days]的数引入了本不属于这个子问题的选手序号m+1.
    // 此时使用公式 rvalue = (count + i - 1) % m + m + 1 会导致 res[1][passed_days] 与 res[1][passed_days + 1]一致
    // 所以需要 + 1 修正： rvalue = (count + fix + i - 1) % m + m + 1   
    // fix的值取决于 res[1][passed_days] 是否为 m+1
    int fix = 0;
    if (res[1][passed_days] == m + 1){
        fix = 1;
    }

    // 纵向构造
    for (int i = 1; i <= m; i++){
        int count = 0;
        for (int j = passed_days + 1;j <= days; j++){
            int rvalue = (count + fix + i - 1) % m + m + 1;
            // 赋值相对应的选手
            res[i][j] = rvalue;
            res[rvalue][j] = i;
            count++;
        }
    }

    // 此时已经构造完毕，但如果n为奇数，是设置了一个虚拟对手的。我们需要将其消除.
    if (n % 2 != 0){
        for (int i = 1; i <= 2 * m; i++)
        {
            for (int j = 1; j <= days; j++)
                if (res[i][j] == n + 1)
                    res[i][j] = 0;
        }
    }
}




void divide(int n){
    // 如果人数小于等于1，没有意义，直接返回
    if (n == 1) return;

    // 如果问题人数为2,直接返回正确答案
    else if (n == 2){
        res[1][1] = 2;
        res[2][1] = 1;
    }

    // 分治
    else{
        // 确保前半部分长于后半部分，可以成功对应赋值，所以使用ceil
        divide((int)ceil(n / 2.0));
        // 根据前半部分横向、纵向构造
        deal(n);
    }
}

void get_res(int n){
    for (int i = 0; i <= n; i++){
        if (i == 0) cout << "pnum:" << ' ';
        else cout << i << ' ';
    }
    cout << endl;
    // cout << endl;


    int days;
    if (n % 2 == 0){
        days = n - 1;
    }
    else{
        days = n;
    }

    for (int j = 1; j <= days; j++){
        cout << "day" << j << ": ";
        for (int i = 1; i <= n; i++){
            cout << res[i][j] << ' ';
        }
        cout << endl;
    }
}


int main(){
    int n;
    cout << "input competitor num" << endl;
    cin >> n;
    divide(n);
    get_res(n);
}