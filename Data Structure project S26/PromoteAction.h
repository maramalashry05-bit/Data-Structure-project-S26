#pragma once
#include "Action.h"

class PromoteAction : public Action
{
private:
    int id;

public:
    PromoteAction(int t, int i);

    void Execute(Restaurant* pRest);
};