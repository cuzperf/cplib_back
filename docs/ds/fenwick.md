# Fenwick

Let $a$ be origin array, $s$ be array in Fenwick, then

$$
s_i = \sum_{j = f(i) + 1}^i a_i
$$

where $f(i)$ is the number erase last $1$ in the binary representation of $i$, thus $f(i) = i - (i \And -i)$

so

$$
\sum_{i = 1}^n a_i = s_n + s_{f(n)} + s_{f(f(n))} + \cdots
$$

let $g(i) = i + (i \And -i)$, the following number will add if $a_i$ add $p$

$$
i, g(i), g(g(i)), \cdots
$$

## initial with array in $O(n)$

