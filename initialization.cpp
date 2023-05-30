#include "initialization.h"

void init()
{
    product::base.loadFromFile( "products.txt " );
    product::base.loadProductsOnSale( "on_sale.txt" );
    recipe::base.loadFromFile( "recipes.txt" );
    plan::week.loadFromFile( "plan.txt" );

    refreshScreen();
}

void close()
{
    refreshScreen();
    drawGoodbye();

    product::base.saveToFile( "products.txt" );
    recipe::base.saveToFile( "recipes.txt" );
    plan::week.saveToFile( "plan.txt" );

    recipe::base.clear();
    product::base.clear();
    ProductBase::clearProductsOnSale();    
}