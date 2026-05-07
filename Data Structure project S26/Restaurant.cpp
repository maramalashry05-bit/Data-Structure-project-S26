#include "Restaurant.h"
#include <iostream>

using namespace std;

// =========================================================================
// Order Management
// =========================================================================

void Restaurant::AddOrder(Order* o)
{
    if (o == nullptr) return;

    ORD_TYPE type = o->GetType();

    switch (type)
    {
    case TYPE_ODN:
        PEND_ODN.enqueue(o);
        break;

    case TYPE_ODG:
        PEND_ODG.enqueue(o);
        break;

    case TYPE_OT:
        PEND_OT.enqueue(o);
        break;

    case TYPE_OVC:
    case TYPE_OVN:
        // Normal and Cold delivery orders go to the standard pending delivery queue
        PEND_OVN.enqueue(o);
        break;

    case TYPE_OVG:
    {
        // Grilled Delivery (VIP) gets priority calculated and added to the priQueue
        int dist = o->GetDistance();
        int priority;

        if (dist > 0) {
            priority = (o->getEatingTime() + o->GetSize()) / dist;
        }
        else {
            priority = (o->getEatingTime() + o->GetSize());
        }

        PEND_OVG.enqueue(o, priority);
        break;
    }

    default:
        break;
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
void Restaurant::PromoteOrder(int orderId, double extraMoney)
{
    LinkedQueue<Order*> tempQueue;
    Order* pOrd = nullptr;
    bool found = false;

    // 1. Search for the order in the Pending Normal Delivery Queue (PEND_OVN)
    // Note: Use your exact queue name here if it's different!
    while (PEND_OVN.dequeue(pOrd))
    {
        if (pOrd->GetID() == orderId)
        {
            found = true;

            // 2. Update the order's total money
            // (Make sure your Order class has GetMoney/SetMoney or equivalent Price getters/setters)
            double newTotalMoney = pOrd->getMoney() + extraMoney;
            pOrd->setMoney(newTotalMoney);

            // 3. Calculate the new VIP Priority
            // A standard priority equation: (Money / Distance) * Size
            int dist = pOrd->GetDistance();
            int size = pOrd->GetSize();
            int priority = 0;

            if (dist > 0) {
                // Higher money & size = higher priority. Longer distance = lower priority.
                priority = (newTotalMoney / dist) * size;
            }
            else {
                // Fallback in case distance is 0 to avoid division by zero
                priority = newTotalMoney * size;
            }

             pOrd->setType(TYPE_OVG); 

            // 4. Move the order to the VIP Priority Queue (PEND_OVG)
            PEND_OVG.enqueue(pOrd, priority);
        }
        else
        {
            // Not the order we are looking for, put it in the temporary queue
            tempQueue.enqueue(pOrd);
        }
    }

    // 5. Restore all the other normal orders back to their original queue
    while (tempQueue.dequeue(pOrd))
    {
        PEND_OVN.enqueue(pOrd);
    }
}



void Restaurant::AutoPromoteOrders(int currentTime)
{
    LinkedQueue<Order*> tempQueue;
    Order* pOrd = nullptr;

    // Go through all pending Normal Delivery orders (Corrected to PEND_OVN!)
    while (PEND_OVN.dequeue(pOrd))
    {
        // Calculate how long this order has been waiting
        int waitTime = currentTime - pOrd->getArrivalTime();

        // Check if it exceeds the restaurant's auto-promote policy
        if (waitTime > overwait_threshold)
        {
            // --- AUTO-PROMOTE LOGIC ---

            // Calculate priority for the VIP queue 
            // (Same formula as Promote, but NO extra money is added!)
            int dist = pOrd->GetDistance();
            int priority;

            if (dist > 0) {
                priority = (pOrd->getEatingTime() + pOrd->GetSize()) / dist;
            }
            else {
                priority = (pOrd->getEatingTime() + pOrd->GetSize());
            }

            // Update the order's type to the specific VIP Delivery enum
            // (Make sure to use 'SetType' or 'setType' depending on exactly how you wrote it in Order.h)
            pOrd->setType(TYPE_OVG);

            // Move the order to the VIP Priority Queue
            PEND_OVG.enqueue(pOrd, priority);
        }
        else
        {
            // Has not waited long enough, put it in the temporary queue
            tempQueue.enqueue(pOrd);
        }
    }

    // Restore the pending normal delivery orders that were NOT promoted
    while (tempQueue.dequeue(pOrd))
    {
        PEND_OVN.enqueue(pOrd);
    }
}
// =========================================================================
// Lifecycle Phase Transitions
// =========================================================================



void Restaurant::MoveToReady(int currentTime)
{
    Order* pOrd = nullptr;
    int finishTimePri;

    // peek looks at the order closest to finishing
    while (Cooking_Orders.peek(pOrd, finishTimePri)) {

        // If finish time is reached or passed
        if (currentTime >= pOrd->getFinishTime()) {

            // 1. Extract from the cooking queue
            Cooking_Orders.dequeue(pOrd, finishTimePri);

            // 2. Update the Order's internal state
            pOrd->setStatus(READY);
            pOrd->setReadyTime(currentTime);

            // 3. Route to the correct Ready Queue based on specific subtypes
            ORD_TYPE type = pOrd->GetType();

            switch (type) {
            case TYPE_OVC:
            case TYPE_OVG:
            case TYPE_OVN:
                RDY_OV.enqueue(pOrd);
                break;

            case TYPE_ODG:
            case TYPE_ODN:
                RDY_OD.enqueue(pOrd);
                break;

            case TYPE_OT:
                RDY_OT.enqueue(pOrd);
                break;
            }

            // --- 4. RETURN THE CHEF TO THEIR FREE QUEUE ---
            Chef* pChef = pOrd->getChef(); // Get the specific chef who cooked this order

            if (pChef != nullptr) {
                // Assuming your Chef class has a GetType() that returns a char or enum
                // Adjust 'N' and 'S' to match exactly how you defined your chef types!
                if (pChef->GetType() == 'N') {
                    Free_CN.enqueue(pChef);
                }
                else if (pChef->GetType() == 'S') {
                    Free_CS.enqueue(pChef);
                }

                // Optional but highly recommended: un-link the chef so the order knows it's no longer being cooked
                pOrd->setChef(nullptr);
            }

        }
        else {
            // Since the priority queue is sorted by finish time, 
            // if the top one isn't done cooking, nobody else is either!
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
void Restaurant::AssignChefToOrder(int currentTime)
{
    Order* pOrd = nullptr;
    Chef* pChef = nullptr;
    int priority;

    // --- 1. HANDLE GRILLED/SPECIALTY ORDERS FIRST ---
    // Try to assign OVG (VIP Grilled) and ODG (Dine-in Grilled) to CS Chefs
    while (!Free_CS.isEmpty()) {
        if (PEND_OVG.dequeue(pOrd, priority)) {
            Free_CS.dequeue(pChef);
            pOrd->setStatus(COOK);
            pOrd->setChef(pChef); // Link chef to order
            pOrd->SetServiceStartTime(currentTime);
            pChef->AssignOrder(pOrd, currentTime);
            Cooking_Orders.enqueue(pOrd, -pOrd->getFinishTime());
        }
        else if (PEND_ODG.dequeue(pOrd)) {
            Free_CS.dequeue(pChef);
            pOrd->setStatus(COOK);
            pOrd->setChef(pChef);
            pOrd->SetServiceStartTime(currentTime);
            pChef->AssignOrder(pOrd, currentTime);
            Cooking_Orders.enqueue(pOrd, -pOrd->getFinishTime());
        }
        else break; // No more grilled orders for CS
    }

    // --- 2. HANDLE NORMAL/COLD/TAKEAWAY ORDERS ---
    while (!Free_CN.isEmpty()) {
        // Priority: Delivery Normal -> Takeaway -> Dine-in Normal
        if (PEND_OVN.dequeue(pOrd)) {
            Free_CN.dequeue(pChef);
        }
        else if (PEND_OT.dequeue(pOrd)) {
            Free_CN.dequeue(pChef);
        }
        else if (PEND_ODN.dequeue(pOrd)) {
            Free_CN.dequeue(pChef);
        }
        else break; // No more orders for Normal Chefs

        pOrd->setStatus(COOK);
        pOrd->setChef(pChef);
        pOrd->SetServiceStartTime(currentTime);
        pChef->AssignOrder(pOrd, currentTime);
        Cooking_Orders.enqueue(pOrd, -pOrd->getFinishTime());
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
                // Remove the table from the busy list
                Table* tempT = nullptr;
                int tempPri = 0; // Variable to hold the priority
                priQueue<Table*> tempBusy; // Assuming tempBusy is also a priQueue

                // Pass BOTH the table pointer and the priority integer
                while (Busy_No_Sharable.dequeue(tempT, tempPri)) {
                    if (tempT != assignedTable) {
                        // Keep the ones that are STILL busy, using their original priority
                        tempBusy.enqueue(tempT, tempPri);
                    }
                }

                // Restore the busy list
                while (tempBusy.dequeue(tempT, tempPri)) {
                    Busy_No_Sharable.enqueue(tempT, tempPri);
                }
            }
        }
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
void Restaurant::HandleOVGOverwait(int currentTime) {
    const int MAX_WAIT = 15;
    Order* pOrd = nullptr;
    LinkedQueue<Order*> tempQ;

    // Scan the Ready Delivery Queue
    while (RDY_OV.dequeue(pOrd)) {
        // If it's a Vegan Grilled order and it's been sitting too long
        if (pOrd->GetType() == TYPE_OVG && (currentTime - pOrd->getReadyTime()) > MAX_WAIT) {
            pOrd->setUrgent(true); // Internal flag for Member 1 to see
        }
        tempQ.enqueue(pOrd);
    }
    // Put them back
    while (tempQ.dequeue(pOrd)) RDY_OV.enqueue(pOrd);
}

    


void Restaurant::UpdateMaintenanceList(int currentTime)
{
    Scooter* pScoot = nullptr;
    LinkedQueue<Scooter*> tempMaint;

    // We use a temporary queue to only release scooters whose time is up
    while (Maint_Scooters.dequeue(pScoot))
    {
        // Check if maintenance period (main_dur) has passed
        if (currentTime >= pScoot->getMaintenanceFinishTime())
        {
            pScoot->resetTrips();
            pScoot->SetAvailable(true);
            Free_Scooters.enqueue(pScoot, pScoot->GetSpeed());
        }
        else
        {
            tempMaint.enqueue(pScoot);
        }
    }

    // Put those still being fixed back into the maintenance queue
    while (tempMaint.dequeue(pScoot))
    {
        Maint_Scooters.enqueue(pScoot);
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
             act->Execute(this); 
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
    // PHASE 1: Process incoming actions (Arrivals, Cancels, Promotes)
    ExecuteActions(t);
    AutoPromoteOrders(t);

    // PHASE 2: Cleanup (Finish what was being served/cooked)
    UpdateServiceStatus(t);       // Deliveries finished?
    CheckFinishedDineInOrders(t); // Table orders finished?
    MoveToReady(t);               // Cooking finished? (Freess Chefs)
    UpdateMaintenanceList(t);     // Repairs finished? (Frees Scooters)

    // PHASE 3: Assignments (New starts)
    AssignChefToOrder(t);         // Pend -> Cooking
    FinalizeTakeawayOrders(t);    // Ready Takeaway -> Finish
    AssignOrdersToTables(t);      // Ready Dine-in -> Tables
    AssignOrdersToScooters(t);    // Ready Delivery -> Scooters

    // PHASE 4: Handle Breakdowns (Random events)
    HandleScooterBreakdowns(t);
}

bool Restaurant::IsFinished()
{
    // The simulation is finished ONLY if ALL queues are completely empty
    return (PEND_ODN.isEmpty() && PEND_OVG.isEmpty() && PEND_OT.isEmpty() &&
        RDY_OD.isEmpty() && RDY_OT.isEmpty() && RDY_OV.isEmpty() &&
        Actions.isEmpty() && InServ_Orders.isEmpty() && Cooking_Orders.isEmpty());
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

void Restaurant::LoadInputFile(const string& filename) {
    ifstream inFile(filename);

    if (!inFile.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    // 1. Read Chefs Information
    int cn_count, cs_count;
    inFile >> cn_count >> cs_count;

    int cn_speed, cs_speed;
    inFile >> cn_speed >> cs_speed;

    int chefID = 1;
    for (int i = 0; i < cn_count; i++) {
        Free_CN.enqueue(new Chef(chefID++, CN, cn_speed));
    }
    for (int i = 0; i < cs_count; i++) {
        Free_CS.enqueue(new Chef(chefID++, CS, cs_speed));
    }

    // 2. Read Scooters Information
    int s_count, s_speed;
    inFile >> s_count >> s_speed;

    int main_ords, main_dur;
    inFile >> main_ords >> main_dur;

    // NOTE: To fix the instant-maintenance bug, you should store main_dur in your Restaurant!
    // Example: this->maintenance_duration = main_dur; 

    int scooterID = 1;
    for (int i = 0; i < s_count; i++) {
        Free_Scooters.enqueue(new Scooter(scooterID++, s_speed, false), s_speed);
    }

    // 3. Read Tables Information
    int total_tables;
    inFile >> total_tables;

    int tables_read = 0;
    int tableID = 1;
    while (tables_read < total_tables) {
        int table_count, capacity;
        inFile >> table_count >> capacity;

        for (int i = 0; i < table_count; i++) {
            // NOTE: Make sure this line is uncommented and matches your table structure!
            // Free_Tables.enqueue(new Table(tableID++, capacity), capacity); 
        }
        tables_read += table_count;
    }

    // 4. Read Threshold
    inFile >> overwait_threshold;

    // 5. Read Actions
    int num_actions;
    inFile >> num_actions;

    for (int i = 0; i < num_actions; i++) {
        char action_letter;
        inFile >> action_letter;

        //  ADD LOGIC ---
        if (action_letter == 'Q') {
            string typ;
            int tq, id, size;
            double price;
            inFile >> typ >> tq >> id >> size >> price;

            // 1. Convert the string 'typ' from the file into your EXACT enum
            ORD_TYPE orderType;

            if (typ == "ODG") {
                orderType = TYPE_ODG;
            }
            else if (typ == "ODN") {
                orderType = TYPE_ODN;
            }
            else if (typ == "OT") {
                orderType = TYPE_OT;
            }
            else if (typ == "OVC") {
                orderType = TYPE_OVC;
            }
            else if (typ == "OVG") {
                orderType = TYPE_OVG;
            }
            else if (typ == "OVN") {
                orderType = TYPE_OVN;
            }

            // 2. Read the extra data from the file, but only pass the 5 variables your constructor allows
            if (typ == "ODG" || typ == "ODN") {
                int seats, duration;
                char canShare;
                inFile >> seats >> duration >> canShare;
                bool shareable = (canShare == 'Y' || canShare == 'y');

                Actions.enqueue(new ArrivalAction(tq, id, orderType, size, price));
            }
            else if (typ == "OVC" || typ == "OVG" || typ == "OVN") {
                double distance;
                inFile >> distance;

                Actions.enqueue(new ArrivalAction(tq, id, orderType, size, price));
            }
            else if (typ == "OT") {
                Actions.enqueue(new ArrivalAction(tq, id, orderType, size, price));
            }
        }
        else if (action_letter == 'X') {
            //  CANCELLATION LOGIC ---
            int tq, id;
            inFile >> tq >> id;

             Actions.enqueue(new CancelAction(tq, id));
        }
        else if (action_letter == 'P') {
            //  PROMOTION LOGIC ---
            int tq, id;
            double extraMoney;
            inFile >> tq >> id >> extraMoney;

             Actions.enqueue(new PromoteAction(tq, id, extraMoney));
        }
    }

    inFile.close();
}

void Restaurant::ExecuteSimulation(UI* pUI, PROG_MODE mode) {
    int currentTime = 1;

    while (!IsFinished()) {
        // ---------------------------------------------------------
        // PHASE 1: External Events & Queue Management
        // ---------------------------------------------------------

        // 1. Execute actions from the input file (Arrival, Cancel, Promote)
        ExecuteActions(currentTime);

        // 2. Auto promote Normal Delivery (ODN) to VIP (OVG) if wait > threshold
        AutoPromoteOrders(currentTime);

        // ---------------------------------------------------------
        // PHASE 2: Finish Services & Free Resources FIRST
        // ---------------------------------------------------------

        // 3. Finish deliveries and free up scooters
        UpdateServiceStatus(currentTime);

        // 4. Finish dine-in orders and free up tables (Member 3)
        CheckFinishedDineInOrders(currentTime);

        // ---------------------------------------------------------
        // PHASE 3: Kitchen & Maintenance Logic
        // ---------------------------------------------------------

        // 5. Check if returning scooters need to go to maintenance
        MoveScooterToMaintenance();

        // 6. Return fixed scooters from maintenance to the available pool
        UpdateMaintenanceList(currentTime);

        // 7. Move finished food from Cooking to Ready Queues, freeing Chefs
        MoveToReady(currentTime);

        // ---------------------------------------------------------
        // PHASE 4: Assignments & Instant Finishes
        // ---------------------------------------------------------

        // 8. Takeaway orders finish immediately once cooked (no tables/scooters needed)
        FinalizeTakeawayOrders(currentTime);

        // 9. Check if any Ready VIP orders are waiting too long (Bonus)
        HandleOVGOverwait(currentTime);

        // 10. Assign available resources to waiting orders
        AssignChefToOrder(currentTime);      // Pending -> Kitchen
        AssignOrdersToTables(currentTime);   // Ready Dine-in -> Tables
        AssignOrdersToScooters(currentTime); // Ready Delivery -> Scooters

        // 11. Random scooter breakdowns (Bonus)
        HandleScooterBreakdowns(currentTime);

        // ---------------------------------------------------------
        // PHASE 5: User Interface
        // ---------------------------------------------------------

        if (mode == INTERACTIVE) {
            pUI->PrintAll(*this, currentTime);
            cin.get(); // Wait for user to press Enter to move to next timestep
        }

        currentTime++;
    }

    // Always generate the final report (and output file) when simulation ends
    GenerateFinalReport();
}
void Restaurant::HandleScooterBreakdowns(int currentTime) {
    priQueue<Order*> tempInServ;
    Order* pOrd = nullptr;
    int priority;

    while (InServ_Orders.dequeue(pOrd, priority)) {
        Scooter* s = pOrd->getScooter();

        if (s != nullptr && s->GetTrips() > 3 && (rand() % 100 < 5)) {
            Maint_Scooters.enqueue(s);

            Scooter* newScooter = nullptr;
            int sPri;
            if (Free_Scooters.dequeue(newScooter, sPri)) {
                pOrd->setScooter(newScooter);
                newScooter->incrementTrips();
                int remainingTime = pOrd->GetDistance() / newScooter->GetSpeed();
                pOrd->SetFinishTime(currentTime + remainingTime);

                tempInServ.enqueue(pOrd, -pOrd->getFinishTime());
            }
            else {
                pOrd->setScooter(nullptr);
                RDY_OD.enqueue(pOrd);
            }
        }
        else {
            tempInServ.enqueue(pOrd, priority);
        }
    }

    while (tempInServ.dequeue(pOrd, priority)) {
        InServ_Orders.enqueue(pOrd, priority);
    }
}

//Destructor
Restaurant::~Restaurant()
{
    Order* pOrd = nullptr;
    Chef* pChef = nullptr;
    Scooter* pScoot = nullptr;
    Table* pTable = nullptr;
    Action* pAct = nullptr;
    int priority;

    // 1. Clear all Pending Orders
    while (PEND_OVG.dequeue(pOrd, priority)) delete pOrd;
    while (PEND_OVN.dequeue(pOrd))           delete pOrd;
    while (PEND_ODG.dequeue(pOrd))           delete pOrd;
    while (PEND_ODN.dequeue(pOrd))           delete pOrd;
    while (PEND_OT.dequeue(pOrd))            delete pOrd;

    // 2. Clear Cooking and Ready Queues
    while (Cooking_Orders.dequeue(pOrd, priority)) delete pOrd;
    while (RDY_OV.dequeue(pOrd))                   delete pOrd;
    while (RDY_OD.dequeue(pOrd))                   delete pOrd;
    while (RDY_OT.dequeue(pOrd))                   delete pOrd;

    // 3. Clear In-Service and Finished Orders
    while (InServ_Orders.dequeue(pOrd, priority))  delete pOrd;
    while (Finished_orders.pop(pOrd))              delete pOrd;
    while (Cancelled_orders.dequeue(pOrd))         delete pOrd;

    // 4. Clear Chefs
    while (Free_CN.dequeue(pChef)) delete pChef;
    while (Free_CS.dequeue(pChef)) delete pChef;

    // 5. Clear Scooters
    while (Free_Scooters.dequeue(pScoot, priority)) delete pScoot;
    while (Maint_Scooters.dequeue(pScoot))          delete pScoot;
    while (Back_Scooters.dequeue(pScoot, priority)) delete pScoot;

    // 6. Clear Tables
    // Assuming Free_Tables is a Priority Queue/BST and Busy is a Priority Queue
    while (Free_Tables.dequeue(pTable, priority))        delete pTable;
    while (Busy_No_Sharable.dequeue(pTable, priority))   delete pTable;

    // 7. Clear Actions
    while (Actions.dequeue(pAct)) delete pAct;
}