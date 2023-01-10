#include <cstring>
#include <iostream>
#include <memory>

using namespace std;

int minimumMoves(string& s);

int count_x(const char* p, char x);

class Vector{
public:
    // 这里的{}相当于是给元素赋初始值
    Vector(int s): elem{new double[s]}, sz{s}{
    }
    double& operator[](int i){return elem[i];};
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

struct B {
    virtual int f() = 0;
};

struct D : B { 
    string s = {"default"};
    int f() { 
        return 100;
    }

    virtual ~D(){
        // delete a;
        cout << "~D" << endl;    
    }
};

int main() {
    unique_ptr<B> p = make_unique<D>();
    cout << p->f() << endl;
    return 0;
}
