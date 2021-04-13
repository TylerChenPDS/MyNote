# 蛮力法

## 百鸡百钱问题

中国古代数学家张丘建在《算经》 中提出了著名的“百鸡百钱问题”：鸡翁一，值钱五；鸡母一值钱三；鸡雏三，值钱一；百钱买百鸡，翁、母、雏各几何？  

设鸡翁、鸡母、鸡雏分别为x, y,z只，则15x+9y+z=300, x+y+z=100。

所以，x从0遍历到100，y从0遍历到100-x，判断是否符合上面的式子，符合则是答案

```java
//鸡1
public void test() {
    for (int x = 0; x <= 100; x++) {
        for (int y = 0; y <= 100 - x; y++) {
            if (15 * x + 9 * y + (100 - x - y) == 300) {
                System.out.println("公鸡：" + x + " 母鸡：" + y + " 小鸡：" + (100 - x - y));
            }
        }
    }
}
//鸡2
public void test1() {
    for (int x = 0; x <= 100; x++) {
        for (int y = 0; y <= 100 - x; y++) {
            for (int z = 0; z <= 100 - x - y; z += 3) {
                if(x + y + z == 100 && 15 * x + 9 * y + z == 300){
                    System.out.println("公鸡：" + x + " 母鸡：" + y + " 小鸡：" + (100 - x - y));
                }
            }
        }
    }
}
```

# 减治法

# 分治算法

## 分治法和减治法的区别

**减治**技术利用了一个问题给定实例的解和同样问题较小实例解之间的某种关系。一旦建立这种关系，我们既可以从顶至下，也可以从底至上地运用关系。减治法有三种主要的变化形式：

- 减去一个常量。求幂：fn = fn-1 * a
- 减去一个常因子。快速幂的递归形式。
- 减去的规模是可变的。最大公约数，辗转相除法：gcd(m,n)=gcd(n, m%n)。

**分治**一般按照以下方案工作：

- 将一个问题分为同一类型的若干子问题，子问题最好规模相等。
- 对这些子问题进行求解（一般使用递归，但是在子问题规模够小的时候，有时也会利用另外一种算法）
- 有必要时合并子问题

如：快速排序、归并排序

## 凸包问题

![image-20210412154534202](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412154534202.png)

### 蛮力解法

任意2点构成一条直线，ax + by - c = 0 ，如果代入其余点(x', y') 都 ax' + by'-c > 0 (或 ax' + by'-c < 0)，则这条线入选，否则这条线不入选。

时间复杂度O(N^3)

### 分治解法

预备知识：两个向量a b的叉乘，结果是与a b组成平面垂直的向量

```
(x1, y1, z1) * (x2 ,y2, z2)=
|i   j  k|
|x1 y1 z1|
|x2 y2 z2|
=(y1*z2-y2*z1, x2*z1-x1*z2, x1*y2-x2*x1)

判断一个向量在另外一个向量左边还是右边？
根据叉乘的右手原则： 右手的四指先表示向量a的方向，然后手指朝着手心的方向摆动到向量b的方向，大拇指的方向就是向量c的方向。
所以可以通过x1*y2-x2*x1来判断b在a的左边还是右边
如果x1*y2-x2*x1 > 0，则b在a的左边，反之则在右边
（自己画个图看下即可）
```

不转换成向量的话可以用下面这个公式：

如何判断一个点 p3 是在直线 p1p2 的左边还是右边呢？（坐标：p1(x1,y1)，p2(x2,y2)，p3(x3,y3)）

![image-20210412160705752](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412160705752.png)

当上式结果为正时，p3在直线 p1p2 的左侧；当结果为负时，p3在直线 p1p2 的右边。

**算法思想**

![image-20210412160850808](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412160850808.png)

1. 把所有的点都放在二维坐标系里面。那么横坐标最小和最大的两个点 P1 和 Pn 一定是凸包上的点，（如果不是，则构造的凸包包裹不住P1或Pn）

2. 对上包：求距离直线 P1Pn 最远的点，即上图中的点 Pmax。如何求？还是这个公式，绝对值为三角形的面积，如果绝对值越大，Pmax离P1 Pn 越远。**Pmax一定是上包的顶点**

   ![image-20210412160705752](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412160705752.png)

   

3. 作直线 P1Pmax 、PnPmax，把直线 P1Pmax 左侧的点当成是上包，把直线 PnPmax 右侧的点也当成是上包。**三角形里面的点不可能是上包的顶点**。

分治算法时间复杂度为O(nlogn)，最差时间复杂度为O(n^2)

**代码如下：**

```java
public class Main {
    static class Point implements Comparable{
        int x;
        int y;

        public Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        public int compareTo(Object o) {
            Point obj = (Point) o;
            if (this.x < obj.x) return -1;
            else if (this.x > obj.x) return 1;
            return 0;
        }
    }

    //    叉乘，判断p3在直线p1p2的左边或右边
    //也就是判断向量p1p3 在 p1p2的左边还是右边，根据上面的知识，只需要判断x1*y2-x2*x1的符号
    //其中p1p2 = (x1, y1) = (p2.x-p1.x, p2.y-p1.y)
    //   p1p3=(x2,y2)=(p3.x-p1.x, p3.y-p1.y)，带入公式即可  
    public static int cross(Point p1, Point p2, Point p3) {
        return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    }

    public static LinkedList<Point> hull(LinkedList<Point> list) {
        LinkedList<Point> res = new LinkedList<>();

        Point p1 = list.removeFirst();
        Point p2 = list.removeLast();
        //两端的点肯定在凸包里
        res.add(p1);
        res.add(p2);

        deal(p1, p2, res, list);
        deal(p2, p1, res, list);

        return res;
    }
	// res为最终凸包上的点，list为去掉当前去掉确定点之后的点集
    private static void deal(Point p1, Point p2, LinkedList<Point> res, LinkedList<Point> list) {
        Iterator<Point> it = list.iterator();

        Point highMax = null;
        int max = 0;

        while (it.hasNext()) {
            Point next = it.next();
            //此时由于Pmax肯定在p1p2的左边，所以compute一定是正数
            int compute = cross(p1, p2, next);
            if (compute > max) {
                max = compute;
                highMax = next;
            }
        }

        if (highMax != null) {
            res.add(highMax);
            list.remove(highMax);

            deal(p1, highMax, res, list);
            deal(highMax, p2, res, list);
        }
    }    
}
```

## 最近对问题

在一个包含n个点的集合里面找出距离最近的两个点。

### 蛮力法

![image-20210412164342091](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412164342091.png)

### 分治法

将点集分成放入P，Q中，其中Q中坐标是按照x轴坐标升序，Q是按照y轴升序

1. n<=3的时候，使用蛮力法，求出最近对

2. n>3时，将点集分成n/2（向上取整）,n/2两个子集，然后使用递归求解两个子问题的解，d1，d2。

3. 令d=min{d1,d2}, d不一定是所有点对的最小距离，我们还需要关注以分隔带为对称、宽度为2d垂直地带的点。初始dmin = d，设垂直带中的点集合为S（S来自Q，也是按照y轴升序的），假设p(x,y)为S中的点，则若下一点p'(x',y')与p的距离小于dmin , 则p'一定在如下的矩形里面，并且|y'-y|<dmin，p'一定在矩形的另外半边。 该算法主要利用了以下事实：矩形内一般只能包含少量的候选点，最多不会大于6（证明不会 : < ），所以最多只用考虑S中点p后面5个点。

   ![image-20210412181421778](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412181421778.png)

   ![image-20210412182606081](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412182606081.png)



伪代码：

![image-20210412182920424](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412182920424.png)

![image-20210412182932816](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412182932816.png)

递归式子里面别看2重循环，时间复杂度为F(n)，所以递推式为T(n)=T(n/2)+f(n)，得到时间复杂度为O(NlogN)。

# 变治法

## 高斯消去法

![image-20210412213249108](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412213249108.png)

将方程组（n元n次方程组）A经过一系列初等变换，变换成A' (上三角的形式)，然后求出xn的值，代入第n-1个方程，求出xn-1....直到求出所有解。

初等变换：经过初等变换无法改变方程的解

- 交换方程组中两个方程的位置
- 某个方程*k(k != 0)
- 一个方程与另外一个方程之间做倍差

![image-20210412213646141](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412213646141.png)

过程如下：

![image-20210412214051156](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412214051156.png)

![image-20210412215658563](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412215658563.png)

时间复杂度为O(N^3)，高斯消去的第二个阶段反向替换也O(N*N),所以整体时间复杂度为 O(N^3)。

高斯消去伪代码

```
算法 getAns(A[1..n, 1..n+1])
    //输入：上三角系数的矩阵
    //输入：解向量ans[1..n]
    ans
    for i<- n downto 1 do 
    	res<-A[i][n+1]
    	for j<-n downto i+1 do
    		res <- res - ans[i+1]*A[i][j]
    	res <- res/A[i][i]
    	ans[i] <- res
    return res
```

## LU分解法

![image-20210412222015307](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412222015307.png)

可以看出，两个矩阵L U的乘积等于A，所以解方程组Ax = b等价于解方程组 LUx=b，设y=Ux，则Ly=b。先求解Ly=b，由于L是一个下三角，所以很容易解，然后解Ux=y，由于U是一个上三角，所以也很容易解。这个解法明显优于上面那个解法，时间复杂度可以降为O(N^2)。请注意，**LU分解不需要额外空间**，可以把L下面的数据保存到U的0位置即可。

## 计算矩阵的逆

![image-20210412223132385](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412223132385.png)

相当于解n个方程组，最好还是用LU分解法

## 计算行列式的值 & 克拉默法则

![image-20210412224024966](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412224024966.png)

对于一个行列式n介行列式可以按照第i行展开转换成 (-1)^(i+j) * aij*det[去掉i行j列的方阵] 之和。如果直接求一个n阶行列式，需要求n!次乘法。所以可以利用高斯消元法，将其转换成一个上三角矩阵，**一个上三角矩阵的行列式等于其主对角线的乘积**。而初等变换，只会改变行列式的倍数（两行相减不会改变行列式的值），找个变量记录下就好。

有了求行列式的方法，可以使用克拉默法则求解方程组

![image-20210412224647993](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210412224647993.png)

detAj 是把A 的j列用b替换后得到的行列式。

## AVL树

平衡二叉树是由2位前苏联数学家提出来的（G.M.Adelse-Velskil 和 E.M.Landis）所以也叫AVL树。

### 定义

AVL树仍然是一棵二叉查找树，只是在其基础上增加了平衡的概念：对于AVL树的任意结点来说，其左子树与右子树的高度之差的绝对值不超过1，其中左子树与右子树的高度之差称为该节点的**平衡因子**。只要能时刻满足每个结点的平衡因子绝对值不超过1，AVL的高度始终能保持在logn级别。

![image-20210331141806778](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331141806778.png)

### 插入时调整

只要把最靠近插入结点的失衡结点调整到正常，路径上所有结点都会平衡。失衡的情况有4种

#### LL型

A的平衡因子是2，A左孩子B的平衡因子是1。则需要A右旋

![image-20210331142650826](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331142650826.png)



#### LR型

A的平衡因子是2，A左孩子平衡因子-1，需要A左孩子左旋，A右旋

![image-20210331142854369](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331142854369.png)



#### RR型

A的平衡因子是-2，A右孩子平衡因子-1，需要A左旋

![image-20210331143013409](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331143013409.png)

#### RL型

A的平衡因子是-2，A右孩子平衡因子1，需要A右孩子右旋，A左旋

![image-20210331143110026](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331143110026.png)



## 2-3树

### 定义

> 一棵2-3查找树为一棵空树或者由以下节点组成：
>
> 2-节点， 含有一个键和2条边，左边指向的2-3树中的键都小于这个键，右键指向的2-3树种的键都大于这个键。
>
> 3-节点， 含有2个键3条边，左边指向的2-3树小于小键，中边指向2-3树的键大于小键小于大键，右边指向的2-3树的键大于大键。

2-3树还有最后一个要求：树中的所有结点必须位于同一层，也就是说一棵2-3树是高度平衡的。

示例如下：

![image-20210301224030317](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301224030317.png)

### **插入新键K的做法**

1. 如果是空树，则新建一个结点插入。否则通过查找找到K合适的位置（这个位置总是叶子结点）
2. 如果找到的叶子是2结点，直接按照大小关系插入
3. 如果是3结点，先插入称为4结点，在分裂成3个2结点，中将的那个并入父节点中。如果父节点也变成了4结点，则需要向上分裂，直到满足定义为止。

栗子：

![image-20210413000457774](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413000457774.png)

### 树的高度问题

如果n个结点的2-3树，里面都是2结点，则h<= log(n+1)-1，如果都是3结点则h>=log~3~(n+1)-1，所以最差情况下树的查找、插入、删除时间效率属于O(logn)



## 堆排序

**堆是一棵完全二叉树**，树种的每个结点的值都不小于其左右孩子结点的值，那么称这样的堆为大顶堆。大顶堆堆一般用于优先队列的实现。

堆使用数组来表示，heap[1]代表堆顶元素。

### 创建堆

1，建立堆，
建立堆的过程总是从右到左从上到下 依此调整有孩子的节点成为最最大/小堆，时间复杂度**O(n)**

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
/删除堆顶元素
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

## 霍纳法则

是一个求多项式和的一个算法，把多项式转换成如下形式

![image-20210413230039331](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413230039331.png)

可以使用一张二维表帮助计算，第一行表示an~a0，第二行第一列是an，其他单元格都存储中间结果。

![image-20210413230452771](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413230452771.png)

![image-20210413230516042](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413230516042.png)



## 二进制幂

### 递归做法

求a^b 快速做法

1. 如果b是奇数则有a^b = a * a^(b-1)
2. 如果b是偶数则有a^b = a ^(b/2) * a ^(b/2)

```java
long binaryPow(long a, long b) {
    if (b == 0) {
        return 1;
    }
    if ((b & 1) == 1) { //如果b是奇数
        return b * binaryPow(a, b - 1);
    }
    long muti = binaryPow(a, b / 2);
    return muti * muti;
}
```

### 迭代做法

如求a^13  13的二进制位1101 

![image-20210413231040640](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413231040640.png)



如果使用霍纳法则（变体）求解，时间复杂度为O(b(n))。 b(n)为n的bit位数，直接看后面代码吧。

![image-20210413231331155](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413231331155.png)

```java
// 比个栗子，
	// 如求a^13  13的二进制位1101 
	// a ^ 13 = (a ^ 1) * (a ^ 4) * (a ^ 8)
long binaryPow(long a, long b) {
    long res = 1;
    while (b > 0){
        if((b & 1) == 1){
            res = res * a;
        }
        a = a * a;
        b >>= 1;
    }
    return res;
}
```

## 



# 时空权衡

## 计数排序

### 比较次数排序

对arr[n]进行排序，设置count[n]，count[i]表示列表中小于arr[i]元素的个数。然后设置新数组newArr[n]，遍历arr[n], 对于arr[i], 他在newArr中的位置是count[n]。直接上代码：

```java
int[] comparisonCountingSort(int[] arr){
    int[] count = new int[arr.length], res = new int[arr.length];
    for (int i = 0; i < arr.length; i++) {
        //每个元素都和arr[i]进行比较
        for (int j = i + 1; j < arr.length; j++) {
            if(arr[i] > arr[j]){
                count[i] ++;
            }else {
                count[j] ++;
            }
        }
    }
    for (int i = 0; i < arr.length; i++) {
        res[count[i]] = arr[i];
    }
    return res;
}
```

**这个算法时间复杂度为O(N*N)，但是可以使得键值移动的次数最少。**

### 分布计数法

如果元素的值是位于[l,r] （l,r差距比较小）之间的，我们可以计算l,r中每个数出现的频率，保存到数组F[0..r-l]之间。**然后对F数组求前缀和，prefix，则prefix[ i ] 表示 小于等于l+i元素的个数**。这个prefix也叫分布值,prefix[i]表明了l + i元素最后一次出现时的正确位置（减一）。

所以可以从右到左遍历arr，则arr[i]的位置就是prefix[arr[i]-l]，然后我们需要prefix[arr[i]-l]--，以便下一遍历到等于arr[i]数时能得到正确的位置。

上代码！

```java
int[] distributionCountingSort(int[] arr, int l, int r) {
    int[] distribution = new int[r - l + 1], res = new int[arr.length];
    //首先计算各个元素出现的频率
    for (int i = 0; i < arr.length; i++) {
        distribution[arr[i] - l] ++;
    }
    //前缀和，直接放到分布数组里里面
    for (int i = 1; i < distribution.length; i++) {
        distribution[i] += distribution[i - 1];
    }
    for (int i = 0; i < arr.length; i++) {
        res[--distribution[arr[i] - l]] = arr[i];
    }
    return res;
}
```



## Horspool算法

![image-20210413154708294](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413154708294.png)

![image-20210413154720343](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413154720343.png)



我们可以预先计算出这个算法每次移动的距离，并把他们存入表中。

![image-20210413154844165](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413154844165.png)

求这个表的算法：

![image-20210413155131069](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413155131069.png)

**Horspool 算法**

1. 先构造上述表
2. 将模式与文本的开始处对齐
3. 从模式的最后一个字符开始，比较模式和文本中相应字符直到：所有m个字符都匹配，或者遇到了一对不匹配的字符。c是当前文本中与m对齐的最后一个字符，则模式串向右移动t(c)距离

![image-20210413161102516](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413161102516.png)

Horspool霍斯普尔 算法的最差效率属于O(MN)，但是对于随机文本来说，他的效率是属于O(N)的。



代码实现：

```java
public class Horspool {
	//返回m中前m-1字符中最右边的c到达最后一个字符的距离
	Map<Character, Integer> shiftTable(String m) {
		int n = m.length();
		Map<Character, Integer> map = new HashMap<>();
		for (int i = 0; i < n - 1; i++) {
			map.put(m.charAt(i), n - 1 - i);
		}
		return map;
	}

	int horspool(String t, String m) {
		Map<Character, Integer> map = shiftTable(m);
		int i = m.length() - 1;
		while (i < t.length()) {
			int k = 0;
			//从后往前匹配
			while (k < m.length() && m.charAt(m.length() - 1 - k) == t.charAt(i - k)) {
				k ++;
			}
			if(k == m.length()){
				return i - m.length() + 1;
			}else {
				i += map.getOrDefault(t.charAt(i), m.length());
			}
		}
		return -1;
	}

	@Test
	public void test(){
		System.out.println(horspool("ABARBERA", "BARBER"));

	}
}
```

## Boyer-Moore算法

再遇到一个不匹配字符前，如果已经有k（0<k<m）个字符成功匹配了，这和Horspool的操作是不同的。这种情况下，BM算法会参考2个数值来确定移动的距离，第一个是文中的第一个不匹配字符c。这种情况下的移动称之为**坏符号移动**。

坏符号移动主要有2种操作：

如果c不在模式m中，我们把模式移动到正好跳过这个字符的位置。如下图，c=s（从右边开始第一个不匹配的字符）

![image-20210413172923993](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413172923993.png)

如果c再模式种，则跳到下次c出现的位置。

![image-20210413173332943](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413173332943.png)

**可以看出，跳的距离为：d1=t(c)-k**

但是d1=t(c)-k是有可能小于0的，此时d1=1。由此可得：**d1=max{t(c)-k, 1}**

**第二种移动称之为好后缀移动**

k > 0个成功匹配，把这k个叫做模式串m的后缀suff(k)。如果模式种存在另外一个串等于suff(k) 并且，另外一个串的前一个字符不等于c（如果等于c会重复失败），此时我们可以把模式向右移动右数第二个suff(k)串，到suff(k)串的距离的长度。如下，如果suff(2)=4,右边第二个串中B距离右边第一个串B距离为4。如果suff(k)前面没有与之相等的串，此时suff(k) = m.length();

![image-20210413220747678](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413220747678.png)

但是这样移动不总是对的，如下图，suff(3)=6，移动6个，会错过一个串，原因是AB是相等前后缀，所以此时最多移动最长前后缀**相距**的长度。

![image-20210413221114453](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413221114453.png)

![image-20210413221403662](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413221403662.png)





BM 算法过程

![image-20210413221511064](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413221511064.png)



## B树

B树的所有数据记录都在叶子结点

![image-20210413234540264](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210413234540264.png)





# Reference

凸包： https://blog.csdn.net/u011001084/article/details/72768075

算法设计与分析基础

算法笔记  胡凡 曾磊

