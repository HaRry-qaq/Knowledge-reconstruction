// #pragma comment(linker,"/STACK:1024000000,1024000000")    增加栈容量
#include<iostream>
#include<chrono>
#include<fstream>
#include<string>
using namespace std;
using namespace std::chrono;

// 堆排序计数
// 比较
int maxHeap_compare_count = 0; 
// 赋值
int maxHeap_assign_count = 0;

// 归并排序计数
int merge_compare_count = 0;
int merge_assign_count = 0;

// 快速排序计数
long long int quick_compare_count = 0;
long long int quick_assign_count = 0;



/********************************************** 堆排序begin **********************************************/
void maxHeap(int* arr, int start, int end){
    // 记录当前节点
    int current = start;


    // 赋值次数累加######################
    // maxHeap_assign_count ++;
    // 记录当前节点值
    int tmp = arr[current];



    // 找左右节点，交换后current指向被交换的子节点
    for (int i = 2 * start + 1; i<= end; i = 2 * i + 1){


        // 取较大的孩子节点

        // 比较次数累加######################
        // maxHeap_compare_count++;

        if (i < end && arr[i] < arr[i + 1]){
            i++;
        }


        // 若子节点较大则交换

        // 比较次数累加######################
        // maxHeap_compare_count++;

        if(tmp < arr[i]){

            arr[current] = arr[i];
            arr[i] = tmp;
            current = i;

            // 赋值次数累加######################
            // maxHeap_assign_count += 2;

        }
        else{
            break;
        }
    }
}
void heapSort(int* arr, int len){
    // 从后往前遍历所有非叶子节点,得到大根堆
    for (int i = len / 2 - 1; i >= 0; i--){
        maxHeap(arr, i, len-1);
    }
    // 对第一个元素（当前最大值）与大根堆最后一个元素进行交换，后将其排除出序列后继续构建大根堆
    for (int i = len - 1; i > 0; i--){
        int swap = arr[0];
        arr[0] = arr[i];
        arr[i] = swap;

        // 赋值次数累加######################
        // maxHeap_assign_count += 3;


        // 交换完只有最顶上的元素是不符合大根堆的，所以只需要maxHeap(arr, 0, i-1)
        maxHeap(arr, 0, i-1);
    }
}
/********************************************** 堆排序end **********************************************/





/********************************************** 归并排序begin **********************************************/
void mergeSort(int* arr, int left, int mid, int right){
    // 定义两个数组的指针和temp数组及其指针
    int p1 = left;
    int p2 = mid + 1;
    int p = 0;
    int* tmp = new int[right - left + 1];

    // 比大小
    while (p1 <= mid && p2 <= right){

        // 比较次数累加
        // merge_compare_count++;

        if (arr[p1] <= arr[p2]){

            // 赋值次数累加
            // merge_assign_count++;

            tmp[p] = arr[p1];
            p++;
            p1++;
        }
        else{

            // 赋值次数累加
            // merge_assign_count++;

            tmp[p] = arr[p2];
            p++;
            p2++;
        }
    }

    // 若数组还有剩下的
    while (p1 <= mid){

        // 赋值次数累加
        // merge_assign_count++;

        tmp[p] = arr[p1];
        p++;
        p1++;
    }
    while (p2 <= right){

        // 赋值次数累加
        // merge_assign_count++;

        tmp[p] = arr[p2];
        p++;
        p2++;
    }

    // 赋值到原数组
    for (int i = 0;i < right - left + 1; i++){

        // 赋值次数累加
        // merge_assign_count++;

        arr[left + i] = tmp[i];
    }

}
void mergeSortWrapper(int* arr, int left, int right){
    // 停止条件
    if (left >= right){
        return;
    }

    // long long:防溢出
    long long int mid = (left + right) / 2;
    mergeSortWrapper(arr, left, mid);
    mergeSortWrapper(arr, mid + 1, right);
    mergeSort(arr, left, mid , right);
}

/********************************************** 归并排序end **********************************************/








/********************************************** 快速排序begin **********************************************/
// 一遍快速排序
int partition(int* arr, int left, int right){
    // 选择基准
    int pivot = arr[left];
    while (left < right){
        // 先从右往左
        while (left < right && arr[right] >= pivot){

            // 比较次数累加
            // quick_compare_count ++;

            right--;
        }
        // 比较次数累加（跳出循环那次）
        // quick_compare_count ++;



        // 找到右边小于pivot的值，赋值到左边的left位置处
        arr[left] = arr[right];
        // 赋值次数累加
        // quick_assign_count ++;

        
        // 再从左往右
        while (left < right && arr[left] <= pivot){

            // 比较次数累加
            // quick_compare_count ++;

            left++;
        }

        // 比较次数累加（跳出循环那次）
        // quick_compare_count ++;



        // 找到左边大于pivot的值，复制到右边的right位置处
        arr[right] = arr[left];
        // 赋值次数累加
        // quick_assign_count ++;

    }

    // left 与 right 重合，将pivot赋值到此位置
    arr[left] = pivot;
    // 赋值次数累加
    // quick_assign_count ++;
    
    // 返回pivot元素的索引
    return left;
}
void quickSort(int* arr, int left, int right){
    if (left < right) {
        int mid = partition(arr, left, right);
        quickSort(arr, left, mid-1);
        quickSort(arr, mid+1, right);
    }
}

/********************************************** 快速排序end **********************************************/


int main(){
    int s;

    // N大小需要与文件大小同时修改！！！！！！！
    int N = 100000;
    ifstream infile;
    // 文件名需要自行修改
    infile.open("../data/rand_100000.txt");
    int* arr = new int[N];
    // string s;
    if (!infile) {
    cerr << "Unable to open file datafile.txt";
    exit(1);   // call system to stop
    }

    int count = 0;

    while (infile >> s){
        arr[count] = s;
        count++;
    }

    cout << "origin_data:" <<endl;
    for (int i = 0; i < count; i++){
        cout << arr[i] << ' ';
    }
    
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;

    // 每次只运行一种排序
    system_clock::time_point start,end;

    string way = "quickSort";

    if (way == "heapSort"){
        start = system_clock::now();
        heapSort(arr, N );
        end = system_clock::now();
    }
    else if (way == "mergeSort"){
        start = system_clock::now();
        mergeSortWrapper(arr, 0, N - 1);
        end = system_clock::now();
    }
    else if (way == "quickSort"){
        start = system_clock::now();
        quickSort(arr, 0, N - 1);
        end = system_clock::now();
    }

    cout << "result:" << endl; 
    for (int i = 0; i < N; i++){
        cout << arr[i] << ' ';
    }

    cout << endl;
    cout << endl;
    
    cout << "way:" << way << endl;
    cout<< "compare_count:" << quick_compare_count << endl;
    cout<< "assign_count:" << quick_assign_count << endl;
	cout << "running time:" << double(duration_cast<microseconds>(end - start).count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;



}
