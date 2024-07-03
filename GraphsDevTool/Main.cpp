#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "Application.hpp"


int main()
{
    Application app;
    app.initialize();
    app.run();

    return 0;
}