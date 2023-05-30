#include "product.h"

Product::Product( std::string name, amount_with_unit AwU, int kcal, Date( date ), int price, bool fav ) :
    _name( name ), _amountWithUnit( AwU ), _kcal( kcal ), _expiryDate( date ), _price( price ), _isFavorite( fav ) {};

Product::Product( std::string name, Date( date ), int price ) :
    _name( name ), _amountWithUnit( { 0,"" } ), _kcal( 0 ), _expiryDate( date ), _price( price ), _isFavorite( true ) {};



std::string Product::getName()
{
    return _name;
}

int Product::getKcal()
{
    return _kcal;
}

Date Product::getDate()
{
    return _expiryDate;
}

int Product::getPrice()
{
    return _price;
}

amount_with_unit Product::getAwU()
{
    return _amountWithUnit;
}

bool Product::getFavorite()
{
    return _isFavorite;
}



void Product::setName( std::string name )
{
    _name = name;
}

void Product::addAmount( float amount )
{
    if( !isInfinite() )
        _amountWithUnit.first += amount;
}

void Product::setAmount( float amount )
{
    _amountWithUnit.first = amount;
}

void Product::setUnit( std::string unit )
{
    _amountWithUnit.second = unit;
}

void Product::consumeAmount( int amount )
{
    if( !isInfinite() )
        _amountWithUnit.first -= amount;
}

void Product::setKcal( int kcal )
{
    _kcal = kcal;
}

void Product::setDate( Date date )
{
    _expiryDate = date;
}

void Product::setPrice( int price )
{
    _price = price;
}

void Product::setFavorite( bool fav )
{
    _isFavorite = fav;
}



bool Product::isHidden()
{
    if( _amountWithUnit.first <= 0 )
    {
        return true;
    }

    return false;
}

bool Product::isInfinite()
{
    for( auto& inf : product::infinite )
    {
        if( _name == inf )
            return true;
    }

    return false;
}

void Product::deleteProduct()
{
    if( isInfinite() )
    {
        pushNotif( "Tego produktu nie mozna usunac" );
        return;
    }

    if( isHidden() )
    {
        pushNotif( "Nie posiadasz tego produktu" );
        return;
    }

    _amountWithUnit.first = 0;
    _expiryDate = { 1,1,1 };

    pushNotif( "Usunieto produkt" );
}



void Product::printInfo( int flags )
{
    if( isInfinite() )
        return;

    if( isHidden() && ( flags & print::HIDDEN ) == 0 )
        return;


    if( !_isFavorite && ( flags & print::FAVORITE ) )
    {
        return;
    }

    std::cout << '\t';

    if( flags & print::NAME )
    {
        std::cout << _name;

        for( int space = _name.length(); space < product::longestName; ++space )
        {
            std::cout << " ";
        }

        std::cout << "   ";
    }

    if( flags & print::AMOUNT )
    {
        float temp = _amountWithUnit.first;
        if( _amountWithUnit.first > 9999999 )
        {
            temp = _amountWithUnit.first;
            _amountWithUnit.first = 9999999;
        }
        std::string amountWithUnit = convertAmountWithUnitToString( amountWithUnitToMacroUnit( _amountWithUnit ) );
        std::cout << amountWithUnit << "   ";

        _amountWithUnit.first = temp;
    }

    if( flags & print::KCAL )
    {
        if( _kcal < 100 )
            std::cout << " ";

        std::cout << _kcal << " kcal   ";
    }

    if( flags & print::DATE )
    {
        if( _amountWithUnit.first == 0 )
            std::cout << "   ----   ";
        else if( daysUntilDate( _expiryDate ) > 5 )
            std::cout << _expiryDate;
        else
            std::cout << whenExpire( _expiryDate );

        std::cout << "   ";
    }

    if( flags & print::PRICE )
    {
        std::cout << priceToString( _price );

        std::cout << "   ";
    }

    if( flags & print::SALE_PRICE )
    {
        bool onSale = false;

        Product* temp = nullptr;

        for( auto& product : product::onSale )
        {
            if( _name == product->getName() )
            {
                temp = product;
                std::cout << priceToString( temp->getPrice() );
                onSale = true;
                break;
            }
        }

        if( onSale )
        {
            std::cout << "   ";

            if( daysUntilDate( temp->getDate() ) > 5 )
                std::cout << _expiryDate;
            else
                std::cout << whenExpire( temp->getDate() );
        }
    }
    
    if( flags & print::IS_FAVORITE )
    {
        if( _isFavorite )
            std::cout << "*";
        else
            std::cout << " ";

        std::cout << "      ";  
    }
    
    std::cout << '\n';
}