
#include <stdio.h>
#include <iostream>

int func(int x)
{
    int y = 0;
    y = x*x;
    return y;
}

int main()
{
    char c;
    std::cout << "waiting for anyinput to continue:\n";
    std::cin >> c;
    int sum = 0;
    for(int i = 0; i < 10; i++) {
        sum += func(i);
        printf("%d, %d\n", i,sum);
    }
}