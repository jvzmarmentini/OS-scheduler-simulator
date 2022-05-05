#include <iostream>
#include "Program.h"
#include "Scheduler.h"

int main()
{
    int systemclock = 0;
    bool inputbreak = true;

    std::cout << "choose the scheduler policy (PSP/PCP/RR)" << std::endl
              << ": ";
    std::string policy;
    std::cin >> policy;
    Scheduler scheduler(policy);
    if (policy == "RR")
    {
        std::cout << "enter the quantum" << std::endl
                  << ": ";
        int quantum;
        std::cin >> quantum;
        scheduler.setQuantum(quantum);
    }
    int pid = 1, arrivaltime, priority = 0;
    std::string path, opc;
    while (true)
    {
        std::cout << "enter the program " << pid << " path" << std::endl
                  << ": ";
        std::cin >> path;
        std::cout << "enter it arrival time" << std::endl
                  << ": ";
        std::cin >> arrivaltime;
        if (policy == "PCP")
        {
            std::cout << "enter it priority" << std::endl
                      << ": ";
            std::cin >> priority;
        }
        Program p(path, arrivaltime, pid, priority);
        scheduler.spawn(p);
        pid++;
        std::cout << "Do you want to add one more? ([y]/n) " << std::endl
                  << ": ";
        std::cin >> opc;
        if (opc == "n" || opc == "N")
            break;
    }
    scheduler.printAll();
    while (true)
    {
        std::cout << "system clock: " << systemclock << std::endl;
        scheduler.admit(systemclock);
        scheduler.dispatch();
        scheduler.process(systemclock);
        scheduler.listenQblocked();
        if (scheduler.isFinished())
            break;
        while (inputbreak)
        {
            std::string tmp;
            std::cout << "> ";
            std::cin >> tmp;

            if (tmp == "exit" || tmp == "e")
                return 0;
            if (tmp == "next" || tmp == "n")
            {
                scheduler.printAll();
                break;
            }
            if (tmp == "finish" || tmp == "f")
                inputbreak = false;
        }
        systemclock++;
        std::cout << std::endl;
    }

    scheduler.printTimes();

    return 0;
}