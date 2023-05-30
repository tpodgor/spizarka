#include "events.h"

// appState::MAIN_MENU
void events_mainMenu( )
{
    product::base.notifyAboutProductsThatAreFewOrExpireSoon();
    refreshScreen();
    drawMainMenu();

    char userInput = _getch();

    switch( userInput )
    {
        case '1':
        {
            AppState::currentState = AppState::PRODUCT_MENU ;
            break;
        }
        case '2':
        {
            AppState::currentState = AppState::RECIPE_MENU;
            break;
        }
        case '3':
        {
            AppState::currentState = AppState::PLAN_MENU;
            break;
        }
        case '4':
        {
            AppState::currentState = AppState::PRODUCT_SALE;
            break;
        }
        case 27:
        {
            AppState::currentState = AppState::QUIT;
            break;
        }
    }
}



