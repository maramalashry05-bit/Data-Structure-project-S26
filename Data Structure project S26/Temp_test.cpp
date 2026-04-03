#include <iostream>
#include "LinkedStack.h"
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Order.h"
#include "Chef.h"
#include "Scooter.h"
#include "Restaurant.h"
#include "Action.h"
#include "ArrivalAction.h"
#include "CancelAction.h"
#include "PromoteAction.h"
#include "MaintenanceAction.h"
#include"DerivedPriQueue.h"
#include "DerivedQueue.h"

using namespace std;

int main()
{
    Restaurant rest;

    LinkedQueue<Action*> actions;

    // ===== Actions =====
    actions.enqueue(new ArrivalAction(0, 1, TYPE_OD, 2, 100));
    actions.enqueue(new ArrivalAction(1, 2, TYPE_OV, 3, 200));
    actions.enqueue(new CancelAction(2, 1));
    actions.enqueue(new PromoteAction(3, 2));
    actions.enqueue(new MaintenanceAction(4));

    Action* act;
    int time = 0;

    while (!actions.isEmpty())
    {
        actions.dequeue(act);

        cout << "\nTime Step: " << time++ << endl;

        act->Execute(&rest);

        rest.MoveToReady();
        rest.FinishOrder();

        delete act;
    }

    cout << "\n--- Finished Orders ---\n";
    rest.PrintFinished();


    cout << "=== SIMULATION START ===\n";

    for (int t = 0; t < 3; t++)
    {
        cout << "\nTime Step: " << t << endl;

        // ===== Lists =====
        LinkedQueue<Order*> pendingNormal;
        priQueue<Order*> pendingVIP;

        priQueue<Order*> cooking;
        LinkedQueue<Order*> ready;

        LinkedQueue<Chef*> availableChefs;

        priQueue<Scooter*> availableScooters;
        LinkedQueue<Scooter*> maintenanceScooters;

        priQueue<Order*> inService;   // حسب finish time
        LinkedStack<Order*> finished;

        // ===== Data =====
        pendingNormal.enqueue(new Order(1, TYPE_OD, 2, 100));
        pendingVIP.enqueue(new Order(2, TYPE_OV, 3, 200), 5); // priority

        availableChefs.enqueue(new Chef(1, 5));
        availableChefs.enqueue(new Chef(2, 6));

        availableScooters.enqueue(new Scooter(1), 10);

        // ================= 3.1 =================
        Order* o;
        Chef* c;

        if (!pendingVIP.isEmpty())
        {
            int pri;
            pendingVIP.dequeue(o, pri);
        }
        else
        {
            pendingNormal.dequeue(o);
        }

        availableChefs.dequeue(c);

        cooking.enqueue(o, 1); // priority fake (finish time later)

        cout << "Moved to cooking: ";
        o->Print();

        //================= 3.2 =================
        int pri;
        Order* o2;

        cooking.dequeue(o2, pri);
        ready.enqueue(o2);

        cout << "Moved to ready: ";
        o2->Print();

        // ================= 3.3 =================
        Order* o3;
        ready.dequeue(o3);

        if (o3->GetType() == TYPE_OT)
        {
            finished.push(o3);
            cout << "Finished (Takeaway): ";
        }
        else if (o3->GetType() == TYPE_OV)
        {
            Scooter* s;
            int sp;

            availableScooters.dequeue(s, sp);
            finished.push(o3);

            cout << "Delivered by scooter: ";
        }
        else
        {
            inService.enqueue(o3, 1);
            cout << "Moved to In-Service: ";
        }

        o3->Print();

        // ================= 3.4 (Cancel) =================
        int cancelID = 1;
        LinkedQueue<Order*> temp;
        Order* cur;

        while (!pendingNormal.isEmpty())
        {
            pendingNormal.dequeue(cur);

            if (cur->GetID() == cancelID)
            {
                cout << "Cancelled: ";
                cur->Print();
                delete cur;
            }
            else
            {
                temp.enqueue(cur);
            }
        }

        while (!temp.isEmpty())
        {
            temp.dequeue(cur);
            pendingNormal.enqueue(cur);
        }

        // ================= 3.7 =================
        Order* o4;
        if (!inService.isEmpty())
        {
            int p;
            inService.dequeue(o4, p);
            finished.push(o4);

            cout << "Finished from table: ";
            o4->Print();
        }

        //// ================= 3.8 =================
        Scooter* s;
        if (!availableScooters.isEmpty())
        {
            int sp;
            availableScooters.dequeue(s, sp);

            if (rand() % 2)
            {
                maintenanceScooters.enqueue(s);
                cout << "Scooter → maintenance\n";
            }
            else
            {
                availableScooters.enqueue(s, sp);
                cout << "Scooter reused\n";
            }
        }

        // ================= 3.9 =================
        Scooter* s2;
        if (!maintenanceScooters.isEmpty())
        {
            maintenanceScooters.dequeue(s2);
            availableScooters.enqueue(s2, 5);

            cout << "Scooter back\n";
        }

        // ================= 3.10 =================
        cout << "\n--- Finished Orders (Stack) ---\n";

        Order* f;
        while (!finished.isEmpty())
        {
            finished.pop(f);
            f->Print();
        }

    }

    cout << " \n===== DerivedQueue Test =====\n\n";

    DerivedQueue<Order*> q;

    
    q.enqueue(new Order(1, TYPE_OD, 2, 100));
    q.enqueue(new Order(2, TYPE_OV, 3, 200));
    q.enqueue(new Order(3, TYPE_OD, 1, 50));

  
    Order* removed = nullptr;

    if (q.removeById(2, removed))
    {
        cout << "Removed: ";
        removed->Print();
    }
    else
    {
        cout << "Not found\n";
    }

    // print remaining
    cout << "\nRemaining orders:\n";

    Order* o;
    while (!q.isEmpty())
    {
        q.dequeue(o);
        o->Print();
    }

    return 0;
}