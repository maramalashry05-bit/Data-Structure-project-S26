#include "MaintenanceAction.h"
#include "Restaurant.h"
#include <iostream>

using namespace std;

MaintenanceAction::MaintenanceAction(int t)
    : Action(t) {
}

void MaintenanceAction::Execute(Restaurant* pRest)
{
    cout << "Scooter to maintenance\n";
    pRest->MoveScooterToMaintenance();
}