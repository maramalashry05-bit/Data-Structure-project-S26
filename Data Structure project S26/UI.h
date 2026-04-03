#pragma once
#include <iostream>
using namespace std;

#include "DerivedQueue.h"
#include "DerivedPriQueue.h"
#include "LinkedStack.h"
#include "Order.h"

class UI
{
public:

    void PrintOutput(
        DerivedPriQueue<Order*>& vip,
        DerivedQueue<Order*>& normal,
        DerivedQueue<Order*>& cold,
        DerivedQueue<Order*>& ready,
        LinkedStack<Order*>& finished
    );
};