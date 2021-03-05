#include "compute.h"
#include <iostream>
#include <chrono>
#include <thread>
Compute::Compute()
{

}

void Compute::run()
{
    printf("Y");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    emit lets_paint();
}

