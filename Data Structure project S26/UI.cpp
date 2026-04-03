#include "UI.h"

void UI::PrintAll(Restaurant& r, int timestep)
{
    cout << "\nTime Step: " << timestep << endl;
    cout << "---------------------------------\n";

    // ===== Pending Orders =====
    cout << r.getVIP().getCount() << " VIP Orders: ";
    r.getVIP().printIDs();

    cout << r.getNormal().getCount() << " Normal Orders: ";
    r.getNormal().printIDs();

    cout << r.getCold().getCount() << " Cold Orders: ";
    r.getCold().printIDs();

    cout << "\n---------------------------------\n";

    // ===== Ready Orders =====
    cout << "Ready Orders (" << r.getReady().getCount() << "): ";
    r.getReady().printIDs();

    cout << "\n---------------------------------\n";

    // ===== Finished Orders =====
    cout << "Finished Orders (descending): ";
    r.getFinished().printIDsReverse();

    cout << endl;
}