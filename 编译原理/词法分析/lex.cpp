#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<fstream>
using namespace std;

#define MAX_BUFFER_SIZE 10000
#define MAX_STR_SIZE 10000
// 每一行的缓冲区 
char buffer[MAX_BUFFER_SIZE];
// 读取文件中被单引号和双引号引住的字符串 或 一串数字 或 变量
char str_in_file[MAX_STR_SIZE];

// 记录各种数据
// 行数
int line_num = 0;
// 字符总数（包括空格）
int sign_num = 0;
// 单词总数（关键字+变量名）
int word_num = 0;
// 关键字数目
int keyword_num = 0;
// 变量名数目
int varible_num = 0;
// 数字常量数目
int number_num = 0;
// 分界符和操作符数目
int operate_num = 0;
// 字符串常量数目
int str_num = 0;
// 单行注释数目
int single_ann_num = 0;
// 多行注释数目
int multi_ann_num = 0;


// 多行注释标记
int multi_annotation = 0;


// 关键字数组
// C语言关键字共有32个
char *key_word[] = {
    // 数据类型关键字
    "char",
    "short",
    "int",
    "long",
    "signed",
    "unsigned",
    "float",
    "double",
    "struct",
    "union",
    "enum",
    "void",
    // 控制语句关键字
    "for",
    "do",
    "while",
    "break",
    "continue",
    "if",
    "else",
    "goto",
    "switch",
    "case",
    "default",
    "return",
    // 存储类型关键字
    "auto",
    "extern",
    "register",
    "static",
    "typedef",
    // 其他关键字
    "const",
    "sizeof",
    "volatille",
};


 // 获取下一个字符的函数，并让forward指针向后移动一位
char get_char(char *&forward){
    char this_char = *forward;
    forward ++;
    sign_num ++;
    return this_char;
}

// 回退一格
void get_backward(char *&forward){
    forward --;
}

/*
    打印结果
    结构：     行数 <word type， word>
*/
void print(int line_index, char *type, char *str){
    cout << "line " << line_index << " <" << type << ", " << str << ">" << endl;
}

// 是否为数字
bool is_num(char ch){
    if (ch >= '0' && ch <= '9') return true;
    else return false;
}

// 是否为字母
bool is_letter(char ch){
    if ((ch >= 'a' && ch <= 'z' ) || (ch >= 'A' && ch <= 'Z')) return true;
    else return false;
}

// 是否为关键字 一共有32个关键字
bool is_keyword(char *str){
    for (int i = 0; i < 32; i++){
        if(strcmp(str, key_word[i]) == 0){
            // 如果是关键字 返回下标（从1开始）
            return true;
        }
    }
    // 否则返回false
    return false;
}



/*
    词法分析函数，传入forward指针(当前行)和当前行的下标
    该函数参考课本中程序的结构
*/
void lexical_analysis(int line_index, char *forward){
    // 状态初始化
    int state = 0;

    // 因为分行 所以只可能到状态6
    if (multi_annotation == 1){
        state = 6;
    }

    // 定义当前字符
    char now_char = ' ';
    // 标记字符串位置
    int pos = 0;
    // 如果当前还没有到达该行的结束
    while (now_char != '\0') {
        switch (state)
        {
            case 0:
            {
                // 当前字符
                now_char = get_char(forward);

                // 根据第一个字符进行状态的切换
                switch (now_char) {
                    // 空格跳过
                    case ' ':
                    {
                        pos = 0;
                        break;
                    }
                        
                    
                    // 分界符
                    case '(':
                    case ')':
                    case '[':
                    case ']':
                    case '{':
                    case '}':
                    {
                        pos = 0;

                        char op[2];
                        op[0] = now_char;
                        
                        // 分界符和操作符的数量+1
                        operate_num ++;
                        // 输出
                        print(line_index, "delimiter", op);
                        break;
                    }
                    // 单引号和双引号
                    case '\'':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        print(line_index, "delimiter", "\'");

                        // 读取字符串内的内容
                        now_char = get_char(forward);

                        // 记录是否有第二个引号
                        int mark_flag = 1;

                        while ((now_char != '\'')){
                            // 不为终止符则加入
                            if (now_char != '\0'){
                                str_in_file[pos] = now_char;
                                pos ++;
                                now_char = get_char(forward);
                            }
                            // 如果在第二个单引号出现之前出现了终止符号 抛出一个错误
                            else{
                                print(line_index, "error:the str end without another quotation mark", str_in_file);
                                mark_flag = 0;
                                break;
                            }
                        }

                        if (mark_flag == 1) {
                            // 正确生成了字符串 为其加入终结符 避免之前的影响
                            str_in_file[pos] = '\0';
                            // 输出正确内容
                            print(line_index, "string", str_in_file);
                            // 输出第二个引号
                            print(line_index, "delimiter", "\'");
                            // 累计字符串数目
                            str_num ++;
                            // 累计分界符和操作符数目
                            operate_num ++;
                        }

                        // reset
                        pos = 0;
                        break;
                    }
                    // 与单引号一样
                    case '"':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        print(line_index, "delimiter", "\"");
                        
                         // 读取字符串内的内容
                        now_char = get_char(forward);

                        // 记录是否有第二个引号
                        int mark_flag = 1;

                        while ((now_char != '"')){
                            // 不为终止符则加入
                            if (now_char != '\0'){
                                str_in_file[pos] = now_char;
                                pos ++;
                                now_char = get_char(forward);
                            }
                            // 如果在第二个双引号出现之前出现了终止符号 抛出一个错误
                            else{
                                print(line_index, "error:the str end without another quotation mark", str_in_file);
                                mark_flag = 0;
                                break;
                            }
                        }

                        if (mark_flag == 1) {
                            // 正确生成了字符串 为其加入终结符 避免之前的影响
                            str_in_file[pos] = '\0';
                            // 输出正确内容
                            print(line_index, "string", str_in_file);
                            // 输出第二个引号
                            print(line_index, "delimiter", "\"");
                            // 累计字符串数目
                            str_num ++;
                            // 累计分界符和操作符数目
                            operate_num ++;
                        }

                        // reset
                        pos = 0;
                        break;
                    }

                    // +
                    // 可能只有一个+ 也可能是++ +=
                    case '+':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);
                        
                        switch (now_char) {
                            // ++
                            case '+':
                            {
                                print(line_index, "operator", "++");
                                pos = 0;
                                break;
                            }
                            // +=
                            case '=':
                            {
                                print(line_index, "operator", "+=");
                                pos = 0;
                                break;
                            }
                            // 其他情况 只有第一个加号或是错误的操作符
                            // 这里不考虑对形如“+-”的错误操作符
                            default:
                            {
                                // 因为刚才读了第一个+的下一个字符，这里只输出一个+，所以指针需要返回
                                get_backward(forward);
                                print(line_index, "operator", "+");
                                pos = 0;
                                break;
                            }
                        }
                        break; 
                    }

                    // - 与 +类似
                    case '-':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '-':
                            {
                                print(line_index, "operator", "--");
                                pos = 0;
                                break;
                            }
                            case '=':
                            {
                                print(line_index, "operator", "-=");
                                pos = 0;
                                break;
                            }
                            // 可能为指针符号
                            case '>':
                            {
                                print(line_index, "operator", "->");
                                pos = 0;
                                break;
                            }
                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", "-");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    // = 与 +类似
                    case '=':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '=':
                            {
                                print(line_index, "operator", "==");
                                pos = 0;
                                break;
                            }
                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", "=");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    // < 与 +类似
                    case '<':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '<':
                            {
                                // 可能为<<=
                                char now_char = get_char(forward);
                                if (now_char == '='){
                                    print(line_index, "operator", "<<=");
                                    pos = 0;
                                    break;
                                }
                                else{
                                    // 回退
                                    get_backward(forward);
                                    print(line_index, "operator", "<<");
                                    pos = 0;
                                    break;
                                }
                            }
                            case '=':
                            {
                                print(line_index, "operator", "<=");
                                pos = 0;
                                break;
                            }
                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", "<");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    // > 与 +类似
                    case '>':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '>':
                            {
                                // 可能为>>=
                                char now_char = get_char(forward);
                                if (now_char == '='){
                                    print(line_index, "operator", ">>=");
                                    pos = 0;
                                    break;
                                }
                                else{
                                    // 回退
                                    get_backward(forward);
                                    print(line_index, "operator", ">>");
                                    pos = 0;
                                    break;
                                }
                            }
                            case '=':
                            {
                                print(line_index, "operator", ">=");
                                pos = 0;
                                break;
                            }
                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", ">");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    case '*':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '=':
                            {
                                print(line_index, "operator", "*=");
                                pos = 0;
                                break;
                            }
                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", "*");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    case '/':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '=':
                            {
                                print(line_index, "operator", "/=");
                                pos = 0;
                                break;
                            }

                            // 行内注释
                            case '/':
                            {
                                // reset
                                state = 0;
                                pos = 0;
                                single_ann_num ++;
                                // 该行剩下的均已经不需要遍历 直接到下一行即可 直接return
                                cout << "line " << line_index << " have a single annotation skipping" << endl;
                                return ;
                            }

                            // 多行注释
                            case '*':
                            {
                                // 到多行注释寻找*状态
                                state = 6;
                                pos = 0;
                                multi_ann_num ++;
                                break;
                            }

                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", "/");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    case '&':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '&':
                            {
                                print(line_index, "operator", "&&");
                                pos = 0;
                                break;
                            }
                            case '=':
                            {
                                print(line_index, "operator", "&=");
                                pos = 0;
                                break;
                            }
                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", "&");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    case '|':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '|':
                            {
                                print(line_index, "operator", "||");
                                pos = 0;
                                break;
                            }
                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", "|");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    case '!':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '=':
                            {
                                print(line_index, "operator", "!=");
                                pos = 0;
                                break;
                            }
                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", "!");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    case '%':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '=':
                            {
                                print(line_index, "operator", "%=");
                                pos = 0;
                                break;
                            }
                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", "%");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    case '^':
                    {
                        state = 0;
                        // 累计分界符和操作符数目
                        operate_num ++;
                        now_char = get_char(forward);

                        switch (now_char) {
                            case '=':
                            {
                                print(line_index, "operator", "^=");
                                pos = 0;
                                break;
                            }
                            default:
                            {
                                get_backward(forward);
                                print(line_index, "operator", "^");
                                pos = 0;
                                break;
                            }
                        }
                        break;
                    }

                    case ',':
                    {
                        // 累计分界符和操作符数目
                        operate_num ++;
                        state = 0;
                        pos = 0;
                        print(line_index, "operator", ",");
                        break;
                    }

                    case ';':
                    {
                        // 累计分界符和操作符数目
                        operate_num ++;
                        state = 0;
                        pos = 0;
                        print(line_index, "operator", ";");
                        break;
                    }

                    case '#':
                    {
                        // 累计分界符和操作符数目
                        operate_num ++;
                        state = 0;
                        pos = 0;
                        print(line_index, "operator", "#");
                        break;
                    }

                    case '.':
                    {
                        // 累计分界符和操作符数目
                        operate_num ++;
                        state = 0;
                        pos = 0;
                        print(line_index, "operator", ".");
                        break;
                    }

                    case '~':
                    {
                        // 累计分界符和操作符数目
                        operate_num ++;
                        state = 0;
                        pos = 0;
                        print(line_index, "operator", "~");
                        break;
                    }

                    // 剩下的就是数字或者word（变量名或关键字）
                    default:
                    {
                        // 数字
                        if (is_num(now_char)){
                            str_in_file[pos] = now_char;
                            pos ++;
                            number_num ++;
                            // 将状态转为2，往下查找整个数字
                            state = 2;
                        }

                        else{
                            // 变量名或关键字
                            if (is_letter(now_char) || now_char == '_'){
                                str_in_file[pos] = now_char;
                                pos ++;
                                word_num ++;
                                // 将状态转为1，往下查找整个word
                                state = 1;
                            }
                        }
                        break;
                    }
                }

                break;
            }

            // 处理word
            case 1:
            {
                // 不读完就不停
                while (1) {
                    now_char = get_char(forward);
                    // 因为第一个已经确定是字母，所以接下来的可以是字母、数字或者下划线
                    if (is_num(now_char) || is_letter(now_char) || now_char == '_'){
                        str_in_file[pos] = now_char;
                        pos ++;
                    }
                    // 如果出现了空格等别的字符
                    else {
                        // 给当前要输出的字符串加上结束符
                        str_in_file[pos] = '\0';
                        // 判断该word是不是关键字  
                        bool keyword_res = is_keyword(str_in_file);
                        // 如果是关键字 
                        if (keyword_res) {
                            print(line_index, "keyword", str_in_file);
                            // word_num 已经加过了 这里只加 keyword_num
                            keyword_num ++;
                        }
                        // 不是关键字
                        else {
                            print(line_index, "variable", str_in_file);
                            // word_num 已经加过了 这里只加 varible_num
                            varible_num ++;
                        }

                        // 处理完毕 进行reset
                        // 这个字符不是属于要输出的字符串之内的，回退
                        get_backward(forward);
                        pos = 0;
                        state = 0;
                        break;
                    }
                }
                break;
            }



            // 处理数字
            case 2:
            {
                // 同样不读完不停
                while (1) {
                    now_char = get_char(forward);

                    // 如果是数字 继续读
                    if (is_num(now_char)){
                        str_in_file[pos] = now_char;
                        pos ++;
                    }

                    // 如果下一个不是数字
                    else {

                        // 可能是小数
                        if (now_char == '.'){
                            str_in_file[pos] = now_char;
                            pos ++;
                            // 切换到小数状态
                            state = 3;
                            // 退出while
                            break;
                        }

                        // 有可能是科学计数法 如1e-3
                        else if (now_char == 'e' || now_char == 'E'){
                            str_in_file[pos] = now_char;
                            pos ++;
                            // 切换到科学计数法状态
                            state = 4;
                            // 退出while
                            break;
                        }

                        // 不是上述的任何符号 则结束数字模式
                        else {
                            // 补终结符
                            str_in_file[pos] = '\0';
                            print(line_index, "integral number", str_in_file);
                            // 该字符没输出 需要回退
                            get_backward(forward);

                            // reset
                            pos = 0;
                            state = 0;
                            // 退出while
                            break;
                        }

                    }
                }
                break;
            }


            // 小数状态
            case 3:
            {
                while (1) {
                    now_char = get_char(forward);
                    // 是数字
                    if (is_num(now_char)) {
                        str_in_file[pos] = now_char;
                        pos ++;
                    }

                    // eg:1.5e-3   转移到科学计数法状态
                    else if (now_char == 'e' || now_char == 'E') {
                        str_in_file[pos] = now_char;
                        pos ++;
                        state = 4;
                        break;
                    }

                    // 不是数字或e
                    else {
                        str_in_file[pos] = '\0';
                        print(line_index, "fraction number", str_in_file);
                        // 该字符没输出 需要回退
                        get_backward(forward);
                        // reset
                        pos = 0;
                        state = 0;
                        // 退出while
                        break;
                    }
                }
                break;
            }

            // 科学计数状态(无负号)
            case 4:
            {
                while (1) {

                    now_char = get_char(forward);
                    // 数字
                    if (is_num(now_char)) {
                        str_in_file[pos] = now_char;
                        pos ++;
                    }
                    
                    // 负号  科学计数法的负次幂
                    else if (now_char == '-') {
                        str_in_file[pos] = now_char;
                        pos ++;
                        state = 5;
                        break;
                    }

                    // 结束幂次正数的科学计数法
                    else {
                        str_in_file[pos] = '\0';
                        print(line_index, "scientific notation number (pos power)", str_in_file);
                        // 该字符没输出 需要回退
                        get_backward(forward);
                        // reset
                        pos = 0;
                        state = 0;
                        // 退出while
                        break;
                    }
                }
                break;
            }

            // 科学技术状态 负幂次
            case 5:
            {
                while (1) {
                    now_char = get_char(forward);
                    // 数字
                    if (is_num(now_char)) {
                        str_in_file[pos] = now_char;
                        pos ++;
                    }

                    // 结束科学计数法
                    else {
                        str_in_file[pos] = '\0';
                        print(line_index, "scientific notation number (neg power)", str_in_file);
                        // 该字符没输出 需要回退
                        get_backward(forward);
                        // reset
                        pos = 0;
                        state = 0;
                        // 退出while
                        break;
                    }
                }
                break;
            }

            // 多行注释 已经检索到/*     寻找*
            case 6:
            {
                while (1) {
                    now_char = get_char(forward);
                    // 如果该行已经遍历完毕 
                    if (now_char == '\0'){
                        // 设置多行注释标记
                        multi_annotation = 1;
                        break;
                    }

                    // 找到 *
                    else if (now_char == '*') {
                        // 寻找 /
                        state = 7;
                        pos = 0;
                        break;
                    }
                    
                    // 没找到就继续找
                    else {
                        continue;
                    }
                }
                break;
            }

            // 多行注释 已经检索到/* *     寻找/
            case 7:
            {
                now_char = get_char(forward);
                
                // 已经检索完多行注释
                if (now_char == '/') {
                    // 直接跳过 回到状态0
                    multi_annotation = 0;
                    cout << "have a multi-annotation skipping" << endl;
                    state = 0;
                    pos = 0;
                    break;
                }

                // 该行已经遍历完毕 (存在这种情况,因为是从case 6 过来的)
                // 出去之后该函数会结束 读下一行 所以不能设置state = 6 还是得用标记
                else if (now_char == '\0') {
                    // 设置多行注释标记
                    multi_annotation = 1;
                    break;
                }

                // 再接收到*
                else if (now_char == '*') {
                    // 仍然为state = 7
                    state = 7;
                    pos = 0;
                    break;
                }

                // 该行没结束 且不是/  则回到状态6
                else {
                    state = 6;
                    pos = 0;
                    break;
                }
                break;
            }

            // 不在任何情况内 抛出一个错误
            default:
            {
                print(line_index, "error! no state is matching.", "-");
                break;
            }

        }
    }
}



int main()
{
    // 打开文件
    FILE *file = fopen("s.txt", "r");
    // 标记当前行数
    int line_index = 0;

    // 为了标记行,选择一行行输入
    while (1){
        char *now_line_str = fgets(buffer, 10000, file);

        if (now_line_str != NULL){
            line_index ++;
            lexical_analysis(line_index, buffer);
        }
        else break;
    }

    // 如果多行注释始终没有配对成功 则报错
    if (multi_annotation == 1) {
        print(line_index, "error: multi-annotation compared unsuccessfully", "-");
    }

    cout <<endl;
    cout <<endl;
    cout << "行数:" << line_index << endl;
    cout << "字符总数:" << sign_num << endl;
    cout << "单词总数:" << word_num << endl;
    cout << "关键字总数:" << keyword_num <<endl;
    cout << "变量名数目:" << varible_num << endl;
    cout << "数学常量数目:" << number_num << endl;
    cout << "单行注释数目:" << single_ann_num << endl;
    cout << "多行注释数目:" << multi_ann_num << endl;
    cout << "分界符和操作符数目(包括注释):" << operate_num << endl;
    cout << "字符串常量数目(包括printf内的输出):" << str_num << endl;
}
