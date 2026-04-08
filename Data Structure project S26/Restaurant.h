#pragma once

#include "LinkedQueue.h"
#include "LinkedStack.h"
#include "priQueue.h"
#include "Order.h"
#include "Scooter.h"
#include "Chef.h"
#include "Action.h"
#include"PEND_OVC.h"
#include"Cook_Ords.h"
#include"RDY_OV.h"
#include"Fit_Tables.h"
class Restaurant
{
private:
    int TotalServedCount;
    float TotalWaitTime;
    float TotalServiceTime;
    LinkedQueue<Order*> PEND_ODN ;
    LinkedQueue<Order*> PEND_OT;
    LinkedQueue<Order*> PEND_OVN;
    priQueue<Order*> PEND_OVG;
    LinkedQueue<Order*> RDY_OT;
    LinkedQueue<Order*> RDY_OD;
    RDY_OV RDY_OV;
    PEND_OVC Pend_OVC;
    Cook_Ords Cooking_Orders;
    LinkedQueue<Order*> Cancelled_orders;
    LinkedStack<Order*> Finished_orders;
    LinkedQueue<Chef*> Free_CS;
    LinkedQueue<Chef*> Free_CN;
    priQueue<Order*> InServ_Orders;
    priQueue<Scooter*> Free_Scooters;
    priQueue<Scooter*> Back_Scooters;
    LinkedQueue<Scooter*> Maint_Scooters;
    Fit_Tables Free_Tables;
    Fit_Tables Busy_Sharable;
    Fit_Tables Busy_No_Sharable;
    LinkedQueue<Action*> Actions;
public:
    void AddOrder(Order* o);
    void CancelOrder(int id);
    void PromoteOrder(int id);

    void MoveToReady();
    void FinishOrder();

    void MoveScooterToMaintenance();

    void PrintFinished();
    LinkedStack<Order*>& getFinished();
    void SimulateStep(int t);
    bool IsFinished();
    void AddChef(Chef* c);
    void AssignChefToOrder();
    void AssignOrdersToTables(int currentTime);
    void AssignOrdersToScooters(int currentTime);
    void UpdateServiceStatus(int currentTime);
    void GenerateFinalReport();
    // Action scheduling
    void AddAction(Action* a);
    void ExecuteActions(int time);
};