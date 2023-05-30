#include "initialization.h"
#include "events.h"
#include "draw.h"

int main()
{
    init();

    // main loop
    while( AppState::currentState != AppState::QUIT )
    {
        handleEvents();
    }

    close();

    return 0;
}