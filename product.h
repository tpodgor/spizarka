#ifndef __PRODUCT_H__
#define __PRODUCT_H__

#include "utils.h"
#include "draw.h"

namespace productData
{
    enum value
    {
        NAME = 49,
        AMOUNT_WITH_UNIT,
        CALORIES,
        EXPIRY_DATE,
        PRICE,
        IS_FAVORITE
    };
};

class CaloriesHistory
{
public:
    int fortnite[ 14 ];

    void addKcalToday( int amount );
    void print();
};

class Product
{
public:
    Product( std::string name, amount_with_unit AwU, int kcal, Date( date ), int price, bool fav);
    Product( std::string name, Date( date ), int price );

    std::string getName();
    int getKcal();
    Date getDate();
    int getPrice();
    amount_with_unit getAwU();
    bool getFavorite();

    void setName( std::string name );
    void addAmount( float amount );
    void setAmount( float amount );
    void setUnit( std::string unit );
    void consumeAmount( int amount );
    void setKcal( int kcal );
    void setDate( Date date );
    void setPrice( int price );
    void setFavorite( bool fav );

    void deleteProduct();
    bool isHidden();
    bool isInfinite();

    void printInfo( int flags );

private:
    std::string _name = "";
    std::pair<float, std::string> _amountWithUnit;
    int _kcal; // na 100g/ml
    Date _expiryDate;
    int _price; // na 100g/ml
    bool _isFavorite;
};

namespace product
{
    inline std::vector<Product*> onSale;

    inline std::vector<std::string> infinite = { "woda", "sol" };

    inline int longestName = 0;
}

namespace plan
{
    inline CaloriesHistory kcalHistory;
}

#endif