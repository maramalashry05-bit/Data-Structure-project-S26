#include "UI.h"
#include "Table.h"

void UI::PrintAll(Restaurant& r, int timestep)
{
  // there was 4 functions i have to add first to make UI 
    // 1. Header and Actions
    cout << "Current Timestep:" << timestep << endl;
    cout << "============== Actions List ==============" << endl;
    // Note: Only printing the count as per DS restrictions, 
    // unless you implemented a peek function for the first 10 actions.
    cout << r.Actions.getCount() << " actions remaining." << endl;

    // 2. Pending Orders so we here adeed the 2 function getCount and printIDs ,  we made them in the LinkedQueue 
    cout << "------------- Pending Orders IDs -------------" << endl;
    cout << r.PEND_OVG.getCount() << " OVG: "; r.PEND_OVG.printIDs(); cout << endl;
    cout << r.PEND_OVN.getCount() << " OVN: "; r.PEND_OVN.printIDs(); cout << endl;
    cout << r.PEND_ODN.getCount() << " ODN: "; r.PEND_ODN.printIDs(); cout << endl;
    cout << r.PEND_OT.getCount() << " OT: ";  r.PEND_OT.printIDs();  cout << endl;

    // 3. Available Chefs
    cout << "------------- Available chefs IDs -------------" << endl;
    cout << r.Free_CS.getCount() << " CS: "; r.Free_CS.printIDs(); cout << endl;
    cout << r.Free_CN.getCount() << " CN: "; r.Free_CN.printIDs(); cout << endl;

    // 4. Cooking Orders  we here added the function printIDsWithCompanions to know the order id and the resources 
    cout << "------------- Cooking orders [Orders ID, chef ID] -------------" << endl;
    // This requires your Cooking_Orders queue to have a specialized print 
    // that shows both the Order ID and the assigned Chef ID.
    r.Cooking_Orders.printIDsWithCompanions();

    // 5. Ready Orders
    cout << "------------- Ready Orders IDs -------------" << endl;
    cout << r.RDY_OV.getCount() << " OVG: "; r.RDY_OV.printIDs(); cout << endl;
    cout << r.RDY_OD.getCount() << " ODN: "; r.RDY_OD.printIDs(); cout << endl;
    cout << r.RDY_OT.getCount() << " OT: ";  r.RDY_OT.printIDs();  cout << endl;

    // 6. Available Scooters and Tables
    cout << "------------- Available scooters IDs -------------" << endl;
    cout << r.Free_Scooters.getCount() << " Scooters: "; r.Free_Scooters.printIDs(); cout << endl;

    cout << "------------- Available tables [ID, capacity, free seats] -------------" << endl;
    r.Free_Tables.printTableDetails();

    // 7. In-Service Orders (Member 4/3 Interface)
    cout << "------------- In-Service orders [order ID, scooter/Table ID] -------------" << endl;
    r.InServ_Orders.printIDsWithCompanions();

    // 8. Maintenance and Returns
    cout << "------------- In-maintenance scooters IDs -------------" << endl;
    cout << r.Maint_Scooters.getCount() << " scooters: "; r.Maint_Scooters.printIDs(); cout << endl;

    cout << "------------- Scooters Back to Restaurant IDs -------------" << endl;
    cout << r.Back_Scooters.getCount() << " scooters: "; r.Back_Scooters.printIDs(); cout << endl;

    // 9. Final Stats
    cout << "------------- Cancelled Orders IDs -------------" << endl;
    cout << r.Cancelled_orders.getCount() << " cancelled: "; r.Cancelled_orders.printIDs(); cout << endl;

    cout << "------------- Finished orders IDs-------------" << endl;
    cout << r.getFinished().getCount() << " Orders: ";
    r.getFinished().printIDsReverse(); // As per requirement: descending order of finish time

    cout << "\nPRESS ANY KEY TO MOVE TO NEXT STEP !" << endl;
}