#include "events.h"

void events_productMenu()
{
    refreshScreen();
    product::base.printProducts( print::NAME | print::AMOUNT | print::DATE | print::KCAL | print::PRICE | print::IS_FAVORITE );
    drawProductsMenu();

    char userInput = _getch();

    switch( userInput )
    {
        case '1':
        {
            AppState::currentState = AppState::PRODUCT_CONSUME;
            break;
        }
        case '2':
        {
            AppState::currentState = AppState::PRODUCT_ADD;
            break;
        }
        case '3':
        {
            AppState::currentState = AppState::PRODUCT_EDIT;
            break;
        }
        case '4':
        {
            AppState::currentState = AppState::PRODUCT_DELETE;
            break;
        }
        case 27:
        {
            AppState::currentState = AppState::MAIN_MENU;
            break;
        }
    }
}

void events_productConsume()
{
    refreshScreen();
    product::base.printProducts( print::NAME | print::AMOUNT | print::KCAL );

    try {
        // nazwa
        std::string name = getlineToString( "Podaj nazwe produktu " );

        if( name == "" )
        {
            throw std::runtime_error( "Nie podano nazwy" ); 
        }

        auto product = findVectorMemberByKey<Product*>( product::base.products, name );

        if( product == product::base.products.end() )
        {
            throw std::runtime_error( "Nie ma takiego produktu w bazie" );
        }
        if( ( *product )->getAwU().first <= 0 )
        {
            throw std::runtime_error( "Nie posiadasz tego produktu" );
        }

        // ilosc
        amount_with_unit AwU = stringToBaseUnit( cinToString( "Podaj ilosc z jednostka (g/kg/ml/L): " ) );

        if( AwU.first < 0 )
        {
            throw std::runtime_error( "Nieprawidlowa ilosc" );
        }
        if( !compareUnits( AwU.second, ( *product )->getAwU().second ) )
        {
            throw std::runtime_error( "Nieprawidlowa jednostka" );
        }
        if( !( *product )->isInfinite() )
        {
            if( AwU.first > ( *product )->getAwU().first )
            {
                throw std::runtime_error( "Nie posiadasz wystarczajacej ilosci produktu" );
            }
        }

        // odlicz
        ( *product )->consumeAmount( AwU.first );
        pushNotif( "Odliczono" + convertAmountWithUnitToString( amountWithUnitToMacroUnit( AwU ) ) + " produktu" );

        // dolicz kalorie do historii
        product::kcalLastFortnite[ 0 ] += ( *product )->getKcal() * AwU.first / 100;
    }

    catch( const std::exception& ex )
    {
        pushNotif( ex.what() );
    }

    AppState::currentState = AppState::PRODUCT_MENU;
}



void events_productAdd()
{
    refreshScreen();
    product::base.printProducts( print::NAME | print::AMOUNT | print::DATE | print::KCAL | print::PRICE | print::IS_FAVORITE | print::HIDDEN );

    try {

        // nazwa
        std::string name = getlineToString( "Podaj nazwe produktu " );

        if( name == "" )
        {
            throw std::runtime_error( "Nie podano nazwy" );
        }

        // jesli juz jest w bazie
        auto product = findVectorMemberByKey<Product*>( product::base.products, name );

        if( product != product::base.products.end() )
        {
            pushNotif( "Ten produkt juz jest w bazie" );
            refreshScreen();
            ( *product )->printInfo( print::NAME | print::AMOUNT | print::DATE | print::KCAL | print::PRICE | print::IS_FAVORITE | print::HIDDEN );

            amount_with_unit AwU = stringToBaseUnit( cinToString( "\nDodaj produkt (ilosc z jednostka (g/kg/ml/L) ): " ) );

            if( AwU.first < 0 )
            {
                throw std::runtime_error( "Nieprawidlowa ilosc produktu" );
            }
            if( formatUnit( AwU.second ) < 0 || !compareUnits( AwU.second, ( *product )->getAwU().second ) )
            {
                throw std::runtime_error( "Nieprawidlowa jednostka" );
            }

            Date expiryDate = cinToDate( "Podaj nowa date waznosci\n" );

            if( !isDateValid( expiryDate ) )
            {
                throw std::runtime_error( "Nieprawidlowa data" );
            }

            ( *product )->addAmount( AwU.first );
            ( *product )->setDate( expiryDate );
        }

        else
        {
            // ilosc z jednostka
            amount_with_unit AwU = stringToBaseUnit( cinToString( "\nPodaj ilosc z jednostka (g/kg/ml/L): " ) );

            if( AwU.first < 0 )
            {
                throw std::runtime_error( "Nieprawidlowa ilosc produktu" );
            }
            if( formatUnit( AwU.second ) < 0 )
            {
                throw std::runtime_error( "Nieprawidlowa jednostka" );
            }

            // kalorie
            int kcal = cinToInt( "\nPodaj ilosc kalorii na 100g/ml produktu: " );

            if( !isKcalValid( kcal ) )
            {
                throw std::runtime_error( "Nierealistyczna wartosc kaloryczna" );
            }

            // data
            Date expiryDate = cinToDate( "Podaj date waznosci\n" );

            if( !isDateValid( expiryDate ) )
            {
                throw std::runtime_error( "Nieprawidlowa data" );
            }

            // cena
            int price = static_cast<int>( cinToFloat( "Podaj cene na 100g/ml produktu: " ) * 100 );

            if( price < 0 || price > 99999 )
            {
                throw std::runtime_error( "Nieprawidlowa wartosc" );
            }

            // ulub
            bool favorite = boolFromCharInput( "Dodac do ulubionych? T / N: " );

            // dodaj
            Product* newProduct = new Product( name, amountWithUnitToMacroUnit( AwU ), kcal, expiryDate, price, favorite );
            product::base.products.push_back( newProduct );

            product::base.isNewLongestName( name );

            sortByName<Product*>( product::base.products );
        }
    }

    catch( const std::exception& ex )
    {
        pushNotif( ex.what() );
    }

    AppState::currentState = AppState::PRODUCT_MENU;
}



void events_productEdit()
{
    refreshScreen();
    product::base.printProducts( print::NAME | print::AMOUNT | print::DATE | print::KCAL | print::PRICE | print::IS_FAVORITE | print::HIDDEN );

    try {
        std::string name = cinToString( "Podaj nazwe produktu: " );

        auto product = findVectorMemberByKey<Product*>( product::base.products, name );

        if( product == product::base.products.end() )
        {
            throw std::runtime_error( "Nie ma takiego produktu na liscie" );
        }

        while( 1 )
        {
            refreshScreen();
            (*product)->printInfo( print::NAME | print::AMOUNT | print::KCAL | print::DATE | print::PRICE | print::IS_FAVORITE | print::HIDDEN );
            drawProductsEditMenu();

            char userInput = _getch();
            switch( userInput )
            {
                case productData::NAME:
                {
                    std::string newName = getlineToString( "\nPodaj nowa nazwe produktu : " );

                    if( newName == "" )
                    {
                        throw std::runtime_error( "Nie podano nazwy" );
                    }

                    ( *product )->setName( newName );

                    pushNotif( "Edytowano nazwê produktu" );

                    product::base.isNewLongestName( newName );
                    sortByName<Product*>( product::base.products );

                    break;
                }
                case productData::AMOUNT_WITH_UNIT:
                {
                    amount_with_unit AwU = stringToBaseUnit( cinToString( "\nPodaj ilosc z jednostka (g/kg/ml/L): " ) );

                    if( AwU.first < 0 )
                    {
                        throw std::runtime_error( "Nieprawidlowa ilosc" );
                    }
                    if( formatUnit( AwU.second ) < 0 )
                    {
                        throw std::runtime_error( " Nieprawidlowa jednostka" );
                    }

                    ( *product )->setAmount( AwU.first );
                    ( *product )->setUnit( AwU.second );

                    pushNotif( "Edytowano iloœæ produktu" );

                    break;
                }
                case productData::CALORIES:
                {
                    int kcal = cinToInt( "\nPodaj ilosc kalorii na 100g/ml: " );

                    if( !isKcalValid( kcal ) )
                    {
                        throw std::runtime_error( "Nierealistyczna wartosc kaloryczna" );
                    }

                    ( *product )->setKcal( kcal );

                    break;
                }
                case productData::EXPIRY_DATE:
                {
                    Date date = cinToDate( "\nPodaj date waznosci\n" );

                    if( !isDateValid( date ) )
                    {
                        throw std::runtime_error( "\tNieprawidlowa data\n" );
                    }

                    ( *product )->setDate( date );

                    pushNotif( "Edytowano datê wa¿noœci produktu" );

                    break;
                }
                case productData::PRICE:
                {
                    int price = static_cast<int>( cinToFloat( "\nPodaj nowa cene produktu (na 100g/ml)" ) * 100 );

                    if( price < 0 )
                    {
                        throw std::runtime_error( "Nieprawidlowa cena" );
                    }

                    ( *product )->setPrice( price );

                    pushNotif( "Edytowano cenê produktu" );

                    break;
                }
                case productData::IS_FAVORITE:
                {
                    bool isFavorite = boolFromCharInput( "\nDodac do ulubionych? T / N: " );

                    ( *product )->setFavorite( isFavorite );

                    if( isFavorite )
                    {
                        pushNotif( "Produkt jest teraz ulubiony" );
                    }
                    else
                    {
                        pushNotif( "Produkt nie jest teraz ulubiony" );
                    }

                    break;
                }
                case 27:
                {
                    throw std::runtime_error("");
                }
            }
        }
    }

    catch( const std::exception& ex )
    {
        pushNotif( ex.what() );
    }

    AppState::currentState = AppState::PRODUCT_MENU;
}



void events_productDelete()
{
    refreshScreen();
    product::base.printProducts( print::NAME | print::AMOUNT | print::DATE | print::KCAL | print::PRICE | print::IS_FAVORITE );

    std::string name = cinToString( "Podaj nazwe produktu: " );

    auto product = findVectorMemberByKey<Product*>( product::base.products, name );

    if( product == product::base.products.end() )
    {
        pushNotif("Nie ma takiego produktu w bazie");
    }
    else
    {
        ( *product )->deleteProduct();
    }

    AppState::currentState = AppState::PRODUCT_MENU;
}



void events_productSale()
{
    refreshScreen();
    product::base.printProducts( print::NAME | print::AMOUNT | print::PRICE | print::SALE_PRICE | print::FAVORITE | print::HIDDEN );

    std::cout << "\n\tESC. Powrot";

    while( char userInput = _getch() )
    {
        if( userInput == 27 )
            break;
    }

    AppState::currentState = AppState::MAIN_MENU;
}