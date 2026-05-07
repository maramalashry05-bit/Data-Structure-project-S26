#include "PromoteAction.h"
#include "Restaurant.h"
#include <iostream>

using namespace std;

PromoteAction::PromoteAction(int t, int i,double em)
    : Action(t)
{
    ID = i;
    ExtraMoney = em;
}

void PromoteAction::Execute(Restaurant* pRest)
{
    cout << "Promote request for " << ID << endl;
    pRest->PromoteOrder(ID,ExtraMoney);
}