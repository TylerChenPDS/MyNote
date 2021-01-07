# 区间贪心

## 无重叠区间

https://leetcode-cn.com/problems/non-overlapping-intervals/

给定一个区间的集合，找到需要移除区间的最小数量，使剩余区间互不重叠。

注意:

可以认为区间的终点总是大于它的起点。
区间 [1,2] 和 [2,3] 的边界相互“接触”，但没有相互重叠。

**示例 1:**

```
输入: [ [1,2], [2,3], [3,4], [1,3] ]

输出: 1

解释: 移除 [1,3] 后，剩下的区间没有重叠。
```

**示例 2:**

```
输入: [ [1,2], [1,2], [1,2] ]

输出: 2

解释: 你需要移除两个 [1,2] 来使剩下的区间没有重叠。
```

 **思路**

这一题可以转化为区间贪心来做：即区间不相交问题：**给出N个开区间（x, y），从中选择尽可能多的开区间，使得这些开区间两两没有交集**。

1. 首先考虑最简单的情况，如果I1区间包含于I2区间，显然选择I1,因为I1较小，选择I1可以容纳选择其他更多的区间。

2. 接下来吧所有开区间按照左端点从大到小排序（如果左端点相等，则按照右端点从小到达排序）。排完序后，就可以从选择，第一次选择排名第一个的区间，第二次选择右端点小于上次选择左端点的那个区间，这样2次选择一定不会有重叠区间。如下图：

   ![image-20201231212930742](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201231212930742.png)

lastx = x1，（lastx表示上次选择区间的左端点），上述排序的结果就是第一个选择(x1, y1)这个区间。

对于第二个区间，x2 == x1 ，但是y2 >= y1 所以第二个区间一定包含第一个区间，不选第二个区间。

对于（x3, y3）, y3>x1，所以（x3, y3）和上次选择的那个区间（x1, y1）一定有重合。

对于（x4, y4） ，y4 < x1，所以（x4, y4）和上次选择的那个区间（x1, y1）没有重合，记lastx=x4，即上次选择的区间变成了（x4, y4）。

对于（x5，y5）,y5<x4，所以（x5,y5）和上次选择的那个区间（x4, y4）没有重合，即选择（x4, y4）至此总共选择了3个互不相交的区间。

对于上题，要去除的区间个数，就是总区间个数-不相交区间的个数

需要移除区间的最小数量 = 总区间个数-不相交区间最大个数

所以可以按照上面思路解题：

```java
public class Leetcode435 {
	public int eraseOverlapIntervals(int[][] intervals) {
		if(intervals.length == 0){
			return 0;
		}
		//按照左边地址从大到小排，如果左边地址相等，则按照从右边地址从小到达排
		//这样如果左边地址相等，选中的一定是区间比较小的那个
		Arrays.sort(intervals, (a,b) -> a[0] != b[0] ? b[0] - a[0] : a[1] - b[1]);
		int ans = 1; //选中互不相交区间的个数
		int lastX = intervals[0][0];//记录，上一个被选中区间的左端点
		for (int i = 1; i < intervals.length; i++) {
			//此时应该删除较大的那个区间
			if(intervals[i][1] <= lastX){ //说明，和上个被选中的区间不相交
				lastX = intervals[i][0];
				ans ++;
			}
		}
		return intervals.length - ans;
	}
}
```



# 队列

## 滑动窗口最大值

https://leetcode-cn.com/problems/sliding-window-maximum/

![image-20210106191129863](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210106191129863.png)





![image-20210106191146231](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210106191146231.png)



**思路**

这一题使用暴力法会超时

```java
public int[] maxSlidingWindow(int[] nums, int k) {
		int index = 0;
		int[] res = new int[nums.length];

		Queue<Integer> queue = new LinkedList<>();
		for (int i = 0; i < nums.length; i++) {
			queue.offer(nums[i]);
			if(queue.size() == k){
				res[index ++] = getMax(queue);
				queue.poll();
			}
		}
		return Arrays.copyOf(res, index);
	}

	int getMax(Queue<Integer> queue){
		int res = Integer.MIN_VALUE;
		for(Integer i : queue){
			res = Integer.max(res, i);
		}
		return res;
	}
```

可以使用双端队列来解决，首先声明一个双端队列`Deque<Integer> deque = new LinkedList<>();`

1. 队列的前端保存当前窗口最大值的下标
2. 当窗口向后滑动的时候，会进来一个新的数（的下标），如果这个数比队列末尾值大，则把末尾弹出，继续比较，直到这个数遇到比它大的数或者队列为空的情况，此时将这个数的下标，放到双端队列的末尾。此时，双端队列一定是有序的。 然后需要取出此时窗口中的最大值：判断当前队列中第一个元素是否在窗口中，如果在，则第一个元素就是，如果不在，则把第一个元素弹出。判断方法为：是否满足deque.getFirst() <= i - k ，如果满足，则第一个元素不在当前窗口。(i-k+1是当前窗口的左边界)

```java
public class LeetCode239_3 {
	public int[] maxSlidingWindow(int[] nums, int k) {
		int index = 0;
		int[] res = new int[nums.length];
		Deque<Integer> deque = new LinkedList<>();
		//初始化双端队列, 
		for (int i = 0; i < k; i++) {
			while (!deque.isEmpty() && nums[i] > nums[deque.getLast()]){
				deque.pollLast();
			}
			deque.addLast(i);
		}

		res[index ++] = nums[deque.getFirst()];

		for (int i = k; i < nums.length; i++) {
			if(!deque.isEmpty() && deque.getFirst() <= i - k){
				deque.pollFirst();
			}
			while (!deque.isEmpty() && nums[i] > nums[deque.getLast()]){
				deque.pollLast();
			}
			deque.addLast(i);
			res[index ++] = nums[deque.getFirst()];
		}
		return Arrays.copyOf(res, index);
	}
}
```

# 矩阵

## 旋转矩阵

https://leetcode-cn.com/problems/rotate-matrix-lcci/

给你一幅由 N × N 矩阵表示的图像，其中每个像素的大小为 4 字节。请你设计一种算法，将图像旋转 90 度。

不占用额外内存空间能否做到？ 

示例 1:

给定 matrix = 
[
  [1,2,3],
  [4,5,6],
  [7,8,9]
],

原地旋转输入矩阵，使其变为:
[
  [7,4,1],
  [8,5,2],
  [9,6,3]
]
示例 2:

给定 matrix =
[
  [ 5, 1, 9,11],
  [ 2, 4, 8,10],
  [13, 3, 6, 7],
  [15,14,12,16]
], 

原地旋转输入矩阵，使其变为:
[
  [15,13, 2, 5],
  [14, 3, 4, 1],
  [12, 6, 8, 9],
  [16, 7,10,11]
]

**思路**

题解如下

https://leetcode-cn.com/problems/rotate-matrix-lcci/solution/xuan-zhuan-ju-zhen-by-leetcode-solution/

![image-20210102133441911](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210102133441911.png)

如果n为奇数

![image-20210102134159821](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210102134159821.png)

经过观察，可得，旋转过后，左上的数字，会到右上，右上的会到，右下，右下的会到左下。

也就是说，旋转后:`m[row][col] 的数字会放到 m[col][n-row-1]`, 那么`m[col][n-row-1]`的数字会换到`m[n-row-1][n-col-1](其实就是将m[col][n-row-1]，看成m[row][col]， 带入第一个式子)`，...

可得四个位置分别为：`m[row][col],m[col][n-row-1],m[n-row-1][n-col-1],m[n-col-1][row]`

遍历的话就只需要遍历做上角就可以了

![image-20210103105210521](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210103105210521.png)

代码如下：

```java
public class S01_07 {
	public void rotate(int[][] matrix) {
		int n = matrix.length;
		for (int i = 0; i < n / 2; i++) { //i向下取整，
			for (int j = 0; j < (n + 1) / 2; j++) { //j向上取整
				int temp = matrix[i][j];
				matrix[i][j] = matrix[n - j - 1][i];//左上等于左下
				matrix[n - j - 1][i] = matrix[n - i - 1][n - j - 1];
				matrix[n - i - 1][n - j - 1] = matrix[j][n - i - 1];
				matrix[j][n - i - 1] = temp;
			}
		}
	}
}
```

# 字符串

## 字符串轮转

https://leetcode-cn.com/problems/string-rotation-lcci/

字符串轮转。给定两个字符串s1和s2，请编写代码检查s2是否为s1旋转而成（比如，waterbottle是erbottlewat旋转后的字符串）。

示例1:

 输入：s1 = "waterbottle", s2 = "erbottlewat"
 输出：True

示例2:

 输入：s1 = "aa", s2 = "aba"
 输出：False
提示：

字符串长度在[0, 100000]范围内。
说明:

你能只调用一次检查子串的方法吗？

### 解法1

```java
class Solution {
  
	public boolean isFlipedString(String s1, String s2) {
		if (s1.length() != s2.length()) {
			return false;
		}
		String a = s1 + s1;
		return a.contains(s2);
	}
}
```

### 解法2：kmp

看似花里胡哨，实则没用

```java
public class S01_09 {

	//获取next数组
	int[] getNext(String str) {
		int[] next = new int[str.length()];
		next[0] = -1;
		int j = -1;
		for (int i = 1; i < str.length(); i++) {
			while (j != -1 && str.charAt(i) != str.charAt(j + 1)) {
				j = next[j];
			}
			if(str.charAt(i) == str.charAt(j + 1)){
				j ++;
			}

			//
//			next[i] = j;


			//优化
			if(j == -1 || i < str.length() - 1 && str.charAt(i + 1) != str.charAt(j + 1)){
				next[i] = j;
			}else {
				next[i] = next[j];
			}

		}
		return next;
	}

	boolean kmp(String text, String pattern){
		if(pattern.equals("")){
			return true;
		}
		int[] next = getNext(pattern);
		int n = text.length();
		int m = pattern.length();
		int j = -1;
		for (int i = 0; i < n; i++) {
			while (j != -1 && text.charAt(i) != pattern.charAt(j + 1)){
				j = next[j];
			}

			if(text.charAt(i) == pattern.charAt(j + 1)){
				j ++;
			}

			if(j == m - 1){
				return true;
			}
		}
		return false;
	}

	public boolean isFlipedString(String s1, String s2) {
		if (s1.length() != s2.length()) {
			return false;
		}
		String a = s1 + s1;
		return kmp(a, s2);
	}
}
```





# 链表

## 快慢指针

### 返回倒数第 k 个节点

https://leetcode-cn.com/problems/kth-node-from-end-of-list-lcci/

实现一种算法，找出单向链表中倒数第 k 个节点。返回该节点的值。

注意：本题相对原题稍作改动

示例：

输入： 1->2->3->4->5 和 k = 2
输出： 4
说明：

给定的 k 保证是有效的。

#### 解法1：

先求长度，然后在重新走一次，走len-k步就可以了

```java
class Solution {
    public int kthToLast(ListNode head, int k) {
		int len = 0;
		ListNode p = head;
		while (p != null){
			len ++;
			p = p.next;
		}
		p = head;
		for (int i = 0; i < len - k; i++) {
			p = p.next;
		}
		return p.val;
	}
}
```

#### 解法2

快慢指针：

快指针先走k-1步，然后快慢指针一起走完快指针剩下的路，直到快指针指向最后一个元素，这样慢指针差k-1步到达最后一个元素，此时慢指针指向倒数第k个元素。

```java
class Solution {
   public int kthToLast(ListNode head, int k) {
		ListNode f = head, l = head;
		for (int i = 0; i < k - 1; i++) {
			f = f.next;
		}
		while (f.next != null){
			f = f.next;
			l = l.next;
		}
		return l.val;
	}
}
```



## 链表反转

https://mp.weixin.qq.com/s?__biz=MzU0ODMyNDk0Mw==&mid=2247488340&idx=1&sn=c3d6adc9f737672aab544931502dda2e&chksm=fb418074cc360962b46cb764068a5818f58bed6a4cd05ef61057823918d95f3a192550f02408&scene=21#wechat_redirect

### 方法一：使用栈

链表的反转是老生常谈的一个问题了，同时也是面试中常考的一道题。最简单的一种方式就是使用**栈**，因为**栈是先进后出**的。实现原理就是把链表节点一个个入栈，当全部入栈完之后再一个个出栈，出栈的时候在把出栈的结点串成一个新的链表。原理如下

![image-20210104114757346](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210104114757346.png)

```java
//反转链表的3种方式
//使用栈的方式 
public ListNode reverseList(ListNode head){
    Stack<ListNode> stack = new Stack<>();
    //把链表节点全部摘掉放到栈中
    while (head != null){
        stack.push(head);
        head = head.next;
    }
    ListNode res = stack.pop(), p = res;
    //栈中的结点全部出栈，然后重新连成一个新的链表
    while (!stack.isEmpty()){
        p.next = stack.pop();
        p = p.next;
    }
    //最后一个结点就是反转前的头结点，一定要让他的next
    //等于空，否则会构成环
    p.next = null;
    return res;
}
```

### 方法二：双链表

时间复杂度O(n)，空间复杂度O(1)

双链表求解是把原链表的结点一个个摘掉，每次摘掉的链表都让他成为新的链表的头结点，然后更新新链表。下面以链表1→2→3→4为例画个图来看下。

![image-20210104123159168](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210104123159168.png)



![image-20210104123224899](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210104123224899.png)

```java
ListNode reverseList2(ListNode head){
	//新的链表
    ListNode newHead = null;
    while (head != null){
        //先保存访问的节点的下一个节点，保存起来
        //留着下一步访问的
        ListNode temp = head.next;
        //每次访问的原链表头节点都会成为新链表的头结点，
        head.next = newHead;
        //更新新链表
        newHead = head;
        //重新赋值，继续访问
        head = temp;
    }
    return newHead;
}
```

### 方法三：使用递归

```java
ListNode reverseList3(ListNode head){
   if(head == null || head.next == null){
      return head;
   }
   //保存当前节点的下一个结点
   ListNode next = head.next;
   //从当前节点的下一个结点开始递归调用
   ListNode reverse = reverseList(next);
   //reverse是反转之后的链表，因为函数reverseList
   // 表示的是对链表的反转，所以反转完之后next肯定
   // 是链表reverse的尾结点，然后我们再把当前节点
   //head挂到next节点的后面就完成了链表的反转。
   next.next = head;
   //这里head相当于变成了尾结点，尾结点都是为空的，否则会构成环
   head.next = null;
   return reverse;
}
```

## 倒着打印链表

```java
//反向打印链表
void printReverse(ListNode head){
    if(head != null){
        printReverse(head.next);
        System.out.print(head.val + " ");
    }else {
        //这句话会首先打印
        System.out.print("反转链表为：");
    }
}
```

## 回文链表

https://leetcode-cn.com/problems/palindrome-linked-list-lcci/

编写一个函数，检查输入的链表是否是回文的。 

示例 1：

输入： 1->2
输出： false 
示例 2：

输入： 1->2->2->1
输出： true 

### 方法一：使用双指针和反转链表

https://leetcode-cn.com/problems/palindrome-linked-list-lcci/solution/di-gui-zhan-deng-3chong-jie-jue-fang-shi-zui-hao-2/

这题是让判断链表是否是回文链表，所谓的回文链表就是以链表中间为中心点两边对称。我们常见的有判断一个字符串是否是回文字符串，这个比较简单，可以使用两个指针，一个最左边一个最右边，两个指针同时往中间靠，判断所指的字符是否相等。

但这题判断的是链表，因为这里是单向链表，只能从前往后访问，不能从后往前访问，所以使用判断字符串的那种方式是行不通的。**但我们可以通过找到链表的中间节（或者偏右一点）点然后把链表后半部分反转，最后再用后半部分反转的链表和前半部分一个个比较即可。这里以示例2为例画个图看一下。**

![image-20210104133852466](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210104133852466.png)

![image-20210104133918392](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210104133918392.png)



```java
public boolean isPalindrome(ListNode head) {
    //首先找到链表的中间，或者中间靠右的那个节点
    //这个可以使用快慢指针来实现
    ListNode fast = head, low = head;
    while (fast != null && fast.next != null){
        fast = fast.next.next;
        low = low.next;
    }
    //fast 不是null,说明节点为奇数个
    if(fast != null){
        low = low.next;
    }
    //low指向的节点开始反转
    low = reverse(low);
    fast = head;
    //
    while (low != null){
        if(low.val != fast.val){
            return false;
        }
        low = low.next;
        fast = fast.next;
    }
    return true;
}

ListNode reverse(ListNode head){
    ListNode newNode = null;
    while (head != null){
        ListNode temp = head.next;
        head.next = newNode;
        newNode = head;
        head = temp;
    }
    return newNode;
}
```

### 方法二：使用栈

可以将链表一个一个放到栈中，这样在访问就是倒序了。稍微优化一点的做法就是，入栈的时候记录链表长度，这样判断回文的时候，就不需要遍历整个链表了。

### 方法三：使用递归--逆序打印列表

```java
ListNode temp;

public boolean isPalindrome(ListNode head) {
    temp = head;
    return check(head);
}

private boolean check(ListNode head) {
    if (head == null)
        return true;
    boolean res = check(head.next) && (temp.val == head.val);
    temp = temp.next;
    return res;
}
```



## 链表相交

![image-20210104144935003](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210104144935003.png)

### 思路

https://leetcode-cn.com/problems/intersection-of-two-linked-lists-lcci/solution/dai-ma-jie-shi-shuang-zhi-zhen-suan-fa-wei-shi-yao/

根据题意,两个链表相交的点是指: 两个指针指向的内容相同,则说明该结点记在A链表上又在B链表上,进而说明A和B是相交的

而对于相交的情况,两条链表一定是这种结构:

![image-20210104145027831](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210104145027831.png)



为什么呢?
因为如果链表A和链表B相交于D的话,那么说明D结点即在A上又在B上,而D之后的元素自然也就均在A和B上了,因为他们是通过next指针相连的.

如果有相交的结点D的话**,每条链的头结点先走完自己的链表长度,然后回头走另外的一条链表,那么两结点一定为相交于D点,因为这时每个头结点走的距离是一样的,都是 AD + BD + DC**,而他们每次又都是前进1,所以距离相同,速度又相同,固然一定会在相同的时间走到相同的结点上,即D点。如果不相交，他们会共同走到null。



# 

## 链表求和

https://leetcode-cn.com/problems/sum-lists-lcci/

给定两个用链表表示的整数，每个节点包含一个数位。

这些数位是反向存放的，也就是个位排在链表首部。

编写函数对这两个整数求和，并用链表形式返回结果。

 

示例：

输入：(7 -> 1 -> 6) + (5 -> 9 -> 2)，即617 + 295
输出：2 -> 1 -> 9，即912
进阶：思考一下，假设这些数位是正向存放的，又该如何解决呢?

示例：

输入：(6 -> 1 -> 7) + (2 -> 9 -> 5)，即617 + 295
输出：9 -> 1 -> 2，即912



```java
class Solution {
    public ListNode addTwoNumbers(ListNode l1, ListNode l2) {
		ListNode p1 = l1, p2 = l2;
		ListNode res = new ListNode(-1);
		ListNode p = res;
		int carry = 0;//进位
		while (p1 != null || p2 != null){
			int a = p1 != null ? p1.val : 0;
			int b = p2 != null ? p2.val : 0;
			int temp = a + b + carry;
			p.next = new ListNode(temp % 10);
			p = p.next;
			carry = temp / 10;
			if(p1 != null){
				p1 = p1.next;
			}
			if(p2 != null){
				p2 = p2.next;
			}
		}
		//此时进位可能不为0
		if(carry != 0){
			p.next = new ListNode(carry);
		}
		return res.next;
	}
}
```

## 环路检测

https://leetcode-cn.com/problems/linked-list-cycle-lcci/

![image-20210104154117106](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210104154117106.png)

假设c为快慢指针相遇的点，那么在c点时，快指针移动的距离是慢指针的2倍，那么可得等式`(m+y)*2 = m + xn +y`  。变形后可得`m = xn-y = n-y+(x-1)*n`。

**此时令快指针重新回到a**，并且令快指针每步走一个单位距离。当快指针走`m= n-y+(x-1)*n`  个单位时, 慢指针走了 `n-y+(x-1)*n` 步，走`(x-1)*n`x相当于没走，然后走`n-y`步，刚好到达b点，所以此时快慢指针又相遇了。

```java
public ListNode detectCycle(ListNode head) {
    if(head == null || head.next == null){
        return null;
    }
    ListNode fast = head, low = head;
    while (fast != null && fast.next != null){
        fast = fast.next.next;
        low = low.next;
        if(fast == low){
            break;
        }
    }
    //说明单链表能遍历完，链表没环
    if(fast != low){
        return null;
    }

    fast = head;
    while (fast != low){
        fast = fast.next;
        low = low.next;
    }
    return fast;
}
```

# 二叉树

## 二叉树的遍历

### 使用栈的方式遍历二叉树

https://leetcode-cn.com/problems/binary-tree-inorder-traversal/solution/yan-se-biao-ji-fa-yi-chong-tong-yong-qie-jian-ming/

栈迭代方法虽然提高了效率，但其嵌套循环却非常烧脑，不易理解，容易造成“一看就懂，一写就废”的窘况。而且对于不同的遍历顺序（前序、中序、后序），循环结构差异很大，更增加了记忆负担。

因此，我在这里介绍一种“颜色标记法”（瞎起的名字……），兼具栈迭代方法的高效，又像递归方法一样简洁易懂，更重要的是，这种方法对于前序、中序、后序遍历，能够写出完全一致的代码。

其核心思想如下：

使用颜色标记节点的状态，新节点为白色，已访问的节点为灰色。
如果遇到的节点为白色，则将其标记为灰色，然后将其右子节点、自身、左子节点依次入栈。
如果遇到的节点为灰色，则将节点的值输出。
使用这种方法实现的中序遍历如下：

```java
public class LC94_2 {
	static class TreeNode {
		int val;
		TreeNode left;
		TreeNode right;

		TreeNode() {
		}

		TreeNode(int val) {
			this.val = val;
		}

		TreeNode(int val, TreeNode left, TreeNode right) {
			this.val = val;
			this.left = left;
			this.right = right;
		}
	}

	Map<Integer, Integer> map = new HashMap<>();

	public TreeNode constructBinaryTree(int[] inorder, int[] preorder) {

		for (int i = 0; i < inorder.length; i++) {
			map.put(inorder[i], i);
		}
		return helper(preorder, 0, preorder.length - 1, 0, inorder.length - 1);
	}

	//通过先跟和中根构造二叉树
	TreeNode helper(int preorder[], int pl, int pr, int il, int ir) {
		if (pr < pl || ir < il) {
			return null;
		}
		TreeNode root = new TreeNode(preorder[pl]);
		int rootIndex = map.get(preorder[pl]);
		int numsLeft = rootIndex - il;
		root.left = helper(preorder, pl + 1, pl + numsLeft, il, rootIndex - 1);
		root.right = helper(preorder, pl + numsLeft + 1, pr, rootIndex + 1, ir);
		return root;
	}



	final static boolean WHITE = true;
	final static boolean BLACK = false;

	static class ColorTreeNode {
		TreeNode node;
		boolean color;

		public ColorTreeNode(TreeNode node, boolean color) {
			this.node = node;
			this.color = color;
		}
	}

    //原始的三种遍历
	public List<Integer> inorderTraversal(TreeNode root) {
		List<Integer> res = new ArrayList<>();
		Stack<ColorTreeNode> stack = new Stack<>();
		stack.push(new ColorTreeNode(root, WHITE));
		while (!stack.empty()) {
			ColorTreeNode node = stack.pop();
			if (node.color == WHITE) {
				if (node.node.right != null) {
					stack.push(new ColorTreeNode(node.node.right, WHITE));
				}
				node.color = BLACK;
				stack.push(node);
				if (node.node.left != null) {
					stack.push(new ColorTreeNode(node.node.left, WHITE));
				}
			} else {
				res.add(node.node.val);
			}
		}
		return res;
	}

	public List<Integer> PreOrderTraversal(TreeNode root) {
		List<Integer> res = new ArrayList<>();
		Stack<ColorTreeNode> stack = new Stack<>();
		stack.push(new ColorTreeNode(root, WHITE));
		while (!stack.empty()) {
			ColorTreeNode node = stack.pop();
			if (node.color == WHITE) {

				if (node.node.right != null) {
					stack.push(new ColorTreeNode(node.node.right, WHITE));
				}
				if (node.node.left != null) {
					stack.push(new ColorTreeNode(node.node.left, WHITE));
				}
				node.color = BLACK;
				stack.push(node);

			} else {
				res.add(node.node.val);
			}
		}
		return res;
	}
	public List<Integer> PostOrderTraversal(TreeNode root) {
		List<Integer> res = new ArrayList<>();
		Stack<ColorTreeNode> stack = new Stack<>();
		stack.push(new ColorTreeNode(root, WHITE));
		while (!stack.empty()) {
			ColorTreeNode node = stack.pop();
			if (node.color == WHITE) {
				node.color = BLACK;
				stack.push(node);
				if (node.node.right != null) {
					stack.push(new ColorTreeNode(node.node.right, WHITE));
				}
				if (node.node.left != null) {
					stack.push(new ColorTreeNode(node.node.left, WHITE));
				}

			} else {
				res.add(node.node.val);
			}
		}
		return res;
	}
	@Test
	public void test() {
		TreeNode root = constructBinaryTree(new int[]{4, 2, 5, 1, 6, 3, 7}, new int[]{1, 2, 4, 5, 3, 6, 7});
		System.out.println(inorderTraversal(root));
		System.out.println(PreOrderTraversal(root));
		System.out.println(PostOrderTraversal(root));

	}	
}
```

### 对上面方法使用策略模式进行封装

```java
//使用策略模式提取重复代码
List<Integer> tranverse(TreeNode root, BiConsumer<ColorTreeNode, Stack<ColorTreeNode>> consumer) {
    List<Integer> res = new ArrayList<>();
    Stack<ColorTreeNode> stack = new Stack<>();
    stack.push(new ColorTreeNode(root, WHITE));
    while (!stack.empty()) {
        ColorTreeNode node = stack.pop();
        if (node.color == WHITE) {
            consumer.accept(node, stack);
        } else {
            res.add(node.node.val);
        }
    }
    return res;
}

BiConsumer<ColorTreeNode, Stack<ColorTreeNode>> INORDER = (node, stack) -> {
    if (node.node.right != null) {
        stack.push(new ColorTreeNode(node.node.right, WHITE));
    }
    node.color = BLACK;
    stack.push(node);
    if (node.node.left != null) {
        stack.push(new ColorTreeNode(node.node.left, WHITE));
    }
};
BiConsumer<ColorTreeNode, Stack<ColorTreeNode>> PREORDER = (node, stack) -> {
    if (node.node.right != null) {
        stack.push(new ColorTreeNode(node.node.right, WHITE));
    }
    if (node.node.left != null) {
        stack.push(new ColorTreeNode(node.node.left, WHITE));
    }
    node.color = BLACK;
    stack.push(node);
};
BiConsumer<ColorTreeNode, Stack<ColorTreeNode>> POSTORDER = (node, stack) -> {
    node.color = BLACK;
    stack.push(node);
    if (node.node.right != null) {
        stack.push(new ColorTreeNode(node.node.right, WHITE));
    }
    if (node.node.left != null) {
        stack.push(new ColorTreeNode(node.node.left, WHITE));
    }
};

@Test
public void test1() {
    TreeNode root = constructBinaryTree(new int[]{4, 2, 5, 1, 6, 3, 7}, new int[]{1, 2, 4, 5, 3, 6, 7});
    System.out.println(tranverse(root, INORDER));
    System.out.println(tranverse(root, PREORDER));
    System.out.println(tranverse(root, POSTORDER));
}
```



# 图

## 339 除法求值

https://leetcode-cn.com/problems/evaluate-division/

![image-20210106172005393](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210106172005393.png)





![image-20210106172016065](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210106172016065.png)

思路：![image-20210106172153578](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210106172153578.png)



### 做法1：BFS

```java
public class LC399 {
	//先根据问题建立一张图，
	// a/b=2 可以抽象成 节点a到节点b的一条路径长度为2，和len(b->a)=0.5
	// b/c=3 可以抽象成 len(b->c)=3 len(c->b)=0.33
	//所以a/c = len(a->c) = len(a->b) * len(a-c) = 2 * 3 = 6
	static class Node {
		String dest;//目的节点
		double value;//到达目的节点的距离

		Node(String dest, double value) {
			this.dest = dest;
			this.value = value;
		}
	}

	Map<String, List<Node>> map;
	public double[] calcEquation(List<List<String>> equations, double[] values, List<List<String>> queries) {
		//构建一张图
		map = new HashMap<>();
		int i = 0;
		for (List<String> strs : equations) {
			String a = strs.get(0);
			String b = strs.get(1);
			List<Node> aList = map.getOrDefault(a, new ArrayList<>());
			aList.add(new Node(b, values[i]));
			map.put(a, aList);
			List<Node> bList = map.getOrDefault(b, new ArrayList<>());
			bList.add(new Node(a, 1 / values[i]));
			map.put(b, bList);
			i ++;
		}

		//使用BFS获得答案
		double[] res = new double[queries.size()];
		i = 0;
		for (List<String> strs : queries){
			String a = strs.get(0);
			String b = strs.get(1);
			if(!map.containsKey(a) || !map.containsKey(b)){
				res[i ++] = -1;
			}else if(a.equals(b)){
				res[i ++] = 1;
			}else {
				res[i ++] = BFS(a, b);
			}
		}
		return res;
	}


	//使用BFS，获得从根节点到达目的节点上路径乘积
	double BFS(String root, String target){
		Queue<Node> q = new LinkedList<>();
		q.offer(new Node(root, 1));
		Set<String> inq = new HashSet<>();
		inq.add(root);
		while (!q.isEmpty()){
			Node p = q.poll();
			List<Node> node = map.get(p.dest);
			for (Node n: node){
				if(!inq.contains(n.dest)){
					if(n.dest.equals(target)){//到达目的站
						return p.value * n.value;
					}else {
						q.offer(new Node(n.dest, p.value * n.value));
						inq.add(n.dest);
					}
				}
			}
		}
		return -1;
	}
}
```



### 做法二： floyd

```java
public class LC399_2 {

	//先根据问题建立一张图，
	// a/b=2 可以抽象成 节点a到节点b的一条路径长度为2，和len(b->a)=0.5
	// b/c=3 可以抽象成 len(b->c)=3 len(c->b)=0.33
	//所以a/c = len(a->c) = len(a->b) * len(a-c) = 2 * 3 = 6

	//可以使用floyd算法，预先求处两个节点之间的距离
	public double[] calcEquation(List<List<String>> equations, double[] values, List<List<String>> queries) {
		//对于节点的处理可以按照第一种做法
		Map<String, Integer> strIndex = new HashMap<>();
		int index = 0;
		for (List<String> strs : equations) {
			String a = strs.get(0);
			String b = strs.get(1);
			strIndex.put(a, index++);
			strIndex.put(b, index++);
		}
		//图
		//初始化
		double INF =  Double.MAX_VALUE;
		double[][] d = new double[index][index];
		for (int i = 0; i < index; i++) {
			Arrays.fill(d[i], INF);
		}
		int w = 0;
		for (List<String> strs : equations) {
			String a = strs.get(0);
			String b = strs.get(1);
			d[strIndex.get(a)][strIndex.get(b)] = values[w];
			d[strIndex.get(b)][strIndex.get(a)] = 1 / values[w];
			w++;
		}

		//求所有的路径乘积
		for (int k = 0; k < index; k++) {
			for (int i = 0; i < index; i++) {
				for (int  j = 0; j < index; j++) {
					if(d[i][k] != INF && d[k][j] != INF ){
						d[i][j] = d[i][k] * d[k][j];
					}
				}
			}
		}

		double[] res = new double[queries.size()];
		int i = 0;
		for (List<String> strs : queries){
			String a = strs.get(0);
			String b = strs.get(1);
			Integer aIndex = strIndex.get(a);
			Integer bIndex = strIndex.get(b);
			if(aIndex == null || bIndex == null){
				res[i ++] = -1;
			}else if(a.equals(b)){
				res[i ++] = 1;
			}else {
				res[i ++] = d[aIndex][bIndex];
			}
		}
		return res;
	}
}
```















