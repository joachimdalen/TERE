//#include <experimental/random>

#include "custom_bag_state.h"
#include <utility/utility.h>
#include <resource/resource_holder.h>

CustomBagState::CustomBagState(StateStack& stack, Context context) : State(stack, context)
{
    sf::RenderWindow &window = *getContext().window;

    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    tile = new Tile(window, 38, true);

    // Reusing this code from the main game to make a logo
    tetromino = new Tetromino();
    waveText = new WaveText(window, "CUSTOMIZE RANDOM BAG");


    // For placing the Tetrominos correctly in the window
    int dy = window.getSize().y;

    sRandomBag = context.settings->randomBag;

    for(int i = 0; i < sRandomBag.length(); i++)
    {
        int tetromino = sRandomBag.at(i) - 48;
        selectedTetrominos[tetromino] = true;
    }

    centerPadding = ((dy * 4 / 3) - tile->tileSize * 34) / 2;

    sf::Font &mainFont = context.fonts->get(Fonts::Prime);

    TDescription.setFont(mainFont);
    TDescription.setFillColor(sf::Color(255, 255, 255));
    TDescription.setCharacterSize(context.settings->cSize);

    std::string description = "\
Select the Tetrominos that you want to play with.\n\
Use the arrow keys to navigate, and Return to select/deselect.\n\n\
At least one Tetromino must be selected.";

    TDescription.setString(description);
    centerOriginHorizontal(TDescription);
    TDescription.setPosition(window.getSize().x / 2, context.settings->cSize * 6);

    TDebug.setFont(mainFont);
    TDebug.setCharacterSize(12);
    TDebug.setString("Blank.");


    mOptions[0] = TextButtonPtr(new TextButton("Dummy TextButtonPtr", mainFont)); // To make things easier, placeholder

    mOptions[1] = TextButtonPtr(new TextButton("Save and Quit", mainFont));
    mOptions[1]->setPosition((context.window->getView().getSize() / 2.f) + sf::Vector2f(0.f, context.settings->cSize * 7.0));

    mOptions[2] = TextButtonPtr(new TextButton("Back", mainFont));
    mOptions[2]->setPosition(mOptions[1]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));
}


void CustomBagState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // draw the starfield first
    waveText->draw(window, getContext().settings->cSize * 2);

    window.draw(TDescription);

    sf::RectangleShape mRectangle; // Used for making the starfield less visible
    mRectangle.setFillColor(sf::Color(0, 0, 0, 223));
    mRectangle.setSize(sf::Vector2f(tile->tileSize, tile->tileSize));

    // Draw Tetrominos
    for(int nTetromino = 0; nTetromino < 7; nTetromino++)
    {
        for(int y = 0; y < 4; y++)
        {
            // Draw all 7 Tetrominos, with their own color indicating selection, otherwise ghost color
            for(int x = 0; x < 4; x++)
            {
                wchar_t temp = tetromino->tetromino[nTetromino][0][y * 4 + x];

                if(temp != L'.')
                {
                    if(selectedTetrominos[nTetromino])
                        tile->mTile.setFillColor(tile->mColors[nTetromino]);
                    else
                        tile->mTile.setFillColor(tile->mColors[8]);
                }
                else
                {
                    sf::Color mCol = tile->mColors[7];

                    if(!selectedTetrominos[nTetromino])
                    {
                        mCol.r = mCol.r / 2;
                        mCol.g = mCol.g / 2;
                        mCol.b = mCol.b / 2;
                    }

                    tile->mTile.setFillColor( mCol );
                }

                int xPos = getContext().settings->hPadding + centerPadding + (x + nTetromino * 5) * tile->tileSize;
                int yPos = (14 + y) * tile->tileSize;

                mRectangle.setPosition( xPos, yPos );
                window.draw(mRectangle);

                tile->mTile.setPosition( xPos, yPos );
                window.draw(tile->mTile);
            }
        }
    }

    for(int i = 1; i < 3; i++) // Draw option texts
        mOptions[i]->draw(window);

    if(currentSelection == 0)
    {

        sf::RectangleShape cRectangle;
        cRectangle.setFillColor(sf::Color::White);
        cRectangle.setSize(sf::Vector2f(tile->tileSize * 4, tile->tileSize / 2));
        cRectangle.setPosition(getContext().settings->hPadding + centerPadding + (cursorTetromino * 5) * tile->tileSize,
                               18.2f * tile->tileSize);

        window.draw(cRectangle); // Draw cursor rectangle
    }
}


bool CustomBagState::update(sf::Time)
{
    mStarfield->update(); // Update the starfield
    waveText->update();

    return true;
}


void CustomBagState::checkTetromino()
{
    if(selectedTetrominos[cursorTetromino])
    {
        int amount = 0;

        for(int t = 0; t < 7; t++) // Check if this is the last one remaining
        {
            if(selectedTetrominos[t])
                amount++;
        }

        if(amount != 1) // It is not, ok to deselect
            selectedTetrominos[cursorTetromino] = !selectedTetrominos[cursorTetromino];
    }
    else
        selectedTetrominos[cursorTetromino] = !selectedTetrominos[cursorTetromino];
}


void CustomBagState::saveCustomBag()
{
    sRandomBag = "";

    for(int t = 0; t < 7; t++)
    {
        if(selectedTetrominos[t])
            sRandomBag += toString(t);
    }

    GameSettings &settings = *getContext().settings; // Save the random bag to settings
    settings.set("random-bag", sRandomBag);
    settings.load();

    requestStackPop();
    requestStackPush(States::CustomizeMenu);
}



bool CustomBagState::handleEvent(const sf::Event& event)
{
    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1))
    {
        requestStackPop();
        requestStackPush(States::CustomizeMenu);
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25))
    {
        mOptions[currentSelection]->setUnSelect();
        currentSelection++;
        if(currentSelection == 3)
            currentSelection = 0;

        mOptions[currentSelection]->setSelect();
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 25))
    {
        mOptions[currentSelection]->setUnSelect();
        currentSelection--;
        if(currentSelection == -1)
            currentSelection = 2;

        mOptions[currentSelection]->setSelect();
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -25))
    {
        if(currentSelection == 0)
        {
            cursorTetromino--;

            if(cursorTetromino < 0)
                cursorTetromino = 6;
        }
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > 25))
    {
        if(currentSelection == 0)
        {
            cursorTetromino++;

            if(cursorTetromino > 6)
                cursorTetromino = 0;
        }
    }


    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
        event.type == sf::Event::MouseButtonPressed || sf::Joystick::isButtonPressed(0, 0))
    {
        switch (currentSelection)
        {
            case 0:
                if(event.type == sf::Event::MouseButtonPressed)
                {
                    if(mouseSelected > -1 && mouseSelected < 7)
                        checkTetromino();
                }
                else
                    checkTetromino();

                break;

            case 1:
                saveCustomBag();
                break;

            case 2:
                requestStackPop();
                requestStackPush(States::CustomizeMenu);
                break;

            default:
                return false;
        }
    }


    if(event.type == sf::Event::MouseMoved)
    {
        mouseX = sf::Mouse::getPosition(*getContext().window).x;
        mouseY = sf::Mouse::getPosition(*getContext().window).y;
        mouseSelected = -1;

        //debugText = "MouseX: " + toString(mouseX) + "  MouseY: " + toString(mouseY) + "\n";

        if(mouseY > 14 * tile->tileSize && mouseY < 18 * tile->tileSize)
        {
            mOptions[currentSelection]->setUnSelect();
            currentSelection = 0;
            mOptions[currentSelection]->setSelect();

            for(int nTetromino = 0; nTetromino < 7; nTetromino++)
            {
                int padding = getContext().settings->hPadding;

                if(mouseX > padding + centerPadding + nTetromino * 5 * tile->tileSize &&
                   mouseX < padding + centerPadding + ((nTetromino + 1) * 5 - 1) * tile->tileSize)
                {
                    //debugText += "nTetromino: " + toString(nTetromino) + "\n";
                    cursorTetromino = nTetromino;

                    if(mouseSelected != nTetromino) // Hovering above a Tetromino currently
                        mouseSelected = nTetromino;

                    //debugText += "mouseSelected: " + toString(mouseSelected) + "\n";
                }
            }
        }

        for(int i = 1; i < 3; i++)
        {
            double top = getContext().window->getSize().y / 2 +
                         getContext().settings->cSize * 5.7 +
                         getContext().settings->cSize * i * 1.3;

            if(mouseY > top && mouseY < top + (1.0 * getContext().settings->cSize))
            {
                mOptions[currentSelection]->setUnSelect();
                currentSelection = i;
                mOptions[currentSelection]->setSelect();
            }
        }
    }
    return true;
}
