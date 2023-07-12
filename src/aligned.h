#pragma once

#ifdef _MSC_VER
typedef __declspec(align(64)) int aligned64_int;
#elif defined(__GNUC__)
typedef int aligned64_int __attribute__((aligned(64)));
#else
#error you may use alignas directly in C11/C++11
#endif
