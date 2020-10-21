#include <iostream>
#include <cmath>
using namespace std;

/* 
输入一个正整数N，则程序输出N皇后问题的全部摆法。
输出结果里的每一行都代表一种摆法。行里的第i个数字如
果是n，就代表第i行的皇后应该放在第n列。
皇后的行、列编号都是从1开始算。
样例输入：
	4
样例输出:
	2 4 1 3
	3 1 4 2
*/ 
int N;
int queenPos[100]; //quueenPos[0]表示第一行皇后的位值 
//用来存放算好的皇后位置。最左上角是(0,0)
void NQueen( int k); 
int main()
{
	cin >> N;
	NQueen(0); //从第0行开始摆皇后
	return 0;
}
void NQueen( int k) { //在0~k-1行皇后已经摆好的情况下，摆第k行及其后的皇后
	int i;
	if( k  == N ) { // N 个皇后已经摆好
		for( i = 0; i <  N;i ++ )
			cout << queenPos[i] + 1 << " ";
		cout << endl;
		return ;
	}
	for( i = 0;i < N;i ++ ) { //逐尝试第k个皇后的位置
		int j;
		for( j = 0; j < k; j ++ ) {
			//和已经摆好的 k 个皇后的位置比较，看是否冲突
			//queenPos[j] == i 说明第j行的皇后在第i列，冲突！ 
			//abs(queenPos[j] - i) == abs(k-j) 说明他们在对角上，列的差减去行的差 
			if( queenPos[j] == i ||
				abs(queenPos[j] - i) == abs(k-j)) {
				break; //冲突，则试下一个位置
			}
		} 
		if( j == k ) { //当前选的位置 i 不冲突
			queenPos[k] = i; //将第k个皇后摆放在位置 i，第k行已经摆好，开始尝试第k+1行 
			NQueen(k+1); //如果第k+1这个位置怎么放都冲突，那么程序会回退到这个地方，循环继续，寻找k上合适的位置 
						//不冲突，则会一直到k==N，输出后继续回退到这个地方，然后寻找k上，下一个合适的位置 
		}
	} 
}
