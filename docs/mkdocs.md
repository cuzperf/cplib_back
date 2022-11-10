> 此处是一些基础功能的测试

<span id="jump">跳转到的地方</span>

## Project layout

``` python
    mkdocs.yml    # The configuration file.
    docs/
        index.md  # The documentation homepage.
        ...       # Other markdown pages, images and other files.
```

## 图标测试

<i class="fa fa-car fa-2x"></i>

<i class="fa fa-weixin fa-3x"></i>

## mathjax 测试

行内(inline)：$\sum_{i = 0}^n {n \choose i}  = 2^n$, 行间(block):

$$
\sum_{i = 0}^n {n \choose i}  = 2^n
$$

## 代码测试

``` C++
#include <bits/stdc++.h>
#define clog(x) std::clog << (#x) << " is " << (x) << '\n';
using LL = long long;
#include "../cpplib/math/mixed.hpp"
int main() {
	//freopen("in", "r", stdin);
	std::cin.tie(nullptr)->sync_with_stdio(false);
	std::vector<std::vector<double>> A{{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
	std::vector<double> b{1.0, 2.0, 3.0};
	for (auto x : Gauss(A, b)) std::cout << x << '\n';

	return 0;
}
```



## 折叠测试

??? note "代码折叠测试"
	``` C++
	#include <bits/stdc++.h>
	#define watch(x) std::cout << (#x) << " is " << (x) << std::endl
	#define print(x) std::cout << (x) << std::endl
	#define println std::cout << std::endl
	using LL = long long;
	using pii = std::pair<int, int>;
	using pll = std::pair<LL, LL>;

	int main() {
		//freopen("in","r",stdin);
		std::ios::sync_with_stdio(false);
		std::cin.tie(nullptr);
		int cas;
		std::cin >> cas;
		while (cas--) {

		}
		return 0;
	}
	```

!!! example "分栏测试"

	=== "分栏 1"
		$$
		E(\mathbf{v}, \mathbf{h}) = -\sum_{i,j}w_{ij}v_i h_j - \sum_i b_i v_i - \sum_j c_j h_j
		$$

	=== "分栏 2"
		```tex
		$$
		E(\mathbf{v}, \mathbf{h}) = -\sum_{i,j}w_{ij}v_i h_j - \sum_i b_i v_i - \sum_j c_j h_j
		$$
		```

## 参考

- [Mkdocs](https://www.mkdocs.org/)
- [Material for MkDocs](https://squidfunk.github.io/mkdocs-material/getting-started/)
- [PyMdown Extension Documentation](https://facelessuser.github.io/pymdown-extensions/)


## HTML 测试

### 跳转测试

``` markdown
<span id="jump">跳转到的地方</span>
[点击跳转](#jump)
```

[点击跳转](#jump)


### 居中 + 颜色

<center> 居中</center>

<font face="黑体" size=10>我是大黑体字</font>

<font color=red size=72>颜色</font>


### 添加视频

``` html
<video id="video" controls="">
<source id="mp4" src="https://cuzperf.github.io/*.mp4" type="video/mp4">
</video>
```


## 评论

<script src="https://utteranc.es/client.js"
        repo="cuzperf/cplib"
        issue-term="pathname"
        theme="github-dark-orange"
        crossorigin="anonymous"
        async>
</script>