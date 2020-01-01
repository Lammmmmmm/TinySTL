#include <iostream>
#include "typetraits.h"

using namespace std;

class A1{
    int a;
    double b;
};

class A2:public A1
{
    A1 num1;
    double num2;
};

int main(){
    A1 a1;
    A2 a2;
    int num;
    cout << std::is_pod<A1>::value << endl;
    cout << std::is_pod<A2>::value << endl;
    cout << mystl::is_const<const int>::value << endl;
    return 0;
}