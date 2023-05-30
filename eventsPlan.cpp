#include "events.h"

void events_planMenu()
{
    refreshScreen();
    plan::week.printPlan();
    drawPlanMenu();

    char userInput = _getch();

    switch( userInput )
    {
        case '1':
        {
            AppState::currentState = AppState::PLAN_COOK;
            break;
        }
        case '2':
        {
            AppState::currentState = AppState::PLAN_EDIT;
            break;
        }
        case '3':
        {
            AppState::currentState = AppState::PLAN_VIEW;
            break;
        }
        case 27:
        {
            AppState::currentState = AppState::MAIN_MENU;
            break;
        }
    }
}

void events_planCook()
{
    refreshScreen();
    plan::week.printDay(plan::today);

    plan::week.cookMealToday();

    AppState::currentState = AppState::PLAN_MENU;
}

void events_planEdit()
{
    refreshScreen();
    plan::week.printPlan();
    drawPlanEditMenu();

    int weekday = weekday::DEFAULT;

    char userInput = _getch();
    switch( userInput )
    {
        case '1':
        {
            weekday = weekday::MON;
            break;
        }
        case '2':
        {
            weekday = weekday::TUE;
            break;
        }
        case '3':
        {
            weekday = weekday::WED;
            break;
        }
        case '4':
        {
            weekday = weekday::THU;
            break;
        }
        case '5':
        {
            weekday = weekday::FRI;
            break;
        }
        case '6':
        {
            weekday = weekday::SAT;
            break;
        }
        case '7':
        {
            weekday = weekday::SUN;
            break;
        }
        case 27:
        {
            break;
        }
    }

    if( weekday != weekday::DEFAULT )
    {
        plan::week.editPlanForDay( weekday );
    }

    AppState::currentState = AppState::PLAN_MENU;
}

void events_planHistory()
{
    refreshScreen();
    plan::week.printKcalHistory();

    std::cout << "\n\tESC. Powrot";
    while( char userInput = _getch() )
    {
        if( userInput == 27 )
            break;
    }

    AppState::currentState = AppState::PLAN_MENU;
}