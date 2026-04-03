#include "Action.h"

Action::Action(int t)
{
    time = t;
}

int Action::getTime()
{
    return time;
}

Action::~Action() {}