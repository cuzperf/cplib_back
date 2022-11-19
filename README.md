# cplib by cuzperf

> InProgress

C++17 template for competitive programming contest, which contains numerous math algorithms.

- Aims: build a stable, fast, easy-to-read C++ template (the more you use it, the more reliable it is)
- Build: using [standalone_gn](https://agora-adc-artifacts.oss-accelerate.aliyuncs.com/standalone_gn_latest.zip) which based on [gn](https://gn.googlesource.com/gn/) and [ninja](https://ninja-build.org/)
- Test: All algorithms are tested in [LuoGu](https://www.luogu.com.cn/) and [Codeforces](https://codeforces.com/). [gtest](https://github.com/google/googletest) and ~~github CI/CD are enabled~~
- Document: leave many comments in the code, unit_test and smoke_test are also a kind of document, and [introduction](https://cuzperf.github.io/cplib) using [mkdocs](https://www.mkdocs.org/)
- Reference: [CP-algorithm](https://cp-algorithms.com/), [AtCoder](https://github.com/atcoder/ac-library) and many [awesome blogs](https://codeforces.com/catalog), for instance [zscoder](https://codeforces.com/profile/zscoder) posted in codeforces and [jiangly's submissions](https://codeforces.com/submissions/jiangly)
- Reference(Chinese): [OI-wiki](https://oi-wiki.org/) and [LuoGu](https://www.luogu.com.cn/)

> Sorry, it's really hard to for me to give a reference for every algorithm.


- 中文可以表露一下心扉
- 别人不会在乎你的细节，头文件一经过尽可能的简单
- 别人 copy 代码是不想在两个地方进行 copy 的！（所以类是否应该放在两个地方有待商榷），但是如果写两份又有维护的问题！但是考虑到代码中有很多模板，写在两处虽然可以，需要提前知道使用的所有可能，很不优雅，所以最终决定，类的实现都写在一起，不拆分了
- 严重牺牲可读性而带来微量的性能优化是毫无意义的！

Q：怎么证明模板是用的 static（如果是的话，拿岂不是会有包体积的问题），如果不是怎么实现呢？

我不是一个优秀的 icpcer，很多代码都是从其它选手那里复刻过来的，特别是 03年的 jiangly

区间总是左开右闭


fenwick.hpp 要好好的测试一下


unit_test 中添加 rand 后缀名的 rand 测试
查找 cpp 中 #pargma once
查找 #include <bits/stdc++.h>
string 和 graph 待搞， math 和 ds 可以先搞了
gn 构建可以向 agoralite 学习，要不要使用相对路径呢？
为了性能牺牲 $O(1)$ 的空间是值得的
多关注一下空间
手写红黑树有必要吗？
最新手写如何快速验证呢？
如果确保一段时间不写能快速里面问题呢？
怎样才是好的命名呢？
归并排序作为最简单的分治算法可以拓展到链表的合并，推广
写一点 vector 到指针链表的函数
怎么保证自己写的每一个都不会失效呢？
怎么检测那些文件没有被编译进去呢？

深究一下为啥底版本支持 project.code-workspace 而高版本反而不支持呢？（利用 gn 的源代码来看），实在不行的话，自己手写一个好了

把 Handbook of MathFunction 好好翻一翻用来提升性能（当然了不可能完全没有代价）
std::numeric_limits<int>::max() replace INT_MAX
