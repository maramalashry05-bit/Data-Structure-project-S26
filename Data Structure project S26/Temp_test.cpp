#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "ArrivalAction.h"
#include "CancelAction.h"
#include "PromoteAction.h"
#include "Chef.h"
#include <cstdlib>
#include <ctime>

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    Restaurant r;

    int idCounter = 1;
    int simulationDuration = 50; 

    r.AddChef(new Chef(1, CN, 5));
    r.AddChef(new Chef(2, CS, 5));

    cout << "--- RESTAURANT SIMULATION STARTING ---" << endl;

    for (int t = 1; t <= simulationDuration; t++) {
        cout << "\n[ Time Step: " << t << " ]" << endl;

        int arrivalProb = rand() % 100 + 1;
        if (arrivalProb <= 70) {
            int typeRoll = rand() % 100 + 1;
            ORD_TYPE type;

            if (typeRoll <= 45) type = TYPE_OD;      
            else if (typeRoll <= 80) type = TYPE_OV; 
            else type = TYPE_OT;                    

            int dist = rand() % 30 + 1;
            double money = (rand() % 450) + 50;


            Action* pAct = new ArrivalAction(t, idCounter++, type, dist, money);
            pAct->Execute(&r);
            delete pAct;

            cout << ">> New order generated! ID: " << (idCounter - 1) << endl;
        }

     
        r.ExecuteActions(t);     
        r.AssignChefToOrder();   
        r.MoveToReady();         

        r.AssignOrdersToTables(t); 
        r.AssignOrdersToScooters(t); 
        r.UpdateServiceStatus(t);    

    
    }

    cout << "\n========================================" << endl;
    cout << "SIMULATION COMPLETE. GENERATING REPORT..." << endl;
    cout << "========================================\n" << endl;

    r.GenerateFinalReport();

    system("pause");
    return 0;
}
