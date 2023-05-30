#ifndef __DRAW_H__
#define __DRAW_H__

#include <iostream>

#include "appState.h"

namespace print
{
    enum value
    {
        HIDDEN = 0x1,
        NAME = 0x2,
        AMOUNT = 0x4,
        KCAL = 0x8,
        PRICE = 0x10,
        DATE = 0x20,
        FAVORITE = 0x40,
        IS_FAVORITE = 0x80,
        SALE_PRICE = 0x100
    };
};

void refreshScreen();
void drawMainMenu();
void drawGoodbye();

void drawProductsMenu();
void drawProductsEditMenu();

void drawRecipesMenu();
void drawRecipeViewMenu();
void drawRecipesEditMenu();

void drawPlanMenu();
void drawPlanEditMenu();
void drawPlanEditRecipeMenu();

#endif