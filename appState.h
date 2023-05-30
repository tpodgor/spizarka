#ifndef __APP_STATE_H__
#define __APP_STATE_H__

namespace AppState
{
    enum value
    {
        QUIT = 0x1,
        MAIN_MENU = 0x2,

        PRODUCT_MENU = 0x4,
        PRODUCT_SALE = 0x40000,
        PRODUCT_CONSUME = 0x8,
        PRODUCT_ADD = 0x10,
        PRODUCT_EDIT = 0x20,
        PRODUCT_DELETE = 0x40,

        RECIPE_MENU = 0x80,
        RECIPE_VIEW = 0x100,
        RECIPE_COOK = 0x200,
        RECIPE_EDIT = 0x400,
        RECIPE_ADD = 0x800,
        RECIPE_DELETE = 0x1000,

        PLAN_MENU = 0x2000, 
        PLAN_COOK = 0x4000,
        PLAN_EDIT = 0x8000,
        PLAN_VIEW = 0x10000,

        CALC = 0x20000
    };

    inline int currentState = MAIN_MENU;

    inline std::string errorMsg = "";
};

inline void pushNotif( std::string msg )
{
    if( msg != "" )
        AppState::errorMsg += '\t' + msg + '\n';
}

#endif