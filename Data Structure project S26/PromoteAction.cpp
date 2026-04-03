#include "PromoteAction.h"
#include "Restaurant.h"
#include <iostream>

using namespace std;

PromoteAction::PromoteAction(int t, int i)
    : Action(t)
{
    id = i;
}

void PromoteAction::Execute(Restaurant* pRest)
{
    cout << "Promote request for " << id << endl;
    pRest->PromoteOrder(id);
}