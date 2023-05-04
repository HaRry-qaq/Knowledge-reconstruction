#include<iostream>
#include<cmath>
#include<ctime>
#include<chrono>
#include<fstream>
using namespace std;
using namespace std::chrono;

int MaxSubsequenceSum1(const int A[],int N) { /* ((O(1)+O(j-i+1)+O(2))*O(N-i))*N + 1 */
	int ThisSum,MaxSum,i,j,k;
	MaxSum = 0;                             /* O(1) */
	for (i = 0; i < N; i++) {               /* ((O(1)+O(j-i+1)+O(2))*O(N-i))*N */
		for (j = i; j < N; j++) {           /* (O(1)+O(j-i+1)+O(2))*O(N-i) */
			ThisSum = 0;                    /* O(1) */
			for (k = i; k <= j; k++) {      /* O(j-i+1) */
				ThisSum += A[k];
			}
			if(ThisSum > MaxSum)            /* each loop: O(1)+O(1) */
				MaxSum = ThisSum;           /* each loop: O(1) */
		}
	}
	return MaxSum;
}
/* j-i+1<N,N-i<N, so  ((O(1)+O(j-i+1)+O(2))*O(N-i))*N + 1 <= O(N)*O(N)*O(N) +1 = O(N^3)    */


int MaxSubsequenceSum2(const int A[],int N)  /* O(N^2) */
{
	int ThisSum,MaxSum,i,j;
	MaxSum = 0;
	for(i = 0;i < N; i++)                    /* (O(1)+(O(1)+O(1)+O(1))*(N-i))*N */
	{
		ThisSum = 0;                         /* O(1) */
		for(j = i;j < N;j++)                 /* (O(1)+O(1)+O(1))*(N-i) */
		{
			ThisSum += A[j];                 /* O(1) */
			if(ThisSum > MaxSum)             /* each loop: O(1)+O(1) */
			{
				MaxSum = ThisSum;            /* each loop: O(1) */
			}
		}
	}
	return MaxSum;
}

int MaxSubsequenceSum3(const int A[],int left,int right) // T(n) = T(n/2) + n  =>   O(nlogn)
{
	int leftmax;
	int rightmax; 
	int search_left_max;
	int search_left = 0;
	int search_right_max;
	int search_right = 0;
	int midmax;
	int mid = (left + right) /2;
	int ThisSum;
	// finished condition
	if (left >= right)
	{
		return A[left];
	}
	else
	{
		// maxsum through mid
		// search left
		for(int i = mid;i >= left; i--)
		{	
			search_left += A[i];
			if(i == mid)
			{
				search_left_max = search_left;
			}
			else{
				if(search_left > search_left_max)
				{
					search_left_max = search_left;
				}
			}
		}

		// search right
		for(int i = mid + 1; i<=right; i++)
		{
			search_right += A[i];
			if(i == mid + 1)
			{
				search_right_max = search_right;
			}
			else{
				if(search_right > search_right_max)
				{
					search_right_max = search_right;
				}
			}
		}

		midmax = search_left_max + search_right_max;

		// maxsum for left
		leftmax = MaxSubsequenceSum3(A,left,mid);
		// maxsum for right
		rightmax = MaxSubsequenceSum3(A,mid+1,right);

		return fmax(fmax(leftmax,rightmax),midmax);

	}
}







int MaxSubsequenceSum4(const int A[],int N) { /* O(1)+O(N)*(O(1)+O(1)+O(1))  */
	int ThisSum,MaxSum,j;
	ThisSum = MaxSum = 0;            /* O(1) */
	for (j = 0; j < N; j++) {        /* total: O(N)*(O(1)+O(1)+O(1)) */
		ThisSum += A[j];             /* each loop: O(1)      ; total: O(N)*O(1) */

		/* if-else part total for each loop:   O(1)+O(1)     ; total: O(N)*(O(1)+O(1))   */

		if(ThisSum > MaxSum) {       /* each loop: O(1)+O(1) ; total: O(N)*(O(1)+O(1)) */
			MaxSum = ThisSum ;       /* each loop: O(1)      ; total: O(N)*O(1) */
		} else if (ThisSum < 0) {    /* each loop: O(1)+O(1) ; total: O(N)*(O(1)+O(1)) */
			ThisSum = 0;             /* each loop: O(1)      ; total: O(N)*O(1) */
		}
	}
	return MaxSum;
}
/*  O(1)+O(N)*(O(1)+O(1)+O(1)) = O(N)  */

int MaxSubsequenceSum5(const int A[],int N) {
	int dp[N];
	int MaxSum = A[0];
	dp[0] = A[0];
	for (int i = 1;i < N; i++) {
		dp[i] = fmax(dp[i-1] + A[i] , A[i]);
		MaxSum = fmax(MaxSum, dp[i]);
	}
	return MaxSum;
}

int MaxSubsequenceSum6(const int A[],int N) {
	int MaxSum = A[0];
	int ThisSum = A[0];
	for (int i = 1;i < N; i++) {
		ThisSum = fmax(ThisSum + A[i] , A[i]);
		MaxSum = fmax(MaxSum, ThisSum);
	}
	return MaxSum;
}





int main() {
	// 使用伪随机种子生成数组，每台机器不一样
	int res1,res2,res3,res4,res5,res6;
	// srand((int)time(0));
	// int A[8] = {-1,3,-2,4,-6,1,6,-1};
	int N = 1000;
	int A[N];
	cout << "create pseudo-random list:" <<endl;
	for (int i = 0;i < N;i++){
		// [a,b] rand() % (b - a + 1) + a
		// [-100,100]
		A[i] = rand() % (100 + 100 + 1) - 100;
		cout << A[i] << " ";
	}
	cout << endl;

	// 输出所生成的随机数组
	// ofstream dataFile;
	// dataFile.open("100000.txt", ofstream::app);
	// fstream file("100000.txt", ios::out);
 
 
	// for(int i = 0;i < N;i++){
	// 	dataFile << A[i] << " ";     // 写入数据
	// }
	// dataFile.close();          // 关闭文档

	




	// system_clock::time_point start1 = system_clock::now();
	// res1 = MaxSubsequenceSum1(A,N);
	// system_clock::time_point end1 = system_clock::now();
	// cout << "Sum1 result:" << res1 << endl;
	// cout << "Sum1 running time:" << double(duration_cast<microseconds>(end1 - start1).count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;

	system_clock::time_point start2 = system_clock::now();
	res2 = MaxSubsequenceSum2(A,N);
	system_clock::time_point end2 = system_clock::now();
	cout << "Sum2 result:" << res2 << endl;
	cout << "Sum2 running time:" << double(duration_cast<microseconds>(end2 - start2).count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;


	system_clock::time_point start3 = system_clock::now();
	res3 = MaxSubsequenceSum3(A,0,N-1);
	system_clock::time_point end3 = system_clock::now();
	cout << "Sum3 result:" << res3 << endl;
	cout << "Sum3 running time:" << double(duration_cast<microseconds>(end3 - start3).count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;

	system_clock::time_point start4 = system_clock::now();
	res4 = MaxSubsequenceSum4(A,N);
	system_clock::time_point end4 = system_clock::now();
	cout << "Sum4 result:" << res4 << endl;
	cout << "Sum4 running time:" << double(duration_cast<microseconds>(end4 - start4).count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;

	system_clock::time_point start5 = system_clock::now();
	res5 = MaxSubsequenceSum5(A,N);
	system_clock::time_point end5 = system_clock::now();
	cout << "Sum5 result:" << res5 << endl;
	cout << "Sum5 running time:" << double(duration_cast<microseconds>(end5 - start5).count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;

	system_clock::time_point start6 = system_clock::now();
	res6 = MaxSubsequenceSum5(A,N);
	system_clock::time_point end6 = system_clock::now();
	cout << "Sum6 result:" << res6 << endl;
	cout << "Sum6 running time:" << double(duration_cast<microseconds>(end6 - start6).count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;
	
	cout << "unit:second" <<endl;
}




