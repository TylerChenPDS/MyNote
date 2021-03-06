# 1 模式比较

## 适配器VS装饰者VS外观

从目的出发，进行比较

- 适配器将一个对象包装起来以改变其接口
- 装饰者将一个对象包装起来以增加新的行为和责任
- 外观将一群对象包装起来以简化其接口

## 外观与适配器的差异？

- 外观和适配器可以包装许多类，但是外观的意图是简化接口,让一个子系统更加容易使用，而适配器的意图是将接口转换成不同接口。

## 对比简单工厂和工厂模式

- 简单工厂把全部事情在一个地方都处理完了，然而工厂方法确实创建一个框架，让子类决定要如何实现。简单工厂只是将对象的创建封装起来，不具备工厂方法的弹性，不能变更正在创建的产品。

## 对比策略模式和状态模式

状态模式和策略模式的类图一摸一样。

区别：

- **状态模式将一群行为封装在状态对象中，context的行为可以随时委托到状态中的一个。随着时间的流逝，当前的状态在状态集合中游走变化，以反映context内部的状态。**
- 策略模式，客户通常主动指定context所要组合的策略对象是哪一个。虽然策略模式让我们更加有弹性，能够在运行时改变策略，但是对于某个context对象来说，通常**只有一个最恰当的策略对象**。
- 把策略模式当成除了继承以外的方案，可以通过组合不同的对象改变类的行为。
- 把状态模式当成不用再context中放置许多条件判断的代替方案。将行为包装进状态的对象中，可以通过在context内简单的改变状态对象来改变context的行为。

## 对比工厂模式和抽象工厂

- 都负责创建对象，将客户从具体类型中解耦，提高弹性。
- 工厂通过子类创建对象，子类决定具体类型，但是只能创建一个产品。

- 抽象工厂可以创建产品家族或将一群相关产品集合起来，但是有新产品把必须修改接口，其中的具体工厂会使用工厂方法创建产品。

## 代理模式和装饰者模式的区别

他们的目的不同。装饰着为对象增加行为，而代理则是控制对象的访问。如何让对象只能使用代理？使用工厂模式，返回的是代理对象

## 代理模式和适配器模式的区别？

代理和适配器都是挡在其他对象前面，并负责将请求转发给他们。但是：适配器会改变对象适配的接口（适配器对象和需要适配的对象继承不同的接口），而代理对象则和需要代理的对象实现同一个接口。

# 2 原则

## OO 原则

- 封装变化；

- 多用组合少用继承；（合成复用原则）

- 针对接口编程，不针对实现编程；（依赖倒转原则）

- 为交互对象之间松耦合设计而努力；

- 类应该对扩展开放，对修改关闭；（开闭原则）

- 依赖抽象，不是依赖具体类；

- 只和朋友交谈；（迪米特法则）

- **别来找我，我会找你**；在这个原则之下，允许低层组件将自己挂钩到系统之上，但是高层组件会决定什么时候和怎样使用这些底层组件。（**工厂方法、观察者**、**模板方法**）都采用了**好莱坞原则**）

- 类应该只有一个改变的理由。（单一职责原则）

## 软件设计原则

- **单一职责原则**（Single responsibility principle）： 一个类，只有一个引起它变化的原因，只有一个职责。

- **开闭原则**： 一个软件实体如类、 模块和函数应该对扩展开放，对修改关闭。模块应尽量在不修改原（是“原”，指原来的代码）代码的情况下进行扩展。

- **里氏替换原则**（ Liskov Substitution Principle）： 任何基类可以出现的地方，子类一定可以出现。 **LSP 是继承复用的基石，只有当衍生类可以替换掉基类，软件单位的功能不受到影响时，基类才能真正被复用，而衍生类也能够在基类的基础上增加新的行为**。里氏代换原则是对“开-闭”原则的补充。实现“开-闭”原则的关键步骤就是抽象化。而基类与子类的继承关系就是抽象化的具体实现，所以里氏代换原则是对实现抽象化的具体步骤的规范。（such as 装饰者模式）

- **依赖倒置原则**（ Dependency Inversion Principle）： 程序要依赖于抽象接口，不要依赖于具体实现。简单的说就是要求对抽象进行编程，不要对实现进行编程，这样就降低了客户与实现模块间的耦合。 **面向接口编程，而不是面向实现编程**。 即要面向抽象编程，而不是面向具体编程。高层模块依赖高层模块，细节依赖抽象。

- **接口隔离原则**： 客户端不应该依赖它不需要的接口， 类间的依赖关系应该建立在最小的接口上。

- **合成复用原则**（Composite/Aggregate Reuse Principle）： 合成/聚合复用原则就是在一个新的对象里面使用一些已有的对象，使之成为新对象的一部分；新的对象通过向这些对象的委派达到复用已有功能的目的。它的设计原则是： 要尽量使用合成/聚合，尽量不要使用继承。

- **迪米特法则**（ Law of Demeter、 Least Knowledge Principle）： 一个对象应当对其他对象有尽可能少的了解， 不和陌生人说话。

# 3 各种模式

## 观察者模式（组合）

Observer，Publish-Subscribe

#### 定义

定义了对象之间的一对多依赖，这样一来，当一个对象改变状态时，它的所有依赖者都会收到通知并自动更新。

#### 类图

![image-20201208141129376](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201208141129376.png)

#### 注意点

- ConcreateObserver需要保存一个对主题的引用，这样以后如果想**取消注册**的时候会比较方便。
- 使用该模式可以从被观察者推（push）或（pull）数据（然而，**推的方式被认为更正确**）。

**拉模式举例**

![image-20201208141341364](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201208141341364.png)

![image-20201208141635620](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201208141635620.png)

- Subject类，调用notifyObservers时没有使用参数，表示将使用拉模式。因为被通知者update被调用的时候将会使用主题的get方法获取自己想要的数据。

**推模式举例**

![image-20201208141844092](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201208141844092.png)

![image-20201208141859010](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201208141859010.png)

![image-20201208141918330](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201208141918330.png)

## 简单工厂

这是一种方法而不是模式，它利用了静态方法，所以又叫作静态工厂，不需要使用创建对象的方法来实例化对象，它将很多变化封装在一起成为一个静态类。但是缺点是不能通过继承来改变创建方法的行为。

![image-20201208142131209](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201208142131209.png)



#### 描述

其优点在于能够根据客户端给定的信息，由产品工厂具体决定产生何种对象，**对于客户端而言，其保证了“开闭原则”，但是当产品增加时，便需要修改工厂类，则工厂类并不符合“开闭原则”。** 该模式缺点也很明显，当产品需要扩展时，必须修改工厂，扩展性不强，并且在工厂类中有会依赖所有的具体产品，破坏了“单一职责原则”。  

适用场景：产品确定且变化不大时可以使用，如 JDK 中对日期格式化类的实现便是通过简单工厂。  

## 工厂方法模式

Factory Method

#### 定义

定义了一个创建对象的接口，但由子类决定要实例化的类是哪一个。**把实例化推迟到子类中**。

#### 类图

![image-20201208142624278](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201208142624278.png)



#### 符合的OO原则

- 增加新的产品类只需要增加一个具体的工厂，而不用修改源代码，符合**开闭原则**
- 每一个工厂对象，只负责生产一个类的对象，符合**单一职责原则**。
- 客户端不需要依赖具体的产品类，符合**依赖抽象，不是依赖具体类**（依赖倒转原则）

#### 描述

工厂方法模式，是对简单工厂模式的扩展，其在简单工厂基础上对工厂类进行抽象，使得工厂类可以被扩展，实现了工厂类的“开闭原则”。在工厂方法模式中，创建具体产品的过程交由具体工厂实现，一种具体工厂只负责生产一种具体产品，也符合“单一职责原则”。  

优点：
将产品的创建过程延迟到子类，使得产品的创建和扩展更加灵活，增强了系统的灵活性；  

具体产品类与客户端的依赖减少，使得客户端不再依赖具体产品，而依赖抽象产品接口，减少系统依赖，符合“迪米特法则”，由于在客户端中只需要依赖抽象，则符合了“里氏替换原则”，由于抽象工厂只依赖抽象产品，符合了“依赖倒置原则”，高层模块只依赖抽象，细节依赖抽象。  

缺点：
当系统产品庞大时，会产生很多产品类和工厂类，增加系统复杂程度；
增加了一层抽象层，增加了系统抽象程度和实现难度。
**当产品和工厂减少时，模式将退化成简单工厂模式。**  

## 抽象工厂模式

#### 定义

提供一个接口，用于创建相关或依赖对象的家族，而不需要明确指定具体类。

在抽象工厂模式中，产品族的概念是使用抽象工厂的关键，**产品族是指一个对象需要依赖一组相关的其他对象。**

#### 类图

![image-20201208142934389](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201208142934389.png)

#### 优缺点

优点：

- 隔离了具体类的生产过程，客户不知道需要创建哪些对象（减少了客户端对产品的依赖；
- 能够保证相互依赖的一组对象始终属于同一个产品族；
- 增加新的产品族和工厂很简单，符合“开闭原则”。

缺点：

- 当需要新增产品的产品族结构时，将需要修改所有工厂类，破坏了“开闭原则”

#### 描述

抽象模式在“开闭原则”上有倾斜，在扩展产品上其符合“开闭原则”，但是扩展产品族结构时将不符合“开闭原则”。  

## 单件模式

#### 定义

单例模式可确保一个类只有一个实例，并提供对其的全局访问点。

#### 优缺点

优点：

隐藏了其他对象实例化单例对象的入口，保证整个系统始终只有一个对象提供服务；

缺点：

没有抽象层，使得单例的扩展性不强；在单例对象中提供了过多的职责，破坏了“单一职责”原则”

#### 多线程下的单件模式

##### synchronized 会减低性能

![image-20201209095555809](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209095555809.png)

##### 饿汉模式

![image-20201209095625953](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209095625953.png)

##### 双重检查加锁

![image-20201209095722434](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209095722434.png)

#### 多例模式

```java
public class MultiplePattern {
    //必须要有容器
    private static List<Chess> chessList = new ArrayList<>();
    private static final Chess white = new Chess("white");
    private static final Chess black = new Chess("black");

    private static final int maxCount = 2;

    static {
        chessList.add(white);
        chessList.add(black);
    }
    private MultiplePattern() {
    }

    //随机拿取棋子
    public static Chess getInstance() {
        Random random = new Random();
        int crt = random.nextInt(maxCount);
        return chessList.get(crt);
    }

    //指定拿取棋子
    public static Chess getInstance(int index) {
        return chessList.get(index);
    }
}
```



## 命令模式

#### 定义

命令模式将请求封装为对象，从而使您可以使用不同的请求，队列或日志请求来参数化其他对象，并支持可撤销的操作。

![image-20201209100336854](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209100336854.png)

#### 例子

##### client

![image-20201209100447728](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209100447728.png)

##### Command 

![image-20201209100626765](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209100626765.png)

![image-20201209100633262](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209100633262.png)

![image-20201209100646685](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209100646685.png)

##### 特殊的命令实现--宏命令

![image-20201209101053734](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209101053734.png)

##### Remote Control 也就是invoker

![image-20201209100848613](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209100848613.png)

请注意上面的NoCommand，使用NoCommand可以是我们免于判断某个插槽是否为null。

## 适配器模式

Adapter

#### 定义

将一个类的接口转换成客户期望的另一个接口，让原本接口不兼容的类可以合作无间。

#### 类图

![image-20201209101241336](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209101241336.png)



#### 描述

Target：目标类，为当前客户端期望的抽象； 

Adaptee：适配者类，被适配的抽象； 

Adapter：适配器类，用于将适配者类转换为目标类的抽象。

优点： 

将适配者类（Adaptee）与目标类（Target）解耦

增强类的复用性，**适配者的细节对客户端或目标类透明**；

具有良好的灵活性和扩展性，当需要适配到新的目标类时，只需要新增适配器类即可，符合“开闭原则”。

缺点： 

类适配器由于需要使用继承，当面对目标类的抽象是类时，将因不支持多继承而产生局限，并且适配者当其子类较多时，需要一 一创建适配器类以



**对象适配器与类适配器**

对象适配器是指在适配器类中引用适配者类，而类适配器是指适配器类 继承自适配者类。

类适配器可以实现在适配器类中，置换适配者类的已有方法，从而具有 更强的灵活性。

对象适配器可以在同一个适配器类中更换不同的适配者，并支持对适配 者类及其子类的适配



## 外观模式

Facade

#### 定义

提供一个统一的接口，用来访问子系统中的一群接口。定义一个高层接口**让子系统更容易使用。**

#### 类图

![image-20201209101706667](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209101706667.png)

#### 注意

- 外观没有封装子系统，只是提供简化的接口。所以客户如果觉得有必要，**依然可以使用子系统的类**。
- 外观模式将客户从组件的子系统中解耦

#### 满足的原则

最少知识原则，只和你的密友谈话。

##### 如何不要赢得太多的朋友和影响太多的对象？

![image-20201209102125078](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209102125078.png)

![image-20201209102215273](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209102215273.png)

## 迭代器模式

Iterator

#### 定义

​	迭代器模式提供一种方法顺序访问一个聚合对象中的各个元素，又不暴露其内部表示（就是不知道这个聚合对象到底是用数组还是链表实现的）。把游走的任务放在迭代器上，而不是聚合类上，这样简化了聚合对象的接口和实现，也让责任各得其所。符合**单一职责原则**

#### 类图

![image-20201209102437531](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209102437531.png)

#### 内部迭代器

使用内部迭代器，迭代器自行在元素之间游走，客户无法控制遍历的过程，所以比较容易使用，但是造成了弹性的损失。

## 组合模式

Composite

#### 定义

组合模式允许你将对象组合成树形结构来表现“整体/部分”的层次结构。组合能让客户以一致的方式处理个别对象以及对象组合。

Composite 其实是一个聚合对象，这样当需要遍历其里面的子项目（包括其子项目的时候，就可以使用迭代器模式）。

#### 类图

![image-20201209103054115](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209103054115.png)

#### 注意

组合组合模式牺牲**单一职责原**则换取了**设计透明性**（即通过让组件的接口同时包含一些管理子节点和叶节点的操作，客户端就可以将组合和叶节点一视同仁）。



#### 遍历对象

##### 使用内部迭代器

![image-20201209103155436](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209103155436.png)



##### 使用外部迭代器

![image-20201209103224270](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209103224270.png)

![image-20201209103559317](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209103559317.png)

![image-20201209103622047](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209103622047.png)

![image-20201209103633873](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209103633873.png)

## 模板方法模式

#### 定义

在一个方法中定义一个算法的骨架，而将一些步骤延迟到子类中。使得子类可以在不改变算法结构的情况下，重新定义算法中的某些步骤。

#### 类图

![image-20201209103856391](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209103856391.png)

#### 钩子

​		钩子是一种被声明在抽象类中的方法，但**是只有空的或者默认的实现**。钩子的存在可以让子类有能力对算法的不同点挂钩。

![image-20201209104017049](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209104017049.png)

##### 栗子

![image-20201209104050909](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209104050909.png)

##### 什么时候使用钩子？

​		当子类必须提供算法中某个方法或步骤的实现时，就使用抽象方法。**如果算法的这个步骤是可选的，就用钩子**。如果是钩子的话子类可以选择实现，也可以选择不实现。

##### 使用钩子的目的？

让子类有能力为其抽象类作一些决定。

#### 使用到的原则

**好莱坞原则：别调用我们，我们会调用你。**在这个原则之下，允许低层组件将自己挂钩到系统之上，但是高层组件会决定什么时候和怎样使用这些底层组件。（工厂方法、观察者都采用了好莱坞原则）

![image-20201209104317507](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209104317507.png)

#### 注意

- 为了防止子类改变模板中的算法，可以将模板方法声明为final
- 策略模式和模板方法模式都封装算法，一个用的组合，一个用的继承。

- 好莱坞原则和依赖倒转原则的区别？
  - 依赖倒转原则教我们尽量避免使用具体类，而多用抽象。而好莱坞原则是在创建框架或组件上的一种技巧，好让底层组件能被挂进计算中，而又不会让高层组件依赖底层组件。

#### 使用到模板方法的栗子

- Arrays.sort(Object[] args); //需要子类继承Comparable，实现里面的compareTo

- Jframe留给子类的钩子方法，paint

  ![image-20201209104703705](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209104703705.png)

## 策略模式

#### 定义

定义了算法族，分别封装起来，让它们之间可以相互替换，此模式让算法的变化独立于使用算法的客户。

#### 类图

![image-20201209104842797](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209104842797.png)

#### 设计原则

- 找出应用中可能需要变化之处，把它们独立出来，不要和那些不需要变化的代码混在一起。（封装变化）
- 针对接口编程，而不是针对实现编程。
- 多用组合，少用继承

## 状态模式

#### 定义

允许对象在内部状态改变时改变它的行为，对象看起来好像修改了它的类

#### 类图

![image-20201209105207269](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209105207269.png)

#### 设计原则

- 封装变化。将每个状态的的行为局部化到它自己的内部。
- 将容易产生问题的if语句删除，以方便日后的维护。
- 符合开闭原则。让每一个状态“对修改关闭”，让糖果机“对扩展开放”，因为可以加入新的状态类。

#### 注意

- 当状态转换是固定的时候，就（将转变这个动作）适合放到context中，当转换是动态的时候，就会放在状态类中，**但是缺点是：状态类之间产生了依赖**。
- 客户不会直接和状态进行交互
-  Context对象如果有许多实例，那么这些实例可以**共享状态对象**，但**前提是这些状态对象不能持有它们自己的内部状态**（就是不能持有context某个特性实例的引用，因为此时状态需要对所有的context实例服务，如果状态需要用到context，那么可以将状态方法定义为： handle(Context context)），此时这些状态都被指定为**静态实例**。

#### 例子

##### 原来的糖果机

![image-20201209115235585](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209115235585.png)

![image-20201209115253536](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209115253536.png)



##### 现在的糖果机

![image-20201209115339568](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209115339568.png)





![image-20201209115420543](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209115420543.png)

![image-20201209115627984](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209115627984.png)





## 装饰者模式

#### 定义

动态地将责任附加到对象上，若要扩展功能，装饰者提供了比继承更有弹性的替代方案。符合开闭原则

#### 类图

![image-20201209105725544](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209105725544.png)

#### 描述

优点： 

更加灵活，使用组合代替继承； 

动态扩展类的功能，继承是静态的； 

组件类与装饰类可独立变化，相互组合，可组合出功能更加强大的类。

缺点： 

产生了粒度小的装饰类，容易产生类爆炸； 

由于它更加灵活，并且是动态扩展的功能，使得调试更加困难，可能涉及递归的执行。



对 JDK 的 装 饰 模 式 的 了 解 ： InputStream 、 FileInputStream 、 BufferedInputStream 、 LineNumberInputStream ， 其 中 FileInputStream 、 和 BufferedInputStream 是对 **InputStream** 的装饰，而 LinedNumberInputStream 是 对 BufferedInputStream 的又一层装饰。



## 代理模式

Proxy/Stub

#### 定义

代理模式为另一个对象提供一个替身或占位符以控制对这个对象的访问。使用代理模式创建代表，让代表对象控制某对象的访问，被代理的对象可以是**远程对象、创建开销大的对象，或者需要安全控制的对象**

#### 类图

![image-20201209110058495](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209110058495.png)

#### 几种代理方式

##### 远程代理

远程代理可以作为另一个JVM上对象的本地代表，调用代理的方法，会被代理利用通过网络转发到远程执行并且结果会通过网络返回给代理，再由代理将结果转给客户

![image-20201209110149511](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209110149511.png)

##### 虚拟代理

虚拟代理作为创建**开销大**的对象的代表。虚拟代理经常直到我们真正需要一个对象的时候才创建它。当对象在创建前和创建中，由虚拟对象来扮演对象的替身。对象创建后，代理就会直接委托给对象。

![image-20201209110234810](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209110234810.png)

##### 保护代理



#### 注意

- 在真实世界代理模式有许多变体，但是这些变体都有共同点：都会将客户对主题施加的方法拦截下来**。（控制对象的访问）**

## 复合模式

观察者加组合模式

![image-20201209233029233](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209233029233.png)





## 模式分类

### **创建型模式**

这些设计模式提供了一种在创建对象的同时隐藏创建逻辑的方式，而不是使用 new 运算符直接实例化对象。这使得程序在判断针对某个给定实例需要创建哪些对象时更加灵活。

- 工厂模式（Factory Pattern）
- 抽象工厂模式（Abstract Factory Pattern）
- 单例模式（Singleton Pattern）
- 建造者模式（Builder Pattern）
- 原型模式（Prototype Pattern）

## **结构型模式**

这些设计模式关注类和对象的组合。继承的概念被用来组合接口和定义组合对象获得新功能的方式。

- 适配器模式（Adapter Pattern）
- 桥接模式（Bridge Pattern）
- 过滤器模式（Filter、Criteria Pattern）
- 组合模式（Composite Pattern）
- 装饰器模式（Decorator Pattern）
- 外观模式（Facade Pattern）
- 享元模式（Flyweight Pattern）
- 代理模式（Proxy Pattern）

## **行为型模式**

**这些设计模式特别关注对象之间的通信。**

- 责任链模式（Chain of Responsibility Pattern）
- 命令模式（Command Pattern）
- 解释器模式（Interpreter Pattern）
- 迭代器模式（Iterator Pattern）
- 中介者模式（Mediator Pattern）
- 备忘录模式（Memento Pattern）
- 观察者模式（Observer Pattern）
- 状态模式（State Pattern）
- 空对象模式（Null Object Pattern）
- 策略模式（Strategy Pattern）
- 模板模式（Template Pattern）
- 访问者模式（Visitor Pattern）





# 4 考试题

**Please name at least three OO principles and explain how they are applied in the Strategy pattern**

- **封装变化**：策略模式将代码中经常变化的部分封装成了一个一个算法类。
- **多用组合少用继承**：策略模式使用组合而不是继承来扩展算法，就是说如果增加算法，这需要增加算法类，而不是通过继承
- **针对接口编程，不针对实现编程（依赖倒转原则）：**策略模式使用策略的超类型，而不是具体的策略类
- **开闭原则**。策略模式对增加新的算法开放。
- **单一职责原则**：策略模式中的算法，只有一种功能就是算法。



**How are design pattern different from class libraies?**

库和框架提供了我们某些特定的实现，让我们可以轻易的引用，但是这并不算是设计模式，有些时候库和框架本身会用到设计模式；设计模式比库的等级更高，设计模式告诉我们如何组织类和对象以解决某种问题。



**Write a class “LowerCaseInputStream” Which is a Java I/O decorator that convert all uppercase characters to lowercase in the input stream. In other words, if we read in “I Know the Decorator Pattern threfore I RULE!” then your decorator converts this to “i know the decorator pattern threfore I rule!”**

```java
public class LowerCaseInputStream extends FilterInputStream {
   protected LowerCaseInputStream(InputStream in) {
      super(in);
   }
   @Override
   public int read() throws IOException {
      int c = super.read();
      return c == -1 ? c : Character.toLowerCase((char)c);
   }
   @Override
   public int read(byte[] b, int offset, int len) throws IOException {
      int result = super.read(b, offset, len);
      for (int i = 0; i < b.length; i++) {
         b[i] = (byte) Character.toLowerCase(b[i]);
      }
      return result;
   }
   public static void main(String[] args) throws IOException {
      LowerCaseInputStream a = new LowerCaseInputStream(System.in);
      int c;
      while ((c = a.read()) >= 0){
         System.out.print((char) c);
      }
      a.close();
   }
}
```



**What is a MacroCommand in the Command Pattern? Please write codes for a general MacroCommand class.**

![image-20201209111129583](https://gitee.com/CTLQAQ/picgo/raw/master/image-20201209111129583.png)



**请写出至少三个 OO 设计原则，并说明在状态模式中是如何体现的**

符合单一指责原则、开闭原则、迪米特法则（最少知道法则） 

一个类一个责任原则（SRP）

类应该对扩展开放，对修改关闭。（开闭原则）

最少知识原则，只和你的密友谈话（迪米特法则）

状态模式首先分析出有几个状态，然后提供一个状态接口，所有的状态都要实现这个接口，将每个状态的行为局部化到自己的类中，让每个状态“对修改关闭”，让context“对扩展开放”，将状态封装成独立的类，并将动作委托到代表当前状态的对象行为会随着内部状态而改变。



**什么是设计模式？ 什么是复合模式？**

模式 是再某情境下，针对某问题的某种解决方案。

情景：不断出现的情况。

问题：某情境下达到的目标/约束。

解决方案：一个通用的设计，用来解决约束，达到目标。

**复合模式结合两个或以上的模式，组成一个解决方案，解决一再发生的一般性问题。**



**MVC 如何体现设计模式？**

- **策略**

  视图和控制器实现了经典的策略模式：视图是一个对象，可以被调整使用不同的策略，而控制器提供了策略，视图只关心系统中的可视部分，对于任何界面行为，都委托给控制器处理。控制器—算法，

- **组合**

  显示包含窗口、面板、按钮等。视图时GUI组件的组合顶层组件包含其他组件，直到叶子节点。

-  **观察者**

  模型是被观察者。只要模型的状态改变，注册到模型上的视图或者控制器都会接收到通知。



**写一个双向适配器的代码，并画出类图。以 Cat 会捉老鼠，Dog 会 bark为例，实现 Cat bark，Dog catch。**

写一个适配器，适配器继承了Cat 和 Dog 两个接口，并且里面维护了2个引用。

