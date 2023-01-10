#include <cstring>
#include <iostream>
#include <array>

using namespace std;

struct Information {
    string firstName;
    string lastName;
    string letter_grade;
    int age;
};


int main(){
    return 0;
}

void Solution4_2(){
    string name, dessert;
    cout << "Enter your name" << endl;
    getline(cin, name);
    cout << "Enter your favorite dessert" << endl;
    getline(cin, dessert);
}

void Solution4_1(){
    Information a;
    cout << "what is your first name?";
    // 这里要读取带空白符的字符串必须用这个，或者使用带固定长度的
    // cin.getline(a.firstName, 20);
    getline(cin, a.firstName);
    cout << endl;
    cout << "what is your last name?";
    cin >> a.lastName;
    cout << endl;
    cout << "what letter grade do you deserve?";
    cin >> a.letter_grade;
    cout << "what is your age?";
    cin >> a.age;
    cout << "Name: " << a.lastName << " " << a.firstName << endl;
    cout << "Grade: " << a.letter_grade << endl;
    cout << "Age: " << a.age << endl;
}