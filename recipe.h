#ifndef __RECIPE_H__
#define __RECIPE_H__

#include "productBase.h"

namespace recipeEdit
{
    enum value
    {
        RENAME = 49,
        ADD_PRODUCT,
        DELETE_PRODUCT,
        NEW_DESCRIPTION
    };
};

class Ingredient
{
public:
    Product* _productPtr;
    amount_with_unit _AwU;

    Ingredient( Product* product, amount_with_unit AwU );
};

class Recipe
{
public:
    Recipe( std::string name );

    std::string getName();
    int getTotalKcal();
    std::string getDescription();
    std::vector<Ingredient>& getIngredients();

    void setName(std::string name );

    void printInfo();

    bool addProduct();
    void addProduct( Product* product, amount_with_unit AwU );
    bool deleteProduct( std::string name );
    void addDescription( std::string desc );

    void calcKcalAndPrice();
    bool canBeCooked( float servings );

private:
    std::string _name;
    std::string _description;
    int _totalKcal;
    int _totalPrice;
    std::vector<Ingredient> _ingredients;
};

#endif