#include "game_selection_state.h"
#include "resource/resource_holder.h"

GameSelectionState::GameSelectionState(StateStack &stack, State::Context &context) : State(stack, context)
{
    sf::RenderWindow &window = *getContext().window;
    sf::Font &mainFont = context.fonts->get(Fonts::Prime);

    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    waveText = new WaveText(window, "NEW GAME");

    std::vector<std::string> zeroToNineteen = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19"};
    std::vector<std::string> zeroToFive = {"0", "1", "2", "3", "4", "5"};
    std::vector<std::string> aiLevels = {"Easy", "Medium", "Hard"};
    std::vector<std::string> linesVec = {"10", "25", "50", "80"};
    std::vector<std::string> gameModes = {"Marathon", "Line Clear", "Battle", "Sprint"};
    std::vector<std::string> playModes = {"Single-player", "Multiplayer"};
    std::vector<std::string> maps = {"None", "Preset A", "Preset B", "Preset C", "Preset D", "Preset E", "Preset F", "Preset G",
                                     "Preset H", "Preset I", "Preset J", "Preset K", "Preset L", "Custom A", "Custom B", "Custom C", "Custom D"};
    std::vector<std::string> randomBags = {"Default", "S & Z", "O, I, L & J", "Custom"};
    std::vector<std::string> onOff = {"On", "Off"};

    GameSettings &settings = *getContext().settings;
    yShift = sf::Vector2f(0.f, context.settings->cSize * 1.3f);


    // mItems[9]
    // 0 - Play Mode, 1 - Game Mode, 2 - Level, 3 - Height, 4 - Lines, 5 - VS CPU, 6 - CPU Lvl, 7 - Map, 8 - RandomBag
    // mOptions[2]
    // 0 - play, 1 - back

    // playMode
    mItems[0] = TextTogglePtr(new TextToggle("Play Mode", mainFont, playModes));
    mItems[0]->setSelectedByValue(playModeToString(settings.playMode));
    mItems[0]->setPosition(sf::Vector2f((context.settings->hPadding + (window.getSize().y / 3 * 4) * 0.15f ),
                    window.getSize().y / 2 - context.settings->cSize * 4.0), window);

    // gameMode
    mItems[1] = TextTogglePtr(new TextToggle("Game Mode", mainFont, gameModes));
    mItems[1]->setSelectedByValue(gameModeToString(settings.gameMode));
    mItems[1]->setPosition(mItems[0]->getPosition() + yShift, window);

    // level
    mItems[2] = TextTogglePtr(new TextToggle("Level", mainFont, zeroToNineteen));
    mItems[2]->setSelectedByValue(toString(settings.level));
    mItems[2]->setPosition(mItems[1]->getPosition() + yShift, window);

    // height
    mItems[3] = TextTogglePtr(new TextToggle("Height", mainFont, zeroToFive));
    mItems[3]->setSelectedByValue(toString(settings.height));

    // lines
    mItems[4] = TextTogglePtr(new TextToggle("Lines", mainFont, linesVec));
    mItems[4]->setSelectedByValue(toString(settings.lines));

    // vsAi
    mItems[5] = TextTogglePtr(new TextToggle("VS CPU", mainFont, onOff));
    mItems[5]->setSelectedByValue(boolToText(settings.vsCpu));

    // aiLevel
    mItems[6] = TextTogglePtr(new TextToggle("CPU Level", mainFont, aiLevels));
    mItems[6]->setSelectedIndex(settings.aiLevel);

    // map
    mItems[7] = TextTogglePtr(new TextToggle("Map", mainFont, maps));
    mItems[7]->setSelectedIndex(settings.map + 1);
    mItems[7]->setPosition(mItems[2]->getPosition() + yShift, window);

    // randomBag
    mItems[8] = TextTogglePtr(new TextToggle("Random Bag", mainFont, randomBags));
    mItems[8]->setSelectedIndex(settings.randomBagIndex); // getIndexForRandomBag(settings.randomBag));
    mItems[8]->setPosition(mItems[7]->getPosition() + yShift, window);


    // play
    mOptions[0] = TextButtonPtr(new TextButton("Play", mainFont));
    mOptions[0]->setPosition(sf::Vector2f(window.getSize().x / 2, mItems[0]->getPosition().y) +
                              sf::Vector2f(0.f, context.settings->cSize * 1.3f * 10.f));

    // back
    mOptions[1] = TextButtonPtr(new TextButton("Back", mainFont));
    mOptions[1]->setPosition(mOptions[0]->getPosition() + yShift);

    mItems[0]->setSelected();
    positionItems();


    TMessage.setFont(mainFont);
    TMessage.setCharacterSize(context.settings->cSize / 2);
    TMessage.setString("Blank");
    TMessage.setPosition( window.getSize().x / 2, context.settings->cSize * 7.0);
}


void GameSelectionState::positionItems()
{
    sf::RenderWindow &window = *getContext().window;

    int numberItems = optionOrder[mItems[0]->getSelectedIndex()][mItems[1]->getSelectedIndex()][10];
    int current, previous;

    for(int i = 3; i < numberItems; i++)
    {
        current = optionOrder[mItems[0]->getSelectedIndex()][mItems[1]->getSelectedIndex()][i];
        previous = optionOrder[mItems[0]->getSelectedIndex()][mItems[1]->getSelectedIndex()][i - 1];

        mItems[current]->setPosition(mItems[previous]->getPosition() + yShift, window);
    }
}


void GameSelectionState::applyChanges(GameSettings &settings)
{
    settings.players = mItems[0]->getSelectedIndex() + 1;
    settings.playMode = mItems[0]->getSelectedIndex() == 0 ? GameSettings::Mode::SinglePlayer : GameSettings::Mode::MultiPlayer;

    settings.gameType = mItems[1]->getSelectedIndex();
    switch(mItems[1]->getSelectedIndex())
    {
        case 0 : settings.gameMode = GameSettings::GameMode::Marathon; break;
        case 1 : settings.gameMode = GameSettings::GameMode::LineClear; break;
        case 2 : settings.gameMode = GameSettings::GameMode::Battle; break;
        case 3 : settings.gameMode = GameSettings::GameMode::Sprint; break;
    }

    settings.level = std::stoi(mItems[2]->getSelectedValue());
    settings.height = std::stoi(mItems[3]->getSelectedValue());
    settings.lines = std::stoi(mItems[4]->getSelectedValue());
    settings.vsCpu = textToBool(mItems[5]->getSelectedValue());
    settings.aiLevel = mItems[6]->getSelectedIndex();
    settings.map = mItems[7]->getSelectedIndex() - 1;
    settings.randomBagIndex = mItems[8]->getSelectedIndex();
}


void GameSelectionState::draw()
{
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // Draw the starfield first
    waveText->draw(window, getContext().settings->cSize * 2);


    mItems[0]->draw(window); // playMode
    mItems[1]->draw(window); // gameMode
    mItems[2]->draw(window); // level

    mItems[7]->draw(window); // map
    mItems[8]->draw(window); // randomBag

    mOptions[0]->draw(window); // play
    mOptions[1]->draw(window); // back

    if(mItems[1]->getSelectedIndex() == 1) // Line Clear
    {
        mItems[3]->draw(window); // height
        mItems[4]->draw(window); // lines
    }

    if(mItems[0]->getSelectedIndex() == 1) // Multiplayer
    {
        mItems[5]->draw(window); // vs CPU
        mItems[6]->draw(window); // CPU lvl
    }

    sMessage = "";

    if(mItems[7]->isSelected() && mItems[0]->getSelectedIndex() == 0)
        sMessage = "Note: You are uneligible for the high score list when playing with a map.";

    if(mItems[8]->isSelected() && mItems[0]->getSelectedIndex() == 0)
        sMessage = "Note: You are only eligible for the high score list when playing with the default random bag.";

    TMessage.setString(sMessage);
    centerOriginHorizontal(TMessage);
    window.draw(TMessage);

}


bool GameSelectionState::update(sf::Time dt)
{
    mStarfield->update(); // Update the starfield
    waveText->update();

    return true;
}


void GameSelectionState::horizontalAction(int playMode, int gameMode, int direction)
{
    int optionIndex;

    if(currentSelection < 9)
    {
        optionIndex = optionOrder[playMode][gameMode][currentSelection];

        if(direction == -1) mItems[optionIndex]->goPrevious();
        else mItems[optionIndex]->goNext();

        if(optionIndex < 2)
        {
            if (mItems[0]->getSelectedIndex() == 1 && mItems[1]->getSelectedIndex() == 3)
            {
                if(direction == -1) mItems[1]->goPrevious();
                else mItems[1]->goNext();
            }

            if (mItems[0]->getSelectedIndex() == 0 && mItems[1]->getSelectedIndex() == 2)
            {
                if(direction == -1) mItems[1]->goPrevious();
                else mItems[1]->goNext();
            }
        }
        positionItems();
    }
}


bool GameSelectionState::handleEvent(const sf::Event &event)
{
    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1))
    {
        requestStackPop();
        requestStackPush(States::MainMenu);
    }

    int playMode = mItems[0]->getSelectedIndex();
    int gameMode = mItems[1]->getSelectedIndex();
    int optionIndex;

    if(event.type == sf::Event::MouseMoved)
    {
        mouseX = sf::Mouse::getPosition(*getContext().window).x;
        mouseY = sf::Mouse::getPosition(*getContext().window).y;

        directionX = -1;
        hoverY = -1;

        for(int i = 0; i < 12; i++)
        {
            double top = getContext().window->getSize().y / 2 -
                         getContext().settings->cSize * 4.0 +
                         getContext().settings->cSize * i * 1.3;

            if(mouseY > top && mouseY < top + (1.0 * getContext().settings->cSize))
            {
                hoverY = i;

                if(currentSelection < 3)
                    mItems[currentSelection]->setUnSelected();
                else if(currentSelection > 2 && currentSelection < 9)
                {
                    optionIndex = optionOrder[playMode][gameMode][currentSelection];

                    if(optionIndex != -1)
                        mItems[optionIndex]->setUnSelected();
                }
                else
                    mOptions[currentSelection - 9]->setUnSelect();

                if(i < 10)
                {
                    optionIndex = optionOrder[playMode][gameMode][i];

                    if(optionIndex != -1)
                        currentSelection = i;
                }
                else
                    currentSelection = i - 1;

                if(currentSelection < 9)
                {
                    optionIndex = optionOrder[playMode][gameMode][currentSelection];
                    mItems[optionIndex]->setSelected();
                }
                else
                    mOptions[currentSelection - 9]->setSelect();


                // Check if mouse is hovering above previous or next buttons

                double left = getContext().window->getSize().x / 2;
                double right = getContext().window->getSize().x -
                               (getContext().settings->hPadding + (getContext().window->getSize().y / 3 * 4) * 0.15f);

                if(mouseX > left && mouseX < left + (right - left) / 2 )
                    directionX = 0; // Hovering above previous

                if(mouseX > left + (right - left) / 2 && mouseX < right )
                    directionX = 1; // Hovering above next
            }
        }
    }


    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
        event.type == sf::Event::MouseButtonPressed || sf::Joystick::isButtonPressed(0, 0))
    {
        if (mOptions[0]->isSelected()) // play
        {
            applyChanges(*getContext().settings);
            requestStackPop();
            requestStackPush(States::Game);
            return true;
        }

        if (mOptions[1]->isSelected()) // back
        {
            requestStackPop();
            requestStackPush(States::MainMenu);
            return true;
        }

        if(currentSelection < 9 && event.type == sf::Event::MouseButtonPressed)
        {
            if(hoverY == currentSelection) // Hovering above a valid item, check if we can click previous/next
            {
                if(directionX == 0)
                    horizontalAction(playMode, gameMode, -1);
                else if(directionX == 1)
                    horizontalAction(playMode, gameMode, 1);

                return true;
            }
        }
        return false;
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25))
    {
        if(currentSelection < 9)
        {
            optionIndex = optionOrder[playMode][gameMode][currentSelection];
            mItems[optionIndex]->setUnSelected();

            if(optionOrder[playMode][gameMode][currentSelection + 1] != -1) // Items remaining for this playMode/gameMode?
                currentSelection++; // Yes
            else
                currentSelection = 9; // No, proceed to play button
        }
        else
        {
            mOptions[currentSelection - 9]->setUnSelect();

            currentSelection++;

            if(currentSelection > 10) // Wrap around at bottom
                currentSelection = 0;
        }

        if(currentSelection < 9)
        {
            optionIndex = optionOrder[playMode][gameMode][currentSelection];
            mItems[optionIndex]->setSelected();
        }
        else
            mOptions[currentSelection - 9]->setSelect();
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 25))
    {
        if(currentSelection < 9)
        {
            optionIndex = optionOrder[playMode][gameMode][currentSelection];
            mItems[optionIndex]->setUnSelected();

            if(currentSelection > 0)
                currentSelection--;
            else currentSelection = 10;
        }
        else
        {
            mOptions[currentSelection - 9]->setUnSelect();

            if(currentSelection == 9) // play
                currentSelection = optionOrder[playMode][gameMode][10] - 1;
            else
                currentSelection--;
        }

        if(currentSelection < 9)
        {
            optionIndex = optionOrder[playMode][gameMode][currentSelection];
            mItems[optionIndex]->setSelected();
        }
        else
            mOptions[currentSelection - 9]->setSelect();
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -25))
    {
        horizontalAction(playMode, gameMode, -1);
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > 25))
    {
        horizontalAction(playMode, gameMode, 1);
    }
}
