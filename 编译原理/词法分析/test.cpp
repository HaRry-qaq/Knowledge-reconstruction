/***
    use for lex-test.
**/

#include <math.h>
#include <stdio.h>
#define harry 916

//this is the program
int main()
{
    int m,flag;
    scanf("%d", &m);
    flag = 1;
    for(int i = 2;i <= sqrt(m);i++){
        if(m%i == 0)
        {
            flag = 0;
            break;
        }
    }
    if(flag){
        //output
        printf("%d is a primer.\n", m);
    }
    else{
        //output
        printf("%d is not a primer.\n", m);
    }
    return 0;
}