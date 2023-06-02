#include "events.h"

// AppState::RECIPE_MENU
void events_recipeMenu()
{
    refreshScreen();
    recipe::base.printRecipes();
    drawRecipesMenu();

    char userInput = _getch();

    switch( userInput )
    {
        case '1':
        {
            AppState::currentState = AppState::RECIPE_VIEW;
            break;
        }
        case '2':
        {
            AppState::currentState = AppState::RECIPE_ADD;
            break;
        }
        case '3':
        {
            AppState::currentState = AppState::RECIPE_DELETE;
            break;
        }
        case 27:
        {
            AppState::currentState = AppState::MAIN_MENU;
            break;
        }
    }
}

void events_recipeView()
{
    try {
        Recipe* selectedRecipe = nullptr;

        while( 1 )
        {
            if( selectedRecipe == nullptr )
            {
                refreshScreen();
                recipe::base.printRecipes();

                std::string name = getlineToString( "Podaj nazwe przepisu: " );

                if( name == "" )
                {
                    throw std::runtime_error( "Nie podano nazwy" );
                }

                auto recipe = findMemberByName<Recipe*>( recipe::base.recipes, name );

                if( recipe == recipe::base.recipes.end() )
                {
                    throw std::runtime_error( "Nie ma takiego przepisu w bazie" );
                }

                selectedRecipe = *recipe;
            }

            refreshScreen();
            selectedRecipe->printInfo();
            drawRecipeViewMenu();

            while( 1 )
            {
                char userInput = _getch();

                if( userInput == '1' )
                {
                    events_recipeCook( selectedRecipe );
                    break;
                }
                else if( userInput == '2' )
                {
                    events_recipeEdit( selectedRecipe );
                    break;
                }
                else if( userInput == 27 )
                {
                    throw std::runtime_error( "" );
                    break;
                }
            }
        }
    }

    catch( const std::exception& ex )
    {
        pushNotif( ex.what() );
    }

    AppState::currentState = AppState::RECIPE_MENU;
}



void events_recipeCook( Recipe* recipe )
{
    refreshScreen();
    recipe->printInfo();
    drawRecipeViewMenu();

    float servings = cinToFloat( "\nIle porcji chcesz przyrzadzic: " );

    recipe->canBeCooked( servings );
}



void events_recipeEdit( Recipe* recipe )
{
    while( 1 )
    {
        try {
            refreshScreen();
            recipe->printInfo();
            drawRecipesEditMenu();

            char userInput = _getch();

            switch( userInput )
            {
                case recipeEdit::RENAME:
                {
                    std::string newName = getlineToString( "\nPodaj nowa nazwe przepisu: " );

                    if( newName == "" || newName == recipe->getName() )
                    {
                        throw std::runtime_error( "Nie zmieniono nazwy" );
                    }

                    auto recipeName = findMemberByName<Recipe*>( recipe::base.recipes, newName );
                    if( recipeName != recipe::base.recipes.end() )
                    {
                        throw std::runtime_error( "Przepis o tej nazwie juz istnieje" );
                    }

                    recipe->setName( newName );
                    pushNotif( "Zmieniono nazwe" );

                    break;
                }
                case recipeEdit::ADD_PRODUCT:
                {
                    recipe->addProduct();

                    break;
                }
                case recipeEdit::DELETE_PRODUCT:
                {
                    std::string productName = getlineToString( "\nPodaj nazwe produktu: " );

                    if( productName == "" )
                    {
                        throw std::runtime_error( "Nie podano nazwy" );
                    }

                    recipe->deleteProduct( productName );

                    break;
                }
                case recipeEdit::NEW_DESCRIPTION:
                {
                    recipe->addDescription( getlineToString( "\nPodaj nowy opis: " ) );

                    break;
                }
                case 27:
                {
                    recipe::base.saveToFile( "recipes.txt" );
                    return;
                }
            }
        }

        catch( const std::exception& ex )
        {
            pushNotif( ex.what() );
        }
    }
}



void events_recipeAdd()
{
    refreshScreen();
    recipe::base.printRecipes();

    recipe::base.addRecipe();

    AppState::currentState = AppState::RECIPE_MENU;
}



void events_recipeDelete()
{
    refreshScreen();
    recipe::base.printRecipes();

    std::string recipeName = getlineToString( "Podaj przepis do usuniecia: " );
    auto recipe = findMemberByName<Recipe*>( recipe::base.recipes, recipeName );

    if( recipe != recipe::base.recipes.end() )
    {
        delete* recipe;
        recipe::base.recipes.erase( recipe );
        pushNotif( "Usunieto przepis" );
    }
    else
    {
        pushNotif( "Nie ma takiego przepisu" );
    }

    recipe::base.saveToFile( "recipes.txt" );

    AppState::currentState = AppState::RECIPE_MENU;
}