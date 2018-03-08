#include <utility/utility.h>
#include "main_menu_state.h"
#include "sound/music_player.h"

MainMenuState::MainMenuState(StateStack &stack, const State::Context &context) : State(stack, context)
{
    sf::Font &mainFont = context.fonts->get(Fonts::Prime);
    sf::RenderWindow &window = *getContext().window;

    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    tile = new Tile(window, 48, true);

    // Reusing this code from the main game to make a logo
    tetromino = new Tetromino();
    tetromino->constructScrollText(0, "TETRIS   REVISITED");

    centerPadding = ((window.getSize().y * 4 / 3) - tile->tileSize * 44) / 2;

    selection[0] = TextButtonPtr(new TextButton("Quick Start", mainFont));
    selection[0]->setPosition((context.window->getView().getSize() / 2.f) + sf::Vector2f(0.f, context.settings->cSize * 2.0));

    selection[1] = TextButtonPtr(new TextButton("New Game", mainFont));
    selection[1]->setPosition(selection[0]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));

    selection[2] = TextButtonPtr(new TextButton("Customize", mainFont));
    selection[2]->setPosition(selection[1]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));

    selection[3] = TextButtonPtr(new TextButton("View High Scores", mainFont));
    selection[3]->setPosition(selection[2]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));

    selection[4] = TextButtonPtr(new TextButton("Credits", mainFont));
    selection[4]->setPosition(selection[3]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));

    selection[5] = TextButtonPtr(new TextButton("Options", mainFont));
    selection[5]->setPosition(selection[4]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));

    selection[6] = TextButtonPtr(new TextButton("Exit Game", mainFont));
    selection[6]->setPosition(selection[5]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));

    selection[0]->setSelect();

    if(getContext().settings->musicEnabled)
        getContext().music->setVolume( getContext().settings->musicVolume );

    context.settings->bDemoMode = false; // Reset the demo mode when opening this state
    context.music->play(Music::MenuTheme); // Play menu theme
}


void MainMenuState::draw()
{
    double mSin;
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // Draw the starfield first

    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 44; x++)
        {
            mSin = sin( 2 * M_PI / 44 * x + nXShift ) * tile->tileSize;

            tile->mTile.setFillColor( tile->mColors[7]);

            if(x < 29)
            {
                if(tetromino->getCharacterFromScrolltext(0, x, y) == L'X')
                    tile->mTile.setFillColor( tile->mColors[9] );

                tile->mTile.setPosition(getContext().settings->hPadding + centerPadding + x * tile->tileSize, (y + 7) * tile->tileSize + mSin);

                window.draw(tile->mTile);
            }

            tile->mTile.setFillColor( tile->mColors[7]);

            if(tetromino->getCharacterFromScrolltext(0, x + 45, y) == L'X')
                tile->mTile.setFillColor( tile->mColors[9] );

            tile->mTile.setPosition(getContext().settings->hPadding + centerPadding + x * tile->tileSize, (y + 13) * tile->tileSize + mSin);

            window.draw(tile->mTile);
        }
    }

    for(int i = 0; i < 7; i++) {
        selection[i]->draw(window);
    }
}


bool MainMenuState::update(sf::Time dt)
{
    double adjustment = 0.05 / (1000.0 / 60) * (Timer.getElapsedTime().asMicroseconds() / 1000.0);

    mStarfield->update(); // Update the starfield

    if(demoTimer.getElapsedTime().asSeconds() > nDemoTime) // Start the demo mode after 30 seconds
    {
        getContext().settings->bDemoMode = true;
        requestStackPop();
        requestStackPush(States::Game);
    }

    // Make the header text wavy on the main menu screen only (handled in wave_text.cpp in other states)
    nXShift = nXShift + adjustment;

    if(nXShift > 2 * M_PI)
        nXShift = 0.0;

    Timer.restart();

    return true;
}


bool MainMenuState::handleEvent(const sf::Event &event)
{
    // Find the mouse location
    if(event.type == sf::Event::MouseMoved)
    {
        mouseY = sf::Mouse::getPosition(*getContext().window).y;

        for(int i = 0; i < 7; i++)
        {
            double top = getContext().window->getSize().y / 2 +
                    getContext().settings->cSize * 2.0 +
                    getContext().settings->cSize * i * 1.3;

            if(mouseY > top && mouseY < top + (1.0 * getContext().settings->cSize))
            {
                selection[currentSelection]->setUnSelect();
                currentSelection = i;
                selection[currentSelection]->setSelect();

                demoTimer.restart(); // Restart the demo count-down timer
            }
        }
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25))
    {
        selection[currentSelection]->setUnSelect();
        currentSelection++;

        if(currentSelection == 7)
            currentSelection = 0;

        selection[currentSelection]->setSelect();

        demoTimer.restart(); // Restart the demo count-down timer
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up ||
            (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 25))
    {
        selection[currentSelection]->setUnSelect();
        currentSelection--;
        if(currentSelection == -1)
            currentSelection = 6;

        selection[currentSelection]->setSelect();

        demoTimer.restart(); // Restart the demo count-down timer
    }


    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
            event.type == sf::Event::MouseButtonPressed || sf::Joystick::isButtonPressed(0, 0))
    {
        switch (currentSelection)
        {
            case 0: // Quick Start
                requestStackPop();
                requestStackPush(States::Game);
                break;

            case 1: // New Game
                requestStackPop();
                requestStackPush(States::MenuSelection);
                break;

            case 2: //Customize
                requestStackPop();
                requestStackPush(States::CustomizeMenu);
                break;

            case 3: // View High scores
                requestStackPop();

                requestStackPush(States::HighScoreDisplay);
                //requestStackPush(States::HighScoreEntry);

                break;

            case 4: // Credits
                requestStackPop();
                requestStackPush(States::Credits);
                break;

            case 5: // Options
                requestStackPop();
                requestStackPush(States::MenuOptions);
                break;

            case 6: // Exit Game
                requestStackPop();
                break;

            default:
                return false;
        }
    }
    return false;
}
