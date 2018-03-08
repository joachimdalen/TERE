#include <iostream>
#include <application/application.h>

int main()
{
    try
    {
        GameSettings gameSettings;

        if (!gameSettings.fileExists())
            gameSettings.reset();

        if (gameSettings.load())
        {
            Application app(gameSettings);
            app.run();
        }
    }
    catch (std::exception &e) {
        std::cout << "\nEXCEPTION: " << e.what() << std::endl;
    }
}
