# cplib by cuzperf

> InProgress

C++17 template for competitive programming contest, which contains numerous math algorithms.

- Aims: build a stable, fast, easy-to-read C++ template (the more you use it, the more reliable it is)
- Build: using [standalone_gn](https://agora-adc-artifacts.oss-accelerate.aliyuncs.com/standalone_gn_latest.zip) which based on [gn](https://gn.googlesource.com/gn/) and [ninja](https://ninja-build.org/)
- Test: All algorithms are tested in [LuoGu](https://www.luogu.com.cn/) and [Codeforces](https://codeforces.com/). [gtest](https://github.com/google/googletest) and github CI/CD are enabled
- Document: leave many comments in the code, unit_test and smoke_test are also a kind of document, and [introduction](https://cuzperf.github.io/cplib) using [mkdocs](https://www.mkdocs.org/)
- Reference: [CP-algorithm](https://cp-algorithms.com/), [AtCoder](https://github.com/atcoder/ac-library) and many awesome blogs, for instance [zscoder](https://codeforces.com/profile/zscoder) posted in codeforces and [jiangly's submissions](https://codeforces.com/submissions/jiangly)
- Reference(Chinese): [OI-wiki](https://oi-wiki.org/) and [LuoGu](https://www.luogu.com.cn/)

> Sorry, it's really hard to for me to give a reference for every algorithm.


- 中文可以表露一下心扉
- 别人不会在乎你的细节，头文件一经过尽可能的简单
- 别人 copy 代码是不想在两个地方进行 copy 的！（所以类是否应该放在两个地方有待商榷），但是如果写两份又有维护的问题！
