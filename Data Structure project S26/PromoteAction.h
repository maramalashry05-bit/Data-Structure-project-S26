#pragma once
#include "Action.h"

class PromoteAction : public Action
{
private:
    int ID;
    double ExtraMoney;

public:
    PromoteAction(int t, int i,double em);

    void Execute(Restaurant* pRest);
};