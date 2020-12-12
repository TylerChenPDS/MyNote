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

