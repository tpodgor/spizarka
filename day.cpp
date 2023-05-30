#include "day.h"

Day::Day( std::string name, int weekday ) : _dayName( name ), _dayOfWeek( weekday ) {};
Day::Day(): _dayOfWeek( NULL ) {};



void Day::printInfo()
{
    if( _dayOfWeek == plan::today )
        std::cout << " Dzisiaj:" << '\n';
    else
        std::cout << " " << _dayName << '\n';


    if( _meals.empty() )
    {
        std::cout << "\t--------------------\n";
    }
    else
    {
        for( auto meal : _meals )
        {
            std::cout << '\t' << meal.first->getName();
            std::cout << "   " << meal.second << " por.";
            std::cout << "   " << static_cast<int>( meal.first->getTotalKcal() * meal.second ) << " kcal\n";
        }
    }

    if( _dayOfWeek == plan::today )
        std::cout << "\n\n";

    std::cout << '\n';
}



void Day::cookMeal()
{
    // podaj nazwe posilku
    std::string mealName = getlineToString( "Podaj nazwe posilku, ktory przyrzadziles: " );

    if( mealName == "" )
    {
        pushNotif( "Nie podano nazwy" );
        return;
    }

    // sprawdz czy jest zaplanowany
    for( auto meal = _meals.begin(); meal < _meals.end(); ++meal )
    {
        // jesli jest
        if( meal->first->getName() == mealName )
        {
            // jesli mozna przyrzadzic
            if( meal->first->canBeCooked( meal->second ) )
            {
                // to usun z listy posilkow
                _meals.erase( meal );
            }

            return;
        }
    }

    // jesli nie ma na liscie
    pushNotif( "Na dzisiaj nie zaplanowales tego posilku" );
}



void Day::addMeal()
{
    // podaj przepis
    std::string recipeName = getlineToString( "Podaj przepis: " );

    // sprawdz czy juz jest zaplanowany na dany dzien
    for( auto meal : _meals )
    {
        if( meal.first->getName() == recipeName )
        {
            pushNotif( "Ten posilek juz jest zaplanowany na dany dzien" );
            return;
        }
    }

    // sprawdz czy przepis jest w bazie
    auto it = findVectorMemberByKey<Recipe*>( recipe::base.recipes, recipeName );
    if( it == recipe::base.recipes.end() )
    {
        pushNotif( "Nie ma takiego przepisu w bazie. Najpierw dodaj przepis do bazy." );
        return;
    }

    // podaj ilosc porcji
    float servings = cinToFloat( "Podaj ilosc porcji: " );

    // jesli niewlasciwa ilosc porcji
    if( servings < 0 )
    {
        pushNotif( "Niewlasciwa ilosc porcji" );
        return;
    }

    // dodaj do posilkow danego dnia
    _meals.emplace_back( *it, servings );
    pushNotif( "Dodano posilek" );
}



void Day::addMeal( Recipe* recipe, float servings )
{
    _meals.emplace_back( recipe, servings );
}



void Day::deleteMeal()
{
    std::string recipeName = getlineToString( "Podaj posilek do usuniecia: " );

    for( auto meal = _meals.begin(); meal < _meals.end(); ++meal )
    {
        if( meal->first->getName() == recipeName )
        {
            _meals.erase( meal );
            pushNotif( "Usunieto zaplanowany posilek" );

            return;
        }
    }

    pushNotif( "Nie zaplanowano tego posilku na ten dzien" );
}



void Day::editInfo()
{
    while( bool repeat = true )
    {
        refreshScreen();
        printInfo();
        drawPlanEditRecipeMenu();

        char userInput = _getch();
        switch( userInput )
        {
            case edit::ADD:
            {
                refreshScreen();
                recipe::base.printRecipes();

                addMeal();

                break;
            }
            case edit::DELETE:
            {
                refreshScreen();
                printInfo();

                deleteMeal();

                break;
            }
            case 27:
            {
                return;
            }
        }
    }
}