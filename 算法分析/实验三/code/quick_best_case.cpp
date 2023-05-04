// 生成快速排序最好情况的数据集（不正确）

#include<iostream>
#include<chrono>
#include<fstream>
#include<string>
using namespace std;


// N
int N = 8;

// 最终得到的数组
int* res = new int[N];

// 最终得到的的数组指针
int point = 0;


struct treenode{
    int data;
    treenode* left;
    treenode* right;
};


treenode* setUpTree(){
    treenode* root = new treenode();
    root -> left = nullptr;
    root -> right = nullptr;
    return root;
}

void recur_set_tree(int* arr, int left, int right, treenode* node){
    long long int mid = (left + right) / 2;
    node -> data = arr[mid];
    if (left <= mid - 1){
        treenode* left_kid = new treenode();
        node -> left = left_kid;
        recur_set_tree(arr, left, mid - 1, left_kid);
    }
    if (mid + 1 <= right){
        treenode* right_kid = new treenode();
        node ->right = right_kid;
        recur_set_tree(arr, mid + 1, right, right_kid);
    }
}


void preorder_tree(treenode* node, int* res){
    if (node != nullptr){
        res[point] = node -> data;
        point++;
        preorder_tree(node -> left, res);
        preorder_tree(node -> right, res);
    }
}



int main(){

    // 先生成长度为N的顺序数组
    int* arr = new int[N];
    for (int i = 0; i < N; i++){
        arr[i] = i;
    }
    

    // 根据该顺序数组作为中序遍历构建平衡二叉树
    treenode* root = setUpTree();
    recur_set_tree(arr, 0, N-1, root);


    // 再考虑前序遍历
    preorder_tree(root, res);

    for (int i = 0; i < N; i++){
        cout << res[i] << ' ';
    }

    // 未考虑数的交换！所以错误。
}