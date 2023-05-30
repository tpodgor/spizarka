#ifndef __DATE_H__
#define __DATE_H__

#include <iostream>
#include <ctime>

namespace weekday
{
    enum value
    {
        SUN = 0,
        MON,
        TUE,
        WED,
        THU,
        FRI,
        SAT,
        DEFAULT
    };
};

struct Date
{
    int _d, _m, _y;

    Date( int d, int m, int y );
    Date( int d, int m );
    Date();

    // do wypisywania
    friend std::ostream& operator <<( std::ostream& out, const Date& date );
};

// zwroc aktualna date w formie Date
Date getDate();

int getDayOfWeek();

// zwroc aktualny dzien, miesiac lub rok
// format przyjmuje 'd' / 'm' / 'y'
int getDate( char format );

// sprawdz czy dany rok jest przestepny
bool isLeapYear( int year ); 

int leapYearsSinceYearOne( int year );

int leapYearsBetween( int earlierYear, int laterYear );

// policz ilosc dni, ktore minely od poczatku roku do danej daty
int daysIntoYear( Date date );

int daysUntilDate( Date date );

bool isDateValid( Date date );

#endif