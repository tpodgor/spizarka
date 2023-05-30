#include "date.h"

Date::Date( int d, int m, int y ) : _d( d ), _m( m ), _y( y ) {};
Date::Date( int d, int m ) : _d( d ), _m( m ), _y( getDate( 'y' ) ) {};
Date::Date() : _d( 0 ), _m( 0 ), _y( 1 ) {};

std::ostream& operator<<( std::ostream& out, const Date& date )
{
    if( date._d < 10 )
        out << "0";
    out << date._d << "/";

    if( date._m < 10 )
        out << "0";
    out << date._m << "/" << date._y;

    return out;
}

Date getDate()
{
    tm newtime;
    time_t now = time( 0 );
    localtime_s( &newtime, &now );

    return {newtime.tm_mday, 1 + newtime.tm_mon, 1900 + newtime.tm_year};
}

int getDayOfWeek()
{
    tm newtime;
    time_t now = time( 0 );
    localtime_s( &newtime, &now );

    return newtime.tm_wday;
}

int getDate( char format )
{
    tm newtime;
    time_t now = time( 0 );
    localtime_s( &newtime, &now );
    switch( format ) {
        case 'd': return newtime.tm_mday;
        case 'm': return 1 + newtime.tm_mon;
        case 'y': return 1900 + newtime.tm_year;
        default: {
            std::cout << "wrong date format\n" << '\n';
            return 0;
        }
    }
}

bool isLeapYear( int year )
{
    if( year % 400 == 0 ) return true;
    if( year % 100 == 0 ) return false;
    if( year % 4 == 0 ) return true;

    return false;
}

int leapYearsSinceYearOne( int year )
{
    return ( year / 4 ) - ( year / 100 ) + ( year / 400 );
}

int leapYearsBetween( int earlierYear, int laterYear )
{
    --earlierYear;
    return leapYearsSinceYearOne( laterYear ) - leapYearsSinceYearOne( earlierYear );
}

int daysIntoYear( Date date )
{
    int daysPerMonth[] = { 31, ( isLeapYear( date._y ) ? 29 : 28 ), 31,30,31,30,31,31,30,31,30,31 };

    int totalDays = date._d;
    for( int i = 0; i < date._m - 1; ++i )
    {
        totalDays += daysPerMonth[ i ];
    }

    return totalDays;
}

int daysUntilDate( Date date )
{
    Date now = getDate();

    int leapYears = leapYearsBetween( now._y, date._y );
    int fullYears = date._y - now._y - 1;

    int daysUntilThisYearEnd = 365 - daysIntoYear( now );
    if( isLeapYear( now._y ) && now._m - 1 >= 3 )
        --leapYears; // juz wliczone

    int daysIntoDate = daysIntoYear( date );

    // bo dzien juz wliczony w daysIntoDate
    if( isLeapYear( date._y ) && date._m >= 3 )
        --leapYears;

    int totalDays = daysUntilThisYearEnd + fullYears * 365 + leapYears + daysIntoDate;

    return totalDays;

}

bool isDateValid( Date date )
{
    if( date._y < 0 ||
        date._m < 1 || date._m > 12 ||
        date._d < 1 )
        return false;

    if( date._m == 1 || date._m == 3 || date._m == 5 || date._m == 7 || date._m == 8 || date._m == 10 || date._m == 12 )
    {
        if( date._d <= 31 )
            return true;
    }
    else if( date._m == 2 )
    {
        if( ( isLeapYear && date._d <= 29 ) || date._d <= 28 )
            return true;
    }
    else if( date._d <= 30 )
        return true;
    else
        return false;
}