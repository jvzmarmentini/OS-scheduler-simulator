#include <iostream>
#include "Program.h"
#include "Scheduler.h"

int main()
{
    int systemclock = 0;
    bool inputbreak = true;

    Scheduler scheduler("RR", 5);

    Program p0("../test-cases/0.txt", 0, 0);
    // Program p1("../test-cases/1.txt", 5, 1);
    // Program p2("../test-cases/0.txt", 20, 2);
    scheduler.spawn(p0);
    // scheduler.spawn(p1);
    // scheduler.spawn(p2);
    while (true)
    {
        std::cout << "system clock: " << systemclock << std::endl;
        scheduler.admit(systemclock);
        scheduler.dispatch();
        scheduler.process();
        scheduler.listenQblocked();
        if (scheduler.getQnew().empty() && scheduler.getQready().empty() && scheduler.getQrunning().empty() && scheduler.getQblocked().empty())
            break;
        while (inputbreak)
        {
            std::string tmp;
            std::cout << "> ";
            std::cin >> tmp;
            std::system("clear");

            if (tmp == "exit" || tmp == "e")
                return 0;
            if (tmp == "next" || tmp == "n")
                break;
            if (tmp == "finish" || tmp == "f")
                inputbreak = false;
            if (tmp == "debbug" || tmp == "d")
            {
                scheduler.printAll();
                break;
            }
        }
        systemclock++;
    }

    return 0;
}