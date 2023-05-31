#include "recipeBase.h"

RecipeBase::RecipeBase() {};



void RecipeBase::printRecipes()
{
    std::cout << "\tPrzepisy:\n\n";

    for( auto recipe : recipes )
    {
        std::cout << '\t' << recipe->getName() << '\n';
    }

    std::cout << '\n';
}



void RecipeBase::addRecipe()
{
    // nazwa
    std::string recipeName = getlineToString( "Podaj nazwe przepisu: " );

    if( recipeName == "" )
    {
        pushNotif( "Nie podano nazwy" );
        return;
    }

    auto recipe = findMemberByName<Recipe*>( recipe::base.recipes, recipeName );

    if( recipe != recipe::base.recipes.end() )
    {
        pushNotif( "Przepis o tej nazwie juz istnieje" );
        return;
    }

    Recipe* newRecipe = new Recipe( recipeName );

    // skladniki
    while( 1 )
    {
        refreshScreen();
        newRecipe->printInfo();
        std::cout << "\tWybierz opcje:\n";
        std::cout << "\t    1. Dodaj produkt\n";
        std::cout << "\tENTER. Kontynuuj\n\n";

        char userInput = _getch();
        if( userInput == '1' )
        {
            newRecipe->addProduct();

            // kalorie i cena
            newRecipe->calcKcalAndPrice();
        }
        if( userInput == 13 )
        {
            break;
        }
    }

    // opis
    refreshScreen();
    newRecipe->printInfo();
    newRecipe->addDescription( getlineToString( "Podaj nowy opis: " ) );


    recipe::base.recipes.push_back( newRecipe );
    pushNotif( "Utworzono przepis" );
}



void RecipeBase::loadFromFile( std::string path )
{
    // zaladuj plik
    std::vector<std::vector<std::string>> file = loadFromFileIntoVector( path );

    for( int line = 0; line < file.size(); ++line ) // dla kazdego przepisu
    {
        Recipe* newRecipe = new Recipe( file[ line ][ 0 ] ); // nazwa
        newRecipe->addDescription( file[ line ][ 1 ] ); // opis


        for( int seg = 2; seg < file[ line ].size(); seg += 3 ) // dla kazdego skladnika
        {
            // znajdŸ produkt w bazie
            auto it = findMemberByName<Product*>( product::base.products, file[ line ][ seg ] );

            newRecipe->addProduct(
                *it, // produkt
                { stoi( file[ line ][ seg + 1 ] ), file[ line ][ seg + 2 ] } // AwU w przepisie
            );
        }

        newRecipe->calcKcalAndPrice();

        // dodaj do vectora wszystkich przepisow
        recipe::base.recipes.push_back( newRecipe );
    }

    sortByName<Recipe*>( recipe::base.recipes );
}



void RecipeBase::saveToFile( std::string path )
{
    // usun dotychczasowe dane
    std::ofstream outputFile{ path };
    outputFile.close();

    for( int i = 0; i < recipe::base.recipes.size(); ++i )
    {
        Recipe* recipe = recipe::base.recipes[ i ];
        std::vector<std::string> segments;

        segments.push_back( recipe->getName() );
        segments.push_back( recipe->getDescription() );

        std::vector<Ingredient> temp = recipe->getIngredients();

        for( auto ingredient = temp.begin(); ingredient < temp.end(); ++ingredient )
        {
            segments.push_back( (*ingredient)._productPtr->getName() );
            segments.push_back( std::to_string( ( *ingredient )._AwU.first ) );
            segments.push_back( ( *ingredient )._AwU.second );
        }

        // zapisz aktualne dane
        saveObjectInFile( segments, path );
    }
}



void RecipeBase::clear()
{
    for( auto& recipe : recipes )
    {
        delete recipe;
    }

    recipes.clear();
}