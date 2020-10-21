#include <iostream>
using namespace std;
//古代有一个梵塔，塔内有三个座A、B、C，A座上有64个盘子，盘子大小
//不等，大的在下，小的在上（如图）。有一个和尚想把这64个盘子从A座移
//到C座，但每次只能允许移动一个盘子，并且在移动过程中，3个座上的盘子
//始终保持大盘在下，小盘在上。在移动过程中可以利用B座，要求输出移动
//的步骤。

//将a上的n个盘子，从a移动到c以b作为中转 
void hanoi(int n, char a, char b, char c){
	if( n == 1){//如果只有一个盘子 ,直接移动 
		cout << a << "->" << c  << endl;
		return;
	}
	//将n-1个盘子移动到b,然后将a上的最后一个盘子，从a移动到c,再将b上的移动到c
	 hanoi(n-1, a,c,b);
	 
	 
	
}
int mian(){
	int N;
	cin >> N; //输入盘子的数目
	hanoi() 
	
	return 0;
} 
