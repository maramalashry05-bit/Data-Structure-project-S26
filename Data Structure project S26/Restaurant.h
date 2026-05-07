#pragma once
#include "ArrivalAction.h"
#include <fstream>
#include "CancelAction.h"
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
#include "UI.h"
#include "PromoteAction.h"
enum PROG_MODE {
    INTERACTIVE,
    SILENT
};
class Restaurant
{
    friend class UI;
private:
    int TotalServedCount;
    float TotalWaitTime;
    float TotalServiceTime;
    int overwait_threshold;
    LinkedQueue<Order*> PEND_ODN ;
    LinkedQueue<Order*> PEND_ODG;

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
    void PromoteOrder(int id,double m);
   

    void AutoPromoteOrders(int currentTime);

    void MoveToReady(int currentTime);
    void FinishOrder(int currentTime);

    void MoveScooterToMaintenance();

    void PrintFinished();
    LinkedStack<Order*>& getFinished();
    void SimulateStep(int t);
    bool IsFinished();
    void AddChef(Chef* c);
    void AssignOrdersToScooters(int currentTime);
    void UpdateServiceStatus(int currentTime);
    void GenerateFinalReport();
    // Action scheduling
    void AddAction(Action* a);
    void ExecuteActions(int time);
    // Add these inside class Restaurant in Restaurant.h
    void AssignOrdersToTables(int currentTime);       // Feature 10
    void FinalizeTakeawayOrders(int currentTime);     // Feature 12
    void CheckFinishedDineInOrders(int currentTime);  // Feature 7
    void HandleOVGOverwait(int currentTime);           // BONUS
     void AssignChefToOrder(int currentTime);     // feature 8 
   void UpdateMaintenanceList(int currentTime); // feature 5
   void HandleComboAssignment(Order* comboOrd, int currentTime); // bonus
   void LoadInputFile(const string& filename);
   void ExecuteSimulation(UI* pUI, PROG_MODE mode);
   void HandleScooterBreakdowns(int currentTime);

   ~Restaurant();
   
};