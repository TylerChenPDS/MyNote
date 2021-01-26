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

## 拓扑排序

### [1203. 项目管理](https://leetcode-cn.com/problems/sort-items-by-groups-respecting-dependencies/)

![image-20210112150042025](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210112150042025.png)



![image-20210112150054571](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210112150054571.png)

思路：首先如果不考虑group的话，有的项目有前置项目，前置项目完成后才可以进行当前项目。如上图：项目1进行之前，项目6必须完成，于是可以构建拓扑图：

![image-20210112150550995](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210112150550995.png)

但是，题目又有相同组的项目先后顺序必须相邻，所以也必须构建组之间的先后顺序，如果对于组有u->v，那么则有排序{u的项目，v的项目}，且这个排序也要满足上面拓扑图，对于不属于任何一个组的项目，我们可以假定它属于一个唯一的组。改造完之后可得：

![image-20210112151212627](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210112151212627.png)

怎么构建？对于item1，属于group3，前置项目为item6，item6属于group0，所以构建一条边：group0->group3，同理可得拓扑图（自身环需要去掉）：

![image-20210112151527025](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210112151527025.png)

最后：创建一个函数：`List<Integer> topuSort(int[] deg, Map<Integer, ArrayList<Integer>> graph, Set<Integer> items)`，返回一个items 里面节点的拓扑排序。

首先得到组的拓扑排序，然后遍历组编号，对于组内项目元素构成一个set集合，调用上面的函数，获得拓扑排序，将排序加入结果集。

细节见代码，可以单步调试看下过程：

```java
package 拓扑排序;

import org.junit.Test;

import java.util.*;

/**
 * @author TylerChen
 * @date 2021/1/12 - 11:16
 */
public class LC1203 {

	/**
	 * @param n           n个项目
	 * @param m           m个小组
	 * @param group       group[i]表示小组 group[i] 负责第i个项目
	 * @param beforeItems beforeItems[i] 表示项目i的前置项目
	 * @return
	 */
	public int[] sortItems(int n, int m, int[] group, List<List<Integer>> beforeItems) {
		//首先建立小组之间的关系图，假设所有的未被小组接手的项目属于一个小组（假设编号从m开始）
		// 示例1中： item1属于小组8，item1的前置项目为item6，item6属于小组0，
		// 因此建立0->8这个小组关系

		//小组拓扑图
		Map<Integer, ArrayList<Integer>> groupMap = new HashMap<>();
		//项目拓扑图
		Map<Integer, ArrayList<Integer>> itemMap = new HashMap<>();
		//首先将-1组，重新编号，从m开始
		int k = m;
		for (int i = 0; i < group.length; i++) {
			if (group[i] == -1) {
				group[i] = k ++;
			}
		}
		//项目拓扑图中，项目节点的入度
		int[] inDegreeItem = new int[n];

		//小组拓扑图中，小组节点的入度
		int[] inDegreeGroup = new int[k];

		//这个循环，创建项目拓扑图中，和小组拓扑图中
		for (int i = 0; i < group.length; i++) {
			//项目i的前置项目
			List<Integer> beforeItemi = beforeItems.get(i);
			for (Integer j : beforeItemi) { //前置项目
				//查找前置项目的组号
				if (group[j] != group[i]) { //防止出现自环的情况
					ArrayList<Integer> list = groupMap.getOrDefault(group[j], new ArrayList<>());
					list.add(group[i]);
					groupMap.put(group[j], list);
					//group[i]的入度加一
					inDegreeGroup[group[i]]++;
				}

				//构建项目拓扑图
				ArrayList<Integer> itemList = itemMap.getOrDefault(j, new ArrayList<>());
				itemList.add(i);
				itemMap.put(j, itemList);
				inDegreeItem[i]++;
			}

		}

		//获取小组的点集合
		Set<Integer> set = new HashSet<>();
		for (int i = 0; i < k; i++) {
			set.add(i);
		}
		//得到的是一个组的排序
		List<Integer> integers = topuSort(inDegreeGroup, groupMap, set);
		//出现环路，构不成一个排序
		if (integers.size() == 0) {
			return new int[0];
		}

		//结果集
		List<Integer> res = new ArrayList<>();

		//构建小组对应的项目集合
		Map<Integer, Set<Integer>> groupIdToItems = new HashMap<>();
		for (int i = 0; i < group.length; i++) {
			Set<Integer> set1 = groupIdToItems.getOrDefault(group[i],new HashSet<>());
			set1.add(i);
			groupIdToItems.put(group[i],set1);
		}

		//对一个组内的项目进行拓扑排序
		for (Integer groupId: integers){
			Set<Integer> itms = groupIdToItems.get(groupId);
			if(itms != null){
				List<Integer> li = topuSort(inDegreeItem, itemMap, itms);
				if(li.size()==0){
					return new int[0];
				}
				res.addAll(li);
			}

		}

		//结果转换
		int[] ans = new int[res.size()];
		int index = 0;
		for(Integer item : res){
			ans[index ++] = item;
		}
		return ans;
	}

	/**
	 * @param deg   对应点的入度
	 * @param graph 对应的图
	 * @param items 需要topu排序的节点
	 * @return
	 */
	List<Integer> topuSort(int[] deg, Map<Integer, ArrayList<Integer>> graph, Set<Integer> items) {
		int n = items.size();
		Queue<Integer> queue = new LinkedList<Integer>();
		for (int item : items) {
			if (deg[item] == 0) {
				queue.offer(item);
			}
		}
		List<Integer> res = new ArrayList<>();
		while (!queue.isEmpty()) {
			int u = queue.poll();
			ArrayList<Integer> integers = graph.get(u);
			//当前项目做完，与它的后置项目的入度需要-1
			if( items.contains(u) && integers != null && integers.size() != 0){
				for (int v : graph.get(u)) {
					deg[v]--;
					if (deg[v] == 0) {
						queue.offer(v);
					}
				}
			}
			if (items.contains(u)) {
				items.remove(u);
				res.add(u);
			}
		}
		return res.size() == n ? res : new ArrayList<>();
	}
	
	@Test
	public void test(){
//		List<List<Integer>> a = new ArrayList<>();
//		a.add(new ArrayList<>());
//		a.add(Arrays.asList(6));
//		a.add(Arrays.asList(5));
//		a.add(Arrays.asList(6));
//		a.add(Arrays.asList(3,6));
//		a.add(new ArrayList<>());
//		a.add(new ArrayList<>());
//		a.add(new ArrayList<>());
//		System.out.println(Arrays.toString(sortItems(8, 2, new int[]{-1, -1, 1, 0, 0, 1, 0, -1}, a)));
		List<List<Integer>> a = new ArrayList<>();
		a.add(new ArrayList<>());
		a.add(Arrays.asList(0));
		a.add(Arrays.asList(1));
		System.out.println(Arrays.toString(sortItems(3, 1, new int[]{-1,0, -1}, a)));
	}
}

```



## 连通图

![image-20210115215457481](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210115215457481.png)

把二维坐标平面上的石头想象成图的顶点，如果两个石头横坐标相同、或者纵坐标相同，在它们之间形成一条边。

![image-20210115215536792](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210115215536792.png)

根据可以移除石头的规则：如果一块石头的 同行或者同列 上有其他石头存在，那么就可以移除这块石头。可以发现：**一定可以把一个连通图里的所有顶点根据这个规则删到只剩下一个顶点。**所以：**最多可以移除的石头的个数 = 所有石头的个数 - 连通分量的个数**。

```java
class Solution {
    public int removeStones(int[][] stones) {
        int n = stones.length;
        List<List<Integer>> edge = new ArrayList<List<Integer>>();
        for (int i = 0; i < n; i++) {
            edge.add(new ArrayList<Integer>());
            for (int j = 0; j < n; j++) {
                if (stones[i][0] == stones[j][0] || stones[i][1] == stones[j][1]) {
                    edge.get(i).add(j);
                }
            }
        }
        boolean[] vis = new boolean[n];
        int num = 0;
        for (int i = 0; i < n; i++) {
            if (!vis[i]) {
                num++;
                dfs(i, edge, vis);
            }
        }
        return n - num;
    }

    public void dfs(int x, List<List<Integer>> edge, boolean[] vis) {
        vis[x] = true;
        for (int y : edge.get(x)) {
            if (!vis[y]) {
                dfs(y, edge, vis);
            }
        }
    }
}
```



## 803 打砖块

![image-20210116131833460](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210116131833460.png)

![image-20210116131848722](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210116131848722.png)

![image-20210116131950546](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210116131950546.png)

思路：容易想到的做法是DFS，打碎砖块前从屋顶DFS，查看与屋顶相连砖块的个数m，打碎后再次从屋顶DFS查看与屋顶相连砖块的个数n，然后m-n-1即为所求。但是超时了。

```java
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

/**
 * @author TylerChen
 * @date 2021/1/16 - 10:39
 */
public class LC803 {

	//使用DFS，发现超时
	boolean[][] vis;
	int m, n;
	int[][] G;
	int[] X = {0, 0, 1, -1};
	int[] Y = {1, -1, 0, 0};

	public int[] hitBricks(int[][] grid, int[][] hits) {
		m = grid.length;
		n = grid[0].length;
		vis = new boolean[m][n];
		G = grid;
		int num = 0; //当前砖的个数
		num = getNum();
		int[] res = new int[hits.length];
		int index = 0;
		for (int[] hit : hits) {
			//使用DFS查看敲掉当前的砖后屋顶还有多少砖
			if (vis[hit[0]][hit[1]]) { //此时是连接屋顶的砖块
				G[hit[0]][hit[1]] = 0;
				vis = new boolean[m][n];
				int a = getNum();
				res[index++] = num - a - 1;
				num = a;
			} else {
				res[index++] = 0;
			}

		}

		return res;
	}

	int getNum() {
		int num = 0;
		for (int i = 0; i < n; i++) {
			if (G[0][i] == 1 && !vis[0][i]) {
				DFS(0, i);
				num += kNum;
				kNum = 0;
			}
		}
		return num;
	}

	int kNum = 0;

	void DFS(int i, int j) {
		vis[i][j] = true;
		kNum++;
		for (int k = 0; k < 4; k++) {
			int x = i + X[k];
			int y = j + Y[k];
			if (x >= 0 && x < m && y >= 0 && y < n
					&& G[x][y] == 1 && !vis[x][y]) {
				DFS(x, y);
			}
		}
	}

	@Test
	public void test() {
//		hitBricks(new int[][]{{1}, {1}, {1}, {1}, {1}}, new int[][]{{3, 0}, {4, 0}, {1, 0}, {2, 0},
//				{0, 0}});

		hitBricks(new int[][]{{1,0,1},{1,1,1}},new int[][]{{0,0},{0,2},{1,1}});
	}
}

```

另外一种思路是使用并查集

https://leetcode-cn.com/problems/bricks-falling-when-hit/solution/803-da-zhuan-kuai-by-leetcode-r5kf/

首先令copy=grid，将需要打碎的砖块在copy种都设置为0，然后把copy种所有相连的砖块放到一个集合中，并且把与顶部相连的砖块放到一个特殊的集合X中，（需要记录集合中元素的个数），然后倒叙把砖块补上，并把补上的砖块加入到与之相连的集合种，这样集合X中会多出n个砖，n-1，就是敲掉补上的砖，掉下去的砖块个数。

```java
public class Solution {

	private int rows;
	private int cols;

	public static final int[][] DIRECTIONS = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

	public int[] hitBricks(int[][] grid, int[][] hits) {
		this.rows = grid.length;
		this.cols = grid[0].length;

		// 第 1 步：把 grid 中的砖头全部击碎，通常算法问题不能修改输入数据，
		// 这一步非必需，可以认为是一种答题规范
		int[][] copy = new int[rows][cols];
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				copy[i][j] = grid[i][j];
			}
		}

		// 把 copy 中的砖头全部击碎
		for (int[] hit : hits) {
			copy[hit[0]][hit[1]] = 0;
		}

		// 第 2 步：建图，把砖块和砖块的连接关系输入并查集，size 表示二维网格的大小，
		// 也表示虚拟的「屋顶」在并查集中的编号
		int size = rows * cols;
		UnionFind unionFind = new UnionFind(size + 1);

		// 将下标为 0 的这一行的砖块与「屋顶」相连
		for (int j = 0; j < cols; j++) {
			if (copy[0][j] == 1) {
				unionFind.union(j, size);
			}
		}

		// 其余网格，如果是砖块向上、向左看一下，如果也是砖块，在并查集中进行合并
		for (int i = 1; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (copy[i][j] == 1) {
					// 如果上方也是砖块
					if (copy[i - 1][j] == 1) {
						unionFind.union(getIndex(i - 1, j), getIndex(i, j));
					}
					// 如果左边也是砖块
					if (j > 0 && copy[i][j - 1] == 1) {
						unionFind.union(getIndex(i, j - 1), getIndex(i, j));
					}
				}
			}
		}

		// 第 3 步：按照 hits 的逆序，在 copy 中补回砖块，把每一次因为补回砖块而与屋顶相连的砖块的增量记录到 res 数组中
		int hitsLen = hits.length;
		int[] res = new int[hitsLen];
		for (int i = hitsLen - 1; i >= 0; i--) {
			int x = hits[i][0];
			int y = hits[i][1];

			// 注意：这里不能用 copy，语义上表示，如果原来在 grid 中，这一块是空白，这一步不会产生任何砖块掉落
			// 逆向补回的时候，与屋顶相连的砖块数量也肯定不会增加
			if (grid[x][y] == 0) {
				continue;
			}

			// 补回之前与屋顶相连的砖块数
			int origin = unionFind.getSize(size);

			// 注意：如果补回的这个结点在第 1 行，要告诉并查集它与屋顶相连（逻辑同第 2 步）
			if (x == 0) {
				unionFind.union(y, size);
			}

			// 在 4 个方向上看一下，如果相邻的 4 个方向有砖块，合并它们
			for (int[] direction : DIRECTIONS) {
				int newX = x + direction[0];
				int newY = y + direction[1];

				if (inArea(newX, newY) && copy[newX][newY] == 1) {
					unionFind.union(getIndex(x, y), getIndex(newX, newY));
				}
			}

			// 补回之后与屋顶相连的砖块数
			int current = unionFind.getSize(size);
			// 减去的 1 是逆向补回的砖块（正向移除的砖块），与 0 比较大小，是因为存在一种情况，添加当前砖块，不会使得与屋顶连接的砖块数更多
			res[i] = Math.max(0, current - origin - 1);

			// 真正补上这个砖块
			copy[x][y] = 1;
		}
		return res;
	}

	/**
	 * 输入坐标在二维网格中是否越界
	 *
	 * @param x
	 * @param y
	 * @return
	 */
	private boolean inArea(int x, int y) {
		return x >= 0 && x < rows && y >= 0 && y < cols;
	}

	/**
	 * 二维坐标转换为一维坐标
	 *
	 * @param x
	 * @param y
	 * @return
	 */
	private int getIndex(int x, int y) {
		return x * cols + y;
	}

	private class UnionFind {

		/**
		 * 当前结点的父亲结点
		 */
		private int[] parent;
		/**
		 * 以当前结点为根结点的子树的结点总数
		 */
		private int[] size;

		public UnionFind(int n) {
			parent = new int[n];
			size = new int[n];
			for (int i = 0; i < n; i++) {
				parent[i] = i;
				size[i] = 1;
			}
		}

		/**
		 * 路径压缩，只要求每个不相交集合的「根结点」的子树包含的结点总数数值正确即可，因此在路径压缩的过程中不用维护数组 size
		 *
		 * @param x
		 * @return
		 */
		public int find(int x) {
			if (x != parent[x]) {
				parent[x] = find(parent[x]);
			}
			return parent[x];
		}

		public void union(int x, int y) {
			int rootX = find(x);
			int rootY = find(y);

			if (rootX == rootY) {
				return;
			}
			parent[rootX] = rootY;
			// 在合并的时候维护数组 size
			size[rootY] += size[rootX];
		}

		/**
		 * @param x
		 * @return x 在并查集的根结点的子树包含的结点总数
		 */
		public int getSize(int x) {
			int root = find(x);
			return size[root];
		}
	}
}

```



#### [1584. 连接所有点的最小费用](https://leetcode-cn.com/problems/min-cost-to-connect-all-points/)

![](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210119160652511.png)

![image-20210119160717297](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210119160717297.png)

![image-20210119160732667](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210119160732667.png)

官方题解注释

https://leetcode-cn.com/problems/min-cost-to-connect-all-points/solution/lian-jie-suo-you-dian-de-zui-xiao-fei-yo-kcx7/

#### 建图优化的Kruskal

所谓优化，就是不把不需要的边加入

先说结论

1. **结论一**：对于图中的任意三点 A,B,C  假设边 AB,AC,BC 中 AB 为最长边，那么最终答案中必然不包含边 AB。

2. **结论二**：对于下图中同属同一个区块的任意两点 B,C，A为原点，那么 BC不可能为三边中最长边

   ![image-20210119161345248](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210119161345248.png)

3. 结论三：假设存在一点 A在原点处，那么对于图中的任意一个 45°区域，我们都至多只选择其中的一个点与 A相连，且该点必然为该区域中距离 A最近的点。

   我们首先利用反证法证明：假设最后答案中包含 AB与 AC，且 B与 C 均位于同一个 45°
    区域中。那么由结论二可知，BC必不为三边中的最长边。即最长边必然为 AB或 AC。由结论一可知，AB与 AC 中必然有一个不包含在答案中，这与假设相悖，因此我们最多仅会选择一个点与 AA 相连。

结论2证明太复杂，其实可以转换成如下问题：

![image-20210119162050279](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210119162050279.png)

**这就很好证了，假设∠ACB，∠ABC 必然至少有一个大于45°（∠ACB+∠ABC>180-45>2*45）,这样，根据三角形的特性，大边对大角，小边对小角，AB,BC至少有一个大于BC。**







# 其他

## 数组反转

https://leetcode-cn.com/problems/rotate-array/solution/javadai-ma-3chong-fang-shi-tu-wen-xiang-q8lz9/

![image-20210108225307438](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210108225307438.png)





思路：

![image-20210108225548227](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210108225548227.png)



```java
public void rotate(int[] nums, int k) {
    int length = nums.length;
    k %= length;
    reverse(nums, 0, length - 1);//先反转全部的元素
    reverse(nums, 0, k - 1);//在反转前k个元素
    reverse(nums, k, length - 1);//接着反转剩余的
}

//把数组中从[start，end]之间的元素两两交换,也就是反转
public void reverse(int[] nums, int start, int end) {
    while (start < end) {
        int temp = nums[start];
        nums[start] = nums[end];
        nums[end] = temp;
        start ++;
        end --;
    }
}
```





# 并查集

## [1202. 交换字符串中的元素](https://leetcode-cn.com/problems/smallest-string-with-swaps/)

![image-20210111105226141](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210111105226141.png)

![image-20210111105337238](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210111105337238.png)

**思路**

https://leetcode-cn.com/problems/smallest-string-with-swaps/solution/1202-jiao-huan-zi-fu-chuan-zhong-de-yuan-wgab/

理解「交换关系具有传递性」：

[0, 3] 和 [0, 2] 有共同索引 0 ，说明索引 0、2、3 可以任意交换；
[1, 2] 和 [0, 2] 有共同索引 2 ，说明索引 0、1、2 可以任意交换； 因此 [0, 2] 把 [0, 3] 和 [1, 2] 中出现的索引 0、1、2、3 连在了一起。

题目中说「可以 任意多次交换 在 pairs 中任意一对索引处的字符」。于是我们可以将 0、1、2、3 这 44 个索引位置上的字符按照 ASCII 值升序排序。采用基于比较的原地排序算法（选择排序、插入排序、冒泡排序、快速排序）均可。

可以使用并查集+优先队列（最小堆）的方式，先使用并查集找到在一个集合中的index，然后把同一个集合中的index对应的字符存到一个优先队列中。

然后遍历0~s.length-1，找到 i 对应的集合，从里面取出一个元素放到i这个位置，最终形成的新字符串即为所求。

**代码：最小堆+并查集**

```java
public class LC1202_3 {
	//使用并查集和小顶堆
	int[] father;
	int n;

	int findFather(int x) {
		int a = x;
		while (x != father[x]) {
			x = father[x];
		}
		while (a != father[a]) {
			int z = a;
			a = father[a];
			father[z] = x;
		}
		return x;
	}

	void union(int a, int b) {
		int faA = findFather(a);
		int faB = findFather(b);
		if (faA != faB) {
			father[faA] = faB;
		}
	}

	public String smallestStringWithSwaps(String s, List<List<Integer>> pairs) {
		n = s.length();
		father = new int[n];
		for (int i = 0; i < n; i++) {
			father[i] = i;
		}
		for (List<Integer> pair : pairs) {
			union(pair.get(0), pair.get(1));
		}
		Map<Integer, MinHeap> map = new HashMap<>();
		for (int i = 0; i < n; i++) {
			MinHeap minHeap = map.getOrDefault(findFather(i), new MinHeap(1000));
			minHeap.add(s.charAt(i));
			map.put(father[i], minHeap);
		}
		//
		StringBuilder res = new StringBuilder(n);
		for (int i = 0; i < n; i++) {
			res.append(map.get(findFather(i)).deleteTop());
		}
		return res.toString();
	}

	@Test
	public void test() {
		List<List<Integer>> a = new ArrayList<>();
		a.add(Arrays.asList(0, 3));
		a.add(Arrays.asList(1, 2));
		a.add(Arrays.asList(0, 2));
		smallestStringWithSwaps("dcab", a);
	}

	//使用堆实现小顶堆
	static class MinHeap {
		char[] heap;
		int n;//堆中元素的个数

		MinHeap(int len) {
			n = 0;//n为堆中元素的个数
			heap = new char[len + 1];
		}

		//向下调整,范围[low,high]
		//low为预调整节点的数组下标，high一般为堆的左后一个元素的数组下标
		void downAdjust(int low, int high) {
			int i = low;
			int j = 2 * i;//左孩子节点
			while (j <= high) {
				//找到孩子节点中较小的那一个
				if (j + 1 <= high && heap[j + 1] < heap[j]) {
					j = j + 1;
				}
				if (heap[j] < heap[i]) {//如果孩子节点比父节点小则交换,否则直接跳出循环
					char temp = heap[j];
					heap[j] = heap[i];
					heap[i] = temp;
					//然后父节点指向孩子节点，接着循环，看能否还能继续向下调整
					i = j;
					j = 2 * j;
				} else {
					break;
				}
			}
		}

		//向上调整
		//对heap数组在[low,high]范围进行向上调整
		//其中low一般设置为1，high表示欲调整的数组下标
		void upAdjust(int low, int high) {
			int i = high, j = i / 2; //j 为i的父亲节点
			while (j >= low) {
				if (heap[j] > heap[i]) {
					char temp = heap[i];
					heap[i] = heap[j];
					heap[j] = temp;
					i = j;
					j = i / 2;
				} else {
					break;
				}
			}
		}

		//向堆中添加一个元素
		void add(char x) {
			if(n + 1 == heap.length){//此时需要扩容
				char[] temp = new char[heap.length * 2];
				for (int i = 1; i <= n ; i++) {
					temp[i] = heap[i];
				}
				heap = temp;
			}
			heap[++n] = x;
			upAdjust(1, n);
		}
		//删除堆中的一个元素
		char deleteTop() {
			char top = heap[1];
			heap[1] = heap[n--];
			downAdjust(1, n);
			return top;
		}
	}
}
```





# 二分法

#### [寻找旋转排序数组中的最小值](https://leetcode-cn.com/problems/find-minimum-in-rotated-sorted-array/)

数组种无重复值

![image-20210125131445297](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210125131445297.png)



比较简单的做法是遍历求最小值 

```java
public int minArray(int[] numbers) {
   int min = numbers[0];
   for (int i = 1; i < numbers.length ; i++) {
      if(numbers[i] < numbers[i-1]){
         return Math.min(min, numbers[i]);
      }
   }
   return min;
}
```

如何判断一个数组是否被反转？

数组种最后一个元素 如果大于 第一个元素，则这个数组被反转了，此时直接可以返回第一个元素。

如果数组是反转的，可以使用而分叉找到其最小值

![image-20210125132030715](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210125132030715.png)

如图，在变化点左侧，所有元素都大于第一个元素，在变化点右侧，所有元素都小于第一个元素。

找到数组的中间元素 mid。

如果中间元素 > 数组第一个元素，我们需要在 mid 右边搜索变化点。

如果中间元素 < 数组第一个元素，我们需要在 mid 左边搜索变化点。

nums[mid] > nums[mid + 1]，因此 mid+1 是最小值。

nums[mid - 1] > nums[mid]，因此 mid 是最小值。

```java
public int findMin(int[] nums) {

    if (nums.length == 1) {
        return nums[0];
    }
    //此时数组没有被反转
    if (nums[nums.length - 1] > nums[0]) {
        return nums[0];
    }

    int left = 0, right = nums.length - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] > nums[mid + 1]) {
            return nums[mid + 1];
        }
        if (nums[mid - 1] > nums[mid]) {
            return nums[mid];
        }

        if (nums[mid] > nums[0]) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }

    }
    return -1;
}
```

也可以与右端点进行比较

```java
public int findMin(int[] nums) {
    int low = 0;
    int high = nums.length - 1;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] < nums[high]) {
            // high 不能是 mid - 1,因为，如果是mid - 1的话，则可能把最小值错过了
            high = mid;
        } else if (nums[mid] > nums[high]) {
            low = mid + 1;
        }
    }
    return nums[low];
}
```

如果里面允许有重复的值：

https://leetcode-cn.com/problems/find-minimum-in-rotated-sorted-array-ii/solution/xun-zhao-xuan-zhuan-pai-xu-shu-zu-zhong-de-zui--16/

```java
public int findMin(int[] nums) {
    int low = 0;
    int high = nums.length - 1;
    while (low < high) {
        int mid = low + (high - low) / 2;
        if (nums[mid] < nums[high]) {
            // high 不能是 mid - 1,因为，如果是mid - 1的话，则可能把最小值错过了
            high = mid;
        } else if (nums[mid] > nums[high]) {
            low = mid + 1;
        } else {
            
            high --;
        }
    }
    return nums[low];
}
```

