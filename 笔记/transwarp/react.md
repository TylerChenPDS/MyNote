https://www.runoob.com/react/react-tutorial.html

# 安装下载

```shell
npm config set registry https://registry.npm.taobao.org #设置淘宝镜像
cnpm install -g create-react-app
create-react-app my-app
cd my-app/
npm start
```

# React JSX

React 使用 JSX 来替代常规的 JavaScript。JSX 是一个看起来很像 XML 的 JavaScript 语法扩展。

- JSX 执行更快，因为它在编译为 JavaScript 代码后进行了优化。
- 它是类型安全的，在编译过程中就能发现错误。
- 使用 JSX 编写模板更加简单快速。

```
let a = "world"
const element = <h1>Hello, {world}</h1>;
```

# 组件

自定义组件

```react
class Welcome extends React.Component {
  render() {
    return <h1>Hello {this.props.name}</h1>;
  }
}


function Name(props) {
    return <h1>网站名称：{props.name}</h1>;
}
function Url(props) {
    return <h1>网站地址：{props.url}</h1>;
}
function Nickname(props) {
    return <h1>网站小名：{props.nickname}</h1>;
}
function App() {
    return (
    <div>
        <Name name="菜鸟教程" />
        <Url url="http://www.runoob.com" />
        <Nickname nickname="Runoob" />
    </div>
    );
}
 
ReactDOM.render(
     <App />,
    document.getElementById('example')
);
```

注意，原生 HTML 元素名以小写字母开头，而**自定义的 React 类名以大写字母开头**，比如 HelloMessage 不能写成 helloMessage。除此之外还需要注意组件类只能包含一个顶层标签，否则也会报错。

## 组件生命周期

```
import React from 'react'
class Clock extends React.Component {
    constructor(props) {
        super(props);
        this.state = {date: new Date()};
    }

    componentDidMount() {
        this.timerID = setInterval(
            () => this.tick(),
            1000
        );
    }

    componentWillUnmount() {
        clearInterval(this.timerID);
    }

    tick() {
        this.setState({
            date: new Date()
        });
    }

    render() {
        return (
            <div>
                <h1>Hello, world!</h1>
                <h2>现在是 {this.state.date.toLocaleTimeString()}.</h2>
            </div>
        );
    }
}

export default Clock

```

**componentDidMount()** 与 **componentWillUnmount()** 方法被称作生命周期钩子。

**componentWillMount** 在渲染前调用,

**componentDidMount**，当对应组件被挂载的时候执行

componentWillReceiveProps 在组件接收到一个新的 prop (更新后)时被调用。这个方法在初始化render时不会被调用。

**componentWillUnmount**，当组件被卸载之前执行







# React Props

## 普通使用

```
function HelloMessage(props) {
    return <h1>Hello {props.name}!</h1>;
}
 
const element = <HelloMessage name="Runoob"/>;
```

## 默认 Props

```
class HelloMessage extends React.Component {
  render() {
    return (
      <h1>Hello, {this.props.name}</h1>
    );
  }
} 
HelloMessage.defaultProps = {
  name: 'Runoob'
}; 
const element = <HelloMessage/>;
```

## Props 验证

首先需要安装引入

```
npm install --save prop-types;
import PropTypes from ‘prop-types’;
```

Props 验证使用 **propTypes**，它可以保证我们的应用组件被正确使用，React.PropTypes 提供很多验证器 (validator) 来验证传入数据是否有效。当向 props 传入无效数据时，JavaScript 控制台会抛出警告。

```js
import React from 'react'
import PropTypes from 'prop-types';
class HelloWorld extends React.Component {
    render() {
        return (
            <div>Hello, {this.props.name}</div>
        );
    }
}
HelloWorld.propTypes = {
    name: PropTypes.string
};
export default HelloWorld;
```



```js
MyComponent.propTypes = {
    // 可以声明 prop 为指定的 JS 基本数据类型，默认情况，这些数据是可选的
   optionalArray: React.PropTypes.array,
    optionalBool: React.PropTypes.bool,
    optionalFunc: React.PropTypes.func,
    optionalNumber: React.PropTypes.number,
    optionalObject: React.PropTypes.object,
    optionalString: React.PropTypes.string,
 
    // 可以被渲染的对象 numbers, strings, elements 或 array
    optionalNode: React.PropTypes.node,
 
    //  React 元素
    optionalElement: React.PropTypes.element,
 
    // 用 JS 的 instanceof 操作符声明 prop 为类的实例。
    optionalMessage: React.PropTypes.instanceOf(Message),
 
    // 用 enum 来限制 prop 只接受指定的值。
    optionalEnum: React.PropTypes.oneOf(['News', 'Photos']),
 
    // 可以是多个对象类型中的一个
    optionalUnion: React.PropTypes.oneOfType([
      React.PropTypes.string,
      React.PropTypes.number,
      React.PropTypes.instanceOf(Message)
    ]),
 
    // 指定类型组成的数组
    optionalArrayOf: React.PropTypes.arrayOf(React.PropTypes.number),
 
    // 指定类型的属性构成的对象
    optionalObjectOf: React.PropTypes.objectOf(React.PropTypes.number),
 
    // 特定 shape 参数的对象
    optionalObjectWithShape: React.PropTypes.shape({
      color: React.PropTypes.string,
      fontSize: React.PropTypes.number
    }),
 
    // 任意类型加上 `isRequired` 来使 prop 不可空。
    requiredFunc: React.PropTypes.func.isRequired,
 
    // 不可空的任意类型
    requiredAny: React.PropTypes.any.isRequired,
 
    // 自定义验证器。如果验证失败需要返回一个 Error 对象。不要直接使用 `console.warn` 或抛异常，因为这样 `oneOfType` 会失效。
    customProp: function(props, propName, componentName) {
      if (!/matchme/.test(props[propName])) {
        return new Error('Validation failed!');
      }
    }
  }
}
```

# 事件处理

https://www.runoob.com/react/react-event-handle.html





# React 组件 API

```
setState(object nextState[, function callback])
nextState，将要设置的新状态，该状态会和当前的state合并
callback，可选参数，回调函数。该函数会在setState设置成功，且组件重新渲染后调用。

replaceState(object nextState[, function callback])
nextState，将要设置的新状态，该状态会替换当前的state。
callback，可选参数，回调函数。该函数会在replaceState设置成功，且组件重新渲染后调用。


setProps(object nextProps[, function callback])
nextProps，将要设置的新属性，该状态会和当前的props合并
callback，可选参数，回调函数。该函数会在setProps设置成功，且组件重新渲染后调用。

replaceProps(object nextProps[, function callback])
nextProps，将要设置的新属性，该属性会替换当前的props。
callback，可选参数，回调函数。该函数会在replaceProps设置成功，且组件重新渲染后调用。

forceUpdate([function callback])
参数说明
callback，可选参数，回调函数。该函数会在组件render()方法调用后调用。
forceUpdate()方法会使组件调用自身的render()方法重新渲染组件，组件的子组件也会调用自己的render()。但是，组件重新渲染时，依然会读取this.props和this.state，如果状态没有改变，那么React只会更新DOM。
forceUpdate()方法适用于this.props和this.state之外的组件重绘（如：修改了this.state后），通过该方法通知React需要调用render()
一般来说，应该尽量避免使用forceUpdate()，而仅从this.props和this.state中读取状态并由React触发render()调用。



```

















