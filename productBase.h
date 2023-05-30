#ifndef __PRODUCT_BASE_H__
#define __PRODUCT_BASE_H__

#include "product.h"

struct ProductBase
{
    ProductBase();

    void printProducts( int flags );

    void loadFromFile( std::string path );
    void loadProductsOnSale( std::string path );
    void saveToFile( std::string path );
    void clear();
    static void clearProductsOnSale();

    void notifyAboutProductsThatAreFewOrExpireSoon();

    void findLongestName();
    void isNewLongestName( std::string name );


    std::vector<Product*> products;
};

namespace product
{
    inline ProductBase base;
}

#endif