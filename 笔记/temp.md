## 拓扑排序

**拓扑排序的很重要的一个应用是判断一个给定的图是否是有向无环图。**

DAG(Directed Acyclic Graph, DAG): 有向无环图。

拓扑排序是将有向无环图G的所有顶点排成一个线性序列，使得对图G中的任意两个顶点u、v，如果存在u->v , 那么在序列中u一定在v的前面。

![image-20201223110749720](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201223110749720.png)

排序后（不唯一）

![image-20201223110821790](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201223110821790.png)

拓扑排序步骤：

1. 定义一个队列Q，并把所有入度为0的结点加入队列。
2. 取队首结点，输出。然后删除所有从他出发的边，并令这些边到达的顶点入度减一，如果某个顶点入度减为0，则将其加入队列。
3. 反复进行2操作，直到队列为空。如果队列为空时入过队的结点数目恰好为n，说明拓扑排序成功，图G为有向无环图；否则拓扑排序失败，图G中有环。

实现：

```c++
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;
const int maxv = 1000;
vector<int> G[maxv];
int n,  inDegree[maxv];//结点个数，某个结点的入度（在读入结点的时候可以确定）
bool topologicalSort(){
    int num = 0; //加入拓扑序列的顶点个数
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if(inDegree[i] == 0){
            q.push(i);//将所有入度为0的顶点加入队列
        }
    }
    while (!q.empty()){
        int u = q.front();
        q.pop();
        for (int i = 0; i < G[u].size(); ++i) {
            int v = G[u][i];//u的后继结点
            inDegree[v] --;
            if(inDegree[v] == 0){
                q.push(v);
            }
        }
        G[u].clear(); //清空顶点u的所有出边（入无必要可以不写）
        num ++; //加入拓扑序列的顶点数加一
    }
    if(num == n){
        return true; //拓扑排序成功，给定的图是有向无环图
    }else{
        return false; //说明图中有环
    }
}
```

