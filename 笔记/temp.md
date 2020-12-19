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



### 例题：



