#include <memory>
#include <string>
#include <cstring>
#include <iostream>
/*

����
	������12ö���ҡ�������11ö��Һ�1ö�ٱҡ��ٱҿ����������û�����𣬵���������ͬ��
	��������֪���ٱұ�����ỹ���ء������������ѽ���һ����ƽ������ϣ�����������ξ����ҳ��ٱҲ���ȷ���ٱ��������ء�
	����:�����������ƽ����öӲ�ң�������ƽƽ�⣬˵����ö������ġ�	���������һö�������һö���ұȽϣ�
	���������������أ�˵�����Ǽٱҡ��������İ���ÿ�εĳ�����������֤�ڳ����κ�ȷ���ٱҡ�
����
	��һ����һ������n����ʾ��n���������������ÿ��������������������У�ÿ�б�ʾһ�γ����Ľ����
	�������Ƚ����ұ��ΪA-L��ÿ�γ����Ľ���������Կո�������ַ�����ʾ��
	��ƽ��߷��õ�Ӳ�� ��ƽ�ұ߷��õ�Ӳ�� ƽ��״̬������ƽ��״̬��``up'', ``down'', �� ``even''��ʾ, 
	�ֱ�Ϊ�Ҷ˸ߡ��Ҷ˵ͺ�ƽ�⡣��ƽ���ҵ�Ӳ����������ȵġ�
���
	�����һ����ŵ������Ǽٱң���˵����������ỹ����(heavy or light)��
��������
	1
	ABCD EFGH even 
	ABCI EFJK up 
	ABIJ EFGH even 
�������
	K is the counterfeit coin and it is light. 
˼·��ʹ��ö�ٵķ���������A�Ǽٵģ�������Ĵ���ȥ�����ǲ��ǳ��� �������������A���صģ������������ж�B��һֱ��L
 
*/
bool isContain(char str[], char a);
char left[3][7];
char right[3][7];
char result[3][7];

//�ж�ĳ���ַ��Ƿ����� �Ǽٱ��� isLight 

bool isStatisFied(char a, bool isLight){
	if(isLight){ //�����öӲ������� 
		for(int i = 0; i < 3; i ++){ //���ȿ���ȷ�������������߲�����ͬʱ��a 
			if(isContain(left[i], a)){//��߰�����öӲ�ң���ô�ұ�Ӧ����down״̬ 
				if(result[i][0] != 'd') {
					return false;
				}
			}else if(isContain(right[i],a)){//���������öӲ�ң���ô�ұ�Ӧ����up״̬ 
				if(result[i][0] != 'u'){
					return false;
				} 
			}
		}
		return true;
	}else{
			for(int i = 0; i < 3; i ++){ //���ȿ���ȷ�������������߲�����ͬʱ��a 
			if(isContain(left[i], a)){//��߰�����öӲ�ң���ô�ұ�Ӧ����up״̬ 
				if(result[i][0] != 'u') {
					return false;
				}
			}else if(isContain(right[i],a)){//���������öӲ�ң���ô�ұ�Ӧ����down״̬ 
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
			//��A��ʼ������A�Ǽٱң��������
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
















