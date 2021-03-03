# 基础知识

## **线程的生命周期包括哪几个阶段？**

线程的生命周期包含5个阶段，包括：新建、就绪、运行、阻塞、销毁。

- 新建：就是刚使用new方法，new出来的线程；
- 就绪：就是调用的线程的start()方法后，这时候线程处于等待CPU分配资源阶段，谁先抢的CPU资源，谁开始执行;
- 运行：当就绪的线程被调度并获得CPU资源时，便进入运行状态，run方法定义了线程的操作和功能;
- 阻塞：在运行状态的时候，可能因为某些原因导致运行状态的线程变成了阻塞状态，比如sleep()、wait()之后线程就处于了阻塞状态，这个时候需要其他机制将处于阻塞状态的线程唤醒，比如调用notify或者notifyAll()方法。唤醒的线程不会立刻执行run方法，它们要再次等待CPU分配资源进入运行状态;
- 销毁：如果线程正常执行完毕后或线程被提前强制性的终止或出现异常导致结束，那么线程就要被销毁，释放资源;



## AQS的子类有哪些？

主要是Sync工具类，FairSync，和UnFiarSync



# Synchronized 相 关 问 题

## Synchronized 用 过 吗 ， 其 原 理 是 什 么 ？ 

查 看 被 Synchronized 修 饰 过 的 程 序 块 编 译 后 的 字 节 码 ， 会 发 现 ， 

被 Synchronized 修 饰 过 的 程 序 块 ， 在 编 译 前 后 被 编 译 器 生 成 了 monitorenter 和 monitorexit 两 个 字 节 码 指 令 。

在 虚 拟 机 执 行 到 monitorenter 指 令 时 ， 首 先 要 尝 试 获 取 对 象 的 锁 ： 

如 果 这 个 对 象 没 有 锁 定 ， 或 者 当 前 线 程 已 经 拥 有 了 这 个 对 象 的 锁 ， 把 锁 的 计 数 器 +1； 当 执 行 monitorexit 指 令 时 将 锁 计 数 器 -1； 当 计 数 器 为 0 时 ， 锁 就 被 释 放 了 。

如 果 获 取 对 象 失 败 了 ， 那 当 前 线 程 就 要 阻 塞 等 待 ， 直 到 对 象 锁 被 另 外 一 个 线 程 释 放 为 止 。 Java 中 Synchronize 通 过 在 对 象 头 设 置 标 记 ， 达 到 了 获 取 锁 和 释 放 锁 的 目 的 。



## synchronized 关键字最主要的三种使用方式

**1.修饰实例方法:** 作用于当前对象实例加锁，进入同步代码前要获得 **当前对象实例的锁**

**2.修饰静态方法:** 也就是给当前类加锁，会作用于类的所有对象实例 ，进入同步代码前要获得 **当前 class 的锁**。

**3.修饰代码块** ：指定加锁对象，对给定对象/类加锁。



## 什 么 是 可 重 入 性 ， 为 什 么 说 Synchronized 是 可 重 入 锁 ？

当一个线程再次获取他自己已经获取的锁，如果不被阻塞，则称该锁为可重入锁。synchronized 就是可重入锁。

实现机制：在锁的内部维护一个线程标识，用来标识该锁被那个线程占用，然后关联一个计数器。一开始 计数器值为0表示锁没有被任何线程占用，当一个线程获取了该锁，计数器就会变成1，如果 这个线程又获取一次这个锁，就会把引用计数器+1， 释放则会减1。为0时，表示该锁被释放了。



## JVM 对 Java 的 原 生 锁 做 了 哪 些 优 化 ？

由 于 Java 层 面 的 线 程 与 操 作 系 统 的 原 生 线 程 有 映 射 关 系 ， 如 果 要 将 一 个 线 程 进 行 阻 塞 或 唤 起 都 需 要 操 作 系 统 的 协 助 ， 这 就 需 要 从 用 户 态 切 换 到 内 核 态 来 执 行 ， 这 种 切 换 代 价 十 分 昂 贵 ， 很 耗 处 理 器 时 间 ， 现 代 JDK 中 做 了 大 量 的 优 化 。

一 种 优 化 是 使 用 自 旋 锁 ， 即 在 把 线 程 进 行 阻 塞 操 作 之 前 先 让 线程 自 旋 等 待 一 段 时 间 ， 可 能 在 等 待 期 间 其 他 线 程 已 经 解 锁 ， 这 时 就 无 需 再 让 线 程 执 行 阻 塞 操 作 ， 避 免 了 用 户 态 到 内 核 态 的 切 换 。



## 为 什 么 说 Synchronized 是 一 个 悲 观 锁 ？ 乐 观 锁 的 实 现 原 理 又 是 什 么 ？ 什 么 是 CAS， 它 有 什 么 特 性 ？

- **悲观锁：传统的锁机制**，例如 java 的 synchronized 关键字，他代表了 java 中悲观锁技术，保证了某一时刻仅有一个线程能访问同步代码/方法。synchronized 能够很好地工作，却有着 (相对) 比较大的性能开销。
- **乐观锁** (相对悲观锁) 对性能会有很大的帮助。他的核心思想是：寄希望于在没有冲突的情况下完成一次更新操作，使用乐观锁技术更新时会进行 “冲突检测” 来判断是否有其他的线程干扰，若是 (有其他线程干扰) 则视本次更新操作失败，一般会进行重试。在数据库中就是多版本控制，最后 更新的时候 会`update ... set version = ${version} + 1 ... where version = ${version}`, 如果version被别人修改了，则我这次修改失败。乐观锁不会使用数据库提供的锁机制，一般在表中添加version字段来实现。

**CAS是通过硬件命令保证了原子性 ** CAS有3个操作数，**内存值V，旧的预期值A，要修改的新值B**。当且仅当预期值A和内存值V相同时，将内存值V修改为B，否则返回V。

就是指当V和A两者进行比较时，如果相等，则证明共享数据没有被修改，替换成新值，然后继续往下运行；如果不相等，说明共享数据已经被修改，放弃已经所做的操作，然后重新执行刚才的操作。这样子，就能保证操作的原子性了。

CAS 具 有 原 子 性 ， 它 的 原 子 性 由 CPU 硬 件 指 令 实 现 保 证 ， 即 使 用 

JNI 调 用 Native 方 法 调 用 由 C++ 编 写 的 硬 件 级 别 指 令 ， JDK 中 提 供 了 Unsafe 类 执 行 这 些 操 作 。



## 乐 观 锁 一 定 就 是 好 的 吗 ？

乐 观 锁 避 免 了 悲 观 锁 独 占 对 象 的 现 象 ， 同 时 也 提 高 了 并 发 性 能 ， 但 它 也 有 缺 点 ：

1. **乐 观 锁 只 能 保 证 一 个 共 享 变 量 的 原 子 操 作** 。 如 果 多 一 个 或 几 个 变 量 ， 乐 观 锁 将 变 得 力 不 从 心 ， 但 互 斥 锁 能 轻 易 解 决 ， 不 管 对 象 数 量 多 少 及 对 象 颗 粒 度 大 小 。
2. **长 时 间 自 旋 可 能 导 致 开 销 大 。** 假 如 CAS 长 时 间 不 成 功 而一 直 自 旋 ， 会 给 CPU 带 来 很 大 的 开 销 。
3. **ABA 问 题 。** CAS 的 核 心 思 想 是 通 过 比 对 内 存 值 与 预 期 值 是 否 一 样 而 判 断 内 存 值 是 否 被 改 过 ， 但 这 个 判 断 逻 辑 不 严 谨 ， 假 如 内 存 值 原 来 是 A， 后 来 被 一 条 线 程 改 为 B， 最 后 又 被 改 成 了 A， 则 CAS 认 为 此 内 存 值 并 没 有 发 生 改 变 ， 但 实 际 上 是 有 被 其 他 线 程 改 过 的 ， 这 种 情 况 对 依 赖 过 程 值 的 情 景 的 运 算 结 果 影 响 很 大 。 解 决 的 思 路 是 引 入 版 本 号 ， 每 次 变 量 更 新 都 把 版 本 号 加 一 。



##  跟 Synchronized 相 比 ， 可 重 入 锁 ReentrantLock 其 实 现 原 理 有 什 么 不 同 ？

锁 的 实 现 原 理 基 本 是 为 了 达 到 一 个 目 的 ： 让 所 有 的 线 程 都 能 看 到 某 种 标 记 。 Synchronized 通 过 在 对 象 头 中 设 置 标 记 实 现 了这 一 目 的 ， 是 一 种 JVM 原 生 的 锁 实 现 方 式 ， 而 ReentrantLock 以 及 所 有 的 基 于 Lock 接 口 的 实 现 类 ， 都 是 通 过 用 一 个 volitile 修 饰 的 int 型 变 量 ， 并 保 证 每 个 线 程 都 能 拥 有 对 该 int 的 可 见 性 和 原 子 修 改 ， 其 本 质 是 基 于 所 谓 的 AQS 框 架 。



## 那 么 请 谈 谈 AQS 框 架 是 怎 么 回 事 儿 ？

AQS（ AbstractQueuedSynchronizer 类 ） 是 一 个 用 来 构 建 锁 和 同 步 器 

的 框 架 ， 各 种 Lock 包 中 的 锁 （ 常 用 的 有 ReentrantLock、 ReadWriteLock） ， 以 及 其 他 如 Semaphore、 CountDownLatch ， 都 是 基 于 AQS 来 构 建 。

同步器的设计是基于模板方法模式的，如果需要自定义同步器一般的方式是这样（模板方法模式很经典的一个应用）：

1. 使用者继承 `AbstractQueuedSynchronizer` 并重写指定的方法。（这些重写方法很简单，无非是对于共享资源 state 的获取和释放）
2. 将 AQS 组合在自定义同步组件的实现中，并调用其模板方法，而这些模板方法会调用使用者重写的方法。

这和我们以往通过实现接口的方式有很大区别，这是模板方法模式很经典的一个运用。

**AQS 使用了模板方法模式，自定义同步器时需要重写下面几个 AQS 提供的模板方法：**

```java
isHeldExclusively()//该线程是否正在独占资源。只有用到condition才需要去实现它。
tryAcquire(int)//独占方式。尝试获取资源，成功则返回true，失败则返回false。
tryRelease(int)//独占方式。尝试释放资源，成功则返回true，失败则返回false。
tryAcquireShared(int)//共享方式。尝试获取资源。负数表示失败；0表示成功，但没有剩余可用资源；正数表示成功，且有剩余资源。
tryReleaseShared(int)//共享方式。尝试释放资源，成功则返回true，失败则返回false。
```

- AQS 在 内 部 定 义 了 一 个 volatile int state 变 量 ， 表 示 同 步 状 态 。以 ReentrantLock 为例，state 初始化为 0，表示未锁定状态。A 线程 lock()时，会调用 tryAcquire()独占该锁并将 state+1。此后，其他线程再 tryAcquire()时就会失败，直到 A 线程 unlock()到 state=0（即释放锁）为止，其它线程才有机会获取该锁。当然，释放锁之前，A 线程自己是可以重复获取此锁的（state 会累加），这就是可重入的概念。但要注意，获取多少次就要释放多么次，这样才能保证 state 是能回到零态的。
- AQS 通 过 Node 内 部 类 构 成 的 一 个 双 向 链 表 结 构 的 同 步 队 列 ， 来 完 成 线 程 获 取 锁 的 排 队 工 作 ， 当 有 线 程 获 取 锁 失 败 后 ， 就 被 添 加 到 队 列 末 尾 。
- Node 类 是 对 要 访 问 同 步 代 码 的 线 程 的 封 装 ， 包 含 了 线 程 本 身 及 其 状 态 叫 waitStatus（ 有 五 种 不 同 取 值 ， 分 别 表 示 是 否 被 阻 塞 ， 是 否 等 待 唤 醒 ， 是 否 已 经 被 取 消 等 ） ， 每 个 Node 结 点 关 联 其 prev 结 点 和 next 结 点 ， 方 便 线 程 释 放 锁 后 快 速 唤 醒 下 一 个 在 等 待 的 线 程 ， 是 一 个 FIFO 的 过 程 。 
- Node 类 有 两 个 常 量 ， SHARED 和 EXCLUSIVE， 分 别 代 表 共 享 模 式 和 独 占 模 式 。 所 谓 共 享 模 式 是 一 个 锁 允 许 多 条 线 程 同 时 操 作 （ 信 号 量 Semaphore 就 是 基 于 AQS 的 共 享 模 式 实 现 的 ） ， 独 占 模 式 是 同 一 个 时 间 段 只 能 有 一 个 线 程 对 共 享 资 源 进 行 操 作 ， 多 余 的 请 求 线 程 需 要 排 队 等 待 （ 如 ReentranLock） 
- AQS 通 过 内 部 类 ConditionObject 构 建 等 待 队 列 （ 可 有 多 个 ） ， 当 Condition 调 用 wait() 方 法 后 ， 线 程 将 会 加 入 等 待 队 列 中 ， 而 当Condition 调 用 signal() 方 法 后 ， 线 程 将 从 等 待 队 列 转 移 动 同 步 队 列 中 进 行 锁 竞 争 。
- AQS 和 Condition 各 自 维 护 了 不 同 的 队 列 ， 在 使 用 Lock 和Condition 的 时 候 ， 其 实 就 是 两 个 队 列 的 互 相 移 动 。

![image-20210224133852610](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210224133852610.png)

## 请 尽 可 能 详 尽 地 对 比 下 Synchronized 和 ReentrantLock 的 异 同 ？

ReentrantLock 有很多Synchronized 没有的功能。如

等 待 可 中 断 ： 当 持 有 锁 的 线 程 长 期 不 释 放 锁 的 时 候 ， 正 在 等 待 的 线 程 可 以 选 择 放 弃 等 待 ， 对 处 理 执 行 时 间 非 常 长 的 同 步 块 很 有 用 。

带 超 时 的 获 取 锁 尝 试 ： 在 指 定 的 时 间 范 围 内 获 取 锁 ， 如 果 时 间 到 了 仍 然 无 法 获 取 则 返 回 。 可 以 判 断 是 否 有 线 程 在 排 队 等 待 获 取 锁 。

可 以 实 现 公 平 锁 。 从 锁 释 放 角 度 ， Synchronized 在 JVM 层 面 上 实 现 的 ， 不 但 可 以 通 过 一 些 监 控 工 具 监 控 Synchronized 的 锁 定 ， 而 且 在 代 码 执 行 出 现 异 常 时 ， JVM 会 自 动 释 放 锁 定 ； 但 是 使 用 Lock 则 不 行 ， Lock 是 通 过 代码 实 现 的 ， 要 保 证 锁 定 一 定 会 被 释 放 ， 就 必 须 将 unLock() 放 到 finally{} 中 。 

锁 的 实 现 原 理 基 本 是 为 了 达 到 一 个 目 的 ： 让 所 有 的 线 程 都 能 看 到 某 种 标 记 。 Synchronized 通 过 在 对 象 头 中 设 置 标 记 实 现 了这 一 目 的 ， 是 一 种 JVM 原 生 的 锁 实 现 方 式 ， 而 ReentrantLock 以 及 所 有 的 基 于 Lock 接 口 的 实 现 类 ， 都 是 通 过 用 一 个 volitile 修 饰 的 int 型 变 量 ， 并 保 证 每 个 线 程 都 能 拥 有 对 该 int 的 可 见 性 和 原 子 修 改 ， 其 本 质 是 基 于 所 谓 的 AQS 框 架 。



## ReentrantLock 是 如 何 实 现 可 重 入 性 的 ？

ReentrantLock 内 部 自 定 义 了 同 步 器 Sync（ Sync 既 实 现 了 AQS，

又 实 现 了 AOS， 而 AOS 提 供 了 一 种 互 斥 锁 持 有 的 方 式 ） ， 其 实 就 是加 锁 的 时 候 通 过 CAS 算 法 ， 将 线 程 对 象 放 到 一 个 双 向 链 表 中 ， 每 次 获 取 锁 的 时 候 ， 看 下 当 前 维 护 的 那 个 线 程 ID 和 当 前 请 求 的 线 程 ID 是 否 一 样 ， 如果一样只需要把AQS的状态值加一。



##  除 了 ReetrantLock， 你 还 接 触 过 JUC 中 的 哪 些 并 发 工 具 ？

通 常 所 说 的 并 发 包 （ JUC） 也 就 是 java.util.concurrent 及 其 子 包 ， 集 中 了 Java 并 发 的 各 种 基 础 工 具 类 ， 具 体 主 要 包 括 几 个 方 面 ： 

提 供 了 CountDownLatch、 CyclicBarrier、 Semaphore 等 ， 比 Synchronized 更 加 高 级 ， 可 以 实 现 更 加 丰 富 多 线 程 操 作 的 同 步 结 构 。

提 供 了 ConcurrentHashMap、 有 序 的 ConcunrrentSkipListMap， 或 者 通 过 类 似 快 照 机 制 实 现 线 程 安 全 的 动 态 数 组 CopyOnWriteArrayList 等 ， 各 种 线 程 安 全 的 容 器 。

强 大 的 Executor 框 架 ， 可 以 创 建 各 种 不 同 类 型 的 线 程 池 ， 调 度 任 务 运 行 等 。



## 请 谈 谈 ReadWriteLock 和 StampedLock

读 写 锁 基 于 的 原 理 是 多 个 读 操 作 不 需 要 互 斥 ， 如 果 读 锁 试 图 锁 定 时 ， 写 锁 是 被 某 个 线 程 持 有 ， 读 锁 将 无 法 获 得 ， 而 只 好 等 待 对 方 操 作 结 束 ， 这 样 就 可 以 自 动 保 证 不 会 读 取 到 有 争 议 的 数 据 。 ReadWriteLock 代 表 了 一 对 锁 ， 下 面 是 一 个 基 于 读 写 锁 实 现 的 数 据 结 构 ， 当 数 据 量 较 大 ， 并 发 读 多 、 并 发 写 少 的 时 候 ， 能 够 比 纯 同 步 版 本 凸 显 出 优 势 。

所 以 ， JDK 在 后 期 引 入 了 StampedLock， 在 提 供 类 似 读 写 锁 的 同 时 ， 还 支 持 优 化 读 模 式 。 优 化 读 基 于 假 设 ， 大 多 数 情 况 下 读 操 作 并 不 会 和 写 操 作 冲 突 ， 其 逻 辑 是 先 试 着 修 改 ， 然 后 通 过 validate 方 法 确 认 是 否 进 入 了 写 模 式 ， 如 果 没 有 进 入 ， 就 成 功 避 免 了 开 销 ； 如 果 进 入 ， 则 尝 试 获 取 读 锁 。



## 如 何 让 Java 的 线 程 彼 此 同 步 ？ 你 了 解 过 哪 些 同 步 器 ？

JUC 中 的 同 步 器 三 个 主 要 的 成 员 ： CountDownLatch、 CyclicBarrier 和 Semaphore， 通 过 它 们 可 以 方 便 地 实 现 很 多 线 程 之 间 协 作 的 功 能 。 

CountDownLatch是一个计数器闭锁，通过它可以完成类似于阻塞当前线程的功能，即：一个线程或多个线程一直等待，直到其他线程执行的操作完成。CountDownLatch用一个给定的计数器来初始化，该计数器的操作是原子操作，即同时只能有一个线程去操作该计数器。调用该类await方法的线程会一直处于阻塞状态，直到其他线程调用countDown方法，每次调用countDown方法，计数器的值减1。当计数器值减至零时，所有因调用await()方法而处于等待状态的线程就会继续往下执行。



Semaphore翻译成字面意思为 信号量，Semaphore可以控同时访问的线程个数，通过 acquire() 获取一个许可，如果没有就等待，而 release() 释放一个许可。

**Semaphore使用例子**

假若一个工厂有5台机器，但是有8个工人，一台机器同时只能被一个工人使用，只有使用完了，其他工人才能继续使用。那么我们就可以通过Semaphore来实现：

```java
public class Test {
    public static void main(String[] args) {
        int N = 8;            //工人数
        Semaphore semaphore = new Semaphore(5); //机器数目
        for(int i=0;i<N;i++)
            new Worker(i,semaphore).start();
    }

    static class Worker extends Thread{
        private int num;
        private Semaphore semaphore;
        public Worker(int num,Semaphore semaphore){
            this.num = num;
            this.semaphore = semaphore;
        }

        @Override
        public void run() {
            try {
                semaphore.acquire();
                System.out.println("工人"+this.num+"占用一个机器在生产...");
                Thread.sleep(2000);
                System.out.println("工人"+this.num+"释放出机器");
                semaphore.release();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
```

字面意思回环栅栏，通过它可以实现让一组线程等待至某个状态之后再全部同时执行。CyclicBarrier 的字面意思是可循环使用（Cyclic）的屏障（Barrier）。它要做的事情是，让一组线程到达一个屏障（也可以叫同步点）时被阻塞，直到最后一个线程到达屏障时，屏障才会开门，所有被屏障拦截的线程都到齐了才会继续干活。

**CyclicBarrier使用例子**

周末公司组织大巴去旅游，总共有三个景点，每个景点约定好游玩时间，一个景点结束后需要集中一起出发到下一个景点。例子中getNumberWaiting方法可以获得CyclicBarrier阻塞的线程数量

```java
public class TestCyclicBarrier {
    public static void main(String[] args) {
        final CyclicBarrier cb = new CyclicBarrier(3); //可以代表有3个线程，cb.await();表示3个线程到齐了之后，才能继续推进
        MyThead mt = new MyThead(cb);
        for(int i=0;i<3;i++){
            new Thread(mt).start();
        }
    }
}
class MyThead implements Runnable{
    private CyclicBarrier cb;
    public MyThead(CyclicBarrier cb){
        this.cb = cb;
    }

    @Override
    public void run() {
        try {
            Thread.sleep((long)(Math.random()*10000));
            System.out.println("线程" + Thread.currentThread().getName() + "即将到达集合地点1，当前已有" + 
		(cb.getNumberWaiting()+1) + "个已经到达，" 
+ (cb.getNumberWaiting()==2?"都到齐了，继续走啊":"正在等候"));
            cb.await();
            Thread.sleep((long)(Math.random()*10000));
            System.out.println("线程" + Thread.currentThread().getName() 
+ "即将到达集合地点2，当前已有" + (cb.getNumberWaiting()+1) 
+ "个已经到达，" + (cb.getNumberWaiting()==2?"都到齐了，继续走啊":"正在等候"));
            cb.await();
            Thread.sleep((long)(Math.random()*10000));
            System.out.println("线程" + Thread.currentThread().getName() 
+ "即将到达集合地点3，当前已有" + (cb.getNumberWaiting() + 1)
 + "个已经到达，" + (cb.getNumberWaiting()==2?"都到齐了，继续走啊":"正在等候"));
            cb.await();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
} 

```

## CyclicBarrier 和 CountDownLatch 看 起 来 很 相 似 ， 请 对 比 下 呢 ？

CountDownLatch 是 不 可 以 重 置 的 ， 所 以 无 法 重 用 ， CyclicBarrier 没 有 这 种 限 制 ， 可 以 重 用 。 CountDownLatch 的 基 本 操 作 组 合 是 countDown/await， 调 用 await 的 线 程 阻 塞 等 待 countDown 足 够 的 次 数 ， 不 管 你 是 在 一 个 线程 还 是 多 个 线 程 里 countDown， 只 要 次 数 足 够 即 可 。

CyclicBarrier 的 基 本 操 作 组 合 就 是 await， 当 所 有 的 伙 伴 都 调 用 了 await， 才 会 继 续 进 行 任 务 ， 并 自 动 进 行 重 置 。

CountDownLatch 目 的 是 让 一 个 线 程 等 待 其 他 N 个 线 程 达 到 某 个 条 件 后 ， 自 己 再 去 做 某 个 事 （ 通 过 CyclicBarrier 的 第 二 个 构 造 方 法 public CyclicBarrier(int parties, Runnable barrierAction)， 在 新 线 程 里 做 事 可 以 达 到 同 样 的 效 果 ） 。 而 CyclicBarrier 的 目 的 是 让 N 多 线 程 互 相 等 待 直 到 所 有 的 都 达 到 某 个 状 态 ， 然 后 这 N 个 线 程 再 继 续 执 行 各 自 后 续 （ 通 过 CountDownLatch 在 某 些 场 合 也 能 完 成 类 似 的 效 果 ） 。

# 线程池相关

## Java 中 的 线 程 池 是 如 何 实 现 的 ？

在 Java 中 ， 所 谓 的 线 程 池 中 的 “ 线 程 ” ， 其 实 是 被 抽 象 为 了一 个 静 态 内 部 类 Worker， 它 基 于 AQS 实 现 ， 存 放 在 线 程 池 的 `HashSet<Worker> workers` 成 员 变 量 中 ； 而 需 要 执 行 的 任 务 则 存 放 在 成 员 变 量 workQueue （ `BlockingQueue<Runnable> workQueue`） 中 。这 样 ， 整 个 线 程 池 实 现 的 基 本 思 想 就 是 ： 从 workQueue 中 不 断 取 出 需 要 执 行 的 任 务 ， 放 在 Workers 中 进 行 处 理 。



## 创 建 线 程 池 的 几 个 核 心 构 造 参 数 ？

Java 中 的 线 程 池 的 创 建 其 实 非 常 灵 活 ， 我 们 可 以 通 过 配 置 不 同 的 参 数 ， 创 建 出 行 为 不 同 的 线 程 池 ， 这 几 个 参 数 包 括 ：

corePoolSize： 线 程 池 的 核 心 线 程 数 。 

maximumPoolSize： 线 程 池 允 许 的 最 大 线 程 数 。 

keepAliveTime： 超 过 核 心 线 程 数 时 闲 置 线 程 的 存 活 时 间 。 

workQueue： 任 务 执 行 前 保 存 任 务 的 队 列 ， 保 存 由 execute 方 法 提 交 的 Runnable 任 务 。



![image-20210225110559539](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210225110559539.png)





## 线 程 池 中 的 线 程 是 怎 么 创 建 的 ？

线 程 池 默 认 初 始 化 后 不 启 动 Worker， 等 待 有 请 求 时 才 启 动 。 每 当 我 们 调 用 execute() 方 法 添 加 一 个 任 务 时 ， 线 程 池 会 做 如 下 判 断 ：

如 果 正 在 运 行 的 线 程 数 量 小 于 corePoolSize， 那 么 马 上 创 建 线 程 运 行 这 个 任 务 ； 如 果 正 在 运 行 的 线 程 数 量 大 于 或 等 于 corePoolSize， 那 么 将 这 个 任 务 放 入 队 列 ；如 果 这 时 候 队 列 满 了 ， 而 且 正 在 运 行 的 线 程 数 量 小 于 maximumPoolSize， 那 么 还 是 要 创 建 非 核 心 线 程 立 刻 运 行 这 个 任 务 ；

如 果 队 列 满 了 ， 而 且 正 在 运 行 的 线 程 数 量 大 于 或 等 于 maximumPoolSize， 那 么 线 程 池 会 抛 出 异 常 RejectExecutionException。

当 一 个 线 程 完 成 任 务 时 ， 它 会 从 队 列 中 取 下 一 个 任 务 来 执 行 。 当 一 个 线 程 无 事 可 做 ， 超 过 一 定 的 时 间 （ keepAliveTime） 时 ， 线 程 池 会 判 断 ：如 果 当 前 运 行 的 线 程 数 大 于 corePoolSize， 那 么 这 个 线 程 就 被 停 掉 。 所 以 线 程 池 的 所 有 任 务 完 成 后 ， 它 最 终 会 收 缩 到 corePoolSize 的 大 小 。





## 那 么 Java 中 默 认 实 现 好 的 线 程 池 又 有 哪 些 呢 ？ 请 比 较 它 们 的 异 同 。

### 1, SingleThreadExecutor

SingleThreadExecutor 线 程 池 这 个 线 程 池 只 有 一 个 核 心 线 程 在 工 作 ， 也 就 是 相 当 于 单 线 程 串 行 执 行 所 有 任 务 。 如 果 这 个 唯 一 的 线 程 因 为 异 常 结 束 ， 那 么 会 有 一 个 新 的 线 程 来 替 代 它 。 此 线 程 池 保 证 所 有 任 务 的 执 行 顺 序 按 照 任 务 的 提 交 顺 序 执 行 。 

corePoolSize： 1， 只 有 一 个 核 心 线 程 在 工 作 。 

maximumPoolSize： 1。 

keepAliveTime： 0L。 

workQueue： `new LinkedBlockingQueue<Runnable>()`， 其 缓 冲 队 列 是 无 界 的 。 

### 2 FixedThreadPool 线 程 池 

FixedThreadPool 是 固 定 大 小 的 线 程 池 ， 只 有 核 心 线 程 。 每 次 提 交 

一 个 任 务 就 创 建 一 个 线 程 ， 直 到 线 程 达 到 线 程 池 的 最 大 大 小 。 线 程 池 的 大 小 一 旦 达 到 最 大 值 就 会 保 持 不 变 ， 如 果 某 个 线 程 因 为 执 行 异 常 而 结 束 ， 那 么 线 程 池 会 补 充 一 个 新 线 程 。FixedThreadPool 多 数 针 对 一 些 很 稳 定 很 固 定 的 正 规 并 发 线 程 ， 多 用 于 服 务 器 。 

corePoolSize： nThreads 

maximumPoolSize： nThreads 

keepAliveTime： 0L 

workQueue： `new LinkedBlockingQueue<Runnable>()`， 其 缓 冲 队 列 是 无 界 的 。 



### 3, CachedThreadPool 

CachedThreadPool 线 程 池 CachedThreadPool 是 无 界 线 程 池 ， 如 果 线 程 池 的 大 小 超 过 了 处 理 任 务 所 需 要 的 线 程 ， 那 么 就 会 回 收 部 分 空 闲 （ 60 秒 不 执 行 任 务 ） 线 程 ， 当 任 务 数 增 加 时 ， 此 线 程 池 又 可 以 智 能 的 添 加 新 线 程 来 处 理 任 务 。  

线 程 池 大 小 完 全 依 赖 于 操 作 系 统 （ 或 者 说 JVM） 能 够 创 建 的 最 大 线 程 大 小 。 SynchronousQueue 是 一 个 是 缓 冲 区 为 1 的 阻 塞 队 列 。缓 存 型 池 子 通 常 用 于 执 行 一 些 生 存 期 很 短 的 异 步 型 任 务 ， 因 此 在 一 些 面 向 连 接 的 daemon 型 SERVER 中 用 得 不 多 。 但 对 于 生 存 期 短 的 异 步 任 务 ， 它 是 Executor 的 首 选 。

corePoolSize： 0 

maximumPoolSize： Integer.MAX_VALUE 

keepAliveTime： 60L 

workQueue： `new SynchronousQueue<Runnable>()`， 一 个 是 缓 冲 区 为 1 的 阻 塞 队 列 。

### 4, ScheduledThreadPool 

ScheduledThreadPool 线 程 池ScheduledThreadPool： 核 心 线 程 池 固 定 ， 大 小 无 限 的 线 程 池 。 此 线 程 池 支 持 定 时 以 及 周 期 性 执 行 任 务 的 需 求 。

创 建 一 个 周 期 性 执 行 任 务 的 线 程 池 。 如 果 闲 置 ， 非 核 心 线 程 池 会 在 DEFAULT_KEEPALIVEMILLIS 时间 内 回 收 。  

corePoolSize： corePoolSize 

maximumPoolSize： Integer.MAX_VALUE 

keepAliveTime： DEFAULT_KEEPALIVE_MILLIS 

workQueue： new DelayedWorkQueue() 

DelayedWorkQueue队列中每个元素都有一个过期时间，当从队列获取元素的时候，只有过期元素才会出队。队列头元素是快要过期的元素。



## 如 何 在 Java 线 程 池 中 提 交 线 程 ？

线 程 池 最 常 用 的 提 交 任 务 的 方 法 有 两 种 ： 

1. execute()： ExecutorService.execute 方 法 接 收 一 个 Runable 实 例 ， 它 用 来 执 行 一 个 任 务 ： 

2. submit()： ExecutorService.submit() 方 法 返 回 的 是 Future 对 象 。 可 以 用 isDone() 来 查 询 Future 是 否 已 经 完 成 ， 当 任 务 完 成 时 ， 它 具 有 一 个 结 果 ， 可 以 调 用 get() 来 获 取 结 果 。 也 可 以 不 用 isDone() 进 行 检 查 就 直 接 调 用 get()， 在 这 种 情 况 下 ， get() 将 阻 塞 ， 直 至 结 果 准 备 就 绪 



## 什 么 是 Java 的 内 存 模 型 ， Java 中 各 个 线 程 是 怎 么 彼 此 看 到 对 方 的 变 量 的 ？

java内存模型规定：所有的变量都存放在主内存中，当线程使用变量的时候，会把主内存里面的变量复制到自己的工作空间。线 程 间 变 量 值 的 传 递 需 要 通 过 主 内 存 。

真实的情况：（双核CPU，L1为cpu的1级缓存（寄存器），l2为高速缓冲存储器）

![image-20210223173356308](https://gitee.com/CTLQAQ/picgo/raw/master/image-20210223173356308.png)

A把共享变量X, L1,L2没有命中，于是把X=0加载到L2, L1。A然后修改X=1,并将其写到L1,L2,和主内存。

B获取X, L1没有命中，L2命中，于是把X=1 加载到B 的L1。然后B修改X=2,并将其写到L1,L2,和主内存。

这时，A还想修改，于是获取X, 此时L1 命中了，把X=1加载到L1, 这就有一个问题，明明我主内存中X=2。





## 请 谈 谈 volatile 有 什 么 特 点 ， 为 什 么 它 能 保 证 变 量 对 所 有 线 程 的 可 见 性 ？

当 一 个 变 量 被 定 义 成 volatile 之 后 ， 具 备 两 种 特 性 ： 

1. 保 证 此 变 量 对 所 有 线 程 的 可 见 性 。 当 一 条 线 程 修 改 了 这 个 变 量 的 值 ， 新 值 对 于 其 他 线 程 是 可 以 立 即 得 知 的 。 而 普 通 变 量 做 不 到 这 一 点 。
2. 禁 止 指 令 重 排 序 优 化 。 普 通 变 量 仅 仅 能 保 证 在 该 方 法 执 行 过 程 中 ， 得 到 正 确 结 果 ， 但 是 不 保 证 程 序 代 码 的 执 行 顺 序

当一个变量被声名成volatile,线程在写入变量的时候不会把值存在寄存器中，而是直接刷新到主内存中。读取时，直接从主存中读取到变量的最新值。



## **synchronized和volatile比较**

- volatile不需要加锁，比synchronized更轻量级，不会阻塞线程，效率更高

- volatile不具备“互斥性”，synchronized就具备“互斥性”

  - 何为互斥性？

    比方说当我们用synchronize修饰方法，

    当一个线程抢到锁执行该方法后另一个线程无法再抢到锁执行该方法

- synchronized既能保证可见性，又能保证原子性，而volatile只能保证可见性，不能保证原子性。

- 如果能用volatile解决问题，还是应尽量使用volatile，因为它的效率更高 

### Sleep方法和wait方法的区别

- 两者最主要的区别在于：**`sleep()` 方法没有释放锁，而 `wait()` 方法释放了锁** 。
- 两者都可以暂停线程的执行。
- `wait()` 通常被用于线程间交互/通信，`sleep()`通常被用于暂停执行。
- `wait()` 方法被调用后，线程不会自动苏醒，需要别的线程调用同一个对象上的 `notify()`或者 `notifyAll()` 方法。`sleep()`方法执行完成后，线程会自动苏醒。或者可以使用 `wait(long timeout)` 超时后线程会自动苏醒。



# ThreadLocal 

## ThreadLocal 原理

Thread 类中定义了2个变量，threadLocals 和 InheritableThreadLocals，它们都是ThreadLocalMap 变量，初始值都为null。

当线程第一次调用ThreadLocal对象的get或者set方法时候，会创建Map并赋值给对应线程的threadLocals 变量。当调用get方法的时候，首先会获取当前线程对应的map对象，然后如果map不为空，则以当前ThreadLocal对象查找相应的值，并返回，如果为空，创建ThreadLocalMap ，并放进去一个键值对key为当前对象，value为。

InheritableThreadLocals的一个特性就是让子线程可以访问父线程中设置的本地变量,他继承自ThreadLocal 类，重写的部分代码，主要目的是把创建的ThreadLocalMap对象设置给当前线程的InheritableThreadLocals变量。

实现子线程访问父线程ThreadLocalMap的值呢？创建子线程的时候，会把父线程的InheritableThreadLocals复制到子线程中去。（注意，是深拷贝，之后，父子线程里面的本地变量就不相关了）

## ThreadLocal 内存泄漏问题

```java
static class Entry extends WeakReference<ThreadLocal<?>> {
    /** The value associated with this ThreadLocal. */
    Object value;
    Entry(ThreadLocal<?> k, Object v) {
        super(k);
        value = v;
    }
}
```

> 弱引用与软引用的区别在于：只具有弱引用的对象拥有更短暂的生命周期。在垃圾回收器线程扫描它 所管辖的内存区域的过程中，一旦发现了只具有弱引用的对象，不管当前内存空间足够与否，都会回收它的内存。不过，由于垃圾回收器是一个优先级很低的线程， 因此不一定会很快发现那些只具有弱引用的对象。

使 用 ThreadLocal 要 注 意 remove！ ThreadLocal 的 实 现 是 基 于 一 个 所 谓 的 ThreadLocalMap， 在 ThreadLocalMap 中 ， 它 的 key 是 一 个 弱 引 用 。 通 常 弱 引 用 都 会 和 引 用 队 列 配 合 清 理 机 制 使 用 ， 但 是 ThreadLocal 是 个 例 外 ， 它 并 没 有 这 么 做 。  

这 意 味 着 ， 废 弃 项 目 的 回 收 依 赖 于 显 式 地 触 发 ， 否 则 就 要 等 待 线 程 结 束 ， 进 而 回 收 相 应 ThreadLocalMap！ 这 就 是 很 多 OOM 的 来 源 ， 所 以 通 常 都 会 建 议 ， 应 用 一 定 要 自 己 负 责 remove， 并 且 不 要 和 线 程 池 配 



# 综合问题

## 现在有 T1、T2、T3 三个线程，你怎样保证 T2 在 T1 执行完后执行，T3 在 T2 执行完后执 行？

在T2中调用t1.join(), 在T3 中调用t3.join()

## 生产者消费者问题

```java
public class Main {
	//生产者消费者问题，生产者消费者共享一个初始为空、大小为n的缓冲区
	//只有缓冲区不满，生产者才能把产品放入缓冲区，否则必须等待
	//缓冲区不空。消费者才能从中取处产品，
	//缓冲区是临界资源，各线程必须互斥的访问
	public static void main(String[] args) {
		int n = 10;// 缓冲区的大小
		Queue<String> queue = new LinkedBlockingQueue<>();
		//实现对缓冲区的互斥访问
		Semaphore mutex = new Semaphore(1);
		Semaphore empty = new Semaphore(n); //表示缓冲区空闲缓冲区的数量
		Semaphore full = new Semaphore(0);//表示产品的数量
		Thread producer = new Thread(() -> {
			try {
				while (true) {
					// 生产产品
					Thread.sleep(100);//模拟生产
					empty.acquire();
					mutex.acquire();
					// 把产品放进缓冲区
					System.out.println("把产品放进缓冲区");
					queue.add("产品");
					mutex.release();
					full.release();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		});

		Thread consumer = new Thread(() -> {
			try {
				while (true) {
					full.acquire();
					mutex.acquire();
					// 把产品放进缓冲区
					String ele = queue.poll();
					System.out.println("把产品取出");
					mutex.release();
					empty.release();
					//消费一个产品
					Thread.sleep(1000);//模拟消费
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		});
		consumer.start();
		producer.start();
	}
}

```

## 线程交替输出奇偶数

```java
public class Main {
	//线程交替输出奇偶数
	Lock lock = new ReentrantLock();
	Condition condition1 = lock.newCondition();
	Condition condition2 = lock.newCondition();
	int i = 0;

	//输出奇数
	void loopA(){
		while (i <= 100){
			lock.lock();
			try{
				if(i % 2 != 0){
					condition1.await();
				}
				//输出奇数
				System.out.println(i ++);
				condition2.signal();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}finally {
				lock.unlock();
			}
		}

	}
	//输出偶数
	void loopB(){
		while (i <= 100){
			lock.lock();
			try{
				if(i % 2 == 0){
					condition2.await();
				}
				//输出奇数
				System.out.println(i ++);
				condition1.signal();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}finally {
				lock.unlock();
			}
		}
	}

	public static void main(String[] args) {
		Main a = new Main();
		new Thread(a::loopA).start();
		new Thread(a::loopB).start();
	}
}
```

方法2

```java
public class Main {
	public static void main(String[] args) {
		//奇数线程需要获得的资源
		Semaphore a = new Semaphore(1);
		//偶数线程需要获得的资源
		Semaphore b = new Semaphore(0);
		new Thread(() -> {
			try {
				for (int i = 1;  i < 100; i += 2) {
					a.acquire();
					System.out.println("线程1：" + i);
					b.release();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}

		}).start();
		new Thread(() -> {
			try {
				for (int i = 2; i < 100; i += 2) {
					b.acquire();
					System.out.println("线程2：" + i);
					a.release();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}).start();
	}
}
```

