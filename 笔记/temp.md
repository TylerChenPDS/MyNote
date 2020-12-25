## 关键路径

### AOV 网和 AOE网

AOV网（Avtivity On Vertex）顶点活动图是指用顶点表示活动，而用边集表示活动间优先关系的有向图。如下图

![image-20201225092956815](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201225092956815.png)

AOE网（Avtivity On Vertex）边活动网是指用带权的边集表示活动，而用顶点表示事件的有向图，其中边权表示完成活动所需要的时间。如下图

![image-20201225093535436](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201225093535436.png)

**考虑到工程总会有一个起始时刻和结束时刻，因此AOV网一般只有一个源点与结束点。**

**AOE网中的最长路径被称为关键路径，如a1 a3 a5 a6 就是关键路径，因为这条路径决定了工程的最短完成时间。**



### 求最长路径

对一个没有正环的图，要求其最长路径，可以将所有的边权乘以-1，令其变为相反数，然后使用Bellman-Ford算法，或SPFA算法求最短路径，最后将结果求反即可。 注意：不能使用Dijkstra算法。



### 关键路径的求法 ** 

比较复杂，算法笔记p419

求解有向无环图的（DAG）最长路径的方法。

**设置两个数组e,l 其中e[r] 和 l[r]分别表示活动a~r~ 最早开始时间和最迟结束时间。求出数组后如果e[r] == l[r] ，表示活动r的最早开始时间和结束时间一样，证明 活动r是关键路径。**

#### 如何求解e, l ？

![image-20201225100728283](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201225100728283.png)

如图，事件V~i~在经过活动a~r~到达事件V~j~ 。设置数组ve,vl, ve[i]，vl[i]表示事件i 最早和最迟的发生时间。

1. 对于a~r~来说，e[r] = ve[i]，活动r的最早发生时间，就是事件i的最早发生时间。
2. l[r] + len[r] = vl[j] 活动r最迟发生事件，加上活动r所需要的时间，就是事件j的最迟发生时间。

于是求解出ve，和vl 就可以求出e 和 l

### 求解ve -- 事件的最早发生时间

![image-20201225101617366](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201225101617366.png)

如图，V~j~的最早发生时间就是ve[i1] + len[r1] ~ ve[ik] + len[rk]中的最大值。因为V~j~的发生要等到所有Vi 都发生才能发生。

可以发现，要想获得ve[j]的正确值，ve[i1] ~ ve[ik]都必须已经得到了。因此，在访问vj时必须保证其前驱结点都已经被访问过，可以使用拓扑排序。**一个好的做法时，在拓扑排序访问到vi时，让ve[i]更新ve[j]的值**，这样访问到ve[j]时，ve[j]的所有前驱都已更新过它，保证了它的正确性。

```c++
#include <cstdio>
#include <algorithm>
#include <queue>
#include <stack>
using namespace std;
struct Node{
    int v,w;
};
const int maxv = 100;
vector<Node> G[maxv];
//拓扑排序
int n;//顶点个数
int inDegree[maxv];
int ve[maxv], vl[maxv];
stack<int> topOrder;


bool topologicalSort(){
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if(inDegree[i] == 0){
            q.push(i);
        }
    }
    while (!q.empty()){
        int u = q.front();
        q.pop();
        topOrder.push(u);
        for (int i = 0; i < G[u].size(); ++i) {
            int v = G[u][i].v;
            inDegree[v] --;
            if(inDegree[v] == 0){
                q.push(v);
            }
            if(ve[u] + G[u][i].w > ve[v]){
                ve[v] = ve[u] + G[u][i].w;
            }

        }
    }
    if(topOrder.size() == n){
        return true;
    }
    return false;
}
```

### 求解vl--事件发生的最晚时间

![image-20201225103139457](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201225103139457.png)

如图，vi 的最迟发生时间，就是vl[j1] - len[r1] ~ vl[jk] - len[rk]中的**最小值**，这样的话能保证所有vj都能在，规定时间内完成。

如何实现？可以使用拓扑逆序列来实现。

![image-20201225100728283](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201225100728283.png)

如图，当访问vj的时候，用vl[j]更新vl[i],这样当v[i] 被访问到的时候就一定是正确的。在拓扑逆序列中vj 一定在vi之前被访问。

```c++
void getVl(){
    //初始值，都设置为终点的最迟（早）发生时间。
    fill(vl, vl + n, ve[n -1 ]);

    while(!topOrder.empty()){
        int u = topOrder.top();
        topOrder.pop();
        for (int i = 0; i < G[u].size(); ++i) {
            int v = G[u][i].v;//
            if(vl[u] - G[u][i].w < vl[u]){
                vl[u] = vl[u] - G[u][i].w;
            }
        }
    }
}
```

### 总结

![image-20201225104342005](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201225104342005.png)



### 最后的关键路径长度

如果事先不知道终点编号，那么关键路径的长度就是ve[]中最大的值。



### 完整代码

这里假设v[n-1]是 终点

```c++
#include <cstdio>
#include <algorithm>
#include <queue>
#include <stack>
#include <cstring>
using namespace std;

struct Node{
    int v,w;
};
const int maxv = 100;
vector<Node> G[maxv];
//拓扑排序
int n;//顶点个数
int inDegree[maxv];
int ve[maxv], vl[maxv];
stack<int> topOrder;


bool topologicalSort(){
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if(inDegree[i] == 0){
            q.push(i);
        }
    }
    while (!q.empty()){
        int u = q.front();
        q.pop();
        topOrder.push(u);
        for (int i = 0; i < G[u].size(); ++i) {
            int v = G[u][i].v;
            inDegree[v] --;
            if(inDegree[v] == 0){
                q.push(v);
            }
            if(ve[u] + G[u][i].w > ve[v]){
                ve[v] = ve[u] + G[u][i].w;
            }

        }
    }
    if(topOrder.size() == n){
        return true;
    }
    return false;
}

void getVl(){
    //初始值，都设置为终点的最迟（早）发生时间。
    fill(vl, vl + n, ve[n -1 ]);

    while(!topOrder.empty()){
        int u = topOrder.top();
        topOrder.pop();
        for (int i = 0; i < G[u].size(); ++i) {
            int v = G[u][i].v;//
            if(vl[u] - G[u][i].w < vl[u]){
                vl[u] = vl[u] - G[u][i].w;
            }
        }
    }
}

int CriticalPath(){
    memset(ve, 0, sizeof(ve));
    if(topologicalSort() == false){
        return -1;
    }//有环
    getVl();
    //遍历所有边，计算活动的最早开始时间和e和最迟开始时间l
    for (int u = 0; u < n; ++u) {
        for (int i = 0; i < G[u].size(); ++i) {
            int v = G[u][i].v, w = G[u][i].w;
            int e = ve[u], l = vl[v] - w;
            if(e == l){
                //输出关键路径
                printf("%d->%d", u,v);
            }
        }
    }
    return ve[n - 1];//返回关键路径长度。
}

```

