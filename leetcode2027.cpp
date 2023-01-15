#include <cstring>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
#include <unistd.h>
#include <numeric>
#include <string>
#include <new>
#include <cstdio>
#include <cstdlib>

using namespace std;

class Vector{
public:
    // 这里的{}相当于是给元素赋初始值
    Vector(int s): elem{new double[s]}, sz{s}
    {
    }

    double& operator[](int i){
        cout << "none const" << endl;
        return elem[i];
    };
    
    const double& operator[](int i) const {
        cout << "const" << endl;
        return elem[i];
    };
    
    int size() const {
        return sz;
    }
private:
    int sz;
    double* elem; // 指向数组的首地址
};

class Container {
public:
    virtual double& operator[](int) = 0;
    virtual int size() const = 0;
    virtual ~Container() = default;
};

// 与java如此相似
class Vector_container : public Container {
    public:
        Vector_container(int s) : v(s){}
        ~Vector_container(){}
        
        // override
        double& operator[](int i) override {
            return v[i];
        }
        // override
        int size() const override {
            return v.size();
        }
    private:
        Vector v;
};

unordered_map<char, int> map;

bool digitCount(string num) {
    const char* c = num.c_str();
    for (int i=0; i < num.size();++i) {
        char tmp = *(c+i);
        cout << tmp << endl;
        if(!map[tmp]) {
            map[tmp] = 1;
        } else {
            ++map[tmp];
        }
    }
    for(auto [k,v] : map) {
        cout << "k: " << k << " v:" << v << endl;
    }
    for (int j=0;j < num.size();++j) {
        // int 转 char
        char tmp = j + '0';
        // char 转 int
        int n = j - '0';
        // int 转 string: to_string();
        if (map[tmp] != (num[j] - '0')) {
            return false;
        }
    }
    return true;
}

 bool check(const vector<int> v, int n){
        for (int i=0;i<n; ++i) {
            if(v[i] != i) {
                return false;                
            }
        }
        return true;
    }

    vector<int> opterate(const vector<int> vc, int n){
        vector<int> newV(n);
        for (int i=0; i < n; ++i) {
            if (i%2==0) {
                newV[i] = vc[i/2];
            }else{
                newV[i] = vc[n / 2 + (i - 1) / 2];
            }
        }
        return newV;
    }

int reinitializePermutation(int n) {
        bool flag = false;
        int count = 0;
        vector<int> v(n);
        for (int i=0;i<n;++i) {
            v[i]=i;
        }
        while(!flag) {
            v = opterate(v, n);
            for(auto a : v) {
                cout << a << " ";
            }
            cout << endl;
            ++count; 
            flag = check(v, n);
            cout<< "go on ... flag: " << flag << endl;
            // usleep(1000 * 500);
        }
        return count;       
    }

std::size_t allocated = 0;

void* operator new(size_t sz) {
    void* p = std::malloc(sz);
    allocated += sz;
    return p;
}

void operator delete(void* p) noexcept {
    return std::free(p);
}


int main() {
    std::string s("length is 15");
    // %d输出int型，%zu输出size_t型
    std::printf("stack space = %zu, heap space = %zu, capacity = %zu, size = %zu", sizeof(s), allocated, s.capacity(), s.size());
    cout << endl;
    allocated = 0;
    std::string s2("length is 15 length is 15");
    std::printf("stack space = %zu, heap space = %zu, capacity = %zu, size = %zu", sizeof(s2), allocated, s2.capacity(), s2.size());
    return 0; 
}
