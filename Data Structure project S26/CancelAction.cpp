#include "CancelAction.h"
#include "Restaurant.h"
#include <iostream>

using namespace std;

CancelAction::CancelAction(int t, int i)
    : Action(t)
{
    id = i;
}

void CancelAction::Execute(Restaurant* pRest)
{
    cout << "Cancel request for " << id << endl;
    pRest->CancelOrder(id);
}