#include<iostream>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<fstream>
#include <iomanip>
#include <unordered_map>
using namespace std;


// 产生式 LL(1)
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

// 终结符
char *terminator[] = {
    "+",
    "-",
    "*",
    "/",
    "(",
    ")",
    "n",
};

// F的first集合和follow集合
char *first_F[] = {
    "(",
    "n",
};
char *follow_F[] = {
    "*",
    "/",
    "+",
    "-",
    "$",
    ")",
};

// T的first集合和follow集合
char *first_T[] = {
    "(",
    "n",
};
char *follow_T[] = {
    "+",
    "-",
    "$",
    ")",
};

// E的first集合和follow集合
char *first_E[] = {
    "(",
    "n",
};
char *follow_E[] = {
    ")",
    "$",
};

// P的first集合和follow集合
char *first_P[] = {
    "+",
    "-",
    "e",
};
char *follow_P[] = {
    ")",
    "$",
};

// Q的first集合和follow集合
char *first_Q[] = {
    "*",
    "/",
    "e",
};
char *follow_Q[] = {
    "+",
    "-",
    ")",
    "$",
};


// 三维数组最后一维的指针
int p[6][9] = {
    {0,0,0,0,0,0,0,0,0},
    {0,-1,-1,-1,-1,-1,-1,-1,-1},
    {0,-1,-1,-1,-1,-1,-1,-1,-1},
    {0,-1,-1,-1,-1,-1,-1,-1,-1},
    {0,-1,-1,-1,-1,-1,-1,-1,-1},
    {0,-1,-1,-1,-1,-1,-1,-1,-1},
};



// table
string table[100][100][100];
/*
        表格式：
            +    -    *    /    (    )    n    $
        E
        T
        F
        P
        Q
*/

// 实现char *的substr
char *sub_str(char *s,int n,int len)
{
	static char p[20];//或者设置为静态变量 
	int i, j = 0;
	while (n--) {
		s++;//确定字符串的首位置 
	}
	for(i = n; i < n + len; i++){
		 p[j++] = *s;
		 s++;
	} 
	return p;
} 

void set_table(){
    table[0][1][0] = "+";
    table[0][2][0] = "-";
    table[0][3][0] = "*";
    table[0][4][0] = "/";
    table[0][5][0] = "(";
    table[0][6][0] = ")";
    table[0][7][0] = "n";
    table[0][8][0] = "$";

    table[1][0][0] = "E";
    table[2][0][0] = "T";
    table[3][0][0] = "F";
    table[4][0][0] = "P";
    table[5][0][0] = "Q";

}

void print_table(){
    for (int i = 1; i <= 5; i++){
        for (int j = 1; j <= 8; j++){

            char left;
            char right;
            switch (i)
            {
                case 1:
                {
                    left = 'E';
                    break;
                }
                case 2:
                {
                    left = 'T';
                    break;
                }
                case 3:
                {
                    left = 'F';
                    break;
                }
                case 4:
                {
                    left = 'P';
                    break;
                }
                case 5:
                {
                    left = 'Q';
                    break;
                }
            }
            switch (j)
            {
                case 1:
                {
                    right = '+';
                    break;
                }
                case 2:
                {
                    right = '-';
                    break;
                }
                case 3:
                {
                    right = '*';
                    break;
                }
                case 4:
                {
                    right = '/';
                    break;
                }
                
                case 5:
                {
                    right = '(';
                    break;
                }
                case 6:
                {
                    right = ')';
                    break;
                }
                case 7:
                {
                    right = 'n';
                    break;
                }
                case 8:
                {
                    right = '$';
                    break;
                }
            }
            cout << "(" << left << "," << right << "): ";
            // if (p[i][j] == -1) cout << "error";
            for (int k = 0; k <= p[i][j]; k++) {
                cout << table[i][j][k] << " ";
            }
            cout << endl;
        }
    }
}


void predict_analysis_table(){
    for (int i = 0; i <= 9; i++){
        char *pro = production[i];
        // first集合
        char *first_set[100];
        int top_first_set = 0;

        // 由于产生式格式一样，直接将指针往右移动三位得到产生式右边的字符串
        pro += 3;

        // 如果第一个为终结符，那么first(α)就是该终结符

        // 构造α的first集合
        switch (pro[0])
        {
            // 如果是终结符
            case '+':
            case '-':
            case '*':
            case '/':
            case '(':
            case ')':
            case 'n':
            case 'e':
            {
                first_set[top_first_set++] = sub_str(pro, 0, 1);
                break;
            }


            // 如果是非终结符
            case 'F':
            {
                for (int i = 0; i < 2; i++){
                    first_set[top_first_set++] = first_F[i];
                }
                break;
            }

            case 'T':
            {
                for (int i = 0; i < 2; i++){
                    first_set[top_first_set++] = first_T[i];
                }
                break;
            }

            case 'E':
            {
                for (int i = 0; i < 2; i++){
                    first_set[top_first_set++] = first_E[i];
                }
                break;
            }

            case 'P':
            {
                for (int i = 0; i < 3; i++){
                    first_set[top_first_set++] = first_P[i];
                }
                break;
            }

            case 'Q':
            {
                for (int i = 0; i < 3; i++){
                    first_set[top_first_set++] = first_Q[i];
                }
                break;
            }
        }





        // 指针倒回去 构造第一种情况
        pro -= 3;
        for (int i = 0; i < top_first_set; i++)
        {
            // cout << pro << endl;
            switch (pro[0])
            {
                case 'E':
                {
                    switch (first_set[i][0])
                    {
                        case '+':
                        {
                            table[1][1][++p[1][1]] = pro;
                            break;
                        }

                        case '-':
                        {
                            table[1][2][++p[1][2]] = pro;
                            break;
                        }

                        case '*':
                        {
                            table[1][3][++p[1][3]] = pro;
                            break;
                        }

                        case '/':
                        {
                            table[1][4][++p[1][4]] = pro;
                            break;
                        }

                        case '(':
                        {
                            table[1][5][++p[1][5]] = pro;
                            break;
                        }

                        case ')':
                        {
                            table[1][6][++p[1][6]] = pro;
                            break;
                        }

                        case 'n':
                        {
                            table[1][7][++p[1][7]] = pro;
                            break;
                        }

                        case '$':
                        {
                            table[1][8][++p[1][8]] = pro;
                            break;
                        }
                    }
                    break;
                }

                case 'T':
                {
                    switch (first_set[i][0])
                    {
                        case '+':
                        {
                            table[2][1][++p[2][1]] = pro;
                            break;
                        }

                        case '-':
                        {
                            table[2][2][++p[2][2]] = pro;
                            break;
                        }

                        case '*':
                        {
                            table[2][3][++p[2][3]] = pro;
                            break;
                        }

                        case '/':
                        {
                            table[2][4][++p[2][4]] = pro;
                            break;
                        }

                        case '(':
                        {
                            table[2][5][++p[2][5]] = pro;
                            break;
                        }

                        case ')':
                        {
                            table[2][6][++p[2][6]] = pro;
                            break;
                        }

                        case 'n':
                        {
                            table[2][7][++p[2][7]] = pro;
                            break;
                        }

                        case '$':
                        {
                            table[2][8][++p[2][8]] = pro;
                            break;
                        }
                    }
                    break;
                }

                case 'F':
                {
                    switch (first_set[i][0])
                    {
                        case '+':
                        {
                            table[3][1][++p[3][1]] = pro;
                            break;
                        }

                        case '-':
                        {
                            table[3][2][++p[3][2]] = pro;
                            break;
                        }

                        case '*':
                        {
                            table[3][3][++p[3][3]] = pro;
                            break;
                        }

                        case '/':
                        {
                            table[3][4][++p[3][4]] = pro;
                            break;
                        }

                        case '(':
                        {
                            table[3][5][++p[3][5]] = pro;
                            break;
                        }

                        case ')':
                        {
                            table[3][6][++p[3][6]] = pro;
                            break;
                        }

                        case 'n':
                        {
                            table[3][7][++p[3][7]] = pro;
                            break;
                        }

                        case '$':
                        {
                            table[3][8][++p[3][8]] = pro;
                            break;
                        }
                    }
                    break;
                }

                case 'P':
                {
                    switch (first_set[i][0])
                    {
                        case '+':
                        {
                            table[4][1][++p[4][1]] = pro;
                            break;
                        }

                        case '-':
                        {
                            table[4][2][++p[4][2]] = pro;
                            break;
                        }

                        case '*':
                        {
                            table[4][3][++p[4][3]] = pro;
                            break;
                        }

                        case '/':
                        {
                            table[4][4][++p[4][4]] = pro;
                            break;
                        }

                        case '(':
                        {
                            table[4][5][++p[4][5]] = pro;
                            break;
                        }

                        case ')':
                        {
                            table[4][6][++p[4][6]] = pro;
                            break;
                        }

                        case 'n':
                        {
                            table[4][7][++p[4][7]] = pro;
                            break;
                        }

                        case '$':
                        {
                            table[4][8][++p[4][8]] = pro;
                            break;
                        }
                    }
                    break;
                }

                case 'Q':
                {
                    switch (first_set[i][0])
                    {
                        case '+':
                        {
                            table[5][1][++p[5][1]] = pro;
                            break;
                        }

                        case '-':
                        {
                            table[5][2][++p[5][2]] = pro;
                            break;
                        }

                        case '*':
                        {
                            table[5][3][++p[5][3]] = pro;
                            break;
                        }

                        case '/':
                        {
                            table[5][4][++p[5][4]] = pro;
                            break;
                        }

                        case '(':
                        {
                            table[5][5][++p[5][5]] = pro;
                            break;
                        }

                        case ')':
                        {
                            table[5][6][++p[5][6]] = pro;
                            break;
                        }

                        case 'n':
                        {
                            table[5][7][++p[5][7]] = pro;
                            break;
                        }

                        case '$':
                        {
                            table[5][8][++p[5][8]] = pro;
                            break;
                        }
                    }
                    break;
                }

            }
        }
        

        // 构造第二种情况
        // 判断是否存在 e ∈ FIRST(α)
        bool empty_flag = 0;
        for (int i = 0; i < top_first_set; i++){
            // cout << first_set[i] << ' ';
            if ('e' == first_set[i][0]) 
            {
                empty_flag = 1;
                break;
            }
        }
        // cout << endl;

        // cout << "empty_flag:" << empty_flag << endl;

        if (empty_flag == 1){
            // 选择pro[0]
            // cout << pro << endl;;
            switch (pro[0])
            {
                case 'E':
                {
                    for (int i = 0; i <= 1; i++){
                        switch (follow_E[i][0])
                        {
                            case '+':
                            {
                                table[1][1][++p[1][1]] = pro;
                                break;
                            }
                            case '-':
                            {
                                table[1][2][++p[1][2]] = pro;
                                break;
                            }
                            case '*':
                            {
                                table[1][3][++p[1][3]] = pro;
                                break;
                            }
                            case '/':
                            {
                                table[1][4][++p[1][4]] = pro;
                                break;
                            }
                            case '(':
                            {
                                table[1][5][++p[1][5]] = pro;
                                break;
                            }
                            case ')':
                            {
                                table[1][6][++p[1][6]] = pro;
                                break;
                            }
                            case 'n':
                            {
                                table[1][7][++p[1][7]] = pro;
                                break;
                            }
                            case '$':
                            {
                                table[1][8][++p[1][8]] = pro;
                                break;
                            }
                            break;
                        }
                    }
                    break;
                }

                case 'T':
                {
                    for (int i = 0; i <= 3; i++){
                        switch (follow_T[i][0])
                        {
                            case '+':
                            {
                                table[2][1][++p[2][1]] = pro;
                                break;
                            }
                            case '-':
                            {
                                table[2][2][++p[2][2]] = pro;
                                break;
                            }
                            case '*':
                            {
                                table[2][3][++p[2][3]] = pro;
                                break;
                            }
                            case '/':
                            {
                                table[2][4][++p[2][4]] = pro;
                                break;
                            }
                            case '(':
                            {
                                table[2][5][++p[2][5]] = pro;
                                break;
                            }
                            case ')':
                            {
                                table[2][6][++p[2][6]] = pro;
                                break;
                            }
                            case 'n':
                            {
                                table[2][7][++p[2][7]] = pro;
                                break;
                            }
                            case '$':
                            {
                                table[2][8][++p[2][8]] = pro;
                                break;
                            }
                            break;
                        }
                    }
                    break;
                }

                case 'F':
                {
                    for (int i = 0; i <= 5; i++){
                        switch (follow_F[i][0])
                        {
                            case '+':
                            {
                                table[3][1][++p[3][1]] = pro;
                                break;
                            }
                            case '-':
                            {
                                table[3][2][++p[3][2]] = pro;
                                break;
                            }
                            case '*':
                            {
                                table[3][3][++p[3][3]] = pro;
                                break;
                            }
                            case '/':
                            {
                                table[3][4][++p[3][4]] = pro;
                                break;
                            }
                            case '(':
                            {
                                table[3][5][++p[3][5]] = pro;
                                break;
                            }
                            case ')':
                            {
                                table[3][6][++p[3][6]] = pro;
                                break;
                            }
                            case 'n':
                            {
                                table[3][7][++p[3][7]] = pro;
                                break;
                            }
                            case '$':
                            {
                                table[3][8][++p[3][8]] = pro;
                                break;
                            }
                            break;
                        }
                    }
                    break;
                }

                case 'P':
                {
                    for (int i = 0; i <= 1; i++){
                        switch (follow_P[i][0])
                        {
                            case '+':
                            {
                                table[4][1][++p[4][1]] = pro;
                                break;
                            }
                            case '-':
                            {
                                table[4][2][++p[4][2]] = pro;
                                break;
                            }
                            case '*':
                            {
                                table[4][3][++p[4][3]] = pro;
                                break;
                            }
                            case '/':
                            {
                                table[4][4][++p[4][4]] = pro;
                                break;
                            }
                            case '(':
                            {
                                table[4][5][++p[4][5]] = pro;
                                break;
                            }
                            case ')':
                            {
                                table[4][6][++p[4][6]] = pro;
                                break;
                            }
                            case 'n':
                            {
                                table[4][7][++p[4][7]] = pro;
                                break;
                            }
                            case '$':
                            {
                                table[4][8][++p[4][8]] = pro;
                                break;
                            }
                            break;
                        }
                    }
                    break;
                }
                
                case 'Q':
                {
                    for (int i = 0; i <= 3; i++){
                        // cout << follow_Q[i][0];
                        switch (follow_Q[i][0])
                        {
                            case '+':
                            {
                                table[5][1][++p[5][1]] = pro;
                                break;
                            }
                            case '-':
                            {
                                table[5][2][++p[5][2]] = pro;
                                break;
                            }
                            case '*':
                            {
                                table[5][3][++p[5][3]] = pro;
                                break;
                            }
                            case '/':
                            {
                                table[5][4][++p[5][4]] = pro;
                                break;
                            }
                            case '(':
                            {
                                table[5][5][++p[5][5]] = pro;
                                break;
                            }
                            case ')':
                            {
                                table[5][6][++p[5][6]] = pro;
                                break;
                            }
                            case 'n':
                            {
                                table[5][7][++p[5][7]] = pro;
                                break;
                            }
                            case '$':
                            {
                                table[5][8][++p[5][8]] = pro;
                                break;
                            }
                            break;
                        }
                        // cout << i;
                    }
                    break;
                }



                default:
                    break;
            }
        }
        

        
    }
}

// 输入数组
char w[10000];
char* input_str(){
    cout << "plz input string for analysis:" << endl;
    int p = 0;
    char c;
    while (1) {
        if ((c = getchar()) == '\n'){
            w[p++] = '\0';
            break;
        }
        w[p++] = c;
    }
    return w;
}
void LL1_prediction(){
    cout << endl;
    cout << "start analysis:" << endl;

    // 建立一个与下标对应的map
    unordered_map<char, int> map{
        {'E', 1},
        {'T', 2},
        {'F', 3},
        {'P', 4},
        {'Q', 5},

        {'+', 1},
        {'-', 2},
        {'*', 3},
        {'/', 4},
        {'(', 5},
        {')', 6},
        {'n', 7},
        {'$', 8},
    };

    // 初始化

    char stack[10000];
    // stack_p指向stack中的栈顶元素
    int stack_p = -1;
    // 将终止符压入栈
    stack[++stack_p] = '$';
    // 将文法开始符号E压入栈
    stack[++stack_p] = 'E';

    char buffer[10000];
    int buffer_p = 0;
    // 将w$放入输入缓冲区
    while (1) {
        if(w[buffer_p] != '\0'){
            buffer[buffer_p] = w[buffer_p];
            buffer_p++;
        }
        else break;
    }
    buffer[buffer_p++] = '$';

    // 设置向前指针指向w$的第一个符号
    int forward = 0;

    // 计数
    int count = 1;

    while (stack[stack_p] != '$') {
        cout << "step" << count << ": " << endl;
        count++;
        // 判断是否为终止符的记号
        int ter_flag = 0;
        for (int i = 0; i <= 6; i++) {
            if (stack[stack_p] == terminator[i][0] || (stack[stack_p]>='0' && stack[stack_p]<='9')){
                ter_flag = 1;
                break;
            }
        }

        // log
        cout << "stack: ";
        for (int i = 0; i <= stack_p; i++) cout << stack[i];
        cout << endl;

        cout << "buffer: ";
        for (int i = forward; i < buffer_p; i++) cout << buffer[i];
        cout << endl;

        
        if (stack[stack_p] == '$' || ter_flag == 1) {
            if (stack[stack_p] == buffer[forward] || (stack[stack_p] == 'n' && (buffer[forward] >= '0' && buffer[forward] <= '9'))) {
                // 弹出X，其实就是stack_p--即可
                stack_p--;
                // forward
                forward++;
            }
            else {
                cout << "error!" << endl;
                return ;
            }
        }

        else {
            // cout << "else" << endl;
            char buffer_forward = buffer[forward];
            // 如果是数字需要将其变为n
            if (buffer[forward] >= '0' && buffer[forward] <= '9') {
                buffer_forward = 'n';
            }

            // 如果有产生式
            if (p[ map[stack[stack_p]] ][ map[buffer_forward] ] != -1) {
                // 坐标
                int x = map[stack[stack_p]];
                int y = map[buffer_forward];
                int z = p[ map[stack[stack_p]] ][ map[buffer_forward] ];
                // 栈顶弹出
                stack_p--;
                // 依次将Y_k Y_k-1 ... Y_1压入栈
                string pro = table[x][y][z];
                for (int i = pro.length() - 1; i >= 0; i--){

                    if (pro[i] == '>') break;
                    else if (pro[i] == 'e') continue;
                    else stack[++stack_p] = pro[i];
                }
                
                // 输出产生式
                cout << "using production: " << pro << endl;
                
            }

            else {
                cout << "error!" << endl;
                return ;
            }
        }
        cout << endl;
    }
    if (buffer[forward] == '$')
        cout << "successful!" << endl;
    else
        cout << "error!" << endl;
}



int main() {
    set_table();
    cout << "analysis_table: " << endl;
    predict_analysis_table();
    print_table();
    cout << endl;
    input_str();
    LL1_prediction();
}