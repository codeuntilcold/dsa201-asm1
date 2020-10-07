#include <iostream>
using namespace std;

template <typename T>
class Enclosing
{
public:
    T x;

    ~Enclosing();
};

template <typename T>
Enclosing<T>::~Enclosing()
{
    
}


int foo(int = 5);

int foo(int a)
{
    return a;
}

int main()
{
    cout << foo();
}

