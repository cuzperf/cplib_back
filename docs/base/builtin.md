# builtin

## mulMod

$$
\text{mulMod(a, b, m)} \doteq a \cdot b \mod m
$$
where all integers are 64-bits. `mulModi` and `mulModu` are signed and unsigned version respectively.

We can get the answer using `__int128` and `unsigned __int128` for clang and gcc and `_mul128`, `_div128`, `_umul128`, `_udiv128` for windows x64 VS2019 and above,for the rest, we use the ideal of `powMod` to get multiplication using addition

## lg2



