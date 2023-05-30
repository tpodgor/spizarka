#ifndef __FILE_H__
#define __FILE_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <conio.h>
#include <algorithm>
#include <stdio.h>
#include <unordered_map>

#include "appState.h"
#include "date.h"

using amount_with_unit = std::pair<float, std::string>;

namespace edit
{
    enum value
    {
        ADD = 49,
        DELETE,
        COOK,
        RENAME
    };
};

// zwraca vector segmentów stringa podzielonego wg. delimitera
std::vector<std::string> splitString( std::string line, char delimiter );

// zwraca vector do vectorów do segmentów 
// segmenty = czesci stringu oddzielone delimiterem ':'
// 
// V: v1: "seg1", "seg2", ...
//    v2: "seg1", "seg2", ...
//    v3: "seg1", "seg2", ...
std::vector<std::vector<std::string>> loadFromFileIntoVector( std::string path );

// konwertuje wartosc z makro jednostki ( kg / L ) na domyslna ( g / ml ) ze stringa
// np. 10.4 kg -> 10400 g
std::pair<float, std::string> stringToBaseUnit( std::string metricValue );

// zwraca wartosc z jednostka jako jeden string - ulatwia wypisywanie
std::string convertAmountWithUnitToString( std::pair<float, std::string> pair );

// konwertuje wartosc na makrojednostke
// np. 1040 ml -> 1.040 L
std::pair<float,std::string> amountWithUnitToMacroUnit( std::pair<float, std::string> pair );

// wypisuje kiedy Produkt traci date waznosci
std::string whenExpire( Date date );

// konwertuje string do formatu Daty
Date stringToDate( std::string dateAsString );

int formatUnit( std::string );

bool compareUnits( std::string u1, std::string u2 );

void saveObjectInFile( std::vector<std::string>& objectSegments, std::string path );

std::string getlineToString( std::string prompt );
std::string cinToString( std::string prompt );
int cinToInt( std::string prompt );
float cinToFloat( std::string prompt );
Date cinToDate( std::string prompt );
bool boolFromCharInput( std::string prompt );

bool isKcalValid( int value );

std::string priceToString( int price );

// zwraca
// obiekt vectora MUSI miec metode getName() !!
template<typename T>
typename std::vector<T>::iterator findVectorMemberByKey( std::vector<T>& vector, std::string key )
{
    auto it = find_if( vector.begin(), vector.end(), [ &key ]( T obj ) {
        return obj->getName() == key;
        } );

    if( it != vector.end() )
        return it;
    else
        return vector.end();
}

// sortuje vector produktow/przepisow rosnaco po nazwie
template<typename T>
void sortByName( std::vector<T>& vector )
{
    std::sort( vector.begin(), vector.end(), []( T obj1, T obj2 ) {
            return obj1->getName() < obj2->getName();
        } );
}

#endif