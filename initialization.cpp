#include "initialization.h"

void init()
{
    product::base.loadFromFile( "products.txt " );
    product::base.loadProductsOnSale( "on_sale.txt" );
    recipe::base.loadFromFile( "recipes.txt" );
    plan::week.loadFromFile( "plan.txt" );
    plan::kcalHistory.loadFromFile( "kcalHistory.txt" );

    refreshScreen();
}

void close()
{
    refreshScreen();
    drawGoodbye();

    recipe::base.clear();
    product::base.clear();
    ProductBase::clearProductsOnSale();    
}