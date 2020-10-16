#include <memory>
#include <string>
#include <cstring>
#include <iostream>
/*

描述
	赛利有12枚银币。其中有11枚真币和1枚假币。假币看起来和真币没有区别，但是重量不同。
	但赛利不知道假币比真币轻还是重。于是他向朋友借了一架天平。朋友希望赛利称三次就能找出假币并且确定假币是轻是重。
	例如:如果赛利用天平称两枚硬币，发现天平平衡，说明两枚都是真的。	如果赛利用一枚真币与另一枚银币比较，
	发现它比真币轻或重，说明它是假币。经过精心安排每次的称量，赛利保证在称三次后确定假币。
输入
	第一行有一个数字n，表示有n组测试用例。对于每组测试用例：输入有三行，每行表示一次称量的结果。
	赛利事先将银币标号为A-L。每次称量的结果用三个以空格隔开的字符串表示：
	天平左边放置的硬币 天平右边放置的硬币 平衡状态。其中平衡状态用``up'', ``down'', 或 ``even''表示, 
	分别为右端高、右端低和平衡。天平左右的硬币数总是相等的。
输出
	输出哪一个标号的银币是假币，并说明它比真币轻还是重(heavy or light)。
样例输入
	1
	ABCD EFGH even 
	ABCI EFJK up 
	ABIJ EFGH even 
样例输出
	K is the counterfeit coin and it is light. 
思路：使用枚举的方法，假设A是假的，且是轻的带进去看看是不是成立 ，不成立则假设A是重的，还不成立则判断B，一直到L
 
*/
bool isContain(char str[], char a);
char left[3][7];
char right[3][7];
char result[3][7];

//判断某个字符是否满足 是假币且 isLight 

bool isStatisFied(char a, bool isLight){
	if(isLight){ //如果这枚硬币是轻的 
		for(int i = 0; i < 3; i ++){ //首先可以确定的是左右两边不可能同时有a 
			if(isContain(left[i], a)){//左边包含这枚硬币，那么右边应该是down状态 
				if(result[i][0] != 'd') {
					return false;
				}
			}else if(isContain(right[i],a)){//右面包含这枚硬币，那么右边应该是up状态 
				if(result[i][0] != 'u'){
					return false;
				} 
			}
		}
		return true;
	}else{
			for(int i = 0; i < 3; i ++){ //首先可以确定的是左右两边不可能同时有a 
			if(isContain(left[i], a)){//左边包含这枚硬币，那么右边应该是up状态 
				if(result[i][0] != 'u') {
					return false;
				}
			}else if(isContain(right[i],a)){//右面包含这枚硬币，那么右边应该是down状态 
				if(result[i][0] != 'd'){
					return false;
				} 
			}
		}
		return true;
	}
	
} 
bool isContain(char str[], char a){
	int len = strlen(str);
	for(int i = 0; i < len; i ++){
		if(str[i] == a)
			return true;
	}
	return false;
}

int main(){
	int N;
	scanf("%d", &N);
	for(int i = 0; i < N; i ++){
		for(int j = 0; j < 3; j ++){
			scanf("%s %s %s", left[j], right[j], result[j]);
		}
		for(char a = 'A'; a <= 'L'; a ++){
			//从A开始，假设A是假币，且是轻的
			if(isStatisFied(a, true)){
				printf("%c is the counterfeit coin and it is light.\n", a);
				break;
			}else if(isStatisFied(a, false)){
				printf("%c is the counterfeit coin and it is weight.\n", a);
				break;
			}
		}
	}
	
	
	
	
	return 0;
}
















