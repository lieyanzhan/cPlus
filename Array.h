#include <iostream>

using namespace std;

#if !defined(ARRAY_H)
#define ARRAY_H

template <typename T, size_t s>
class Array {
public:
    // constexpr修饰的函数，简单的来说，如果其传入的参数可以在编译时期计算出来，那么这个函数就会产生编译时期的值。
    // 但是，传入的参数如果不能在编译时期计算出来，那么constexpr修饰的函数就和普通函数一样了。编译优化
    constexpr size_t Size() const { return s;}
    // 关于重载的const函数和非const函数，参见: href="https://blog.csdn.net/u014630623/article/details/51290954"
    T& operator[](size_t index) {
        return data[index];
    }
    const T& operator[](size_t index) const {
        return data[index];
    }

    T* Data() { return data;}
    const T* Data() const { return data;}

private:
    T data[s];        
};
#endif // MACRO