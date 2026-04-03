#pragma once
#include "Action.h"

class CancelAction : public Action
{
private:
    int id;

public:
    CancelAction(int t, int i);

    void Execute(Restaurant* pRest);
};