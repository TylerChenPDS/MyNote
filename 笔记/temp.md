## Floyd算法

Floyd算法解决全员最短路径问题，时间复杂度是O(n^3)。

Floyd算法：如果存在顶点k，使得以k作为中介点i和j的当前最短距离缩短，则使用顶点k作为顶点i和j的中介点。即`dis[i][k] + dis[k][j] < dis[i][j],则令 dis[i][j] = dis[i][k] + dis[k][j] `

Floyd算法伪代码：

```go
枚举顶点k
	以顶点k作为中介点，枚举所有顶点对i,j
		如果 dis[i][k] + dis[k][j] < dis[i][j]
			则dis[i][j] = dis[i][k] + dis[k][j]
```

实现

```c++
const int INF = 0x3fffffff;
const int maxv = 200;
int n, m; //顶点数，边数
int dis[maxv][maxv];
int G[maxv][maxv];

void Floyd(){
    //初始化
    //将图中的数据赋值给dis，图中不存在的边为INF，dis中需要修改下
    for (int i = 0; i < n; ++i) {
        dis[i][i] = 0;
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if(dis[i][k] != INF && dis[k][j] != INF && dis[i][k] + dis[k][j] < dis[i][j]){
                    dis[i][j] = dis[i][k] + dis[k][j];
                }
            }
        }
    }
}
```

## 最小生成树算法

最小生成树（MST Minimum Spanning Tree）：给定一个**无向图**G(V,E)，求一棵树T使得这棵树包含G的所有顶点，所有边都来自G，并且满足整棵树的边权最小。

### Prim算法

#### 基本思想

对图G(V,E) 设置集合S，存放已被访问的顶点, V-S表示未被访问的点。每次从集合V-S中选择与集合S的最短距离最小的一个顶点u，访问并加入集合。之后以u为中介点，优化所有能从u到达的顶点v与集合S之间的最短距离。

和Dijkstra算法很像。只有优化的部分不一样。

#### 直接上代码

邻接表实现

```c++
const int maxv = 510; //最大点的个数
const int INF = 0x3fffffff;
struct Node {
    int v; //node的目标点
    int d;// 到达目标点的距离
};
vector<Node> G[maxv]; //图
int d[maxv];//距离集合S的最短路径，
bool visited[maxv] = {false};
int N, C1;//N,点的个数，C1，起点
int ans = 0; //存放最小生成树的边权集合
void prim() {
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
        ans += d[u];
        for (int v = 0; v < G[u].size(); v++) {
            Node node = G[u][v];
            if (visited[node.v] == false) {
                if (node.d < d[node.v]) {//如果u->v的距离，小于d[v]，则更新
                    d[node.v] = node.d; 
                }
            }
        }
    }
}
```

#### 注意

prim算法时间复杂度为O(V^2)，使用邻接表通过堆优化时间复杂度降为O(VlogV + E) 。说明，尽量在图的顶点较少而边较多的情况下使用prim算法。

### Kruskal算法

#### 基本思想：

克鲁斯卡尔算法使用**边贪心**算法，基本思想为：

初始情况下，擦掉所有的边。然后执行下面步骤：

1. 对所有边按边权从小到达进行排序。
2. 按照边权从小到达测试所有边，如果当前测试边所连接的顶点不在一个连同块中，则把这条边加入当前的最小生成树中。
3. 执行步骤2直到最小生成树的边等于V-1

#### 伪代码

```c++
void kruskal(){
    令最小生成树的边权和ans=0,最小生成树当前边数num=0;
    将所有边排序;
    for(从小到达枚举边e){
        if(e的两个顶点不在一个联通块中){
            ans = e.cost;
            num ++;
            if(num == v-1){
                return;
            }
        }
    }
}
```

如何判断两个顶点是否在一个集合中？  并查集

#### 实现

```c++
#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;
const int maxv = 500;
struct Edge {
    int u, v;
    int cost;
}edge[maxv];
//边权从小到达排序
bool cmp(Edge a, Edge b){
    return a.cost < b.cost;
}

int n;//顶点个数
int father[maxv];
void init(){
    for (int i = 0; i < n; ++i) {
        father[i] = i;
    }
}
int findFather(int x){
    int a = x;
    while (x != father[x]){
        x = father[x];
    }
    while (a != father[a]){
        int z = a;
        a = father[a];
        father[z] = x;
    }
    return x;
}
int kruskal(int m){//m为边的个数
    int ans = 0,num_edge = 0;
    init();//并查集初始化
    sort(edge,edge + m, cmp);
    for (int i = 0; i < m; ++i) {
        int faU = findFather(edge[i].u);
        int faV = findFather(edge[i].v);
        if(faU != faV){ //如果不在一个集合中
            father[faU] = faV;//合并集合
            ans += edge[i].cost;
            num_edge ++;
            if(num_edge == n -1){//边已经够了，
                break;
            }
        }
    }
    return ans;
}


```

