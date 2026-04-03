#include "ArrivalAction.h"
#include "Restaurant.h"
#include "Order.h"
#include <iostream>

using namespace std;

ArrivalAction::ArrivalAction(int t, int i, ORD_TYPE tp, int s, int m)
    : Action(t)
{
    id = i;
    type = tp;
    size = s;
    money = m;
}

void ArrivalAction::Execute(Restaurant* pRest)
{
    Order* o = new Order(id, type, size, money);
    pRest->AddOrder(o);

    cout << "Arrival: ";
    o->Print();
}