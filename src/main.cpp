#include <iostream>
#include "Program.h"
using namespace std;

int main()
{
    Program p1("../test-cases/1.txt");
    while (true)
    {
        int rt = p1.run();

        if (rt == 1)
            break;
    }

    p1.run();
    return 0;
    // int x, y;
    // int sum;
    // cout << "Type a number: ";
    // cin >> x;
    // cout << "Type another number: ";
    // cin >> y;
    // sum = x + y;
    // cout << "Sum is: " << sum;
    // return 0;
}