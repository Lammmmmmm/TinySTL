# A Tiny STL

这是一个**练习性质**的微型STL库实现，主要目的是为了学习数据结构与算法，以及加深对 C++ 语法以及 STL 的理解。

## 项目特色

github 上的 STL 实现项目繁多，珠玉在前，其中有很多值得参考学习的项目。本项目从学习目的出发，对比其他项目，将会：

- 命名规则上，内部实现**在不与现有STL接口冲突的情况下不采用下划线开头命名**，外部接口名称与 STL 一致

- **适当增加注释**帮助阅读者进行学习和理解

- **撰写博客**分析代码的逻辑关系，以及记录实现过程中的思考

[点击这里](https://choubin.site/)进入我的博客即可看到相关文章。

这么做一方面是想对代码逻辑做好梳理而不是简单地抄代码，另一方面也希望本项目能为像我这样的初学者带来一定帮助，这样我才觉得这个**造玩具轮子**的项目才有一些意义。

## 语言标准与参考代码

主要参考代码将会是 SGI STL 2.91版本，与侯捷先生所著《STL源码剖析》中讲解版本一致。项目初期代码可能混用各标准如C++98、C++11，后期有时间会尽量使用C++11重写。

## 编译环境

Windows 10 && VS2017 Community

g++ (Ubuntu 7.4.0-1ubuntu1~18.04.1) 7.4.0

## 开发计划

由于新旧 STL 标准内容繁多且存在差异，本项目出于学习目的故而必然需要有所取舍。影响取舍主要因素为接口的必要性以及时间。初步的开发计划如下：

- 具有 SGI 特色的两级 allocator，保证内存安全

- iterator模板

- 常用container：vector、list、deque、set、map、multiset、multimap

- 头文件 functional 常用算法

- 头文件 algorithm 常用算法

## 测试

将会包括简单的接口测试。
