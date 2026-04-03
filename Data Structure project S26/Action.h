#pragma once

class Restaurant;

class Action
{
protected:
    int time;

public:
    Action(int t);

    int getTime();

    virtual void Execute(Restaurant* pRest) = 0;

    virtual ~Action();
};