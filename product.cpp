#include "product.h"

void CaloriesHistory::addKcalToday( int amount )
{
    fortnite[ 0 ] += amount;
}

void CaloriesHistory::print()
{
    std::cout << "\tSuma kalorii dla ostatnich 14 dni:\n\n";

    std::cout << "\tDzisiaj: " << fortnite[ 0 ] << " kcal\n\n";

    std::cout << "\tPoprzednie dni:\n";
    for( int day = 1; day < 14; ++day )
    {
        std::cout << "\t         " << fortnite[ day ] << " kcal\n";
    }
}

void CaloriesHistory::loadFromFile( std::string path )
{
    // zaladuj plik
    std::vector<std::vector<std::string>> file = loadFromFileIntoVector( path );

    for( int seg = 0; seg < file[ 1 ].size(); ++seg )
    {
        plan::kcalHistory.fortnite[ seg ] = stoi( file[ 1 ][ seg ] );
    }

    // TODO przesunac save'y o tyle dni
    int dayOffset = abs( daysUntilDate( { stoi( file[ 0 ][ 0 ] ), stoi( file[ 0 ][ 1 ] ) ,stoi( file[ 0 ][ 2 ] ) } ) );

    if( dayOffset )
    {
        for( int day = 13; day >= 1; --day )
        {
            plan::kcalHistory.fortnite[ day ] = plan::kcalHistory.fortnite[ day - 1 ];
        }

        plan::kcalHistory.fortnite[ 0 ] = 0;
    }
}

void CaloriesHistory::saveToFile( std::string path )
{
    // usun dotychczasowe dane
    std::ofstream outputFile{ path };
    outputFile.close();

    // zapisz date
    std::vector<std::string> today = { std::to_string( getDate( 'd' ) ) ,std::to_string( getDate( 'm' ) ) ,std::to_string( getDate( 'y' ) ) };
    saveObjectInFile( today, path );

    // zapisz
    std::vector<std::string> kcalHistory;

    for( int day = 0; day < 14; ++day )
    {
        kcalHistory.push_back( std::to_string( plan::kcalHistory.fortnite[ day ] ) );
    }

    saveObjectInFile( kcalHistory, path );
}



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

    plan::kcalHistory.addKcalToday( getKcal() * amount / 100 );
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

void Product::hideProduct()
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