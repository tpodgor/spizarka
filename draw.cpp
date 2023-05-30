#include "draw.h"

void refreshScreen()
{
    system( "CLS" );
    std::cout << "#################################### iSpizarka ####################################\n\n";

    if( AppState::errorMsg != "" )
    {
        std::cout << "-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n\n";
        std::cout << "\tPowiadomienie:\n";
        std::cout << AppState::errorMsg;
        AppState::errorMsg = "";
        std::cout << "\n_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n\n";
    }
}

void drawMainMenu()
{
    std::cout << "\n\tWybierz opcje:\n";
    std::cout << "\t  1. Produkty\n";
    std::cout << "\t  2. Przepisy\n";
    std::cout << "\t  3. Plan zywieniowy\n";
    std::cout << "\t  4. Promocje\n";
    std::cout << "\tESC. Wyjscie\n\n";
}

void drawGoodbye()
{
    std::cout << "\tcya";
}

void drawProductsMenu()
{
    std::cout << "\n\tWybierz opcje:\n";
    std::cout << "\t  1. Spozyj\n";
    std::cout << "\t  2. Dodaj\n";
    std::cout << "\t  3. Edytuj\n";
    std::cout << "\t  4. Usun\n";
    std::cout << "\tESC. Powrot\n";
}

void drawProductsEditMenu()
{
    std::cout << "\n\tWybierz pole do edycji: \n";
    std::cout << "\t  1. Nazwa\n";
    std::cout << "\t  2. Ilosc\n";
    std::cout << "\t  3. Kalorie\n";
    std::cout << "\t  4. Data waznosci\n";
    std::cout << "\t  5. Cena\n";
    std::cout << "\t  6. Czy ulubiony\n";
    std::cout << "\tESC. Powrot\n";
}

void drawRecipesMenu()
{
    std::cout << "\n\tWybierz opcje:\n";
    std::cout << "\t  1. Pokaz\n";
    std::cout << "\t  2. Dodaj \n";
    std::cout << "\t  3. Usun\n";
    std::cout << "\tESC. Powrot\n";
}

void drawRecipeViewMenu()
{
    std::cout << "\n\tWybierz opcje:\n";
    std::cout << "\t  1. Przyrzadz\n";
    std::cout << "\t  2. Edytuj\n";
    std::cout << "\tESC. Powrot\n";
}

void drawRecipesEditMenu()
{
    std::cout << "\n\tWybierz opcje:\n";
    std::cout << "\t  1. Zmien nazwe\n";
    std::cout << "\t  2. Dodaj produkt\n";
    std::cout << "\t  3. Usun produkt\n";
    std::cout << "\t  4. Nowy opis\n";
    std::cout << "\tESC. Powrot\n";
}

void drawPlanMenu()
{
    std::cout << "\n\tWybierz opcje:\n";
    std::cout << "\t  1. Przyrzadz posilek\n";
    std::cout << "\t  2. Edytuj plan\n";
    std::cout << "\t  3. Historia\n";
    std::cout << "\tESC. Powrot\n";
}

void drawPlanEditMenu()
{
    std::cout << "\tPodaj dzien tygodnia:\n";
    std::cout << "\t  1. Poniedzialek\n";
    std::cout << "\t  2. Wtorek\n";
    std::cout << "\t  3. Sroda\n";
    std::cout << "\t  4. Czwartek\n";
    std::cout << "\t  5. Piatek\n";
    std::cout << "\t  6. Sobota\n";
    std::cout << "\t  7. Niedziela\n";
    std::cout << "\tESC. Powrot\n";
}

void drawPlanEditRecipeMenu()
{
    std::cout << "\tWybierz opcje:\n";
    std::cout << "\t  1. Dodaj przepis\n";
    std::cout << "\t  2. Usun przepis\n";
    std::cout << "\tESC. Powrot\n";
}