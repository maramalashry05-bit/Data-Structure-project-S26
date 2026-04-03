#include "Table.h"
Table::Table(int id)
{
    ID = id;
    available = true;
}
bool Table::IsAvailable() const
{
    return available;
}
void Table::SetAvailable(bool a)
{
    available = a;
}
void Table::Print() const
{
    cout << "Table ID: " << ID << endl;
}