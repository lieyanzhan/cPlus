#include <iostream>
#include <cmath>

using namespace std;

void NewtonF();

/// @output
// answer is: 4.49341
// answer is: 7.72525
// answer is: -9.57219e-05
void NewtonF(){
    // 当从4开始搜索的时候结果就是inf，但是从4.5开始搜索的时候结果就是4.49341，
    // 表明牛顿法的结果非常依赖于 init 初始点的选取
    double init_first = 4.5, init_second = 7.7, init_third = 1.0;
    double var; 

    var = init_first;
    while(abs(var-tan(var)) > 0.00001) {
        var = var - (var - tan(var)) / (1 - 1.0 / (cos(var)* cos(var)));
        //cout << "var: " << var << endl;
    }
    cout << "answer is: " << var << endl;

    var = init_second;
    while(abs(var-tan(var)) > 0.00001) {
        var = var - (var - tan(var)) / (1 - 1.0 / (cos(var)* cos(var)));
    }
    cout << "answer is: " << var << endl;

    var = init_third;

    // 注意: 这里必须用 |f(x)| 来判断
    while(abs(sin(var)) > 0.01) {
        // update
        var = var - tan(var);
    }
    cout << "answer is: " << var << endl;
}

int main(){
    NewtonF();
    cout << "Running" << endl;
    return 0;
}