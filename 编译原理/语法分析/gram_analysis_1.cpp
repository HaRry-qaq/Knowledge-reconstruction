#include<iostream>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<fstream>
#include <iomanip>
#include <unordered_map>
using namespace std;

// 方法一  本质：从左到右递归遍历求解

// 设置一个标记，如果有任何一个地方与产生式不符则置0
int flag = 1;

// 接收字符串
char str[100000];
// 指针
int p = 0;

void handle_F();
void handle_Q();
void handle_T();
void handle_P();
void handle_E();




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


// 产生式
char *production[] = {
    "E->TP",
    "P->+TP",
    "P->-TP",
    "P->e",
    "T->FQ",
    "Q->*FQ",
    "Q->/FQ",
    "Q->e",
    "F->(E)",
    "F->n",
};

void print_str(){
    cout << "left char: ";
    for(int i = p; ;){
        if(str[i] != '$'){
            cout << str[i];
            i++;
        }
        else
            break; 
    }
    cout << endl;
    cout << endl;
}


void handle_F(){
    if (str[p] == '(') {
        cout << "F->(E)" << endl;
        print_str();
        p++;
        handle_E();
        if (str[p] == ')') {
            p++;
        }
        
        else {
            flag = 0;
        }
    }

    else if (str[p] >= '0' && str[p] <= '9') {
        cout << "F->num" << endl;
        print_str();
        p++;
    }

    else {
        flag = 0;
    }
}

void handle_Q(){
    if (str[p] == '*') {
        cout << "Q->*FQ" << endl;
        print_str();
        p++;
        handle_F();
        handle_Q();
    }

    else if (str[p] == '/') {
        cout << "Q->/FQ" << endl;
        print_str();
        p++;
        handle_F();
        handle_Q();
    }
    else {
        cout << "Q->e" << endl;
        print_str();
    }
}


void handle_T(){
    cout << "T->FQ" <<endl;
    print_str();
    handle_F();
    handle_Q();
}

void handle_P(){
    if (str[p] == '+') {
        cout << "P->+TP" << endl;
        print_str();
        p++;
        handle_T();
        handle_P();
    }

    else if (str[p] == '-') {
        cout << "P->-TP" << endl;
        print_str();
        p++;
        handle_T();
        handle_P();
    }
    else {
        cout << "P->e" << endl;
        print_str();
    }
}


void handle_E(){
    cout << "E->TP" << endl;
    print_str();
    handle_T();
    handle_P();
}

void is_correct(){
    if (flag == 1 && str[p] == '$')
        cout << "the sentence is correct for the grammar." << endl;
    else
        cout << "the sentence is wrong for the grammar." << endl;
}

int main(){
    input_str();
    cout << endl;
    cout << "start analysing:" << endl;
    handle_E();
    is_correct();
}