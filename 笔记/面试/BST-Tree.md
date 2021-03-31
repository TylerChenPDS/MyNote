# BST树

二叉查找树（Binary Search Tree）

## 定义

- BST要么是一颗空树
- 要么二叉树由根节点、左子树、右子树组成，其中左子树和右子树都是二叉查找树，且左子树上所有节点的数据域均小于或等于根节点的数据域，右子树上所有数据均大于根节点的数据域。

## 查找

思路：

- 如果root节点为空，查找失败
- 要查找的x == root.data，查找成功
- 要查找的x < root.data， 应当往左子树上查找，因此向root.left 递归
- 要查找的x > root.data， 应当往右子树上查找，因此向root.right 递归

查找将会是从根节点到查找结点的一条路径，故最坏时间复杂度为O(h), 其中h是树的高度

## 插入

如果要插入x，则要先找x，找到为null的地方就是需要插入的地方。如果成功找到x则插入失败。时间复杂度也是O(h)。

构造树的时候也是一个结点一个结点的插入，如果用户给的数据是顺序的数据，就会导致树的高度很高，查找退化成O(N)，这也就是后面为什么需要构造平衡树的原因。

![image-20210331135053597](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331135053597.png)



## 删除

也是先要查找到要删除的结点X，

- 如果X是叶子结点，则令其父节点指向null即可

- 如果X是1度结点（有一个孩子结点），令其父节点指向X的孩子结点

  ![image-20210331134246776](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331134246776.png)

- 如果X是2度结点（有左右2个孩子），则先令其与其后序/前驱结点交换数据域，然后删掉其后序/前驱结点。

  ![image-20210331134406316](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331134406316.png)

  如图，若要删除结点5，则将结点6的数据放到5里面，然后转为删除6。

  ![image-20210331134757846](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331134757846.png)



# 左旋&右旋

要点：

- 左旋&右旋要保证旋转之后仍然是平衡二叉树
- 左旋&右旋是针对树根的
- 左旋可能会改变树的平衡信息，这也是实现平衡树的关键



## 左旋

1. 让B的左子树称为A的右子树

2. 让A称为B的左子树

3. 将根节点设置为B（返回B）

   ![image-20210331135645981](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331135645981.png)

![image-20210331135420805](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331135420805.png)

## 右旋

1. 让A的右子树称为B的右子树
2. 让B称为A的右子树
3. 根节点设置为A (返回A)



![image-20210331140932228](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331140932228.png)

![image-20210331140953197](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331140953197.png)

# AVL树

平衡二叉树是由2位前苏联数学家提出来的（G.M.Adelse-Velskil 和 E.M.Landis）所以也叫AVL树。

## 定义

AVL树仍然是一棵二叉查找树，只是在其基础上增加了平衡的概念：对于AVL树的任意结点来说，其左子树与右子树的高度之差的绝对值不超过1，其中左子树与右子树的高度之差称为该节点的**平衡因子**。只要能时刻满足每个结点的平衡因子绝对值不超过1，AVL的高度始终能保持在logn级别。

![image-20210331141806778](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331141806778.png)

## 插入时调整

只要把最靠近插入结点的失衡结点调整到正常，路径上所有结点都会平衡。失衡的情况有4种

### LL型

A的平衡因子是2，A左孩子B的平衡因子是1。则需要A右旋

![image-20210331142650826](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331142650826.png)



### LR型

A的平衡因子是2，A左孩子平衡因子-1，需要A左孩子左旋，A右旋

![image-20210331142854369](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331142854369.png)



### RR型

A的平衡因子是-2，A右孩子平衡因子-1，需要A左旋

![image-20210331143013409](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331143013409.png)

### RL型

A的平衡因子是-2，A右孩子平衡因子1，需要A右孩子右旋，A左旋

![image-20210331143110026](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331143110026.png)



# 2-3 查找树

## 定义

> 一棵2-3查找树为一棵空树或者由以下节点组成：
>
> 2-节点， 含有一个键和2条边，左边指向的2-3树中的键都小于这个键，右键指向的2-3树种的键都大于这个键。
>
> 3-节点， 含有2个键3条边，左边指向的2-3树小于小键，中边指向2-3树的键大于小键小于大键，右边指向的2-3树的键大于大键。

示例如下：

![image-20210301224030317](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301224030317.png)

查找算法和普通BST类似

主要看插入算法

## 插入

### 向2-节点中插入新键

直接把2-节点变成3-节点即可。如图：

![image-20210301224355013](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301224355013.png)

### 向一棵只含有3-节点的树中插入新键

把新键存入这个3-节点，使之称为4-节点，然后把转化成3个2-节点。如图：

![image-20210301224559413](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301224559413.png)

### 向一个父节点为2-节点的3-节点插入新键

把新键存入这个3-节点，使之称为4-节点，但此时不会为中键创建新节点，而是将其移动至父亲节点中。

![image-20210301224840517](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301224840517.png)

### 向一个父节点为3-节点的3-节点插入新键

把新键存入这个3-节点，使之称为4-节点，将其移动至父亲节点中，但是父亲节点又成了4-节点，然后接着往上操作，直到遇到一个2-节点，或者是遇到3-节点的根。

如果是遇到了3-节点的根，则可以按照方法：向一棵只含有3-节点的树中插入新键。将其变成4-节点然后分解。注意：此时树的高度加一。

![image-20210301225822653](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301225822653.png)

## 局部变换不影响全局性质

将一个4-节点分解成2-3树可能有6种情况，但是不论怎么变都不会改变全局的有序性和平衡性。

![image-20210301230232101](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301230232101.png)

变换之前空链到根节点的长度为h，变换之后还是h（除了分解那个会变成h+1）

# 红黑二叉查找树

## 定义

红黑二叉查找树背后的思想是使用标准的二叉查找树（全是2-节点构成的）和一些额外信息。

我们将树中的连接（边）分成2种类型：

- 红链接将两个2-节点连接起来构成3-节点
- 黑链接则是2-3中的普通连接

也就是我们把3-节点表示成一条**左**斜的红色连接。如图：（粗边表示红连接）

![image-20210301231021102](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301231021102.png)

对于任何2-3树，只要进行进行节点转换，我们就可以派生出来对应的二叉查找树。我们把这种方式表示的二叉查找树称为红黑二叉查找树（红黑树）。

另外一种等价的定义是：

含有红黑连接并满足下列条件的二叉查找树：

- 红链接均为左连接
- 没有任何一个结点同时和2条红连接相连
- 该树是完美黑色平衡的，即任意空连接到根节点的路径上的黑连接数量相同

性质：

- 一棵大小为N的红黑树的高度不会超过2lgN。

  证明：红黑树的最坏情况是它对应的2-3树中构成的最左边路径全都是3-节点，其余均为2-节点。此时最左边路径长度为只包含2-节点的2倍（lgn）。

颜色表示：

把连接的颜色保存在Node中的color中。如果指向它的连接时红色的，该变量为true，黑色则为false。**当我们提到一个结点的颜色的时候指的是指向该结点连接的颜色。**

注意：约定空节点为黑色。

部分代码:

```java
static final boolean RED = true;
static final boolean BLACK = false;
Node root;
private class Node {
    int value;
    Node left, right;
    int n;
    boolean color;

    Node(int value, int n, boolean color) {
        this.value = value;
        this.n = n;
        this.color = color;
    }
}

boolean isRed(Node x) {
    if (x == null) {
        return false;
    }
    return x.color == RED;
}
```

旋转

左旋：

![image-20210301232637347](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301232637347.png)

![image-20210301232649558](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301232649558.png)

右旋：

![image-20210301232706241](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301232706241.png)

![image-20210301232713498](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301232713498.png)

旋转会返回一个连接，可能是黑的，也可能是红的，所以我们将用x.color=h.color保留原连接的红黑状态。部分代码如下：

```java
Node rotateLeft(Node h) {
		Node x = h.right;
		h.right = x.left;
		x.left = h;
		x.color = h.color;
		h.color = RED;
		x.n = h.n;
//		h.n = 1 + size(h.left) + size(h.right);
		return x;
	}

	Node rotateRight(Node h) {
		Node x = h.left;
		h.left = x.right;
		x.right = h;
		x.color = h.color;
		h.color = RED;
		x.n = h.n;
//		h.n = 1 + size(h.left) + size(h.right);
		return x;
	}
```

## 插入

### 向单个2-节点插入新键

（新增的一定是红色节点，这样可以尽可能保证黑色平衡）

如果新键小于老键，新增一个红色节点即可（即指向新键的红链），如果新键大于老键，新增的红色节点将产生右链，不满足条件，需要将老键进行左旋：`root = rotate(root)`，如图：

![image-20210301233455803](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301233455803.png)

### 向3-节点插入新建

可以分为3种子情况：

- **新键大于原树中的2个键**。这样中间的那个键就会有2条红边，直接把2条红边变黑，然后中间那个节点变红（根节点除外）。如图左1。
- **新键小于原树中的2个键。**会产生两个相连的左红连接，需要右旋，然后变色。如图左2.
- **新键位于原树2个件之间。**会产生一个左红连接和右红连接相连。需要先左旋变成情况2，然后右旋变成情况1，然后变色。如图左3。



![image-20210301233920564](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301233920564.png)

颜色变换的时候需要将父节点的颜色由黑变红。这样是为了不影响整棵树的黑色平衡性。



变色之后呢？红链将向上传递。

![image-20210301234921499](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210301234921499.png)

总结：

插入之后：

- 如果右子节点是红色的而左子节点是黑色的则左旋，
- 如果左子节点是红的且左子节点的左子节点也是红的，则右旋。
- 如果左右节点均为红色，则颜色转换。

## 

# 跳表

SkipList  

跳跃表其实也是一种通过“**空间来换取时间**”的一个算法，通过在每个节点中增加了向前的指针，从而提升查找的效率。每一个结点不单单只包含指向下一个结点的指针，可能包含很多个指向后续结点的指针，这样就可以**跳过**一些不必要的结点，从而加快查找、删除等操作。对于一个链表内每一个结点包含多少个指向后续元素的指针，后续节点个数是通过一个随机函数生成器得到，这样子就构成了一个跳跃表。时间复杂度为O(N)

![image-20210331145445703](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331145445703.png)

跳跃表使用**概率均衡技术**而不是使用强制性均衡技术，**因此，对于插入和删除结点比传统上的平衡树算法更为简洁高效**。

## 查找

查找就是给定一个key，查找这个key是否出现在跳跃表中，如果出现，则返回其值，如果不存在，则返回不存在。如果我们想查找19是否存在？如何查找呢？我们从头结点开始，首先和9进行判断，此时大于9，然后和21进行判断，小于21，此时这个值肯定在9结点和21结点之间，此时，我们和17进行判断，大于17，然后和21进行判断，小于21，此时肯定在17结点和21结点之间，此时和19进行判断

![image-20210331145712413](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331145712413.png)

## 插入

插入包含如下几个操作：1、查找到需要插入的位置  2、申请新的结点   3、调整指针。 还包括随机生成的指针个数。

**查找路径**如下图的***灰色的线***所示  申请新的结点如17结点所示， 调整指向新结点17的指针以及17结点指向后续结点的指针。这里有一个小技巧，就是使用update数组保存大于17结点的位置，***update数组的内容如红线所示***，这些位置才是**有可能**更新指针的位置。

![image-20210331150137710](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331150137710.png)

## 删除

删除操作类似于插入操作，包含如下3步：1、查找到需要删除的结点 2、删除结点  3、调整指针

![image-20210331150322343](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210331150322343.png)

# Reference

[红黑树]  算法（第4版）

[BST树]   算法笔记  胡凡 曾磊

[跳表] https://blog.csdn.net/sunxianghuang/article/details/52221913

[所有源代码参见] https://gitee.com/CTLQAQ/study/tree/master/Leetcode/src/main/java/LeftGod/%E5%9F%BA%E7%A1%80%E6%8F%90%E5%8D%87/%E5%B9%B3%E8%A1%A1%E6%A0%91