#pragma once
#include <iostream>
using namespace std;

class Table
{
private:
    int ID;
    bool available;

public:
    Table(int id);

    bool IsAvailable() const;
    void SetAvailable(bool a);

    void Print() const;
};