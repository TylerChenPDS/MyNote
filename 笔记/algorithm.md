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

