#ifndef __HANDLE_EVENTS_H__
#define __HANDLE_EVENTS_H__

#include "plan.h"

void events_mainMenu();

void events_productMenu();
void events_productConsume();
void events_productSale();
void events_productAdd();
void events_productEdit();
void events_productDelete();

void events_recipeMenu();
void events_recipeView();
void events_recipeCook( Recipe* recipe );
void events_recipeEdit( Recipe* recipe );
void events_recipeAdd();
void events_recipeDelete();

void events_planMenu();
void events_planCook();
void events_planEdit();
void events_planHistory();

inline void handleEvents()
{
    switch( AppState::currentState )
    {
        case AppState::MAIN_MENU:
        {
            events_mainMenu();
            break;
        }
        case AppState::PRODUCT_MENU:
        {
            events_productMenu();
            break;
        }
        case AppState::PRODUCT_CONSUME:
        {
            events_productConsume();
            break;
        }
        case AppState::PRODUCT_SALE:
        {
            events_productSale();
            break;
        }
        case AppState::PRODUCT_ADD:
        {
            events_productAdd();
            break;
        }
        case AppState::PRODUCT_EDIT:
        {
            events_productEdit();
            break;
        }
        case AppState::PRODUCT_DELETE:
        {
            events_productDelete();
            break;
        }
        case AppState::RECIPE_MENU:
        {
            events_recipeMenu();
            break;
        }
        case AppState::RECIPE_VIEW:
        {
            events_recipeView();
            break;
        }
        case AppState::RECIPE_ADD:
        {
            events_recipeAdd();
            break;
        }
        case AppState::RECIPE_DELETE:
        {
            events_recipeDelete();
            break;
        }
        case AppState::PLAN_MENU:
        {
            events_planMenu();
            break;
        }
        case AppState::PLAN_COOK:
        {
            events_planCook();
            break;
        }
        case AppState::PLAN_EDIT:
        {
            events_planEdit();
            break;
        }
        case AppState::PLAN_VIEW:
        {
            events_planHistory();
            break;
        }
        default:
            break;
    }
}

#endif