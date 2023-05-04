#include<iostream>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<fstream>
#include <iomanip>
#include <unordered_map>
#include<typeinfo>
#include <stdio.h>
using namespace std;


// 接收字符串
char str[100000];
int ip = 0;

// 状态栈
int state_stack[10000];
int state_stack_p = 0;

// 符号栈
char sign_stack[10000];
int sign_stack_p = 0;


// 文法分析表
// 建立map
unordered_map<char, int> map{
    {'(', 0},
    {')', 1},
    {'+', 2},
    {'-', 3},
    {'*', 4},
    {'/', 5},
    {'n', 6},
    {'$', 7},

    {'S', 0},
    {'E', 1},
    {'T', 2},
    {'F', 3},
};




// S为正数 R为负数 acc为100 -100为err
int ACTION[16][8] = {
    {4,-100,-100,-100,-100,-100,5,-100},
    {-100,-100,6,7,-100,-100,-100,100},
    {-100,-4,-4,-4,8,9,-100,-4},
    {-100,-7,-7,-7,-7,-7,-100,-7},
    {4,-100,-100,-100,-100,-100,5,-100},
    {-100,-9,-9,-9,-9,-9,-100,-9},
    {4,-100,-100,-100,-100,-100,5,-100},
    {4,-100,-100,-100,-100,-100,5,-100},
    {4,-100,-100,-100,-100,-100,5,-100},
    {4,-100,-100,-100,-100,-100,5,-100},
    {-100,15,6,7,-100,-100,-100,-100},
    {-100,-2,-2,-2,8,9,-100,-2},
    {-100,-3,-3,-3,8,9,-100,-3},
    {-100,-5,-5,-5,-5,-5,-100,-5},
    {-100,-6,-6,-6,-6,-6,-100,-6},
    {-100,-8,-8,-8,-8,-8,-100,-8},
};

// -100为err
int GOTO[16][4] = {
    {-100,1,2,3},
    {-100,-100,-100,-100},
    {-100,-100,-100,-100},
    {-100,-100,-100,-100},
    {-100,10,2,3},
    {-100,-100,-100,-100},
    {-100,-100,11,3},
    {-100,-100,12,3},
    {-100,-100,-100,13},
    {-100,-100,-100,14},
    {-100,-100,-100,-100},
    {-100,-100,-100,-100},
    {-100,-100,-100,-100},
    {-100,-100,-100,-100},
    {-100,-100,-100,-100},
    {-100,-100,-100,-100},
};






// 产生式 LR
// 因为E在多个产生式的左部出现，接受项目不唯一，所以需要增加S->E
char *production[] = {
    "S->E",
    "E->E+T",
    "E->E-T",
    "E->T",
    "T->T*F",
    "T->T/F",
    "T->F",
    "F->(E)",
    "F->n",
};



void set_up_stack(){
    state_stack[0] = 0;
    sign_stack[0] = '$';
}




void input_str(){
    cout << "plz input string for analysis:" << endl;
    int q = 0;
    char c;
    while (1) {
        if ((c = getchar()) == '\n'){
            str[q++] = '$';
            str[q++] = '\0';
            break;
        }
        str[q++] = c;
    }
}



void LR_analisis(){
    while (1) {
        char str_ip = str[ip];
        if (str_ip >= '0' && str_ip <= '9') str_ip = 'n'; 
        
        // log
        // cout << "ACTION:";
        // cout <<ACTION[ state_stack[state_stack_p] ][ map[str_ip] ] << endl;

        // cout << "state_p:";
        // cout << state_stack_p << endl;

        // cout << "ip:";
        // cout << ip << endl;

        cout << "state stack:";
        for (int i = 0;i <= state_stack_p; i++) cout << state_stack[i] << ' ';
        cout << endl;

        cout << "sign stack:";
        for (int i = 0;i <= sign_stack_p; i++) cout << sign_stack[i];
        cout << endl;

        cout << "the rest:";
        int temp_ip = ip;
        while (str[temp_ip] != '$') {
            cout << str[temp_ip];
            temp_ip++;
        }
        cout << endl;



        


        // 如果是S
        if (ACTION[ state_stack[state_stack_p] ][ map[str_ip] ] > 0 && ACTION[ state_stack[state_stack_p] ][ map[str_ip] ] != 100) {
            cout << "S" << ACTION[ state_stack[state_stack_p] ][ map[str_ip] ] << endl;
            

            // 将状态加入

            state_stack[state_stack_p + 1] = ACTION[ state_stack[state_stack_p] ][ map[str_ip] ];

            state_stack_p++;

            // 将符号加入
            sign_stack[++sign_stack_p] = str[ip];
            // 推进ip
            ip++;

            cout << endl;
            cout << endl;
        }

        // 如果是R
        else if (ACTION[ state_stack[state_stack_p] ][ map[str_ip] ] < 0 && ACTION[ state_stack[state_stack_p] ][ map[str_ip] ] != -100) {
            cout << "R" << abs(ACTION[ state_stack[state_stack_p] ][ map[str_ip] ]) << '\t';
            // 绝对值的下标从1开始，所以我们需要先取绝对值再减一得到对应production数组的下标
            int pro_p = abs(ACTION[ state_stack[state_stack_p] ][ map[str_ip] ]) - 1;

            // 标记到箭头右边
            int right_flag = 0;
            // 计数
            int count = 0;
            for (int i = 0; ; i++) {
                if (production[pro_p][i] == '>') {
                    right_flag = 1;
                    continue;
                }
                else if (production[pro_p][i] == '\0') {
                    break;
                }
                else if (right_flag == 1) {
                    count++;
                }
            }
            // 两个栈分别弹出count个符号
            state_stack_p -= count;
            sign_stack_p -= count;

            // 将当前状态的goto压入状态栈栈顶， 将产生式左边的非终结符压入符号栈栈顶
            // 将状态加入

            state_stack[state_stack_p + 1] = GOTO[ state_stack[state_stack_p] ][ map[production[pro_p][0]] ];
            state_stack_p++;
            sign_stack[++sign_stack_p] = production[pro_p][0];
            
            // 输出
            cout << production[pro_p] << endl;

            cout << endl;
        }
        // 如果是acc
        else if (ACTION[ state_stack[state_stack_p] ][ map[str_ip] ] == 100) {
            cout << "accept!" << endl;
            return ;
        }

        else if (ACTION[ state_stack[state_stack_p] ][ map[str_ip] ] == -100) {
            cout << "error!" << endl;
            return ;
        }
    }
}

int main(){
    set_up_stack();
    input_str();
    LR_analisis();
}