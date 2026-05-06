#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "ArrivalAction.h"
#include "CancelAction.h"
#include "PromoteAction.h"
#include "Chef.h"
#include <cstdlib>
#include <ctime>

//int main()
//{
//    srand(static_cast<unsigned int>(time(0)));
//    Restaurant r;
//
//    int idCounter = 1;
//    int simulationDuration = 50; 
//
//    r.AddChef(new Chef(1, CN, 5));
//    r.AddChef(new Chef(2, CS, 5));
//
//    cout << "--- RESTAURANT SIMULATION STARTING ---" << endl;
//
//    for (int t = 1; t <= simulationDuration; t++) {
//        cout << "\n[ Time Step: " << t << " ]" << endl;
//
//        int arrivalProb = rand() % 100 + 1;
//        if (arrivalProb <= 70) {
//            int typeRoll = rand() % 100 + 1;
//            ORD_TYPE type;
//
//            if (typeRoll <= 45) type = TYPE_OD;      
//            else if (typeRoll <= 80) type = TYPE_OV; 
//            else type = TYPE_OT;                    
//
//            int dist = rand() % 30 + 1;
//            double money = (rand() % 450) + 50;
//
//
//            Action* pAct = new ArrivalAction(t, idCounter++, type, dist, money);
//            pAct->Execute(&r);
//            delete pAct;
//
//            cout << ">> New order generated! ID: " << (idCounter - 1) << endl;
//        }
//
//     
//        r.ExecuteActions(t);     
//        r.AssignChefToOrder();   
//        r.MoveToReady(t);         
//
//        r.AssignOrdersToTables(t); 
//        r.AssignOrdersToScooters(t); 
//        r.UpdateServiceStatus(t);    
//
//    
//    }
//
//    cout << "\n========================================" << endl;
//    cout << "SIMULATION COMPLETE. GENERATING REPORT..." << endl;
//    cout << "========================================\n" << endl;
//
//    r.GenerateFinalReport();
//
//    system("pause");
//
//
//
//
//    return 0;
//}


//
//#include <iostream>
//#include "Restaurant.h"
//#include "Order.h"
//#include "Table.h"
//
//using namespace std;
//
//int main()
//{
//    cout << "==================================================\n";
//    cout << "        MEMBER 3:  TEST                           \n";
//    cout << "==================================================\n\n";
//
//    Restaurant r;
//
//    // ==========================================================
//    // 1. SETUP MOCK DATA (Using 'new' for Pointers!)
//    // ==========================================================
//
//    // --- Setup Tables ---
//    Table* t1 = new Table(1);
//    t1->SetAvailable(true); // Assuming you have this setter
//    // t1->setCapacity(4);  // Make sure your Table class has a way to set Capacity!
//    r.Free_Tables.enqueue(t1, 4); // Priority is capacity (4)
//
//    Table* t2 = new Table(2);
//    t2->SetAvailable(true);
//    // t2->setCapacity(2);
//    r.Free_Tables.enqueue(t2, 2); // Priority is capacity (2)
//
//    cout << "[SETUP] Added 2 Tables (Capacities: 4 and 2).\n";
//
//    // --- Setup Dine-in Orders (TYPE_OT) ---
//    // Order 101: Needs 3 seats, Eating time is 5 mins
//    Order* dineInOrder = new Order(101, TYPE_OT, 3, 250.0);
//    dineInOrder->SetEatingTime(5);
//    r.RDY_OT.enqueue(dineInOrder);
//
//    cout << "[SETUP] Added Dine-In Order 101 (Needs 3 seats).\n";
//
//    // --- Setup VIP Vegan Orders (TYPE_OV) for Bonus Test ---
//    // Order 201: Finished cooking at timestep 5
//    Order* vipVegan = new Order(201, TYPE_OV, 2, 150.0);
//    vipVegan->setReadyTime(5);
//    r.RDY_OV.enqueue(vipVegan);
//
//    cout << "[SETUP] Added VIP Vegan Order 201 (Ready since T=5).\n";
//    cout << "--------------------------------------------------\n\n";
//
//    // ==========================================================
//    // 2. RUN THE SIMULATION LOOP (Timesteps 1 to 25)
//    // ==========================================================
//
//    for (int currentTime = 1; currentTime <= 25; currentTime++)
//    {
//        // Uncomment this to watch the loop progress
//        // cout << "--- Timestep: " << currentTime << " ---" << endl;
//
//        // 1. F10: Assign Tables
//        // Triggers at T=1 and assigns Order 101 to Table 1
//        r.AssignOrdersToTables(currentTime);
//
//        // 2. F7: Check Finished Dine-in Orders
//        // Order 101 finishes at T=6 (1 + 5). 
//        r.CheckFinishedDineInOrders(currentTime);
//
//        // 3. Bonus: Check VIP Vegan Overwait
//        // Order 201 ready at T=5. Max wait is 15. Triggers at T=21.
//        r.CheckOverwaitOVG(currentTime);
//
//        // Debug prints to watch the state change
//        if (currentTime == 1) {
//            cout << "[T=1]  > Notice: Order 101 should now be assigned to Table 1." << endl;
//        }
//        if (currentTime == 6) {
//            cout << "[T=6]  > Notice: Order 101 should finish now, and Table 1 should be freed." << endl;
//        }
//        if (currentTime == 21) {
//            cout << "[T=21] > Notice: The Overwait emergency should trigger right now!" << endl;
//        }
//    }
//
//    // ==========================================================
//    // 3. FINAL VERIFICATION
//    // ==========================================================
//    cout << "\n==================================================\n";
//    cout << "                  FINAL STATE                     \n";
//    cout << "==================================================\n";
//
//    // NOTE: Depending on how your FinishedList is implemented, 
//    // you may need to adjust the print statement below to match your code.
//    cout << "Test completed! If you saw the emergency message at T=21, the bonus works!\n";
//
//    return 0;
//}

// Test for the work of Member 2 

//#include <iostream>
//#include "Restaurant.h"
//#include "Chef.h"
//#include "Order.h"
//
//#include <iostream>
//#include "Restaurant.h"
//#include "Chef.h"
//#include "Order.h"
//
//using namespace std;
//
//int main() {
//    cout << "--- Initializing Restaurant Test ---" << endl;
//    Restaurant myResto;
//
//    // 1. Add Chefs (Speed > 0 to prevent crash)
//    cout << "Adding Chefs..." << endl;
//    myResto.AddChef(new Chef(1, CS, 10)); // Speedy Chef
//    myResto.AddChef(new Chef(2, CN, 5));  // Normal Chef
//
//    // 2. Create and Add Orders (Distance > 0 to prevent crash)
//    cout << "Adding Orders..." << endl;
//
//    Order* vOrder = new Order(501, TYPE_OV, 20, 100.0); // VIP
//    vOrder->SetDistance(10); // CRITICAL: Avoid division by zero
//    myResto.AddOrder(vOrder);
//
//    Order* nOrder = new Order(101, TYPE_OD, 10, 50.0);  // Normal
//    nOrder->SetDistance(5);
//    myResto.AddOrder(nOrder);
//
//    // 3. Run Simulation Step
//    int T = 1;
//    cout << "\n--- Simulating Step T=" << T << " ---" << endl;
//
//    // Assignment Logic
//    myResto.AssignChefToOrder(T);
//
//    // Check if assignments happened (you can add a print function in Restaurant to verify)
//    cout << "Assignment complete. Check 'Cooking_Orders' queue status." << endl;
//
//    // 4. Advance time to see items move to READY
//    T = 5;
//    cout << "\n--- Advancing to T=" << T << " ---" << endl;
//    myResto.MoveToReady(T);
//
//    cout << "Simulation check complete. No crashes detected." << endl;
//
//    return 0;
//}



// Test for Member 4 
#include <iostream>
#include "Restaurant.h"
#include "Order.h"

using namespace std;

int main() {
    /*Restaurant r;

    Order* o1 = new Order(1, TYPE_OD, 2, 100);
    o1->setArrivalTime(0);
    o1->SetServiceStartTime(1);
    o1->SetFinishTime(5);

    Scooter* s1 = new Scooter(1, 10, false);
    o1->setScooter(s1);

    r.InServ_Orders.enqueue(o1, -o1->getFinishTime());
    Order* o2 = new Order(2, TYPE_OD, 1, 50);
    o2->setArrivalTime(0);
    o2->SetServiceStartTime(2);
    o2->SetFinishTime(8);

    Scooter* s2 = new Scooter(2, 5, false);
    o2->setScooter(s2);

    r.InServ_Orders.enqueue(o2, -o2->getFinishTime());
    cout << "Before:\n";
    cout << "Finished = " << r.TotalServedCount << endl;
    r.UpdateServiceStatus(6);

    cout << "\nAt time 6:\n";
    cout << "Finished = " << r.TotalServedCount << endl;
    r.PrintFinished();
    r.UpdateServiceStatus(9);

    cout << "\nAt time 9:\n";
    cout << "Finished = " << r.TotalServedCount << endl;
    r.PrintFinished();*/

    // 4. Generate Output (Feature 13)
   /* cout << "Generating Final Report File..." << endl;
    myResto.GenerateFinalReport();

    cout << "\n--- Test Complete ---" << endl;
    cout << "Check 'Simulation_Report.txt' for the results." << endl;*/

    return 0;
}


