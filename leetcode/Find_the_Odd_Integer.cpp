#include <iostream>
#include <vector>

using namespace std;

int findOdd(vector<int> v);

int findOdd(vector<int> v){
    if (v.size() <= 0) {
        return -1;
    }
    int res = v[0];
    for (int i = 1; i < v.size(); ++i) {
        res=res ^ v[i];  
    }
    return res;
}


int main(){
    vector<int> a{1,2,1,3,2,2,2};
    int res = findOdd(a);
    cout << "res: " << res << endl;
    return 0;
}