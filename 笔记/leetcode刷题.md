https://leetcode-cn.com/problems/cheapest-flights-within-k-stops/solution/

令`dp[i][k]`表示经过k个中转站到达i的最小费用；

例如有一条路径A->B, A->B的费用为cost，则`dp[B][K]`经过k站到达B的费用等于原来费用，和从A经过K-1站+ A->B 这条路上的花费：

所以状态转移方程为`dp[i][k]=min(dp[i][k],dp[A][k-1] + cost[A->i])，A=(所有的A->i)`

边界条件为：

```java
class Solution {
   public int findCheapestPrice(int n, int[][] flights, int src, int dst, int K) {
		int[][] dp = new int[n][K + 1];
		for (int i = 0; i < n; i++) {
			//默认都是最大值
			Arrays.fill(dp[i],Integer.MAX_VALUE);
		}
		for (int[] flight : flights){
			int s = flight[0];//起点
			int d = flight[1];//终点
			int price = flight[2];//起点到终点的距离
			if(s == src){
				dp[d][0] = price;//从src直达d的价钱
			}
		}
		//到达原站最小价钱是0
		for (int i = 0; i <= K; i++) {
			dp[src][i] = 0;
		}

		for (int i = 1; i <= K ; i++) {
			for (int[] flight : flights){
				int s = flight[0];//起点
				int d = flight[1];//终点
				int price = flight[2];//起点到终点的距离
				if(dp[s][i-1] != Integer.MAX_VALUE){ //如果可以经过i-1站到达d
					dp[d][i] = Integer.min(dp[s][i-1] + price, dp[d][i]);
				}
			}
		}
		return dp[dst][K] == Integer.MAX_VALUE ? -1 : dp[dst][K];
	}
}
```

