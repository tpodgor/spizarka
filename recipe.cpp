#include "recipe.h"

Recipe::Recipe( std::string name ) : _name( name ), _totalKcal( NULL ), _totalPrice( NULL ), _description("") {};

Ingredient::Ingredient( Product* product, amount_with_unit AwU ) : _productPtr(product), _AwU(AwU) {};



std::string Recipe::getName()
{
    return _name;
}

int Recipe::getTotalKcal()
{
    return _totalKcal;
}

std::string Recipe::getDescription()
{
    return _description;
}

std::vector<Ingredient>& Recipe::getIngredients()
{
    return _ingredients;
}



void Recipe::setName( std::string name )
{
    _name = name;
}




void Recipe::printInfo()
{
    int charLimit = 0;

    // nazwa
    std::cout << '\t' << _name << "\n\n";
    int kcal = 0;

    // skladniki
    if( _ingredients.begin() != _ingredients.end() )
    {
        std::cout << '\t' << "Skladniki:\n";

        for( auto& product : _ingredients )
        {
            std::cout << '\t' << product._productPtr->getName() << "   ";
            std::cout << convertAmountWithUnitToString( amountWithUnitToMacroUnit( product._AwU ) );
            std::cout << '\n';
        }
    }

    // kalorie
    std::cout << "\t--------------------\n";
    std::cout << '\t' << _totalKcal << " kcal,  ";

    // cena
    std::cout << priceToString( _totalPrice ) << "\n\n";

    // opis
    if( _description != "" )
    {
        std::cout << '\t' << "Opis:\n";
        std::cout << '\t' << _description;
        std::cout << "\n\n";
    }
}



// automatyczna wersja loadFromFile
void Recipe::addProduct( Product* product, amount_with_unit AwU )
{
    _ingredients.emplace_back( product, AwU );
    calcKcalAndPrice();
}



// manualna wersja add/editRecipe
bool Recipe::addProduct()
{
        refreshScreen();
        product::base.printProducts( print::NAME | print::HIDDEN );

    bool success = true;

    try {
        std::string productName = getlineToString( "Podaj nazwe nowego produktu: " );

        if( productName == "" )
        {
            throw std::runtime_error( "Nie podano nazwy" );
        }

        // sprawdz czy produkt juz jest w przepisie
        for( auto& ingredient : _ingredients )
        {
            // jesli jest
            if( ingredient._productPtr->getName() == productName )
            {
                // zapytaj o zmiane ilosci
                if( boolFromCharInput( "\nTen produkt juz jest w przepisie. Zmienic ilosc?  T / N:  \n" ) )
                {
                    amount_with_unit AwU = stringToBaseUnit( cinToString( "Podaj ilosc z jednostka (g/kg/ml/L): " ) );

                    if( AwU.first < 0 )
                    {
                        throw std::runtime_error( "Nieprawidlowa ilosc" );
                    }
                    if( formatUnit( AwU.second ) < 0 || !compareUnits( AwU.second, ingredient._productPtr->getAwU().second ) )
                    {
                        throw std::runtime_error( "Nieprawidlowa jednostka" );
                    }

                    ingredient._AwU.first = AwU.first;
                    pushNotif( "Edytowano ilosc produktu" );
                }
                break;
            }
        }

        auto product = findMemberByName<Product*>( product::base.products, productName );

        if( product == product::base.products.end() )
        {
            throw std::runtime_error( "Nie ma " + productName + " w bazie. Najpierw dodaj ten produkt do bazy." );
        }

        amount_with_unit AwU = stringToBaseUnit( cinToString( "Podaj ilosc z jednostka (g/kg/ml/L): " ) );

        if( AwU.first <= 0 )
        {
            throw std::runtime_error( "Nieprawidlowa ilosc" );
        }
        if( formatUnit( AwU.second ) < 0 || !compareUnits( AwU.second, ( *product )->getAwU().second ) )
        {
            throw std::runtime_error( "Nieprawidlowa jednostka" );
        }

        _ingredients.emplace_back( *product, AwU );
        pushNotif( "Dodano produkt" );
    }

    catch( const std::exception& ex )
    {
        pushNotif( ex.what() );
        success = false;
    }

    calcKcalAndPrice();

    return success;
}



bool Recipe::deleteProduct( std::string name )
{
    bool success = true;

    for( auto ingredient = _ingredients.begin(); ingredient != _ingredients.end(); ++ingredient )
    {
        if( ( *ingredient )._productPtr->getName() == name )
        {
            _ingredients.erase( ingredient );
            pushNotif( "Usunieto produkt" );

            calcKcalAndPrice();

            return success;
        }
    }
    pushNotif( "Nie ma takiego produktu na liscie" );

    return success = false;
}



void Recipe::addDescription( std::string desc )
{
    if( desc == "" && _description == "" )
    {
        pushNotif( "Nie podano opisu" );
    }
    else if( desc == "" && _description != "" )
    {
        pushNotif( "Usunieto opis" );
    }
    else if( desc != "" && _description == "" )
    {
        pushNotif( "Dodano opis" );
    }
    else
        pushNotif( "Zmieniono opis" );

    _description = desc;
}



// success: odejmuje produkty z bazy
// fail: wypisuje ile brakuje i ile max porcji mozesz zrobic
bool Recipe::canBeCooked( float servings )
{   
    bool success = true;

    try {
        if( servings < 0.f || servings > 9999.f )
        {
            throw std::runtime_error( "Nieprawidlowa ilosc porcji" );
        }

        float maxServings = 9999;
        int totalKcal = 0;

        for( auto& ingredient : _ingredients )
        {
            if( ingredient._productPtr->isInfinite() )
                continue;

            // porownuje ilosc wymagana i posiadana
            int balance = ingredient._productPtr->getAwU().first - static_cast<int>( ( ingredient._AwU.first * servings ) );

            if( balance < 0 )
            {
                if( success )
                {
                    pushNotif( "Brakuje ci:" );
                    success = false;
                }

                // jak za malo to pisze o ile
                pushNotif( convertAmountWithUnitToString( amountWithUnitToMacroUnit( { abs( balance ), ingredient._AwU.second } ) )
                    + " produktu: " + ingredient._productPtr->getName() );
            }
            else
            {
                // jak ok to dodaje kalorie
                totalKcal += static_cast<int>( ingredient._productPtr->getKcal() * ingredient._AwU.first * servings / 100 );
            }

            maxServings = std::min( maxServings, ingredient._productPtr->getAwU().first / ingredient._AwU.first );
        }

        if( !success )
        {
            throw std::runtime_error( "Mozesz przyrzadzic max " + std::to_string( maxServings ).substr( 0, 4 ) + " porcji" );
        }

        // odlicza od ilosci produktow
        for( auto& ingredient : _ingredients )
        {
            ingredient._productPtr->consumeAmount( static_cast<int>( ingredient._AwU.first * servings ) );
        }
        pushNotif( "Przygotowales " + std::to_string( servings ).substr( 0, 4 ) + "x " + _name + "! Odliczono produkty" );

        plan::kcalHistory.fortnite[ 0 ] += totalKcal;
        pushNotif( "Suma kalorii: " + std::to_string( totalKcal ) + " kcal" );
    }

    catch( const std::exception& ex )
    {
        pushNotif( ex.what() );
    }

    return success;
}



void Recipe::calcKcalAndPrice()
{
    int totalKcal = 0;
    int totalPrice = 0;

    for( auto& ingredient : _ingredients )
    {
        totalKcal += static_cast<int>( ingredient._productPtr->getKcal() * ingredient._AwU.first / 100 );
        totalPrice += static_cast<int>( ingredient._productPtr->getPrice() * ingredient._AwU.first / 1000 );
    }

    _totalKcal = totalKcal;
    _totalPrice = totalPrice;
}