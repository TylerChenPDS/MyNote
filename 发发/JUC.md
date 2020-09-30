# 1，线程的创建与启动

## 1.1 继承Thread类创建线程类

- 1.定义Thread类的子类，并重写该类的run()方法，该run()方法的方法体就代表了线程需要完成的任务，  因此经常把run（）方法称为线程执行体。

- 2.创建Thread子类的实例，即创建了线程对象

- 3.调用线程对象的start()方法就可以启动该线程

## 1.2 实现Runnable接口创建线程类

- 定义Runnable接口的实现类，并重写该接口的run（）方法，该run（）方法的方法体也称之为线程方法执行体。
- 创建Runnable实现类的实例，并将此实例作为形参传入new Thread（）的构造函数中，就可创建Thread线程对象

## 1.3 使用Callable和Future创建线程

- 创建Callable接口的实现类，重写call()方法，该方法就是线程方法执行体，call()方法有返回值，再创建Callable实现类的实例。

- 使用FutureTask类的实例，来包装Callable对象，即把callable的实例以形参的方式传入new FutureTask()的构造函数中

- 使用FutureTask对象作为Thread对象的target创建启动线程。

- 通过FutureTask实例对象调用get()方法得到子线程的返回值。

```java
public class Demo {
    public static void main(String[] args) {
        //创建Callable对象
        ThirdThreadCallable callable = new  ThirdThreadCallable();
        //创建FutureTask对象，并把callable以形参的方式传入FutureTask的构造方法内
        FutureTask<Integer> futureTask = new FutureTask<>(callable);
        for (int i = 0; i < 30; i++) {
            // 通过Thread类的currentThread方法可以得到当前的线程名
            System.out.println(Thread.currentThread().getName()+" "+i);
            if (i==20) {
                //创建线程并启动
                new Thread(futureTask, "有返回值的线程").start();
            }
        }
        //获取子线程的返回值
        try {
            System.out.println("子线程的返回值： "+futureTask.get());
        } catch (InterruptedException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (ExecutionException e) {
            e.printStackTrace();
        }
    }
}

class ThirdThreadCallable implements Callable<Integer> {
    int i ;
    //call（）方法称之为线程方法执行体，且该方法有返回值，可通过FutureTask实例对象调用get（）方法得到子线程的返回值
    @Override
    public Integer call() throws Exception {
        for (; i <30; i++) {
            System.out.println(Thread.currentThread().getName()+" "+i);
        }
        return i;
    }

}
```

## 1.4 创建线程的三种方式对比

**使用实现Runnable 、Callable接口的方式创建多线程**

优点

- 线程类只是实现了Runnable接口或Callable接口，同时还可以继承其他类。

- 多个线程可以共享一个target对象，非常适合多个相同线程来处理同一份资源的情况，从而可以将CPU、代码、数据分开，形成清晰的模型，较好的体现了面向对象的思想

缺点

- 编程稍微复杂，如果需要访问当前线程，必须使用Thread.currentThread()方法。



**使用继承Thread类的方式创建多线程**

优点：

- 编写简单，如果要访问当前线程，无需使用Thread.currentThread（）方法，可以直接使用this的方式获取当前线程

缺点：

- 因为线程类已经继承了Thread类，所以不能再继承其他的父类。

# 2 线程的生命周期图

![](./img/1.png)





**新建状态（New）**

当程序使用new关键字创建了一个线程之后，该线程就处于新建状态。例：

Thread t = new MyThread();

**就绪状态（Runnable）**

调用start()方法后，线程就会处于就绪状态；此时只能说明此线程已经做好了执行的就绪准备，表示可以运行了，但还不是正在运行的线程。

**运行状态（Running）**

当线程获得到JVM中线程调度器的调度时，线程就会处于运行状态，开始执行run()方法的线程执行体。

**阻塞状态（Blocked）**

这是线程仍处于活动状态但当前没有资格运行（因为一些原因，暂时放弃对CPU的使用权，停止执行）时的状态。

**线程进入阻塞的三种状态：**

**1、等待阻塞：**

1）线程调用wait()方法，使本线程进入到等待状态；

2）调用join()方法，使本线程进入到另一种等待状态：等待别线程终止或者超时后才执行自己这个线程。

**2、同步阻塞**

线程在获取synchronized同步锁失败(因为锁被其它线程所占用)，它会进入同步阻塞状态；

**3、其他阻塞**

通过调用线程的sleep()或join()或发出了I/O请求时，线程会进入到阻塞状态。当sleep()状态超时、join()等待线程终止或者超时、或者I/O处理完毕时，线程重新转入就绪状态。

**死亡状态（Dead）**

当退出run（）方法时，线程就会自然死亡，处于终止或死亡状态，也就结束了生命周期。

# 3, 控制线程

## 3.1 join

- 在A线程中调用了B线程的join()方法时，表示只有当B线程执行完毕时，A线程才能继续执行。
- join方法可以传递参数，join(10)表示A线程会等待B线程10毫秒，10毫秒过去后。
- 需要注意的是，jdk规定，join(0)的意思不是A线程等待B线程0秒，而是A线程等待B线程无限时间，直到B线程执行完毕，即join(0)等价于join()。
- join方法必须在线程start方法调用之后调用才有意义。这个也很容易理解：如果一个线程都没有start，那它也就无法同步了。

```java
public class JoinTest {
	public static void main(String[] args) throws InterruptedException {
		ThreadJoinTest t1 = new ThreadJoinTest("小明");
		ThreadJoinTest t2 = new ThreadJoinTest("小东");
		t1.start();
		/**join的意思是使得放弃当前线程的执行，并返回对应的线程，例如下面代码的意思就是：
		 程序在main线程中调用t1线程的join方法，则main线程放弃cpu控制权，并返回t1线程继续执行直到线程t1执行完毕
		 所以结果是t1线程执行完后，才到主线程执行，相当于在main线程中同步t1线程，t1执行完了，main线程才有执行的机会
		 join方法可以传递参数，join(10)表示main线程会等待t1线程10毫秒，10毫秒过去后，
         * main线程和t1线程之间执行顺序由串行执行变为普通的并行执行
		 */
		t1.join();
        //t1.join(10)
		t2.start();
	}
}
class ThreadJoinTest extends Thread {
	public ThreadJoinTest(String name) {
		super(name);
	}

	@Override
	public void run() {
		for (int i = 0; i < 10; i++) {
			System.out.println(this.getName() + ":" + i);
		}
	}
}

```

## 3.2 守护线程setDaemon(true)

​		后台线程，它是在后台运行的，它的任务是为其他线程提供服务，这种线程被称为“后台线程（Daemon Thread）”,又称为“守护线程”或“精灵线程”。JVM的垃圾回收线程就是典型的后台线程。

**后台线程的特征：**如果**所有的前台线程都死亡了，后台线程也会自动死亡。**

调用Thread对象的**setDaemon(true)**可将制定线程设置为后台线程。所有的前台线程都死亡时，后天线程也死亡，程序就退出了。

注意：**设置为后台线程setDaemon(true)，注意必须在start前掉用，否则会报错！**

## 3.3 控制线程睡眠。sleep

注意：**sleep方法只能让当前线程睡眠**。调用某一个线程类的对象t.sleep()，睡眠的不是t，而是当前线程。

1, Thread.sleep();

sleep(long millis) 线程睡眠 millis 毫秒

sleep(long millis, int nanos) 线程睡眠 millis 毫秒 + nanos 纳秒

2, TimeUnit.单位.sleep()

**TimeUnit.SECONDS.sleep(1);**

**TimeUnit.MINUTES.sleep(**1);

**TimeUnit.HOURS.sleep(1);**

**TimeUnit.DAYS.sleep(1);**

## 3.4 改变线程优先级 setPriority()

- 每个线程在执行时都具有一定的优先级，优先级高的线程获得较多的执行机会，而优先级低的线程则获得较少的执行机会。
- 每个线程默认的优先级都与创建它的父线程的优先级相同，在默认情况下，main线程具有普通优先级，由main线程创建的子线程也具有普通优先级。
- **java 中的线程优先级的范围是1～10，1的优先级最低，10的优先级最高。**通过**setPriority()**方法，可以改变线程的优先级。

## 3.5 线程让步 yield()

Thread中有一个线程让步方法（静态方法）yield()，作用就是让步。它能让当前线程由“运行状态”进入到“就绪状态”，从而让其它具有相同优先级的等待线程获取执行权；但是，并不能保证在当前线程调用yield()之后，其它具有相同优先级的线程就一定能获得执行权；也有可能是当前线程又进入到“运行状态”继续运行！

**在多CPU并行的环境下，yield方法的功能有时候不明显，可能看不到效果。**

# 4， 线程同步

**一、什么情况下会产生线程安全问题？**

同时满足以下两个条件时：

1，多个线程在操作共享的数据。

2，操作共享数据的线程代码有多条。

当一个线程在执行操作共享数据的多条代码过程中，其他线程参与了运算，就会导致线程安全问题的产生。

**线程不安全的例子**

```java
public class TicketDemo implements Runnable {
	private  int tickets = 100; //定义在这个地方的变量是所有线程共享的变量
	@Override
	public void run() {
		while (tickets > 0) {
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
			}
			System.out.println(Thread.currentThread().getName() + "....卖出的票号:...." + tickets);
			tickets--;
		}
	}
	public static void main(String[] args) {
		TicketDemo ticketDemo = new TicketDemo();
		Thread t1 = new Thread(ticketDemo);
		Thread t2 = new Thread(ticketDemo);
		Thread t3 = new Thread(ticketDemo);
		Thread t4 = new Thread(ticketDemo);
		t1.start();
		t2.start();
		t3.start();
		t4.start();
	}
}

```

**我们发现可能会有多个线程卖同一张票的情况发生，这就是线程安全问题。**

## 4.1 线程同步概念

**什么是线程同步：**

- 同步就是协同步调，按预定的先后次序进行运行。如：你说完，我再说。这里的同步千万不要理解成那个同时进行，应是指协同、协助、互相配合。
- 线程同步是指多线程通过特定的设置来控制线程之间的执行顺序（即所谓的同步）也可以说是在线程之间通过同步建立起执行顺序的关系，如果没有同步，那线程之间是各自运行各自的！

## 4.2 线程同步的方式

### 4.2.1 **同步代码块** 

即有**synchronized**关键字修饰的语句块。 

被该关键字修饰的语句块会自动被加上内置锁，被保护的语句代码所在的线程要执行，需要获得内置锁，否则就处于阻塞状态。

代码如： 

  synchronized(object){  **同步代码块中的锁对象可以是任意对象**

  }

括号里的这个对象可以是任意对象，这个对象一般称为**同步锁**。

**同步的前提**：同步中必须有多个线程并使用同一个锁。

**同步的好处**：解决了线程的安全问题。

**注：**同步是一种高开销的操作，因此应该尽量减少同步的内容。 

  通常没有必要同步整个方法，使用synchronized代码块同步关键代码即可。

```java
public void run() {
		while (true) {
			synchronized (this){
				if (tickets > 0) {
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
					}
					System.out.println(Thread.currentThread().getName() + "....卖出的票号:...." + tickets);
					tickets--;
				}else {
					break;
				}
			}
		}
	}
```

### 4.2.2 synchronized关键字修饰的方法

即有synchronized关键字修饰的方法。

由于java的每个对象都有一个内置锁，当用此关键字修饰方法时，

内置锁会保护整个方法。在调用该方法前，需要获得内置锁，否则就处于阻塞状态。

代码如： 

  public synchronized void save(){}

**注：**

  **对于普通同步方法，锁是当前实例对象。**

  **对于静态同步方法，锁是当前类的Class对象。**

  **对于同步方法块，锁是Synchonized括号里配置的对象。**

### 4.2.3 **使用重入锁实现线程同步**

在JDK1.5中新增了一个java.util.concurrent包来支持同步。 

使用JUC里的Lock与使用synchronized方法和块具有相同的基本行为和语义，并且扩展了其能力

**前面讲了关键字synchronized实现的同步的锁，是隐藏的，所以我们并不明确是在哪里加上了锁，在哪里释放了锁。**

为了更明确的控制从哪里开始锁，在哪里释放锁，JDK1.5提供了Lock。

Lock是一个接口，我们真正用的是它的实现类ReentrantLock。

ReenreantLock类的常用方法有：

ReentrantLock() : 创建一个ReentrantLock实例 

lock() : 获得锁 

unlock() : 释放锁 

**注：**ReentrantLock()还有一个可以创建公平锁的构造方法，但由于能大幅度降低程序运行效率，不推荐使用 

**注：关于Lock对象和synchronized关键字的选择：** 

在性能上来说，如果竞争资源不激烈，两者的性能是差不多的，**而当竞争资源非常激烈时（即有大量线程同时竞争），此时Lock的性能要远远优于synchronized**。所以说，在具体使用时要根据适当情况选择。

```java
public class TicketDemo implements Runnable {
	private int tickets = 100;
	private Lock lock = new ReentrantLock();

	@Override
	public void run() {
		while (true) {
			// 在对共享数据tickets操作之前加锁
			lock.lock();
			try {
				if (tickets > 0) {
					System.out.println(Thread.currentThread().getName() + "正在出票...   " + tickets--);
					try {
						Thread.sleep(10);
					} catch (InterruptedException e) {
						throw new RuntimeException(e);
					}
				} else {
					break;
				}
			} finally {
				// 在finally确保锁得到了释放。
				lock.unlock();
			}
		}
	}
	public static void main(String[] args) {
		TicketDemo ticketDemo = new TicketDemo();
		Thread t1 = new Thread(ticketDemo);
		Thread t2 = new Thread(ticketDemo);
		Thread t3 = new Thread(ticketDemo);
		Thread t4 = new Thread(ticketDemo);
		t1.start();
		t2.start();
		t3.start();
		t4.start();
	}
}

```

### 4.2.4  **使用局部变量实现线程同步** 

如果使用ThreadLocal管理变量，则每一个使用该变量的线程都获得该变量的副本， 

副本之间相互独立，这样每一个线程都可以随意修改自己的变量副本，而不会对其他线程产生影响。

**ThreadLocal 类的常用方法**

  ThreadLocal() : 创建一个线程本地变量 

  get() : 返回此线程局部变量的当前线程副本中的值 

  initialValue() : 返回此线程局部变量的当前线程的"初始值" 

  set(T value) : 将此线程局部变量的当前线程副本中的值设置为value

```java
package org.example;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * @author TylerChen
 * @date 2020/9/28 - 23:12
 */
public class TicketDemo  {
	public static void main(String[] args) {
		SychronizedThread5 st = new SychronizedThread5();
		//创建并开启4个线程来卖100张票
		new Thread(st, "线程1").start();
		new Thread(st, "线程2").start();
		new Thread(st, "线程3").start();
		new Thread(st, "线程4").start();
	}
}

class SychronizedThread5 implements Runnable {
	//定义在这里的属性是所有线程共享的变量数据
	private  ThreadLocal<Integer>  ticketNumber = new ThreadLocal<Integer>(){
		@Override
		protected Integer initialValue() {
			return 10;
		}
	};

	@Override
	public void run() {
		//子线程做的任务的卖票
		while (true) {
			if (ticketNumber.get() > 0) {
				try {
					Thread.sleep(100); //起到放大线程安全问题的作用
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
				System.out.println("线程" + Thread.currentThread().getName() + "卖掉第" + ticketNumber.get() + "号票");
				ticketNumber.set(ticketNumber.get()-1);
			} else {
				break;
			}
		}
	}
}

```

 **注：ThreadLocal与其他同步机制** 

1. ThreadLocal与同步机制都是为了解决多线程中相同变量的访问冲突问题。 
2. ThreadLocal并不能代替同步机制，两者面向的问题领域不同。**同步机制是为了同步多个线程对相同资源的并发访问，是多个线程之间进行通信，并且协同的有效方式**；而ThreadLocal是为了隔离多个线程的数据共享，从而避免多个线程之间对共享资源的竞争，也就不需要对多个线程进行同步了。ThreadLocal采用以"空间换时间"的方法，其他同步机制采用以"时间换空间"的方式。
3. **ThreadLocal适用的场景是，多个线程都需要使用一个变量，但这个变量的值不需要在各个线程间共享，各个线程都只使用自己的这个变量的值。这样的场景下，可以使用ThreadLocal。**

### 4.2.5**同步锁的释放问题**

​		前面我们用关键字synchronized构成同步代码块和同步方法，来实现多线程的同步，本质上我们可以理解为底层的程序给线程加了一把我们**看不见的隐藏的锁**，只有获取到这把锁的线程才能被执行，没拿到的线程你就给我等着，从而控制线程的执行顺序，达到同步效果，所以，任何线程进入同步代码块、同步方法之前，必须先获得对于同步监测器的锁定，

​		那么谁释放对同步监测器的锁定呢？
**在Java中，程序无法显式的释放对同步监测器的锁定，释放权在底层的JVM上，JVM会从释放机制中自动的释放，**

**下面看看都是在什么情况下会进行同步监测器锁定的释放呢，如下所示：**

1.  当前线程的同步方法、同步代码块执行结束，当前线程即释放随同步监测器的锁定；
2.  当前线程的同步方法、同步代码块中遇到break、return终止了该代码块、方法的继续执行，当前线程会释放同步监测器的锁定；
3. 当前线程在同步方法、同步代码块中出现了未处理的error或者exception，导致了该代码块、该方法异常结束时，当前线程会释放同步监测器的锁定；
4. 当前线程执行同步代码块或同步方法时，程序调用了同步监测器的wait()方法，当前线程暂停，则当前线程会释放同步监测器的锁定。

**但是在如下情况下，当前线程不会释放对同步监测器的锁定：**

1.  线程执行同步代码块或者同步方法时，程序调用了Thread.sleep()、Thread.yield()方法来暂停当前线程执行，当前线程不会释放对同步监测器的锁定；
2. 线程执行同步代码块时，其他线程调用了该线程的suspend()方法（suspend会阻塞线程直到另一个线程调用resume，这个方法容易死锁，已经不推荐使用了，了解一下就ok）将该线程挂起，也不会释放同步监测器的锁定。

## 4.3 死锁

### 4.3.1 产生死锁的四个必要条件

- 互斥条件（mutual exclusion）：系统中存在临界资源，进程应互斥地使用这些资源；
- 占有和等待条件（hold and wait）：进程请求资源等不到满足而等待时，不释放已占有的资源；
- 不剥夺条件（no preemption）：已被占有的资源只能由属主释放，不允许被其它进程剥夺；
- 循环等待条件（circular wait）：存在循环等待链，其中，每个进程都在链中等待下一个进程所持有的资源，造成这组进程永远等待。

### 4.3.2 **决进程死锁问题**

i.死锁预防

①破坏互斥条件,使资源同时访问,而非互斥访问；

②破坏资源剥夺条件,采用静态分配策略；

③破坏占有和等待条件,采用刹车式调度方法；

④破坏循环等待条件,采用层次分配策略。

ii.死锁避免

  Dijkstra银行家算法,不对进程随意强加规则,而是动态地确定是否分配资源给提出请求的进程。

iii.死锁检测和恢复

  ①进程回退法;②进程撤销法;③资源剥夺法;④系统重启法。



### 4.3.3 案例

**锁顺序死锁**

```java
public class ThreadTest6 {
    public static void main(String[] args) {
        Demo demo = new Demo();
        new Thread(demo,"1").start();
        new Thread(demo,"2").start();
        new Thread(demo,"3").start();
        new Thread(demo,"4").start();
    }
}
class Demo implements Runnable{
    Account a = new Account("A",1000);
    Account b = new Account("B",1000);

    @Override
    public void run() {
        transferMoney(a,b,100);
        transferMoney(b,a,100);
    }

    public void  transferMoney(Account fromAccount, Account toAccount,double money) {
        synchronized (fromAccount) {
            System.out.println("线程" + Thread.currentThread().getName() + "得到锁" + fromAccount.getName());
            synchronized (toAccount) {
                System.out.println("线程" + Thread.currentThread().getName() + "得到锁" + toAccount.getName());
                if(fromAccount.getMoney() < money) {
                    System.out.println("余额不足");
                } else {
                    fromAccount.setMoney(fromAccount.getMoney()-money);
                    toAccount.setMoney(toAccount.getMoney() + money);
                    System.out.println("转账后：" + fromAccount.getName() + "有：" + fromAccount.getMoney());
                    System.out.println("转账后：" + toAccount.getName() + "有：" + toAccount.getMoney());
                }
            }
        }
    }
}
class Account{
    public Account(String name, double money) {
        this.name = name;
        this.money = money;
    }
    private String name;
    private double money;

}
```

### 4.3.4 **避免死锁的方法**

避免死锁可以概括成三种方法：

- **固定加锁的顺序**(针对锁顺序死锁)
- **开放调用**(针对对象之间协作造成的死锁)
- **使用定时锁**-->tryLock()

- - 如果等待获取锁时间超时，则**抛出异常而不是一直等待**



#### **4.3.4.1固定锁顺序避免死锁**

上面transferMoney()发生死锁的原因是因为**加锁顺序**不一致而出现的~

如果所有线程**以固定的顺序来获得锁**，那么程序中就不会出现锁顺序死锁问题！

那么上面的例子我们就可以**改造**成这样子：

```java
public class InduceLockOrder {

    // 额外的锁、避免两个对象hash值相等的情况(即使很少)
    private static final Object tieLock = new Object();

    public void transferMoney(final Account fromAcct,  final Account toAcct, final DollarAmount amount)  throws InsufficientFundsException {
        class Helper {
            public void transfer()  throws InsufficientFundsException {
                if (fromAcct.getBalance().compareTo(amount) < 0)
                    throw new InsufficientFundsException();
                else {
                    fromAcct.debit(amount);
                    toAcct.credit(amount);
                }
            }
        }
        // 得到锁的hash值
        int fromHash = System.identityHashCode(fromAcct);
        int toHash = System.identityHashCode(toAcct);

        // 根据hash值来上锁
        if (fromHash < toHash) {
            synchronized (fromAcct) {
                synchronized (toAcct) {
                    new Helper().transfer();
                }
            }

        } else if (fromHash > toHash) {// 根据hash值来上锁
            synchronized (toAcct) {
                synchronized (fromAcct) {
                    new Helper().transfer();
                }
            }
        } else {// 额外的锁、避免两个对象hash值相等的情况(即使很少)
            synchronized (tieLock) {
                synchronized (fromAcct) {
                    synchronized (toAcct) {
                        new Helper().transfer();
                    }
                }
            }
        }
    }
}
```

#### 4.3.4.2**开放调用避免死锁**

在协作对象之间发生死锁的例子中，主要是因为在**调用某个方法时就需要持有锁**，并且在方法内部也调用了其他带锁的方法！

- **如果在调用某个方法时不需要持有锁，那么这种调用被称为开放调用**！

我们可以这样来改造：

- **同步代码块**最好**仅被用于保护那些涉及共享状态的操作**！

#### 4.3.4.3**使用定时锁**

使用显式Lock锁，在获取锁时使用tryLock()方法。当等待**超过时限**的时候，tryLock()不会一直等待，而是返回错误信息。

使用tryLock()能够有效避免死锁问题~~

tryLock 是防止自锁的一个重要方式。

tryLock()方法是有返回值的，它表示用来尝试获取锁，如果获取成功，则返回true，如果获取失败（即锁已被其他线程获取），则返回false，这个方法无论如何都会立即返回。在拿不到锁时不会一直在那等待。

```java
public class tryLock {
    public static void main(String[] args) {

        System.out.println("开始");
        final Lock lock = new ReentrantLock();
        new Thread() {
            @Override
            public void run() {
                String tName = Thread.currentThread().getName();
                if (lock.tryLock()) {
                    System.out.println(tName + "获取到锁！");
                } else {
                    System.out.println(tName + "获取不到锁！");
                    return;
                }
                try {
                    for (int i = 0; i < 5; i++) {
                        System.out.println(tName + ":" + i);
                    }
                    Thread.sleep(5000);
                } catch (Exception e) {
                    System.out.println(tName + "出错了！！！");
                } finally {
                    System.out.println(tName + "释放锁！！");
                    lock.unlock();
                }

            }
        }.start();

        new Thread() {
            @Override
            public void run() {
                String tName = Thread.currentThread().getName();

                if (lock.tryLock()) {
                    System.out.println(tName + "获取到锁！");
                } else {
                    System.out.println(tName + "获取不到锁！");
                    return;
                }

                try {
                    for (int i = 0; i < 5; i++) {
                        System.out.println(tName + ":" + i);
                    }

                } catch (Exception e) {
                    System.out.println(tName + "出错了！！！");
                } finally {
                    System.out.println(tName + "释放锁！！");
                    lock.unlock();
                }
            }
        }.start();
        System.out.println("结束");
    }
}
```

### 4.3.5 总结

发生死锁的原因主要由于：

- 线程之间交错执行

- - 解决：**以固定的顺序加锁**

- 执行某方法时就需要持有锁，且不释放

- - 解决：**缩减同步代码块范围，最好仅操作共享变量时才加锁**

- 永久等待

- - 解决：使用**tryLock()**定时锁，超过时限则返回错误信息

# 5 volatile关键字与内存可见性

问题例子：**先来一段程序，对比，有volatile和没有volatile的区别：**

```java
public class TestVolatile {
    public static void main(String[] args) {
        ThreadDemo td = new ThreadDemo();
        new Thread(td).start();
        while(true){
            if(td.isFlag()){
                System.out.println("--------主线中的程序读到flag为true了----------");
                break;
            }
//这里不能有语句，有语句循环之间就有间隙
        }
    }
}

class ThreadDemo implements Runnable {
    private boolean flag = false;
    //private volatile boolean flag = false; 加上volatile之后，程序可以正常终止
    @Override
    public void run() {
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
        }
        flag = true;
        System.out.println("flag=" + isFlag());
    }
    public boolean isFlag() {
        return flag;
    }
    public void setFlag(boolean flag) {
        this.flag = flag;
    }
}
```

**运行发现程序不会停止：**

即使我们在子线程中将线程的共享变量flag的值修改成了false，

但是主线程在while条件判断的时候读到的flag一直是false，这是什么原因导致的呢？

## 5.1 内存可见性

**内存可见性其实就是共享变量在线程间的可见性**

- **共享变量**：如果一个变量在多个线程的工作内存中都存在**副本**，那么这个变量就是这几个线程的共享变量

- **可见性**：一个线程对共享变量值的修改，能够及时的被其他线程看到

**Java内存模型**

- 所有的变量都存储在主内存中（**操作系统给进程分配的内存空间**）

- **每个线程都有自己独立的工作内存，里面保存该线程使用到的变量的副本**

![](./img/2.png)

出问题的程序分析：主内存中存在flag = false，线程td启动后，将flag = false，放到自己的工作内存中，然后睡眠1秒，这期间，（因为要使用）主线程也把flag = false放到自己的工作内存放到自己的工作空间。td线程把flag = true由刷新到主内存中，但是主线程工作空间内的flag还是false。也就是说，td对共享变量的修改对主线程“不可见了”

**特别注意**：

- 线程对共享变量的所有操作都必须在自己的工作内存（working memory,是cache和寄存器的一个抽象，而并不是内存中的某个部分）**不同线程之间，当前线程无法直接访问其他线程的工作内存中的变量，线程间变量值得传递需要通过主内存来完成**

**共享变量可见性的实现原理**

- **把工作内存1中更新过的共享变量刷新到主内存中**

- **将主内存中最新的共享变量的值更新到工作内存2中**

## 5.2 解决内存可见性的方法

### 5.2.1**volatile**

**volatile关键字实现可见性，尤其是在嵌入式开发中，这个关键字在多线程开发不可或缺**

上面代码改写：给flag加上volatile关键字

```java
class ThreadDemo implements Runnable {
 private volatile boolean flag = false;

}
```

使用volatile之后，td将更改刷到主存中后，会告知main线程

### 5.2.2 **synchronized**

**前面学习的synchronized实现可见性**

JMM关于synchronized的两条规定：

- **线程解锁前，必须把共享变量的最新值刷新到主内存中**

- **线程加锁时，将清空工作内存中共享变量的值，从而使用共享变量时需要从主内存中重新读取最新的值** 

```java
  synchronized (td){
                if(td.isFlag()){
                   break;
               }
            }
```

**缺点：程序效率低，对计算机硬件资源是高开销动作。**

### 5.2.3 **synchronized和volatile比较**

- volatile不需要加锁，比synchronized更轻量级，不会阻塞线程，效率更高

- volatile不具备“互斥性”，synchronized就具备“互斥性”

  - 何为互斥性？

    比方说当我们用synchronize修饰方法，

    当一个线程抢到锁执行该方法后另一个线程无法再抢到锁执行该方法

- synchronized既能保证可见性，又能保证原子性，而volatile只能保证可见性，不能保证原子性。

- 如果能用volatile解决问题，还是应尽量使用volatile，因为它的效率更高 

# 6  原子性

## 6.1 提出问题原子性问题及解决

例子：

```java
public class TestAtomicDemo {
    public static void main(String[] args) {
        AtomicDemo ad = new AtomicDemo();
        for (int i = 0; i < 10; i++) {
            new Thread(ad).start();
        }
    }
}
class AtomicDemo implements Runnable{
   private volatile int serialNumber = 0; //线程共享变量
    @Override
    public void run() {
        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
        }
        System.out.println(getSerialNumber());
    }
    public int getSerialNumber(){
        return serialNumber++;
    }
}
```

- volatile关键字能保证内存可见性没有错，但是上面的程序**错在没能保证原子性**。

- 可见性只能保证每次读取的是最新的值，但是volatile没办法保证**对变量的操作**的**原子性**。

- **自增操作是不具备原子性的，它包括读取变量的原始值、进行加1操作、写入工作内存。**

问题解决：

**JUC这个工具包里，有一个包含原子变量子包，**java.util.concurrent.atomic，

里边封装了一系列常用的数据类型对应的封装类，

Java.util.concurrent.atomic中实现的原子操作类包括：

AtomicBoolean、AtomicInteger、AtomicLong、AtomicReference。

**这些类都保证了两点：**

- **类里的变量都用了volatile保证内存是可见的**
- 使用了一个算法**CAS**，保证对这些数据的操作具有原子性

代码修改：

```java
public class AtomicDemo implements Runnable{
	//private volatile int serialNumber = 0; //线程共享变量
	private AtomicInteger serialNumber  =  new AtomicInteger (0);
	@Override
	public void run() {
		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
		}
		System.out.println(serialNumber.getAndIncrement());
	}
}
```

**上面的代码就效率上讲，比synchronized和Lock实现的线程同步都高，**

**同时既保证了内存可见性，有保证了操作原子性，也解决了线程安全问题，是一种无锁的线程同步。**

## 6.2 CAS （Compare and Swap）算法逻辑的理解 ※

### 6.2.1 **乐观锁和悲观锁**

- **悲观锁：传统的锁机制**，例如 java 的 synchronized 关键字，他代表了 java 中悲观锁技术，保证了某一时刻仅有一个线程能访问同步代码/方法。synchronized 能够很好地工作，却有着 (相对) 比较大的性能开销。

- **乐观锁** (相对悲观锁) 对性能会有很大的帮助。他的核心思想是：你寄希望于在没有冲突的情况下完成一次更新操作，使用乐观锁技术更新时会进行 “冲突检测” 来判断是否有其他的线程干扰，若是 (有其他线程干扰) 则视本次更新操作失败，一般会进行重试。

- Compare and Swap 就是典型的乐观锁技术

### 6.2.2 CAS

CAS指令在Intel CPU上称为CMPXCHG指令，**它的作用是将指定内存地址的内容与所给的某个值相比，如果相等，则将其内容替换为指令中提供的新值，如果不相等，则更新失败。**这一比较并交换的操作是原子的，不可以被中断。初一看，CAS也包含了读取、比较 (这也是种操作)和写入这三个操作，和之前的i++并没有太大区别，的确在操作上没有区别，但**CAS是通过硬件命令保证了原子性**，而i++没有，且硬件级别的原子性比i++这样高级语言的软件级别的运行速度要快地多。虽然CAS也包含了多个操作，但其的运算是固定的(就是个比较)，这样的锁定性能开销很小。

简单的来说，**CAS有3个操作数，内存值V，旧的预期值A，要修改的新值B。当且仅当预期值A和内存值V相同时，将内存值V修改为B，否则返回V。**

例子：

![](./img/3.png)

1. t1，t2线程是同时更新同一变量56的值。因为t1和t2线程都同时去访问同一变量值56，所以他们会把主内存的值完全拷贝一份到自己的工作内存空间，所以t1和t2线程的预期值都为56。
2. 假设t1在与t2线程竞争中，线程t1竞争到了，能去更新变量的值，t2线程没争到。
3. 失败的线程并不会被挂起，而是被告知这次竞争中失败，并可以再次发起尝试
4. t1线程去更新变量值改为57，然后写到内存中。
5. 此时对于t2来讲它会检测到，内存值变为了57，与预期值56不一致，就不能把它修改的值写回主内存
6. 就会判定就操作失败了（想改的值不再是原来的值，因为很明显，有其它操作先改变了这个值）

总结：**就是指当V和A两者进行比较时，如果相等，则证明共享数据没有被修改，替换成新值，然后继续往下运行；如果不相等，说明共享数据已经被修改，放弃已经所做的操作，然后重新执行刚才的操作。这样子，就能保证操作的原子性了。**



