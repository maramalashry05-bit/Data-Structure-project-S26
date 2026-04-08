#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "ArrivalAction.h"
#include "CancelAction.h"
#include "PromoteAction.h"
#include "Chef.h"

int main()
{
    Restaurant r;

    cout << "===== SYSTEM TEST START =====\n";

    // =========================
    // 1) Add Chefs (Member 2)
    // =========================
    r.AddChef(new Chef(1, CN, 5));
    r.AddChef(new Chef(2, CS, 8));
    r.AddChef(new Chef(3, CN, 4));

    // =========================
    // 2) Add Actions (Member 1)
    // =========================
    r.AddAction(new ArrivalAction(1, 1, TYPE_OD, 2, 100));
    r.AddAction(new ArrivalAction(2, 2, TYPE_OV, 3, 200));
    r.AddAction(new ArrivalAction(3, 3, TYPE_OT, 1, 50));

    r.AddAction(new CancelAction(4, 1));
    r.AddAction(new PromoteAction(5, 3));

    // =========================
    // 3) Simulation Loop
    // =========================
    for (int t = 1; t <= 7; t++)
    {
        cout << "\n====== Time Step: " << t << " ======\n";

        // Execute actions
        r.ExecuteActions(t);

        // Assign chefs
        r.AssignChefToOrder();

        // Move to ready
        r.MoveToReady();

        // Finish orders
        r.FinishOrder();

        // =========================
        // 4) Print UI (Member 3)
        // =========================
       /* cout << "\n--- CURRENT STATE ---\n";

        cout << "VIP Orders: ";
        r.getVIP().printIDs();

        cout << "Normal Orders: ";
        r.getNormal().printIDs();

        cout << "Cold Orders: ";
        r.getCold().printIDs();

        cout << "Ready Orders: ";
        r.getReady().printIDs();

        cout << "Finished Orders: ";
        r.getFinished().printIDsReverse();

        cout << "\n----------------------\n";


    cout << "\n===== TEST FINISHED =====\n";*/
    }
    return 0;
}