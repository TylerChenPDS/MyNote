#include <iostream>
#include <cstdio>
using namespace std;

int main(){
	int p,e,i,d;
	int k;
	while(cin >> p >> e >> i >> d && p != -1){
		for(k = d + 1; k < 21252 && (k - p) % 23 != 0; k ++);
		for(;k < 21252 && (k-e) % 28 != 0; k += 23 );
		for(;k < 21252 && (k-i) % 33 != 0; k += 23 * 28);
		printf("Case 1: the next triple peak occurs in d% days.", k - d);		
	} 
	return 0;
}
