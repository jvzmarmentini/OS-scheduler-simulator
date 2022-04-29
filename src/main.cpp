#include <iostream>
#include <queue>
#include "Program.h"
using namespace std;

int main()
{
    Program p0("../test-cases/0.txt");
    Program p1("../test-cases/1.txt");
    queue<Program> qnew;
    qnew.push(p0);

    while (true)
    {
        int rt = qnew.front().run();

        if (rt == 1)
        {
            qnew.pop();
            if (qnew.empty())
            {
                break;
            }
        }
    }

    return 0;
}