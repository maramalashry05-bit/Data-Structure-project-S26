#pragma once
#include "Action.h"
#include "Order.h"
class ArrivalAction : public Action
{
private:
    int id, size;
    double money; 
    ORD_TYPE type;

public:
    ArrivalAction(int t, int i, ORD_TYPE tp, int s, int m);

    void Execute(Restaurant* pRest);
};