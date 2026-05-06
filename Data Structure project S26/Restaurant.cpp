#include "Restaurant.h"
#include <iostream>

using namespace std;

// =========================================================================
// Order Management
// =========================================================================

void Restaurant::AddOrder(Order* o)
{
    ORD_TYPE type = o->GetType();

    if (type == TYPE_OD) {
        PEND_ODN.enqueue(o);
    }
    else if (type == TYPE_OV) {
       
        int dist = o->GetDistance();
        int priority;

        if (dist > 0) {
            priority = (o->getEatingTime() + o->GetSize()) / dist;
        }
        else {
           
            priority = (o->getEatingTime() + o->GetSize());
        }

        PEND_OVG.enqueue(o, priority);
    }
    else {
        PEND_OT.enqueue(o);
    }
}

void Restaurant::CancelOrder(int id)
{
    // 1. Check special cancellation container if it exists
    if (Pend_OVC.CancelOrder(id)) {
        cout << "Order " << id << " cancelled successfully." << endl;
        return;
    }

    Order* pOrd = nullptr;
    LinkedQueue<Order*> tempQueue;
    bool found = false;

    // 2. Manual search in the Normal Delivery Queue (PEND_ODN)
    // Note: We use PEND_ODN to match your Restaurant.h definition
    while (PEND_ODN.dequeue(pOrd)) {
        if (pOrd != nullptr && pOrd->GetID() == id) {
            found = true;
            Cancelled_orders.enqueue(pOrd); // Move to cancelled list
            // We don't 'break' here because we need to move the rest of the 
            // orders into tempQueue to maintain their original order.
        }
        else {
            tempQueue.enqueue(pOrd);
        }
    }

    // 3. Restore the original queue from tempQueue
    while (tempQueue.dequeue(pOrd)) {
        PEND_ODN.enqueue(pOrd);
    }

    if (found) {
        cout << "Order " << id << " cancelled successfully from pending list." << endl;
    }
    else {
        cout << "Order " << id << " not found or already in the kitchen." << endl;
    }
}
void Restaurant::PromoteOrder(int id)
{
    LinkedQueue<Order*> tempQueue;
    Order* currentOrder = nullptr;
    bool found = false;

    // Search through normal pending orders
    while (PEND_ODN.dequeue(currentOrder))
    {
        if (currentOrder->GetID() == id)
        {
            found = true;
            // Promote to VIP: Calculate priority and push to PEND_OVG
            int newPriority = 100; // Replace with actual priority equation
            PEND_OVG.enqueue(currentOrder, newPriority);
        }
        else
        {
            tempQueue.enqueue(currentOrder);
        }
    }

    // Restore the normal queue
    while (tempQueue.dequeue(currentOrder))
    {
        PEND_ODN.enqueue(currentOrder);
    }
}

// =========================================================================
// Lifecycle Phase Transitions
// =========================================================================



void Restaurant::MoveToReady(int currentTime)
{
    Order* pOrd = nullptr;
    int priority;

    while (Cooking_Orders.peek(pOrd, priority)) {
        // If finish time is reached or passed
        if (currentTime >= pOrd->getFinishTime()) {
            Cooking_Orders.dequeue(pOrd, priority);
            pOrd->setReadyTime(currentTime);

            // Route to correct Ready queue
            if (pOrd->GetType() == TYPE_OV) RDY_OV.enqueue(pOrd);
            else if (pOrd->GetType() == TYPE_OD) RDY_OD.enqueue(pOrd);
            else RDY_OT.enqueue(pOrd);

            // Note: In full logic, return the chef to their Free list here
        }
        else {
            break;
        }
    }
}

void Restaurant::FinishOrder()
{
    Order* finishedOrder = nullptr;
    int priority;

    // Move completed orders from InServ_Orders to the finished list
    // (Requires checking if current time >= service finish time)
    // Example conceptual loop:
    /*
    while (InServ_Orders.peek(finishedOrder, priority)) {
        if (currentTime >= finishedOrder->getFinishTime()) {
            InServ_Orders.dequeue(finishedOrder, priority);
            // Assuming you add a FinishedList (LinkedStack) to your .h
            // FinishedList.push(finishedOrder);

            TotalServedCount++;
            TotalWaitTime += (finishedOrder->getServiceStartTime() - finishedOrder->getArrivalTime());
            TotalServiceTime += (finishedOrder->getFinishTime() - finishedOrder->getServiceStartTime());
        } else {
            break;
        }
    }
    */
}

void Restaurant::MoveScooterToMaintenance()
{
    Scooter* returningScooter = nullptr;
    int priority;

    // Check scooters that are cutting back
    while (Back_Scooters.peek(returningScooter, priority))
    {
        // Check if it reached the restaurant
        Back_Scooters.dequeue(returningScooter, priority);

        // Check maintenance threshold (e.g., 5 trips)
        if (returningScooter->GetTrips() >= 5)
        {
            Maint_Scooters.enqueue(returningScooter);
        }
        else
        {
            // Back to free scooters
            returningScooter->SetAvailable(true);
            Free_Scooters.enqueue(returningScooter, returningScooter->GetSpeed());
        }
    }
}

// =========================================================================
// Resource Assignment
// =========================================================================

void Restaurant::AddChef(Chef* c)
{
    if (c->GetType() == CN) {
        Free_CN.enqueue(c);
    }
    else {
        Free_CS.enqueue(c);
    }
}

void Restaurant::AssignChefToOrder()
{
    // Example logic: Assign a Normal Chef to a Normal Order
    Order* pendingOrder = nullptr;
    Chef* availableChef = nullptr;

    if (!PEND_ODN.isEmpty() && !Free_CN.isEmpty())
    {
        PEND_ODN.dequeue(pendingOrder);
        Free_CN.dequeue(availableChef);

        // Uses the AssignOrder method you defined in Chef.cpp
        int currentTime = 0; // Replace with actual simulation time variable
        availableChef->AssignOrder(pendingOrder, currentTime);
        pendingOrder->SetServiceStartTime(currentTime);
        pendingOrder->SetFinishTime(availableChef->GetFinishTime());

        // Note: Your Cook_Ords queue is defined as priQueue<Order> (by value).
        // If pendingOrder is a pointer, you will need to dereference it:
        // Cooking_Orders.enqueue(*pendingOrder, priority); 
    }
}

void Restaurant::AssignOrdersToScooters(int currentTime)
{
    Order* readyDeliveryOrder = nullptr;
    Scooter* availableScooter = nullptr;
    int scooterPri;

    if (!RDY_OD.isEmpty() && !Free_Scooters.isEmpty())
    {
        RDY_OD.dequeue(readyDeliveryOrder);
        Free_Scooters.dequeue(availableScooter, scooterPri);

        readyDeliveryOrder->setScooter(availableScooter);
        availableScooter->incrementTrips();
        availableScooter->SetAvailable(false);
        readyDeliveryOrder->SetServiceStartTime(currentTime);

        // Calculate delivery duration
        int deliveryDuration = readyDeliveryOrder->GetDistance() / availableScooter->GetSpeed();
        readyDeliveryOrder->SetFinishTime(currentTime + deliveryDuration);

        // Add to In-Service priority queue (priority based on finish time)
        InServ_Orders.enqueue(readyDeliveryOrder, -readyDeliveryOrder->getFinishTime());
    }
}

// =========================================================================
// MEMBER 3 WORKLOAD: Tables, Takeaway, and VIP Overwait
// =========================================================================

// Feature 10: Assign Table
void Restaurant::AssignOrdersToTables(int currentTime)
{
    Order* readyDineInOrder = nullptr;

    // Check the Ready queue for Dine-in orders (Assuming TYPE_OT is Table/Dine-in)
    while (RDY_OT.peek(readyDineInOrder))
    {
        Table  * bestTable=nullptr;

        // Find best table using the updated getBestFit function
        if (Free_Tables.getBestFit(readyDineInOrder->GetNumSeats(), bestTable))
        {
            // Successfully found a table, so remove the order from Ready queue
            RDY_OT.dequeue(readyDineInOrder);

            // Assign table and calculate service times
            // NOTE: We use new Table(bestTable) so the order holds a valid pointer
            // to a table object on the heap, not a local variable address.
            readyDineInOrder->setTable(bestTable);
            readyDineInOrder->SetServiceStartTime(currentTime);
            readyDineInOrder->SetFinishTime(currentTime + readyDineInOrder->getEatingTime());

            // Push to In-Service priority queue. 
            // We use negative finish time so orders finishing sooner stay at the front
            InServ_Orders.enqueue(readyDineInOrder, -readyDineInOrder->getFinishTime());

            // Move Table to the appropriate busy queue
            // (Assumes you have a way to define if a table is sharable or not)
            // For now, placing in Busy_No_Sharable
            Busy_No_Sharable.enqueue(bestTable, 0);
        } 
        else
        {
            // No tables available that fit this order's size. Stop trying.
            break;
        }
    }
}

// Feature 7: Check finished Dine-In orders
void Restaurant::CheckFinishedDineInOrders(int currentTime)
{
    Order* finishedOrder = nullptr;
    int priority;
    priQueue<Order*> tempQueue;

    // Go through the In-Service queue
    while (InServ_Orders.dequeue(finishedOrder, priority))
    {
        // If it's a Table order AND its finish time has arrived
        if (finishedOrder->GetType() == TYPE_OT && currentTime >= finishedOrder->getFinishTime())
        {
            // 1. Move to Finished list
            Finished_orders.push(finishedOrder);

            // 2. Free the table
            Table* assignedTable = finishedOrder->getTable();
            if (assignedTable != nullptr)
            {
                // Put table back into Free_Tables (Capacity is its priority)
                assignedTable->SetAvailable(true);
                Free_Tables.enqueue(assignedTable, assignedTable->getCapacity());

                // Note: You will also need to remove it from Busy_No_Sharable
                // using a loop similar to what we did in getBestFit().
            }
        }
        else
        {
            // Not finished, or not a Dine-In order. Save it in temp queue.
             tempQueue.enqueue(finishedOrder, priority);
        }
    }

    // Restore the In-Service queue
    while (tempQueue.dequeue(finishedOrder, priority))
    {
        InServ_Orders.enqueue(finishedOrder, priority);
    }
}

// Feature 12: Finalize Takeaway Orders
void Restaurant::FinalizeTakeawayOrders(int currentTime)
{
    Order* takeawayOrder = nullptr;

   
    while (RDY_OT.dequeue(takeawayOrder))
    {
        takeawayOrder->SetServiceStartTime(currentTime);
        takeawayOrder->SetFinishTime(currentTime); // Instant service

        // Push directly to finished list
        Finished_orders.push(takeawayOrder);
    }
    
}

// ? BONUS: Order overwait for Ready OVG only
void Restaurant::CheckOverwaitOVG(int currentTime)
{
    int MAX_WAIT_TIME = 15;
    Order *currentOrder=nullptr;
    LinkedQueue<Order> tempQueue;

    while (RDY_OV.dequeue(currentOrder))
    {
        // Now using getReadyTime()!
        int waitTime = currentTime - currentOrder->getReadyTime();

        if (waitTime > MAX_WAIT_TIME)
        {
            cout << ">> [EMERGENCY] VIP Vegan Order " << currentOrder->GetID()
                << " has been waiting for a scooter for " << waitTime << " mins!" << endl;

            // (Handle priority boost or complain logic here)
        }

        tempQueue.enqueue(*currentOrder);
    }

    while (tempQueue.dequeue(*currentOrder)) {
        RDY_OV.enqueue(currentOrder);
    }
}

void Restaurant::AssignChefToOrder(int currentTime)
{
    Order* pOrd = nullptr;
    Chef* pChef = nullptr;
    int priority;

    // 1. Assign VIP Vegan (OVG) to Speedy Chefs first, then Normal
    while (!PEND_OVG.isEmpty()) {
        if (!Free_CS.dequeue(pChef)) {
            if (!Free_CN.dequeue(pChef)) break; // No chefs available, stop loop
        }

        if (PEND_OVG.dequeue(pOrd, priority)) {
            if (pOrd != nullptr) {
                pOrd->SetServiceStartTime(currentTime);
                pChef->AssignOrder(pOrd, currentTime);
                // Enqueue with negative finish time for min-priority behavior
                Cooking_Orders.enqueue(pOrd, -pChef->GetFinishTime());
            }
        }
    }

    // 2. Assign Normal Delivery (ODN) to Normal Chefs
    while (!PEND_ODN.isEmpty() && !Free_CN.isEmpty()) {
        if (PEND_ODN.dequeue(pOrd) && Free_CN.dequeue(pChef)) {
            if (pOrd != nullptr && pChef != nullptr) {
                pOrd->SetServiceStartTime(currentTime);
                pChef->AssignOrder(pOrd, currentTime);
                Cooking_Orders.enqueue(pOrd, -pChef->GetFinishTime());
            }
        }
    }
}

    



void Restaurant::UpdateMaintenanceList(int currentTime)
{
    Scooter* pScoot = nullptr;


    while (Maint_Scooters.peek(pScoot))
    {

        Maint_Scooters.dequeue(pScoot);
        pScoot->resetTrips();
        pScoot->SetAvailable(true);
        Free_Scooters.enqueue(pScoot, pScoot->GetSpeed());

    }
}
// Logic for handling a Combo in the Kitchen
void Restaurant::HandleComboAssignment(Order* comboOrd, int currentTime) {
    // If an order is a Combo, it needs multiple chefs
    // This logic ensures parts are assigned to different chefs 
    // but the order only moves to READY when all parts == 0.

    int parts = 2; // Example: Combo has 2 parts
    for (int i = 0; i < parts; i++) {
        Chef* pChef = nullptr;
        if (Free_CN.dequeue(pChef)) {
            // Assign sub-part to chef
            // Logic to track that this specific Chef is working on Part X of Order ID
        }
    }
}


void Restaurant::UpdateServiceStatus(int currentTime)
{
    Order* pOrd = nullptr;
    int priority;

    priQueue<Order*> tempQueue;

    while (InServ_Orders.dequeue(pOrd, priority))
    {
        if (currentTime >= pOrd->getFinishTime())
        {
            // 1. Move to finished
            Finished_orders.push(pOrd);

            // 2. Update statistics
            TotalServedCount++;

            int waitTime = pOrd->getServiceStartTime() - pOrd->getArrivalTime();
            int serviceTime = pOrd->getFinishTime() - pOrd->getServiceStartTime();

            TotalWaitTime += waitTime;
            TotalServiceTime += serviceTime;

            // 3. Handle scooter return
            Scooter* s = pOrd->getScooter();
            if (s != nullptr)
            {
                Back_Scooters.enqueue(s, s->GetSpeed());
            }
        }
        else
        {
            // áÓå ãÎáÕÔ ? ÑÌÚå ÊÇäí
            tempQueue.enqueue(pOrd, priority);
        }
    }

    // restore queue
    while (tempQueue.dequeue(pOrd, priority))
    {
        InServ_Orders.enqueue(pOrd, priority);
    }
}// =========================================================================
// Action Scheduling & Engine
// =========================================================================

void Restaurant::AddAction(Action* a)
{
    Actions.enqueue(a);
}

void Restaurant::ExecuteActions(int time)
{
    Action* act = nullptr;
    while (Actions.peek(act))
    {
        if (act->getTime() == time)
        {
            Actions.dequeue(act);
            // Assuming your actions have an Execute(this) method
            // act->Execute(this); 
            delete act;
        }
        else
        {
            break; // Actions are usually sorted by time; if this is in the future, stop checking.
        }
    }
}

void Restaurant::PrintFinished()
{
    Finished_orders.printIDsReverse();
}

LinkedStack<Order*>& Restaurant::getFinished()
{
    return Finished_orders;
}

void Restaurant::SimulateStep(int t)
{
    ExecuteActions(t);
    UpdateServiceStatus(t);
    MoveScooterToMaintenance();
    MoveToReady(t);

    AssignChefToOrder();
    AssignOrdersToTables(t);
    AssignOrdersToScooters(t);

    FinishOrder();
}

bool Restaurant::IsFinished()
{
    // The simulation is finished if all queues are empty
    return (PEND_ODN.isEmpty() && PEND_OVG.isEmpty() && PEND_OT.isEmpty() &&
        RDY_OD.isEmpty() && RDY_OT.isEmpty() /* && RDY_OV.isEmpty() */ &&
        Actions.isEmpty() && InServ_Orders.isEmpty());
}

void Restaurant::GenerateFinalReport()
{
    cout << "========== FINISHED ORDERS ==========\n";

    LinkedStack<Order*> tempStack = Finished_orders;
    LinkedStack<Order*> sortedStack;

    Order* o = nullptr;

    // =========================
    // STEP 1: SORT (DESC by Tf)
    // =========================
    while (!tempStack.isEmpty())
    {
        Order* maxOrder = nullptr;
        LinkedStack<Order*> temp2;

        while (tempStack.pop(o))
        {
            if (!maxOrder || o->getFinishTime() > maxOrder->getFinishTime())
            {
                if (maxOrder)
                    temp2.push(maxOrder);
                maxOrder = o;
            }
            else
            {
                temp2.push(o);
            }
        }

        if (maxOrder)
            sortedStack.push(maxOrder);

        while (temp2.pop(o))
            tempStack.push(o);
    }

    // =========================
    // STEP 2: FIX STACK ORDER
    // =========================
    LinkedStack<Order*> finalStack;

    while (sortedStack.pop(o))
        finalStack.push(o);

    // =========================
    // STEP 3: PRINT
    // =========================
    while (finalStack.pop(o))
    {
        int Tf = o->getFinishTime();
        int Ta = o->getArrivalTime();
        int Tr = o->getReadyTime();
        int Ts = o->getServiceStartTime();

        int Tw = Ts - Ta;
        int Tserv = Tf - Ts;
        int Tq = Tr - Ta;
        int Tc = Ts - Tr;
        int Ti = Tf - Tr;

     
        cout << Tf << " ";
        cout << o->GetID() << " ";
        cout << Tq << " ";
        cout << Ta << " ";
        cout << Tr << " ";
        cout << Ts << " ";
        cout << Ti << " ";
        cout << Tc << " ";
        cout << Tw << " ";
        cout << Tserv << endl;
    }
    cout << "\n========== STATISTICS ==========\n";

    int cancelled = Cancelled_orders.getCount();
    int finished = TotalServedCount;
    int totalOrders = finished + cancelled;

    cout << "Total Orders: " << totalOrders << endl;
    cout << "Finished Orders: " << finished << endl;
    cout << "Cancelled Orders: " << cancelled << endl;

    if (totalOrders > 0)
    {
        cout << "Finished %: " << (finished * 100.0 / totalOrders) << "%" << endl;
        cout << "Cancelled %: " << (cancelled * 100.0 / totalOrders) << "%" << endl;
    }

    if (finished > 0)
    {
        cout << "Average Wait Time: " << (TotalWaitTime / finished) << endl;
        cout << "Average Service Time: " << (TotalServiceTime / finished) << endl;
    }
    cout << "\n========== SCOOTERS ==========\n";

    cout << "Free Scooters: " << Free_Scooters.getCount() << endl;
    cout << "Maintenance Scooters: " << Maint_Scooters.getCount() << endl;
    cout << "\n========== CHEFS ==========\n";

    cout << "Normal Chefs: " << Free_CN.getCount() << endl;
    cout << "Speedy Chefs: " << Free_CS.getCount() << endl;
}