#include "utils.h"

std::vector<std::string> splitString( std::string line, char delimiter )
{
    std::vector<std::string> result;
    std::stringstream ss;
    ss << line;
    std::string segment;

    while( std::getline( ss, segment, delimiter ) )
    {
        result.push_back( segment );
    }

    return result;
}

std::vector<std::vector<std::string>> loadFromFileIntoVector( std::string path )
{
    std::vector<std::vector<std::string>> result;

    // otworz plik
    std::ifstream inputFile( path );
    if( !inputFile )
    {
        pushNotif( path + ": blad podczas otiwerania pliku" );
    }

    // buffer
    std::string line;
    while( std::getline( inputFile, line ))
    {
        if( line == "" )
        {
            std::vector<std::string> temp = { "" };
            result.push_back( temp );
        }
        else
            result.push_back(splitString(line, ':'));
    }
    return result;
}

void saveObjectInFile( std::vector<std::string>& objectSegments, std::string path )
{
    // otworz plik
    std::ofstream outputFile{ path, std::ios::app };

    if( !outputFile )
    {
        pushNotif( path + ": blad podczas zapisywania pliku" );
        return;
    }

    if( objectSegments.size() > 0 )
    {
        for( int i = 0; i < objectSegments.size() - 1; ++i )
        {
            outputFile << objectSegments[ i ] << ":";
        }

        outputFile << objectSegments.at( objectSegments.size() - 1 ) << '\n';
    }
    else
    {
        outputFile << "" << '\n';
    }
}
namespace unit
{
    enum value
    {
        g = 0,
        kg = 1,
        ml = 2,
        L = 3
    };
};

int formatUnit( std::string unit )
{
    int result = -1;

    if( unit == "g" || unit == "G" )
        result = unit::g;
    else if( unit == "kg" || unit == "Kg" || unit == "kG" || unit == "KG" )
        result = unit::kg;
    else if( unit == "ml" || unit == "Ml" || unit == "mL" || unit == "ML" )
        result = unit::ml;
    else if( unit == "l" || unit == "L" )
        result = unit::L;
 
    return result;
}

bool compareUnits( std::string u1, std::string u2 )
{
    if( formatUnit( u1 ) != formatUnit( u2 ) )
    {
        return false;
    }

    return true;
}

std::pair<float, std::string> stringToBaseUnit( std::string metricValue )
{
    float amount = 0;
    std::string unit = "";

    int floatingPointOffset = 0;
    for( char& c : metricValue )
    {
        // if whitespace
        if( c == ' ' )
            continue;

        // if , or .
        if( c == ',' || c == '.' )
        {
            ++floatingPointOffset;
            continue;
        }

        // if 0 - 9
        if( c >= '0' && c <= '9' )       //495.75L
        {
            if( !floatingPointOffset )
            {
                amount *= 10;
                amount += c - 48;
            }
            else
            {
                amount += (c - 48) / pow(10, floatingPointOffset );
                ++floatingPointOffset;
            }
            continue;
        }

        // if kg / l / L
        if( c == 'k' || c == 'K' || c == 'm' || c == 'M' || c == 'g' || c == 'G' || c == 'l' || c == 'L' )
        {
            unit += c;
            continue;
        }
    }
    
    switch( formatUnit(unit) )
    {
        case unit::g:
            unit = "g";
            break;
        case unit::kg:
            unit = "g";
            amount *= 1000;
            break;
        case unit::ml:
            unit = "ml";
            break;
        case unit::L:
            unit = "ml";
            amount *= 1000;
            break;
        // error
        default:
            return { 0,unit };
    }
    return { amount, unit };
}

std::pair<float, std::string> amountWithUnitToMacroUnit( std::pair<float, std::string> pair ) // _____4.0
{
    // konwersja na makrojednostke
    if( pair.first > 1000 )
    {
        pair.first /= 1000;

        if( pair.second == "g" )
            pair.second = "kg";
        else if( pair.second == "ml" )
            pair.second = "L";
        // jesli jednostka jest bledna zwraca 0
        else
            pair.first = 0;
    }

    return { pair.first, pair.second };
}

std::string convertAmountWithUnitToString( std::pair<float, std::string> pair )
{
    std::string result = "";

    // bez konwersji - obetnij zera
    if( pair.second == "g" || pair.second == "ml" && pair.first < 1000 )
    {
        if( pair.first < 10 )
            result += "    " + std::to_string( pair.first ).substr( 0, 1 );
        else if( pair.first < 100 )
            result += "   " + std::to_string( pair.first ).substr( 0, 2 );
        else
            result += "  " + std::to_string( pair.first ).substr( 0, 3 );
    }
    // po konwersji
    else
    {
        result = std::to_string( pair.first );

        if( result.length() > 5 )
        {
            result = result.substr( 0, 5 );
            if( pair.first >= 1000 )
                result = " " + result.substr(0, 4);
        }
    }

    result += " " + pair.second;

    if( pair.second == "g" || pair.second == "L" )
        result += " ";

    return result;
}

std::string whenExpire( Date date )
{
    int daysToExpire = daysUntilDate( date );
    std::string result = "";

    if( daysToExpire > 1 )
        result = "    za " + std::to_string( daysToExpire ) + " d";
    else if( daysToExpire == 1 )
        result = "     jutro";
    else if( daysToExpire == 0 )
        result = "  dzisiaj!";
    else if( daysToExpire > -10 )
        result = "  " + std::to_string(-daysToExpire) + " d temu";
    else
        result = ">10 d temu";

    return result;
}

Date stringToDate( std::string dateAsString )
{
    // d,m,y
    std::string dateBuffer[ 3 ];
    int offset = 0;

    for( char& c : dateAsString )
    {
        if( offset > 2 )
            break;

        if( c >= '0' && c <= '9' )
            dateBuffer[ offset ] += c;
        else if( c == '/' || c == ' ' )
            ++offset;
        else
            break;
    }

    Date result;
    if( dateBuffer[ 0 ] != "" )
        result._d = stoi( dateBuffer[ 0 ] );
    if( dateBuffer[ 1 ] != "" )
        result._m = stoi( dateBuffer[ 1 ] );
    if( dateBuffer[ 2 ] != "" )
        result._y = stoi( dateBuffer[ 2 ] );

    if( isDateValid( result ) )
        return result;
    else
    {
        std::cout << "Bledny format daty\n";
        return { 0,0,0 };
    }
}

std::string getlineToString( std::string prompt )
{
    std::string result;
    std::cout << prompt;
    getline( std::cin, result );

    return result;
}

std::string cinToString( std::string prompt )
{
    std::string result;
    std::cout << prompt;
    std::cin >> result;
    std::cin.ignore();

    return result;
}

int cinToInt( std::string prompt )
{
    int result;
    std::cout << prompt;
    std::cin >> result;
    std::cin.ignore();

    return result;
}

float cinToFloat( std::string prompt )
{
    float result;
    std::cout << prompt;
    std::cin >> result;
    std::cin.ignore();

    return result;
}

bool boolFromCharInput( std::string prompt)
{
    std::cout << prompt;

    bool result = false;

    while( char userInput = _getch() )
    {
        if( userInput == 't' || userInput == 'T' )
        {
            result = true;
            break;
        }
        else if( userInput == 'n' || userInput == 'N' || userInput == 27 )
            break;
    }

    return result;
}

bool isKcalValid( int value )
{
    if( value < 10 || value > 999 )
        return false;
    else
        return true;
}

std::string priceToString( int price )
{
    float converted = static_cast<float>( price / 10.f );

    std::string result = "";

    if( converted < 10 )
        result += "  " + std::to_string( converted ).substr( 0, 4 );
    else if( converted < 100 )
        result += " " + std::to_string( converted ).substr( 0, 5 );
    else
        result += std::to_string( converted ).substr( 0, 6 );

    result += " zl";

    return result;
}      

Date cinToDate( std::string prompt )
{
    std::cout << prompt;

    Date result;

    result._d = cinToInt( "Dzien: " );
    result._m = cinToInt( "Miesiac: " );
    result._y = cinToInt( "Rok: " );

    return result;
}