#include "Table.h"
Table::Table()
{

}
Table::Table(int id)
{
    ID = id;
    isAvailable = true;
}
bool Table::IsAvailable() const
{
    return isAvailable;
}
void Table::SetAvailable(bool a)
{
    isAvailable = a;
}
void Table::Print() const
{
    cout << "Table ID: " << ID << endl;
}
int Table::getCapacity() const
{
    return Capacity;
}