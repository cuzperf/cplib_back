# cplib by cuzperf

> InProgress

C++17 single-thread template for competitive programming contest, which contains numerous math algorithms.

- Aims: build a stable, fast, easy-to-read C++ template (the more you use it, the more reliable it is)
- Build: using [standalone_gn](https://agora-adc-artifacts.oss-accelerate.aliyuncs.com/standalone_gn_latest.zip) which based on [gn](https://gn.googlesource.com/gn/) and [ninja](https://ninja-build.org/)
- Test: All algorithms are tested in [LuoGu](https://www.luogu.com.cn/) and [Codeforces](https://codeforces.com/). [gtest](https://github.com/google/googletest) ~~and github CI/CD are~~ enabled
- Document: leave many comments in the code, unit_test and smoke_test are also a kind of document, and [introduction](https://cuzperf.github.io/cplib) using [mkdocs](https://www.mkdocs.org/)
- Reference: [CP-algorithm](https://cp-algorithms.com/), [AtCoder](https://github.com/atcoder/ac-library) and many [awesome blogs](https://codeforces.com/catalog), for instance [zscoder](https://codeforces.com/profile/zscoder) posted in codeforces and [jiangly's submissions](https://codeforces.com/submissions/jiangly)
- Reference(Chinese): [OI-wiki](https://oi-wiki.org/) and [LuoGu](https://www.luogu.com.cn/)

> Sorry, it's really hard to for me to give a reference for every algorithm.


## 中文（以下内容在 release 时候会删掉）

我不是一个优秀的 icpcer，很多代码都是从其它选手那里复刻（并不是复制，全量测试覆盖，全量 sanitizer）过来的，特别是 03年的 jiangly

- 中文可以表露一下心扉
- 别人不会在乎你的细节，头文件一经过尽可能的简单
- 别人 copy 代码是不想在两个地方进行 copy 的！（所以类是否应该放在两个地方有待商榷），但是如果写两份又有维护的问题！但是考虑到代码中有很多模板，写在两处虽然可以，需要提前知道使用的所有可能，很不优雅，所以最终决定，类的实现都写在一起，不拆分了
- 严重牺牲可读性而带来微量的性能优化是毫无意义的！
- 常数优化： 为了性能牺牲 $O(1)$ 的空间是值得的
- 多关注一下空间（内存消耗和内存泄漏）在工作中特别重视
- 模板的实现对于相同类型只会有一份代码
- 区间总是左开右闭



## To

- 更新 Gtest（GTest 要不要拓展时间 呢？），本质上是保证正确性
- 各个版本性能比较怎么做呢？ std::any ? 对应不同的 type 应该怎么搞呢？难道用模板？（貌似也不是不行）
- 递归版本 fft
- 各种实现的数学原理写在文档中！
- 多考虑空间优化版本
- 以后能否总结以下通过 UT 测出了那些 bug 吗
- 很久没写的算法如何快速理解的
- 别人如何加入进来呢？
- 是否有一个规范的搞法呢？一个规范的 PR 和使用说明
- 时间复杂度，空间复杂度，可否多线程，是否缓存友好性，benchmark
- Gmoke 能否用上?
- 答案能否和 python 或其它工具得到的答案做对比

一份想要深耕的工作，谈薪资的时候更要好好谈！找工作不要瞎投递

多想想具体的算法在各种复杂结构下的推广，考虑分析的方法在代数形式下的表现（在文档中要有表现，要不要写中文文档？）

> 当前情况下能支持写纯英文的文档吗？

写了那么多算法，你真的都理解了吗？能否简单的讲解呢？一句话说出来呢？不用看别的资料的那种，能随时推广并应用到其它领域吗？能跟从来没学过但理解能力不错的人讲清楚吗？

https://negiizhao.blog.uoj.ac/blog/4671

把 Handbook of MathFunction 好好翻一翻用来提升性能（当然了不可能完全没有代价）
msvc int128 的支持

fenwick.hpp 要好好的测试一下

手写红黑树有必要吗？
最新手写如何快速验证呢？
如何确保一段时间不写能快速里面问题呢？
怎样才是好的命名呢？

powMod 可以推广到矩阵
归并排序作为最简单的分治算法可以拓展到链表的合并，推广
写一点 vector 到指针链表的函数

怎么保证自己写的每一个都不会失效呢？
怎么检测那些文件没有被编译进去呢？

clang-tidy
standalone_gn 有一个 bug： config(debug) 是无效的
成为一个优秀的 C++ 开发者还是要 学习 cmake, QT, CUDA 的

immintrin.h - Meta Header file for Intel(R) Architecture intrinsic functions.

Intel 有 mkl： https://scc.ustc.edu.cn/zlsc/user_doc/html/index.html

回顾数学课题，计算数学数值分析（运筹学线性规划和非线性规划），偏微分方程数值解（看下本科成绩单）

https://www.luogu.com.cn/record/47535577

多线程也要回顾一下了，特别是 condition_variable, enable_shared_from_this
计算机两大公理：

- 计算机总是对的
- 未经测试的代码总是错的


## Linux VScode

`.vscode/launch.json`

``` json
{
  // Use IntelliSense to learn about possible attributes.
  // Hover to view descriptions of existing attributes.
  // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
  "version": "0.2.0",
  "configurations": [
    {
      "type": "lldb",
      "request": "launch",
      "name": "unit_test",
      "program": "${workspaceFolder}/out/linux/x64/unit_test",
      "args": [],
      "cwd": "${workspaceFolder}"
    },
    {
      "type": "lldb",
      "request": "launch",
      "name": "smoke_test",
      "program": "${workspaceFolder}/out/linux/x64/smoke_test",
      "args": [],
      "cwd": "${workspaceFolder}"
    },
  ]
}
```
