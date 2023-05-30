#ifndef __RECIPE_BASE_H__
#define __RECIPE_BASE_H__

#include "recipe.h"

struct RecipeBase
{
    RecipeBase();

    void printRecipes();

    void addRecipe();

    void loadFromFile( std::string path );
    void saveToFile( std::string path );
    void clear();


    std::vector<Recipe*> recipes;
};

namespace recipe
{
    inline RecipeBase base;
}

#endif