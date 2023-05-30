#include "productBase.h"

ProductBase::ProductBase() {};



void ProductBase::printProducts( int flags )
{
    std::cout << '\t';

    if( flags & print::NAME )
    {
        std::cout << "Nazwa";

        for( int space = 5; space < product::longestName; ++space )
            std::cout << " ";

        std::cout << "   ";
    }
    if( flags & print::AMOUNT )
    {
        std::cout << "Ilosc      ";
    }
    if( flags & print::KCAL )
    {
        std::cout << "Kalorie    ";
    }
    if( flags & print::DATE )
    {
        std::cout << "Waznosc      ";
    }
    if( flags & print::PRICE )
    {
        std::cout << "Cena        ";
    }
    if( flags & print::SALE_PRICE )
    {
        std::cout << "Promocja    Konczy sie";
    }
    if( flags & print::IS_FAVORITE )
    {
        std::cout << "Ulub";
    }

    std::cout << "\n\n";

    for( auto product : products )
    {
        product->printInfo( flags );
    }

    std::cout << '\n';
}



void ProductBase::loadFromFile( std::string path )
{
    // zaladuj plik
    std::vector<std::vector<std::string>> file = loadFromFileIntoVector( path );

    for( int i = 0; i < file.size(); ++i )
    {
        Product* newProduct = new Product
        (
            file[ i ][ 0 ], // nazwa
            { stoi( file[ i ][ 1 ] ), file[ i ][ 2 ] }, // ilosc z jednostka
            stoi( file[ i ][ 3 ] ), // kalorie
            { stoi( file[ i ][ 4 ] ), stoi( file[ i ][ 5 ] ), stoi( file[ i ][ 6 ] ) }, // data
            stoi( file[ i ][ 7 ] ), // cena
            stoi( file[ i ][ 8 ] ) // ulub
        );

        // zaktualizuj najdluzsza nazwe wsrod produktow
        product::base.isNewLongestName( file[i][0] );

        // dodaj do vectora wszystkich produktow
        product::base.products.push_back( newProduct );
    }

    sortByName<Product*>( product::base.products );
}



void ProductBase::loadProductsOnSale( std::string path )
{
    // zaladuj plik
    std::vector<std::vector<std::string>> file = loadFromFileIntoVector( path );

    for( int i = 0; i < file.size(); ++i )
    {
        Product* newProduct = new Product
        (
            file[ i ][ 0 ], // nazwa
            { stoi( file[ i ][ 1 ] ), stoi( file[ i ][ 2 ] ), stoi( file[ i ][ 3 ] ) }, // data konca promocji
            stoi( file[ i ][ 4 ] ) // cena
        );

        // dodaj do vectora wszystkich produktow
        product::onSale.push_back( newProduct );
    }
}



void ProductBase::saveToFile( std::string path )
{
    // usun dotychczasowe dane
    std::ofstream outputFile{ path };
    outputFile.close();

    for( int i = 0; i < product::base.products.size(); ++i )
    {
        Product* buffer = product::base.products[ i ];
        std::vector<std::string> segments;

        segments.push_back( buffer->getName() );
        segments.push_back( std::to_string( buffer->getAwU().first ) );
        segments.push_back( buffer->getAwU().second );
        segments.push_back( std::to_string( buffer->getKcal() ) );
        segments.push_back( std::to_string( buffer->getDate()._d ) );
        segments.push_back( std::to_string( buffer->getDate()._m ) );
        segments.push_back( std::to_string( buffer->getDate()._y ) );
        segments.push_back( std::to_string( buffer->getPrice() ) );
        segments.push_back( std::to_string( buffer->getFavorite() ) );

        // zapisz aktualne dane
        saveObjectInFile( segments, path );
    }
}



void ProductBase::clear()
{
    for( auto& product : products )
    {
        delete product;
    }

    products.clear();
}



void ProductBase::clearProductsOnSale()
{
    for( auto& sale : product::onSale )
    {
        delete sale;
    }

    product::onSale.clear();
}


void ProductBase::notifyAboutProductsThatAreFewOrExpireSoon()
{
    // konczy sie ilosc
    for( auto product : products )
    {
        if( product->getAwU().first > 0 )
        {
            if( product->getAwU().first < 200 )
            {
                pushNotif( "Pozostalo jedynie: " + convertAmountWithUnitToString( product->getAwU() ) + " produktu: " + product->getName() );
            }
        }
    }

    // konczy sie waznosc
    for( auto product : products )
    {
        if( product->getAwU().first > 0 )
        {
            if( daysUntilDate( product->getDate() ) <= 5 )
            {
                pushNotif( "Koniec waznosci : " + whenExpire( product->getDate() ) + "  dla produktu: " + product->getName() );
            }
        }
    }
}



void ProductBase::findLongestName()
{
    product::longestName = 0;

    for( auto product : products )
    {
        if( product->isHidden() )
        {
            continue;
        }
        if( product->getName().length() > product::longestName )
        {
            product::longestName = product->getName().length();
        }
    }
}

void ProductBase::isNewLongestName( std::string name )
{
    if( name.length() > product::longestName )
        product::longestName = name.length();
}