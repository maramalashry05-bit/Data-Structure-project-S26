#include "Restaurant.h"
#include "UI.h"

int main()
{
    Restaurant r;
    UI ui;

    r.AddOrder(new Order(1, TYPE_OD, 2, 100));
    r.AddOrder(new Order(2, TYPE_OV, 3, 200));
    r.AddOrder(new Order(3, TYPE_OT, 1, 50));

    for (int t = 1; t <= 3; t++)
    {
        r.MoveToReady();
        r.FinishOrder();

        ui.PrintAll(r, t);
    }

    return 0;
}