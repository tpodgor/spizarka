#ifndef __DAY_H__
#define __DAY_H__

#include "utils.h"
#include "recipeBase.h"

class Day
{
public:
    Day( std::string name, int weekday );
    Day();

    void printInfo();

    void cookMeal();
    void addMeal();
    void addMeal( Recipe* recipe, float servings );
    void deleteMeal();
    void editInfo();


    std::string _dayName;
    int _dayOfWeek;
    std::vector<std::pair<Recipe*, float>> _meals;
};

namespace plan
{
    inline const int today = getDayOfWeek();
}

#endif