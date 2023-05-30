#ifndef __PLAN_H__
#define __PLAN_H__

#include "day.h"

class Plan
{
public:
    Plan();

    void printDay( int day );
    void printPlan();

    void cookMealToday();
    void editPlanForDay( int day );

    void loadFromFile( std::string path );
    void saveToFile( std::string path );
    void printKcalHistory();

private:
    Day days[ 7 ];
};

namespace plan
{
    inline Plan week;
}

#endif