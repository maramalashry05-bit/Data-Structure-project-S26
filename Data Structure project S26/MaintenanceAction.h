#pragma once
#include "Action.h"

class MaintenanceAction : public Action
{
public:
    MaintenanceAction(int t);

    void Execute(Restaurant* pRest);
};