摘自：算法笔记

## 全排列问题

求 1- n的全排列

generateP(int index)； 假设当前已经填好了p[1]~p[index - 1],正准备向p[index]位置填入数字。显然需要枚举1~n填入，但是需要当前数字x不在p[1] ~ p[index-1]中，所以就需要通过hashTable[x]==false来判断。如果可以填入，则p[index]=x，并将hashTable[x]=true;  然后通过递归处理index+1位。递归结果后需要设置hashTable[x]=false。

递归边界：index == n + 1 表示前面 1~ index 已经填好数据。

```java
public class MyTest {
	//举个例子，求123的全排列
	int[] p = new int[11];
	boolean[] hasTable = new boolean[11];//这个数组可以使用map代替
	int n;
	//当前处理到第index号位，默认 1~index-1 已经放好数字了
	void generateP(int index){
		if(index == n ){
			for (int i = 0; i < n ; i++) {
				System.out.print(p[i]);
			}
			System.out.println();
			return;
		}
//		for (int x = 0; x < n; x ++){  //求0到n-1的全排列
		for (int x = 1; x <= n; x ++){
			if(hasTable[x] == false){//如果x不在 p[1]~p[index]之中
				p[index] = x;
				hasTable[x] = true;
				generateP(index + 1);
				hasTable[x] = false;//x放到index位已经处理完，还原状态，这样x可以放到index+1之后的位置了
			}
		}
	}
	@Test
	public void test(){
	    n = 3;
	    generateP(0);
	}
}

```



### n皇后问题(leetcode 51)

https://leetcode-cn.com/problems/n-queens/

给定一个整数 n，返回所有不同的 n 皇后问题的解决方案。每一种解法包含一个明确的 n 皇后问题的棋子放置方案，该方案中 'Q' 和 '.' 分别代表了皇后和空位。

```
输入：4
输出：[
 [".Q..",  // 解法 1
  "...Q",
  "Q...",
  "..Q."],

 ["..Q.",  // 解法 2
  "Q...",
  "...Q",
  ".Q.."]
]
解释: 4 皇后问题存在两个不同的解法。
```



```java
package perday;
import org.junit.Test;
import java.util.ArrayList;
import java.util.List;

public class LeetCode51 {
	List<List<String>> res = new ArrayList<>();
	public List<List<String>> solveNQueens(int n) {
		N = n;
		p = new int[N];
		hashTable = new boolean[N];
		for (int i = 0; i < N; i++) {
			st += ".";
		}
		generateP(0);
		return res;
	}

	String st = "";
	//每行都有一个皇后，index代表第index行的皇后，
	int[] p;//p[index]代表 index行的那个皇后所在的列数
	boolean[] hashTable;
	//假设1 ~ index - 1 的皇后已经排好了，并且没有冲突
	int N;// N 个皇后
	int count = 0;
	void generateP(int index){
		if(index == N){ //
			count ++;
			List<String> tmp = new ArrayList<>(N + 1);
			for (int i = 0; i < N; i++) {
				StringBuilder str = new StringBuilder(st);
				str.setCharAt(p[i],'Q');
				tmp.add(str.toString());
			}
			res.add(tmp);
			return;
		}
		for (int x = 0; x < N; x ++){
			if(hashTable[x] == false){
				boolean flag = true; //表示第index行的皇后放到x位置不会有冲突
				for(int i = 0; i < index; i ++){
					if(Math.abs(index - i) == Math.abs(x - p[i])){//在对角线上了，有冲突
						flag = false;
						break;
					}
				}
				if(flag){ //没有冲突才进行递归
					hashTable[x] = true;
					p[index] = x;
					generateP(index + 1);
					hashTable[x] = false;
				}
			}
		}
	}
	@Test
	public void test(){
		System.out.println(solveNQueens(4));
		System.out.println(count);
	}
}
```



### n皇后问题2（leetcode 52）

给定一个整数 *n*，返回 *n* 皇后不同的解决方案的数量。

同上。



## 二分查找

```java
package perday;
import org.junit.Test;
public class MyTest {

	//返回x对应的下标
	//查找的二分区间为[left, right],
	int binarySearch(int[] a, int left, int right, int x){
		int mid;
		while(left <= right){
			mid = (left + right) / 2;
			if(a[mid] == x){ //查找成功
				return mid;
			}
			if(a[mid] > x){ //x在[left, mid)
				right = mid - 1;
			}else {
				left = mid + 1;
			}
		}
		return -1;
	}

	@Test
	public void test(){
		int[] a = {1,2,4,5,6,9,10};
		System.out.println(binarySearch(a, 0, a.length-1, 2));//1
		System.out.println(binarySearch(a, 0, a.length-1, 6));//4
		System.out.println(binarySearch(a, 0, a.length-1, 7));//-1
	}
}

```

### Deformation

#### lower_bound 

在A中找到第一个大于等于x的位置

```java
package perday;

import org.junit.Test;

import java.util.Hashtable;

/**
 * @author TylerChen
 * @date 2020/12/12 - 12:19
 */
public class MyTest {
	int lowerBound(int[] a, int left, int right, int x){
		int mid;
		while(left < right){
			mid = (left + right) / 2;
			if(a[mid] >=  x){ //第一个大于等于x的数还有可能在左边
				right = mid;//
			}else {
				left = mid + 1;
			}
		}
		return left;
	}

	@Test
	public void test(){
		int[] a = {1,1,2,2,5,5};
		//如果这个数存在，则找到的是第一个满足条件的数的下标
		System.out.println(lowerBound(a, 0, a.length, 1));//0
		//如果这个数不存在，则返回这个数应该插入的位置
		System.out.println(lowerBound(a, 0, a.length, 4));
		System.out.println(lowerBound(a, 0, a.length, 0));
		System.out.println(lowerBound(a, 0, a.length, 6));
	}
}
```

模板：解决“寻找有序序列第一个满足条件的元素的位置”问题的模板

```java
int solve(int[] a, int left, int right, int x){
		int mid;
		while(left < right){
			mid = (left + right) / 2;
			if(条件){ //第一个满足条件的位置还有可能在左边
				right = mid;//
			}else {
				left = mid + 1;
			}
		}
		return left;
	}
```

> **如果要寻找最后一个满足条件C元素的位置，可以先求第一个满足条件!C的位置，然后再将这个位置减一即可。**



### 计算右侧小于当前元素的个数 (leetcode 315)

https://leetcode-cn.com/problems/count-of-smaller-numbers-after-self/

给定一个整数数组 nums，按要求返回一个新数组 counts。数组 counts 有该性质： counts[i] 的值是  nums[i] 右侧小于 nums[i] 的元素的数量。

```
示例：

输入：nums = [5,2,6,1]
输出：[2,1,1,0] 
解释：
5 的右侧有 2 个更小的元素 (2 和 1)
2 的右侧仅有 1 个更小的元素 (1)
6 的右侧有 1 个更小的元素 (1)
1 的右侧有 0 个更小的元素
```

思路：使用二分法，否则会超时。

从最后一个元素x开始，找其右侧元素集合中小于x的个数，其右侧集合用temp表示，最后一个寻找完毕之后，将x加入temp中，以供倒数第二个元素查找。

查找时使用二分法，这样也可以顺便得到 查找元素应该在temp中的位置， 这样使用temp.add就可以将对应元素加入，得到的temp仍然是一个有序的序列。

```java
class Solution {
  public List<Integer> countSmaller(int[] nums) {
		List<Integer> res = new ArrayList<>(nums.length + 1);
		//每次都从temp中查找符合条件的元素
		List<Integer> temp = new ArrayList<>();
		for (int i = nums.length - 1; i >= 0; i--) {
			int index = lowerBound(temp, nums[i]);
			res.add(index);
			temp.add(index, nums[i]);
		}
		Collections.reverse(res);
		return res;
	}
	//找到第一个满足 >= x的下标，也就是x应该在temp中的位置，还是<x 元素的个数
	int lowerBound(List<Integer> list, int x) {
		int left = 0, right = list.size();
		while (left < right) {
			int mid = (left + right) / 2;
			if (list.get(mid) >= x) {
				right = mid;
			} else {
				left = mid + 1;
			}
		}
		return left;
	}
}
```

## 排序

### 归并排序

将序列两两分组，组内单独排序，然后再将这些组合并

首先是两个有序区间的合并

```java
//将数组[l1,r1]与[l2,r2]区间合并为有序区间（此处l2即为r1+1）
	void merge(int[] a, int l1, int r1, int l2, int r2){
		int i = l1, j = l2;
		int[] temp = new int[a.length];
		int index = 0;
		while (i <= r1 && j <= r2){
			if(a[i] <= a[j]){ //将小的加入到temp[index]
				temp[index ++] = a[i ++];
			}else {
				temp[index ++] = a[j ++];
			}
		}
		while (i <= r1){
			temp[index ++] = a[i ++]; //可能会有剩余
		}
		while (j <= r2){
			temp[index ++] = a[j ++];
		}
		//将temp中的排好序的元素赋值给a
		for (i = 0; i < index; i ++){
			a[l1 + i] = temp[i];
		}
	}
```

#### 递归实现

```java
public class MyTest {
	//在当前区间[left, right]进行归并排序
	void mergeSort(int a[], int left, int right){
		if(left < right){
			int mid = (left + right) / 2;
			mergeSort(a, left, mid);
			mergeSort(a, mid + 1, right);
			merge(a, left, mid, mid + 1, right);
		}
	}
	@Test
	public void test(){
		int[] a = {1,2,7,3,5,9,-1,0,12,11};
		mergeSort(a, 0, a.length - 1);
		System.out.println(Arrays.toString(a));
	}

}
```

#### 非递归实现

```java
void mergeSort(int[] a) {
		//step 为组内元素的个数，step / 2 为左子区间元素的个数
		for (int step = 2; step < a.length; step *= 2) {
			for (int i = 0; i < a.length; i += step) {
				int mid = i + step / 2 - 1;
				if (mid + 1 < a.length) {
					merge(a, i, mid, mid + 1, Math.min(i + step - 1, a.length - 1));
				}
			}
		}
	}
```

### 快速排序

第一轮，a[] , left = 0, right = a.length - 1

令 a[left]为主元，从right开始，如果a[right] > 主元，则right --; 等循环结束，a[right] <= 主元，将a[right]中的元素放到a[left]中。如果a[left] <= 主元，则left ++; 等循环结束 a[left] > 主元，然后将a[right] = a[left]；反复上面过程。结束后，中间剩余的那个位置就是主元应该在的位置，左边的元素都比它小，右边的都比他大。

```java
package perday;

import org.junit.Test;

import java.util.Arrays;
import java.util.Hashtable;

/**
 * @author TylerChen
 * @date 2020/12/12 - 12:19
 */
public class MyTest {

	//对区间进行划分，返回主元应该的位置
	int partation(int[] a, int left, int right) {
		int temp = a[left];//a[left]为主元
		while (left < right) {
			//把小于等于主元的元素，换到左边
			while (left < right && a[right] > temp) {
				right--;
			}
			a[left] = a[right];
			//把大于主元的元素，换到左边
			while (left < right && a[left] <= temp) {
				left++;
			}
			a[right] = a[left];
		}
		a[left] = temp;
		return left;
	}

	void quickSort(int a[], int left, int right) {
		if (left < right) {
			int pos = partation(a, left, right);
			quickSort(a, left, pos - 1);
			quickSort(a, pos + 1, right);
		}
	}

	void quickSort(int a[]) {
		quickSort(a, 0, a.length - 1);
	}

	@Test
	public void test() {
		int[] a = {1, 2, 7, 3, 5, 9, -1, 0, 12, 11};
		quickSort(a);
		System.out.println(Arrays.toString(a));
	}
}
```

## 数学问题

### 最大公约数

辗转相除法（欧几里得算法）：两个整数的最大公约数等于其中较小的那个数和两数相除余数的最大公约数。设a,b均为正整数，则gcd(a,b)=gcd(b,a%b)

```java
int gcd(int a, int b){
		//a为较大的那个数
		int temp = Math.max(a, b);
		a = temp;
		b = a + b - temp;
		while (a % b != 0){
			int c = a % b;
			a = b;
			b = c;
		}
		return b;
	}

	//使用递归
	int gcd2(int a, int b){
		if(b == 0){
			return a;
		}else {
			return gcd(b, a % b);
		}
	}
```

### 最小公倍数

a, b的最小公倍数为：a * b / gcd(a,b)；

### 分数的四则运算

分数的表示：

```java
class Fraction {
   int up,down;
}
```

规则：

- down为非负数。如果down为负数，那么令up为负数。
- 如果改分数为0，规定其分子为0，分母为1。
- 分子和分母没有除了1以外的公约数。

分时的化简，以及四则运算：

```java
package perday;

/**
 * @author TylerChen
 * @date 2020/12/13 - 14:31
 */
public class Fraction {
	int up, down;
	Fraction(int up, int down){
		this.up = up;
		this.down = down;
	}
	Fraction(){};
	int gcd(int a, int b){
		int temp = Math.max(a, b);
		a = temp;
		b = a + b - temp;
		while (a % b != 0){
			int c = a % b;
			a = b;
			b = c;
		}
		return b;
	}
	//分式化简
    //按照上面的规则，进行化简
	Fraction reduction(Fraction res){
		if(res.down < 0){
			res.up = -res.up;
			res.down = -res.down;
		}
		if(res.up == 0){
			res.down = 1;
		}else {
			int d = gcd(Math.abs(res.up), Math.abs(res.down));
			res.up /= d;
			res.down /= down;
		}
		return new Fraction(res.up,res.down);
	}

	//加法 a/b + c/d = (ad + bc)/bd 
	Fraction add(Fraction f1, Fraction f2){
		Fraction res = new Fraction();
		res.up = f1.up * f2.down + f2.up * f1.down;
		res.down = f1.down * f2.down;
		return reduction(res);
	}
    //减法，乘法，除法略。
}

```

### 素数

素数：除了1和本身以外不能被其他数整除的数

#### 素数的判断

```java
boolean isPrime(int a){
    if(a <= 1) {
        return false;
    }
    int sqrt = (int) Math.sqrt(a);
    for (int i = 0; i <= sqrt; i++) {
        if(a % i == 0){
            return false;
        }
    }
    return true;
}
```

如果k是a的因子，那么a/k, 也是a的因子，并且 k 和 a/k 一个大于等于sqrt(a)，另外一个小于等于sqrt(a)；也就是说，在sqrt(a) 两边，如果有因子，则一定是成对出现的，所以只需要遍历到sqrt(a)即可。时间复杂度为O(logn)

#### 获取素数表

Eratosthenes筛选法，时间复杂度O(nloglogn)

只需要确定2是素数即可，例如求1~15的素数

2是素数，2的倍数都不是素数，所以剩下的数为

2 3 5 7 9 11 13 15

3没有被筛选掉，所以3也是素数，把3的倍数都去掉

2 3 5 7 11 13

5 没有被筛选掉，所以5也是素数，把5的倍数筛选掉。

。。。。。

最终可以得到一个素数表

2 3 5 7 11 13

```java
public class MyTest {
	//寻找1 ~ n的素数表
	List<Integer> findPrime(int n){
		//flag[index] = false,代表index是宿舍，假设初始全都是素数
		//默认2是素数
		boolean[] flag = new boolean[n + 1];
		List<Integer> res = new ArrayList<>();
		for (int i = 2; i <= n; i ++){
			if(flag[i] == false){
				res.add(i);
				for (int j = i + i; j <= n ; j += i) {
					flag[j] = true;
				}
			}
		}
		return res;
	}
	@Test
	public void test(){
		System.out.println(findPrime(15));
	}
}
```

### 质因子分解

所谓质因子就是将一个正整数n写成一个或者多个质数的乘积的形式，如6=2\*3， 8=2\*2\*2

**结论：**对于一个正数n，如果它存在[2, n]范围内的质因子，要么这些质因子全部小于等于sqrt(n)，要么只有一个大于sqrt(n)，（如果有2个，那么相乘之后就大于n了）

思路：1，枚举1~sqrt(n)范围内的所有质因子p，判断p是否为n的因子，如果是，则令n/=p，直到p不是n的质因子为止。n/=p的次数，就是质因子p的个数。

​		   2，如果经过步骤1 n仍然大于1，则说明n，有且仅有一个大于sqrt(n)的质因子，就是此时的n。

```java
package perday;

import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

/**
 * @author TylerChen
 * @date 2020/12/13 - 15:19
 */
public class Factor {
	static class Fac{
		int x;//质因子
		int count;//质因子对应的个数
	}
	Fac[] fac = new Fac[10];
    //考虑到2*3*5*7*11*13*17*19*23*29就超过int范围，所以，数组开到10就可以了
	int num = 0;
	//寻找某个数的质因子
	void findFac(int n){
		System.out.print(n + " = ");
        num = 0;
		List<Integer> prime = findPrime(20);
		int sqrt = (int) Math.sqrt(n);
		for (Integer p : prime){
			if(n % p == 0){
				fac[num] = new Fac();
				fac[num].x = p;
				fac[num].count = 0;
				while (n % p == 0){
					n /= p;
					fac[num].count ++;
				}
				num ++;
			}
		}
		if(n != 1){
			fac[num] = new Fac();
			fac[num].x = n;
			fac[num].count = 1;
			num ++;
		}

		printFac();
	}

	void printFac(){
		for (int i = 0; i < num; i++) {
			System.out.print(fac[i].x + "^" + fac[i].count);
			if(i != num - 1){
				System.out.print(" * ");
			}
		}
	}


	//寻找素数表
	List<Integer> findPrime(int n){
		boolean[] flag = new boolean[n + 1];
		List<Integer> res = new ArrayList<>();
		for (int i = 2; i <= n ; i++) {
			if(flag[i] == false){
				res.add(i);
				for (int j = i * i;j <= n; j += i){
					flag[j] = true;
				}
			}
		}
		return  res;
	}
	@Test
	public void test(){
	    findFac(180);//180 = 2^2 * 3^2 * 5^1
	}
}
```



## 大整数运算

### 存储

```java
// 如123456 则d[0]=6 d[1]=5
int[] d = new int[1000];
int len;
```

### 比较大小

1. 如果两个bigint长度不相同，则长的大。
2. 相同则从高位往到低位进行比较，出现某位不相等则可以判断大小。
3. 遍历完还没有比较出来大小，则相等

### 四则运算

注意：这里的四则运算都是对于正整数而言的，如果是负数，可以处理完了再用四则运算，然后在处理一下

和小学列式子计算式一样的，需要有一个进位。

这种写法式对于两个对象都是正数而言的，如果一个数是负数，则可以使用高精度减法。如果都是负数，则去掉➖相加后再加上负号。

```java
//高精度加法/
//和小学算加法一样
public static MyBigInt add(MyBigInt a, MyBigInt b){
    MyBigInt res = new MyBigInt();
    int carry = 0;//carry 是进位
    for(int i = 0; i < a.len || i < b.len; i ++){
        int temp = a.d[i] + b.d[i] + carry;
        res.d[res.len ++] = temp % 10;
        carry = temp / 10;
    }
    //如果最后进位不为0，则直接赋值给结果最高的那一位
    if(carry != 0){
        res.d[res.len ++] = carry;
    }
    return  res;
}
```

乘法 高精度147 乘以 35

![image-20201214104348138](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201214104348138.png)



除法

![image-20201214105220547](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201214105220547.png)

**归纳：**上一步的余数除以10加上该步的位，得到该步临时的被除数，将其与除数比较：如果不够除，则改位的商位0；如果够除，则上即为对应的上，余数为对应的余数。最后要注意，减去高位可能有多余的0

### 完整代码

```java
package perday;

import org.junit.Test;
import java.util.Arrays;

/**
 * @author TylerChen
 * @date 2020/12/14 - 10:02
 */
public class MyBigInt implements Comparable<MyBigInt> {
	int[] d = new int[1000];
	int len;

	public MyBigInt() {

	}

	public MyBigInt(String str) {
		this.len = str.length();
		for (int i = 0; i < str.length(); i++) {
			this.d[i] = str.charAt(str.length() - i - 1) - '0';
		}
	}

	//通过string获取大整数
	// 如123456 则d[0]=6 d[1]=5
	MyBigInt getByStr(String str) {
		MyBigInt big = new MyBigInt();
		big.len = str.length();
		for (int i = 0; i < str.length(); i++) {
			big.d[i] = str.charAt(str.length() - i - 1);
		}
		return big;
	}

	@Override
	public int compareTo(MyBigInt o) {
		if (this.len > o.len) {
			return 1;
		} else if (this.len < o.len) {
			return -1;
		} else {
			for (int i = this.len - 1; i >= 0; i--) {
				if (this.d[i] > o.d[i]) {
					return 1;
				} else if (this.d[i] < o.d[i]) {
					return -1;
				}
			}
		}
		return 0;
	}

	//高精度加法/
	//和小学算加法一样
	public static MyBigInt add(MyBigInt a, MyBigInt b) {
		MyBigInt res = new MyBigInt();
		int carry = 0;//carry 是进位
		for (int i = 0; i < a.len || i < b.len; i++) {
			int temp = a.d[i] + b.d[i] + carry;
			res.d[res.len++] = temp % 10;
			carry = temp / 10;
		}
		//如果最后进位不为0，则直接赋值给结果最高的那一位
		if (carry != 0) {
			res.d[res.len++] = carry;
		}
		return res;
	}

	//对于减法，a一定要比b大，
	public static MyBigInt sub(MyBigInt a, MyBigInt b) {
		MyBigInt res = new MyBigInt();
		for (int i = 0; i < a.len || i < b.len; i++) {
			if (a.d[i] < b.d[i]) {//不够减需要向高位借位
				a.d[i + 1]--;
				a.d[i] += 10;//借1当10
			}
			res.d[res.len++] = a.d[i] - b.d[i];
		}
		//去掉高位的0;
		while (res.len - 1 >= 1 && res.d[res.len - 1] == 0) {
			res.len--;
		}
		return res;
	}
    
    //高精度与低精度的乘法
	public static MyBigInt muti(MyBigInt a, int b) {
		MyBigInt res = new MyBigInt();
		int carry = 0;
		for (int i = 0; i < a.len; i++) {
			int temp = a.d[i] * b + carry;
			//个位作为改位的结果
			res.d[res.len++] = temp % 10;
			//高位作为进位
			carry = temp /= 10;
		}
		//和加法不一样，乘法的进位可能不止一位，
		while (carry != 0) {
			res.d[res.len++] = carry % 10;
			carry /= 10;
		}
		return res;
	}

	public static MyBigInt divide(MyBigInt a, int b) {
		int r = 0;
		MyBigInt res = new MyBigInt();
		res.len = a.len;
		for (int i = a.len - 1; i >= 0; i--) {
			r = r * 10 + a.d[i];
			if(r < b){ //不够除
				res.d[i] = 0;
			}else {
				res.d[i] = r / b;
				r = r % b;
			}
		}

		while (res.len - 1 >= 1 && res.d[res.len - 1] == 0){
			res.len --;
		}
		return res;
	}

	

	@Override
	public String toString() {
		StringBuilder stringBuilder = new StringBuilder();
		for (int i = len - 1; i >= 0; i--) {
			stringBuilder.append(d[i]);
		}
		return stringBuilder.toString();
	}
	public static void main(String[] args) {
		MyBigInt a1 = new MyBigInt("123");
		MyBigInt a2 = new MyBigInt("458");
		MyBigInt a3 = new MyBigInt("121");
		MyBigInt a4 = new MyBigInt("102");
		MyBigInt[] aa = {a1, a2, a3, a4};
		Arrays.sort(aa);
		System.out.println(Arrays.toString(aa));//[102, 121, 123, 458]
		System.out.println(add(a1, a4));//225
		System.out.println(sub(a1, a4));//21
		System.out.println(muti(a1, 2));//246
		System.out.println(divide(a1, 6));//20

	}
}
```



## 搜索专题

### DFS 深度优先搜索

问题：给定n个整数，从中选选择k个数，使得这k个数之和恰好等于一个给定的数x；如果有多种方案，选择他们中元素平方和最大的一个。例如：从{2，3，3，4}中选择2个数，使得他们的和为6，显然有2中方案{2，4}与{3，3}，其中平方和最大的方案为{2，4}；

```java
package perday;

import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

public class MyTest {
	//从n个数中选择k个数使得和为x，最大平方和为maxSumSqu
	int n = 4, k = 2, x = 6, maxSumSqu = -1;
	int[] a = {2, 3, 3, 4};
	ArrayList<Integer> temp = new ArrayList<>(), ans = new ArrayList<>();
	/**
	 * @param index  当前处理的整数编号
	 * @param nowK   已经选择数的个数
	 * @param sum    选择整数和
	 * @param sumSeq 平方和
	 */
	void DFS(int index, int nowK, int sum, int sumSeq) {
		if(nowK == k && sum == x){
			if(sumSeq > maxSumSqu){
				maxSumSqu = sumSeq;
				ans = new ArrayList<>(temp); //更新最优方案
			}
			return;
		}

		if(index == n || nowK > k || sum > x){
			return;
		}
		//选择index号的数
		temp.add(a[index]);
		DFS(index + 1, nowK + 1, sum + a[index], sumSeq + a[index] * a[index]);
		//不选index号的数
		temp.remove(temp.size() - 1);
		DFS(index + 1, nowK, sum , sumSeq);
	}

	@Test
	public void test(){
	    DFS(0, 0, 0, 0);
		System.out.println(ans);
	}
}
```

修改题目为：假设N个整数中的每一个数都可以被选择多次。

这个问题只需要对原来的代码进行少量的修改即可。由于每个整数都可以被选择多次，因此当选择了index号数时，不应该直接进入index+1号数的处理。显然，应当能够继续选择index号的数，因此只需要把“选择index号数”这条分支的代码修改为`DFS(index + 1, nowK + 1, sum + a[index], sumSeq + a[index] * a[index]);`即可

### BFS 广度优先搜索

广度优先遍历一般使用队列来实现，基本写法如下模板：

```java
void BFS(T s){
    Queue<T> q;
    q.push(s);
    设置为已入队;
    while(!q.isEmpty()){
        取出队首元素;
        访问;
        队首元素出队;
        top的下一层节点未曾入队列的节点全部入队列，并设置为已入队;
    }
}
```

例题：给出一个m*n的矩阵，矩阵中的元素为0或1.称位置（x,y）与其上下左右四个位置相邻。如果矩阵中有若干个1是相邻的（不必两两相邻），那么称这些1构成了一个“块”。求给定矩阵中块的个数。

```
{0,1,1,1,0,0,1},
{0,0,1,0,0,0,0},
{0,0,0,0,1,0,0},
{0,0,0,1,1,1,0},
{1,1,1,0,1,0,0},
{1,1,1,1,0,0,0}

//对于这个矩阵，块数为4
```

```java
package perday;

import org.junit.Test;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

public class MyTest {
	static class Node {
		int x, y;

		Node(int xx, int yy) {
			x = xx;
			y = yy;
		}
	}

	int[][] matrix = {
			{0, 1, 1, 1, 0, 0, 1},
			{0, 0, 1, 0, 0, 0, 0},
			{0, 0, 0, 0, 1, 0, 0},
			{0, 0, 0, 1, 1, 1, 0},
			{1, 1, 1, 0, 1, 0, 0},
			{1, 1, 1, 1, 0, 0, 0}
	};
	boolean[][] inq = new boolean[6][7];
	int m = 6, n = 7;// m * n的矩阵
	int[] X = {0, 0, 1, -1};
	int[] Y = {1, -1, 0, 0};

	//判断某个坐标是否需要判断
	//1，越界  2，当前位置已经进入过队列 3,当前位置是0 不是 1
	boolean judge(int x, int y) {
		if (x >= m || y >= n || x < 0 || y < 0) {
			return false;
		}
		if (matrix[x][y] == 0 || inq[x][y]) {
			return false;
		}
		return true;
	}

	void BFS(int x, int y) {
		Queue<Node> q = new LinkedList<>();
		Node a = new Node(x, y);
		q.offer(a);
		inq[x][y] = true;
		while (!q.isEmpty()) {
			Node p = q.poll();
			for (int i = 0; i < X.length; i++) {
				int newX = p.x + X[i];
				int newY = p.y + Y[i];
				if (judge(newX, newY)) {
					q.offer(new Node(newX, newY));
					inq[newX][newY] = true;
				}
			}
		}
	}

	@Test
	public void test() {
		int ans = 0;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				if (matrix[i][j] == 1 && inq[i][j] == false) {
					ans++;
					BFS(i, j);
				}
			}
		}
		System.out.println(ans);//
	}
}
```





## 树

### 二叉树

#### 根据先跟和中跟创建二叉树

**结论：中序序列可以与先序序列、后序序列、层次序列中的任意一个来构建唯一的二叉树。而后者两两搭配或是三个一起都无法构建唯一的二叉树。** 原因是先序、后序、层次均是提供根节点，作用是相同的，都必须由中序序列来区分出左右子树。

#### 先中后根遍历、层次遍历

```java
package perday;

import org.junit.Test;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;
import java.util.Queue;
public class BinaryTree {
	static class Node {
		int data;
		int layer;//该节点所在的层次
		Node left, right;
	}
	//根据先根和后跟创建一棵二叉树
	int[] pre, inOr;
	Map<Integer, Integer> inOrToIndex = new HashMap<>();
	Node create(int[] pre, int[] inOr) {
		this.pre = pre;
		this.inOr = inOr;
		for (int i = 0; i < inOr.length; i++) {
			inOrToIndex.put(inOr[i], i);
		}
		return create(0, pre.length - 1, 0, inOr.length - 1);
	}
	Node create(int preL, int preR, int inL, int inR) {
		if (preL > preR) {//如果先序长度小于等于0，直接返回
			return null;
		}
		Node root = new Node();
		root.data = pre[preL];//
		//获得先跟第一个节点在后跟中的位置
		int k = inOrToIndex.get(pre[preL]);
		//所以当前root左子树的节点个数为
		int numLeft = k - inL;
		//左子树的先序区间为 [preL, preL + numLeft] 中序区间为
		root.left = create(preL + 1, preL + numLeft, inL, k - 1);
		//右子树的先序区间为[preL+numLeft + 1,preR] 中序区间为[k + 1,inR]
		root.right = create(preL + numLeft + 1, preR, k + 1, inR);
		return root;
	}

	void preOrder(Node root){
		if(root == null){
			return;
		}
		System.out.print(root.data + " ");
		preOrder(root.left);
		preOrder(root.right);
	}
	void inOrder(Node root){
		if(root == null){
			return;
		}
		inOrder(root.left);
		System.out.print(root.data + " ");
		inOrder(root.right);
	}
	void postOrder(Node root){
		if(root == null){
			return;
		}
		postOrder(root.left);
		postOrder(root.right);
		System.out.print(root.data + " ");
	}
	void layerOrder(Node root){
		Queue<Node> q = new LinkedList<>();
		q.offer(root);
		root.layer = 1;
		while (!q.isEmpty()){
			Node p = q.poll();
			System.out.print(p.data + "(" + p.layer + ")" + " ");
			if(p.left != null){
				p.left.layer = p.layer + 1;
				q.offer(p.left);
			}
			if(p.right != null){
				p.right.layer = p.layer + 1;
				q.offer(p.right);
			}
		}
	}


	@Test
	public void test(){
		int[] a = {1,2,4,5,3,6,7};
		int[] b = {4,2,5,1,6,3,7};
		Node root = create(a, b);
		preOrder(root);
		System.out.println();
		inOrder(root);
		System.out.println();
		layerOrder(root);
	}
}
```

### 普通树的定义

```java
class Node{
    int layer;//层号
    int data;
    List<Node> children;
}
```



### 二叉查找树BST

BST 又称排序二叉树，二叉搜索树。

定义：1，要么二叉查找树是一颗空树。

​		    2，要么二叉树由左右子树组成，其中左子树和右子树都是二叉查找树，且左子树上所有节点的数据域均小于或等于根节点的数据域，右子树上的所有节点的数据域均大于根节点的数据域。





### 平衡二叉树



## 并查集

并查集是一种维护集合的数据结构，并查集支持下面两个操作：

1，合并：合并2个集合。2，查找：判断两个元素是否在一个集合。

并查集使用一个数组来实现：`int father[n];`其中，father[i]表示元素i的父亲节点，如果father[i] = [i]则说明元素i是该集合的根节点。

```java
package perday;

import org.junit.Test;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

public class MyTest {
	int n = 10;
	int[] father = new int[10];

	/**
	 * 初始化，一开始每个元素都是一个独立的集合，因此需要所有的father[i]=i
	 */
	void init(){
		for (int i = 0; i < n; i++) {
			father[i] = i;
		}
	}

	/**
	 * 返回元素x所在集合的根节点(使用路径压缩)
	 * @param x
	 * @return
	 */
	int findFather(int x){
		int a = x;
		while (x != father[x]){
			x = father[x];
		}
		// 此时x = father[x] x即为传来参数a的根节点
		while (a != father[a]){
			int z = a;
			a = father[a];//a指向下一个节点
			father[z] = x;//原来a的父亲节点指向x，也就是跟根节点
		}
		return x;
	}

	/**
	 * 把a，和b所在的集合合并起来
	 * @param a
	 * @param b
	 */
	void union(int a, int b){
		int faA = findFather(a);
		int faB = findFather(b);
		if(faA != faB){
			father[faA] = faB;
		}
	}
}

```

### 朋友圈 LeetCode547

https://leetcode-cn.com/problems/friend-circles/

班上有 N 名学生。其中有些人是朋友，有些则不是。他们的友谊具有是传递性。如果已知 A 是 B 的朋友，B 是 C 的朋友，那么我们可以认为 A 也是 C 的朋友。所谓的朋友圈，是指所有朋友的集合。

给定一个 N * N 的矩阵 M，表示班级中学生之间的朋友关系。如果M[i][j] = 1，表示已知第 i 个和 j 个学生互为朋友关系，否则为不知道。你必须输出所有学生中的已知的朋友圈总数。

```
示例 1：
输入：
[[1,1,0],
 [1,1,0],
 [0,0,1]]
输出：2 
解释：已知学生 0 和学生 1 互为朋友，他们在一个朋友圈。
第2个学生自己在一个朋友圈。所以返回 2 。
```

```java
class Solution {
	//使用并查集
	int[] father = new int[201];
	void init(int n){
		for (int i = 0; i < n; i++) {
			father[i] = i;
		}
	}
	int findFather(int x){
		int a = x;
		while (x != father[x]){
			x = father[x];
		}
		//路径压缩
		while (a != father[a]){
			int z = a;
			a = father[a];
			father[z] = x;
		}
		return x;
	}
	void union(int a, int b){
		int faA = findFather(a);
		int fab = findFather(b);
		if (faA != fab){
			father[faA] = fab;
		}
	}

	public int findCircleNum(int[][] M) {
		int n = M.length;
		init(n);
		for (int i = 0; i < n; i ++){ //只用遍历一半就行了
			for (int j = i + 1; j < n; j ++){
				if(M[i][j] == 1){ //说明i和j是朋友
					union(i, j);
				}
			}
		}
		int ans = 0;
		for (int i = 0; i < n; i++) {
			if(i == father[i]){
				ans ++;
			}
		}
		return ans;
	}
}
```



## 堆

**堆是一棵完全二叉树**，树种的每个结点的值都不小于其左右孩子结点的值，那么称这样的堆为大顶堆。大顶堆堆一般用于优先队列的实现。

堆使用数组来表示，heap[1]代表堆顶元素。

### 创建堆

1，建立堆，
建立堆的过程总是从右到左从上到下 依此调整有孩子的节点成为最最大/小堆，时间复杂度O(n)

```java
//1，建立大顶堆，
//建立堆的过程总是从右到左从上到下 依此调整有孩子的节点成为最最大/小堆
void createHeap(int[] a) {
    n = a.length;
    for (int i = 1; i <= a.length; i++) {
        heap[i] = a[i - 1];
    }
    for (int i = n / 2; i >= 1; i --){
        downAdjust(i, n);
    }
}
//向下调整,范围[low,high]
//low为预调整节点的数组下标，high一般为堆的左后一个元素的数组下标
void downAdjust(int low, int high) {
		int i = low, j = i * 2;
		while (j <= high) {
			//使j指向较大的孩子节点
			if (j + 1 <= high && heap[j + 1] > heap[j]) {
				j = j + 1;
			}
			//如果孩子的权值比要i大，则可以调整，否则调结束
			if (heap[j] > heap[i]) {
				int temp = heap[j];
				heap[j] = heap[i];
				heap[i] = temp;
				i = j;
				j = 2 * i;
			} else {
				break;
			}
		}
	}
```

### 删除堆顶元素

如果删除堆中的最大元素，并让其保持堆的结构，只需要让最后一个元素覆盖堆顶元素，然后只需要调整堆顶元素即可（因为此时除了堆顶元素，其他节点都已经是最大堆）。时间复杂度O(logn)

```java
//删除堆顶元素
int deleteTop() {
    int top = heap[1];
    heap[1] = heap[n--];
    downAdjust(1, n);
    return top;
}
```

### 添加一个元素

如果要添加一个元素，可以把元素放到数组的最后面，然后不断的向上调整这个元素即可。

```java
//插入一个节点
void insert(int x){
    heap[++n] = x;
    upAdjust(1, n);
}
//对heap数组在[low,high]范围进行向上调整
//其中low一般设置为1，high表示欲调整的数组下标
void upAdjust(int low, int high){
    int i = high, j = i / 2;//j 是i 的父亲节点
    while (j >= low){
        //如果孩子的权值较大，则需要调整
        if(heap[i] > heap[j]){
            int temp = heap[j];
            heap[j] = heap[i];
            heap[i] = temp;
            i = j;
            j = i / 2;
        }else {
            break;
        }
    }
}
```

### 堆排序

- 建立最大堆可以进行升序排序
- 最小堆可以降序排序

对于一个最大堆来说，取出堆顶元素，然后将堆的最后一个元素替换至堆顶。在进行一次针对堆顶元素的向下调整。

```java
void heapSort(int[] a) {
		createHeap(a);
		for (int i = n; i >= 1; i--) {
			int temp = heap[i];
			heap[i] = heap[1];
			heap[1] = temp;
			downAdjust(1, i - 1);
		}
	}
```

### 完整代码

```java
package perday;

import org.junit.Test;

/**
 * @author TylerChen
 * @date 2020/12/15 - 9:54
 */
public class Heap {
	int maxn = 100;
	int[] heap = new int[maxn];
	int n;//堆种元素的个数

	//1，建立大顶堆，
	//建立堆的过程总是从右到左从上到下 依此调整有孩子的节点成为最最大/小堆
	void createHeap(int[] a) {
		n = a.length;
		for (int i = 1; i <= a.length; i++) {
			heap[i] = a[i - 1];
		}
		for (int i = n / 2; i >= 1; i--) {
			downAdjust(i, n);
		}
	}

	//删除堆顶元素
	int deleteTop() {
		int top = heap[1];
		heap[1] = heap[n--];
		downAdjust(1, n);
		return top;
	}

	//插入一个节点
	void insert(int x) {
		heap[++n] = x;
		upAdjust(1, n);
	}

	void heapSort(int[] a) {
		createHeap(a);
		for (int i = n; i >= 1; i--) {
			int temp = heap[i];
			heap[i] = heap[1];
			heap[1] = temp;
			downAdjust(1, i - 1);
		}
	}

	void print(){
		for (int i = 1; i <= n ; i++) {
			System.out.print(heap[i] + " ");
		}
	}

	@Test
	public void test(){
	    int[] a = {9,3,5,2,6,1,8,4,7};
	    createHeap(a);
	    print();//9 7 8 4 6 1 5 3 2
		System.out.println();

		deleteTop();
		print();// 8 7 5 4 6 1 2 3
		System.out.println();

		insert(7);
		print();// 8 7 5 7 6 1 2 3 4
		System.out.println();
		
		heapSort(a);
		print();//1 2 3 4 5 6 7 8 9
	}

	//向下调整,范围[low,high]
	//low为预调整节点的数组下标，high一般为堆的左后一个元素的数组下标
	void downAdjust(int low, int high) {
		int i = low, j = i * 2;
		while (j <= high) {
			//使j指向较大的孩子节点
			if (j + 1 <= high && heap[j + 1] > heap[j]) {
				j = j + 1;
			}
			//如果孩子的权值比要i大，则可以调整，否则调结束
			if (heap[j] > heap[i]) {
				int temp = heap[j];
				heap[j] = heap[i];
				heap[i] = temp;
				i = j;
				j = 2 * i;
			} else {
				break;
			}
		}
	}

	//对heap数组在[low,high]范围进行向上调整
	//其中low一般设置为1，high表示欲调整的数组下标
	void upAdjust(int low, int high) {
		int i = high, j = i / 2;//j 是i 的父亲节点
		while (j >= low) {
			//如果孩子的权值较大，则需要调整
			if (heap[i] > heap[j]) {
				int temp = heap[j];
				heap[j] = heap[i];
				heap[i] = temp;
				i = j;
				j = i / 2;
			} else {
				break;
			}
		}
	}
}
```



# 图

## Dijkstra算法 （算法笔记367）

Dijkstra算法解决**单源最短路径**问题，即给定图G(V,E)，和起点s，求从起点s到达其他顶点的最短距离

### 算法策略：

设置集合S存放已访问的顶点，然后执行n次下面2个步骤：

1. 每次从集合V-S（为被访问的顶点集合）中选择与起点s的最短距离最小的一个顶点（记为u），访问并加入集合S。
2. 之后，令顶点u为中介点，优化起点与所有从u能到达的顶点v之间的最短距离。

### 关键点：

1. 集合S可以用一个bool型的数组vis[] 来实现，即当vis[i] == true的时候，表示顶点Vi 已被访问，反之则未被访问。
2. 令int型数组d[] 表示起点s到达顶点Vi 的最短距离，**初始时除了起点d[s]赋值为0，其余顶点都赋值为一个很大的数来表示inf**（可以是100000000 10^9  也可以时 0x3fffffff (30位1)，不要使用0x7fffffff 因为2个这样的数相加可能会超出整数范围）。

### 模板伪代码

```java
初始化
for (循环n次){
    u = 使d[u]最小的且还未被访问的顶点;
    设置u被访问;
    for(从u出发能到达的所有顶点v){
        if(v没有被访问&&以u为中介使s到达顶点v的最短距离d[v]更优){
            优化d[v]
        }
    }
}
```

### 实现：C++

使用邻接表实现

```C++
const int maxv = 510; //最大点的个数
const int INF = 0x3fffffff;
struct Node {
    int v; //node的目标点
    int d;// 到达目标点的距离
};
vector<Node> G[maxv]; //图
int d[maxv];//最短路径，
bool visited[maxv] = {false};
int N, C1, C2;//N,点的个数，C1，起点
void dijkstra() {
    //使用dijkstra算法求最短路径
    fill(d, d + maxv, INF);
    d[C1] = 0;   
    for (int i = 0; i < N; ++i) {
        int u = -1, min = INF;
        //寻找从起点开始到达点的最小值
        for (int j = 0; j < N; ++j) {
            if (visited[j] == false && d[j] < min) {
                u = j;
                min = d[u];
            }
        }
        if (u == -1) { //找不到对应点了
            return;
        }
        visited[u] = true;//将这个点放到已访问集合中
        for (int v = 0; v < G[u].size(); v++) {
            Node node = G[u][v];
            if (visited[node.v] == false) {
                if (node.d + d[u] < d[node.v]) {
                    d[node.v] = node.d + d[u]; 
                }
            }
        }
    }
}
```

#### 求最短路径

使d[v]变得更小得方案是让u作为作为s到顶点v路径最短路径上v得前一个节点，不妨把这个信息记录下来。设置一个数组pre[], 令pre[v]表示从起点s到顶点v得最短路径上v的前一个顶点（即前驱结点）的编号。

做法：

```java
初始化
初始状态设置每一个点的前驱结点为自身
for(int i=0;i<n;i ++){
    pre[i]=i;
}
....
if(v没有被访问&&以u为中介使s到达顶点v的最短距离d[v]更优){
     优化d[v];
     pre[v] = u
}
```

通过递归打印最短路径

```c++
//求s到v的最短路径
void DFS(int s, int v){
    if(v == s){
        printf("%d",s);
        return;
    }
    DFS(s,pre[v]);
    printf("->%d",v);
}
```

#### 获得所有最短路径

使用一个线性表数组来记录所有最端路径

`vector<int> pre[maxv]` , 例如pre[v] = {1,2}表示从 起点s到达v 可以通过连个前驱到达，而且路径长度都是一样长的。(也可以使用treeset 这样对于查询某个顶点u是否再顶点v的前驱中的题目很方便 )

做法：

只需要在优化d[v]的时候做操作就可。  

```c++
for(int v = 0; v < n; v ++){
    if(vis[v] == false && G[u][v] != INF){
        if(d[u] + G[u][v] < d[v]){//以u为中介点可以使得d[v]更优
			d[v] = d[u] + G[u][v];
           	pre[v].clear();// 被清除的不是最短路径的前驱
           	pre[v].push_back(u);
        }else if(d[u] + G[u][v] == d[v]){
			pre[v].push_back(u);
        }
    }
}
```

获得所有最短路径：

```c++
//获取从s到达v的所有最短路径
vector< vector<int> >  allpath;
vector<int> temp;
void getALLShortest(int s, int v){
    if(v == s){   //到达了叶子结点st。即路径的起点
        temp.push_back(v);
        allpath.push_back(temp);
        //将temp倒序输出即可得到正确的路径
        for (int i = temp.size() - 1; i >= 0 ; --i) {
            printf("%d ",temp[i]);
        }
        printf("\n");
        temp.pop_back();
        return;
    }
    temp.push_back(v);
    for (int i = 0; i < pre1[v].size(); ++i) {
        int a = pre1[v][i];
        getALLShortest(s,a);
    }
    temp.pop_back();
}
```



#### 其他标尺

对于以下三种情况，都只需要增加一个数组来存放新增的边权或点权或最短路径的条数。然后只需要修改优化d[v]那部分代码即可。

##### 增加边权

给每条边在增加一个边权（例如花费），然后求在最短路径有多条的情况下要求路径上的花费之和最小

用`cost[u][v]`表示u->v的花费，增加一个数组c[]，令从起点s到达顶点u的最小花费为c[u]。**初始化的时候只有c[s]为0、其余c[u]均为INF**。

```c++
for(int v = 0; v < n; v ++){
    if(vis[v] == false && G[u][v] != INF){
        if(d[u] + G[u][v] < d[v]){//以u为中介点可以使得d[v]更优
			d[v] = d[u] + G[u][v];
            c[v] = c[u] + cost[u][v];//因为此时最短路径要经过u，所以花费必须经过u到达v 
        }else if(d[u] + G[u][v] == d[v]){
            if(c[u] + cost[u][v] < c[v]){//当前这个也是最短路径，看看经过当前u到达v的花费是否更小
                c[v] = c[u] + cost[u][v];
            }
        }
    }
}
```

##### 增加点权

给每个点增加一个点权（例如每个城市可以收集到的物资），然后再最短路径有多条时要求路径上的点权之和最大。

用weight[u]表示点权（城市中物资的个数），求最短路径情况下的最大点权（物资）。增加一个数组w[]，w[u]表示最短路径情况下从起点s到达点u的能收集到的最大物资数（最短路径下各个点权之和）。**初始化的时候w[s] = weight[s]、其余w[u]均为0**。

```c++
for(int v = 0; v < n; v ++){
    if(vis[v] == false && G[u][v] != INF){
        if(d[u] + G[u][v] < d[v]){//以u为中介点可以使得d[v]更优
			d[v] = d[u] + G[u][v];
            w[v] = w[u] + weight[v];//因为此时最短路径要经过u，所以收集到的物资必须从u到v 
        }else if(d[u] + G[u][v] == d[v]){
            if(w[u] + weight[v] > w[v]){//当前这个也是最短路径，看看经过当前u到达v的物资是否更多
                w[v] = w[u] + weight[v];
            }
        }
    }
}
```

##### 最短路径的条数

增加一个数组nums[]，nums[u]表示从起点s出发到达u最短路径的条数。**初始化时只有nums[s]=1，其余均为0**

```c++
for(int v = 0; v < n; v ++){
    if(vis[v] == false && G[u][v] != INF){
        if(d[u] + G[u][v] < d[v]){//以u为中介点可以使得d[v]更优
			d[v] = d[u] + G[u][v];
            nums[v] = nums[u]; //等于能到达u的所有最短路径个数
        }else if(d[u] + G[u][v] == d[v]){
            nums[v] += nums[u]; //加上能到达u的所有最短路径个数
        }
    }
}
```

#### 使用DFS获得其他标尺 Dijkstra + DFS

参看获得所有最端路径，可得递归树：

![image-20201218165443487](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201218165443487.png)

对这样一棵进行遍历的时候，每次到达叶子结点（起点），就会产生一条完整的路径。因此每次得到这样一条路径，就可以对这条路径计算其第二标尺（例如把路径上的边权或者是点权累加出来），令其与当前第二标尺的最优值进行比较。如果比当前最优值更优，则更新最优值，并用这条路径覆盖最优路径。这样当所有最短路径都遍历完毕后，就可以得到最短路径下的最优标尺了。

这样依赖，可以先通过dijkstra获得最短路径，以及其**前驱pre**，然后通过DFS即可求得题目所需要的标尺啦！

```C++
const int maxv = 510; //最大点的个数
const int INF = 0x3fffffff;
struct Node {
    int v; //node的目标点
    int d;// 到达目标点的距离
};
vector<Node> G[maxv]; //图
int d[maxv];//最短路径，
bool visited[maxv] = {false};
int N, C1, C2;//N,点的个数，C1，起点
void dijkstra() {
    //使用dijkstra算法求最短路径
    fill(d, d + maxv, INF);
    d[C1] = 0;   
    for (int i = 0; i < N; ++i) {
        int u = -1, min = INF;
        //寻找从起点开始到达点的最小值
        for (int j = 0; j < N; ++j) {
            if (visited[j] == false && d[j] < min) {
                u = j;
                min = d[u];
            }
        }
        if (u == -1) { //找不到对应点了
            return;
        }
        visited[u] = true;//将这个点放到已访问集合中
        for (int v = 0; v < G[u].size(); v++) {
            Node node = G[u][v];
            if (visited[node.v] == false) {
                if (node.d + d[u] < d[node.v]) {
                    d[node.v] = node.d + d[u]; 
                    pre[v].clear();
                    pre[v].push_back(u);
                }else if(node.d + d[u] == d[node.v]){
                     pre[v].push_back(u);
                }
            }
        }
    }
}
```

使用DFS 解题！伪代码如下：

```C++
int optvalue; //第二标尺的最优值
vector<int> pre[maxv];//存放结点的前驱结点
vector<int> temp,path; //临时路径、最优路径
void getALLShortest(int s, int v){ //v为当前访问的点
    if(v == s){   //到达了叶子结点s。即路径的起点
        temp.push_back(v);    
        int value;
        计算路径temp上面的value值;
        if(value优于optValue){
            optValue=value;
            path = temp;
        }
        temp.pop_back();
        return;
    }
    temp.push_back(v);
    for (int i = 0; i < pre1[v].size(); ++i) {
        int a = pre[v][i];
        getALLShortest(s,a);
    }
    temp.pop_back();
}
```

注意：通过这种做法也可以求最短路径的个数，可以开一个全局变量，每当到达叶子结点的时候令这个全局变量加1即可。



y





```shell
docker run -d \
     -v /root/nextcloud/html:/var/www/html \
      -v /root/nextcloud/apps:/var/www/html/custom_apps \
      -v /root/nextcloud/config:/var/www/html/config \
      -v /root/nextcloud/nextcloud/data:/var/www/html/data \
      -v /root/nextcloud/themes:/var/www/html/themes \
      -p 80:80 \
      --restart always \
      --name nextcloud \
      nextcloud:17-apache
      
      
      docker run -d -v ~/docker/nextcloud:/var/www/html -p 11180:80 --name nextcloud --restart always nextcloud
      
      
      docker run -d -p 11280:80 --name onlyoffice --restart always onlyoffice/documentserver
```

