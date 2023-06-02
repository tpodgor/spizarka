#include "plan.h"

Plan::Plan()
{
    days[ 0 ] = Day( "Niedziela", 0 );
    days[ 1 ] = Day( "Poniedzialek", 1 );
    days[ 2 ] = Day( "Wtorek", 2 );
    days[ 3 ] = Day( "Sroda", 3 );
    days[ 4 ] = Day( "Czwartek", 4 );
    days[ 5 ] = Day( "Piatek", 5 );
    days[ 6 ] = Day( "Sobota", 6 );
}



void Plan::printDay( int day )
{
    days[ day ].printInfo();
}



void Plan::printPlan()
{
    std::cout << "\tPlan na najblizszy tydzien:\n\n";

    for( int weekday = plan::today; weekday < 7; ++weekday )
    {
        days[ weekday ].printInfo();
    }
    for( int weekday = 0; weekday < plan::today; ++weekday )
    {
        days[ weekday ].printInfo();
    }
}



void Plan::cookMealToday()
{
    days[ plan::today ].cookMeal();

    product::base.saveToFile( "products.txt" );
    plan::week.saveToFile( "plan.txt" );
    plan::kcalHistory.saveToFile( "kcalHistory.txt" );
}



void Plan::editPlanForDay( int day )
{
    days[ day ].editInfo();

    plan::week.saveToFile( "plan.txt" );
}



void Plan::loadFromFile( std::string path )
{
    // zaladuj plik
    std::vector<std::vector<std::string>> file = loadFromFileIntoVector( path );

    // aktualny tydzien
    for( int line = 1; line <= 7; ++line ) // dla kazdego dnia
    {
        // jesli sa przepisy w danym dniu
        if( file[ line ][ 0 ] != "" )
        { 
            // dla kazdego przepisu
            for( int seg = 0; seg < file[ line ].size(); seg += 2 )
            {
                // sprawdza czy jest w recipe::base
                auto it = findMemberByName<Recipe*>( recipe::base.recipes, file[ line ][ seg ] );

                // nie ma
                if( it == recipe::base.recipes.end() )
                {
                    pushNotif( "Baza przepisow nie zawiera: " + file[ line ][ seg ] );
                    return;
                }

                // dodaj do posilkow danego dnia
                plan::week.days[ line - 1 ].addMeal( *it, stof( file[ line ][ seg + 1 ] ) );
            }
        }
    }

    // TODO przesunac save'y o tyle dni
    int dayOffset = abs( daysUntilDate( { stoi( file[ 0 ][ 0 ] ), stoi( file[ 0 ][ 1 ] ) ,stoi( file[ 0 ][ 2 ] ) } ) );

    if( dayOffset )
    {
        // nie minal jeszcze zaplanowany tydzien
        if( dayOffset < 7 )
        {
            dayOffset = dayOffset % 7;
            Plan temp;

            int day = getDayOfWeek();
            for( int moves = 7 - dayOffset; moves > 0; --moves )
            {
                if( day > 6 )
                    day -= 7;
                temp.days[ day ]._meals = plan::week.days[ day ]._meals;
                ++day;
            }

            for( int day = 0; day < 7; ++day )
            {
                plan::week.days[ day ]._meals = temp.days[ day ]._meals;
            }
        }
        // minal tydzien i dluzej
        else
        {
            for( auto& day : plan::week.days )
            {
                day._meals.clear();
            }
        }
    }
}



void Plan::saveToFile( std::string path )
{
    // usun dotychczasowe dane
    std::ofstream outputFile{ path };
    outputFile.close();

    // data
    std::vector<std::string> today = { std::to_string( getDate( 'd' ) ) ,std::to_string( getDate( 'm' ) ) ,std::to_string( getDate( 'y' ) ) };
    saveObjectInFile( today, path );

    // tygodniowy plan
    for( int day = 0; day < 7; ++day )
    {
        std::vector<std::string> meals;

        for( auto& meal : plan::week.days[ day ]._meals )
        {
            meals.push_back( meal.first->getName() );
            meals.push_back( std::to_string( meal.second ) );
        }

        saveObjectInFile( meals, path );
    }
}