#include <utility/utility.h>
#include "menu_options_control_state.h"
#include "resource/resource_holder.h"

MenuOptionsControlsState::MenuOptionsControlsState(StateStack &stack, State::Context &context) : State(stack, context)
{
    sf::Font &mainFont = context.fonts->get(Fonts::Prime);
    sf::RenderWindow &window = *getContext().window;
    sf::Vector2f size = getContext().window->getView().getSize();

    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    waveText = new WaveText(window, "RECONFIGURE CONTROLS");

    this->keyMapInProgress = false;

    subHeader.setFont(mainFont);
    subHeader.setCharacterSize( context.settings->cSize / 2 );
    subHeader.setString(sMessage);
    centerOriginHorizontal(subHeader);
    subHeader.setPosition(window.getSize().x / 2, context.settings->cSize * 5.0 );

    float space = (GameSettings::cSize * 1.3);
    float posX = (context.settings->hPadding + (window.getSize().y / 3 * 4) * 0.15f );

    GameSettings &settings = *getContext().settings;

    int maxSize = context.settings->cSize;

    // mKeys
    // 0 - plLeft       5 - p2Left
    // 1 - plRight      6 - p2Right
    // 2 - plDown       7 - p2Down
    // 3 - plRotateCW   8 - p2 CW
    // 4 - plRotateCCW  9 - p2 CCW

    // mOptions
    // 0 - save         1 - back

    mKeys[0] = TextKeyMapperPtr(new TextKeyMapper("Player 1 - Move Left", mainFont, settings.playerOneKeyMap->Left));
    mKeys[0]->setPosition(sf::Vector2f(posX, window.getSize().y / 2 - maxSize * 6.0), window);

    mKeys[1] = TextKeyMapperPtr(new TextKeyMapper("Player 1 - Move Right", mainFont, settings.playerOneKeyMap->Right));
    mKeys[1]->setPosition(sf::Vector2f(posX, mKeys[0]->getPosition().y + space), window);

    mKeys[2] = TextKeyMapperPtr(new TextKeyMapper("Player 1 - Soft Drop", mainFont, settings.playerOneKeyMap->Down));
    mKeys[2]->setPosition(sf::Vector2f(posX, mKeys[1]->getPosition().y + space), window);

    mKeys[3] = TextKeyMapperPtr(new TextKeyMapper("Player 1 - Rotate CW", mainFont, settings.playerOneKeyMap->RotateClockwise));
    mKeys[3]->setPosition(sf::Vector2f(posX, mKeys[2]->getPosition().y + space), window);

    mKeys[4] = TextKeyMapperPtr(new TextKeyMapper("Player 1 - Rotate CCW", mainFont, settings.playerOneKeyMap->RotateCounterClockwise));
    mKeys[4]->setPosition(sf::Vector2f(posX, mKeys[3]->getPosition().y + space), window);

    mKeys[5] = TextKeyMapperPtr(new TextKeyMapper("Player 2 - Move Left", mainFont, settings.playerTwoKeyMap->Left));
    mKeys[5]->setPosition(sf::Vector2f(posX, mKeys[4]->getPosition().y + space), window);

    mKeys[6] = TextKeyMapperPtr(new TextKeyMapper("Player 2 - Move Right", mainFont, settings.playerTwoKeyMap->Right));
    mKeys[6]->setPosition(sf::Vector2f(posX, mKeys[5]->getPosition().y + space), window);

    mKeys[7] = TextKeyMapperPtr(new TextKeyMapper("Player 2 - Soft Drop", mainFont, settings.playerTwoKeyMap->Down));
    mKeys[7]->setPosition(sf::Vector2f(posX, mKeys[6]->getPosition().y + space), window);

    mKeys[8] = TextKeyMapperPtr(new TextKeyMapper("Player 2 - Rotate CW", mainFont, settings.playerTwoKeyMap->RotateClockwise));
    mKeys[8]->setPosition(sf::Vector2f(posX, mKeys[7]->getPosition().y + space), window);

    mKeys[9] = TextKeyMapperPtr(new TextKeyMapper("Player 2 - Rotate CCW", mainFont, settings.playerTwoKeyMap->RotateCounterClockwise));
    mKeys[9]->setPosition(sf::Vector2f(posX, mKeys[8]->getPosition().y + space), window);

    mKeys[10] = TextKeyMapperPtr(new TextKeyMapper("Pause Game", mainFont, settings.playerOneKeyMap->PauseGame));
    mKeys[10]->setPosition(sf::Vector2f(posX, mKeys[9]->getPosition().y + space), window);




    mOptions[0] = TextButtonPtr(new TextButton("Apply Changes", mainFont));
    mOptions[0]->setPosition(sf::Vector2f((context.window->getView().getSize().x / 2.f), mKeys[10]->getPosition().y + space + space));

    mOptions[1] = TextButtonPtr(new TextButton("Back", mainFont));
    mOptions[1]->setPosition(mOptions[0]->getPosition() + sf::Vector2f(0.f, space));

    mKeys[0]->setSelect();


    // Make sprites for the controller icons

    controllerTextures[0].loadFromFile("media/controllerLeft.png");
    controllerTextures[1].loadFromFile("media/controllerRight.png");
    controllerTextures[2].loadFromFile("media/controllerDown.png");
    controllerTextures[3].loadFromFile("media/controllerB.png"); // CW
    controllerTextures[4].loadFromFile("media/controllerA.png"); // CCW
    controllerTextures[5].loadFromFile("media/controllerStart.png"); // CCW

    for(int i = 0; i < 6; i++)
    {
        controllerTextures[i].setSmooth(true);

        controllerSprite[i].setTexture( controllerTextures[i] );
        controllerSprite[i].setScale(sf::Vector2f( context.settings->cSize / 100.f, context.settings->cSize / 100.f) );
        controllerSprite[i].setPosition( mKeys[i]->getPosition() - sf::Vector2f(context.settings->cSize * 2, 0.f));
    }

    controllerSprite[5].setPosition( mKeys[10]->getPosition() - sf::Vector2f(context.settings->cSize * 2, 0.f));


}


void MenuOptionsControlsState::draw()
{
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // draw the starfield first
    waveText->draw(window, getContext().settings->cSize * 2);

    if(timerMessage.getElapsedTime().asSeconds() > 1 && !keyMapInProgress)
        subHeader.setString(sMessage);

    centerOriginHorizontal(subHeader);
    window.draw(subHeader);


    for(int i = 0; i < 11; i++) // Draw the configurable keys texts
        mKeys[i]->draw(window);

    for(int i = 0; i < 6; i++) // Draw the controller sprites
        window.draw(controllerSprite[i]);

    for(int i = 0; i < 2; i++) // Draw option texts
        mOptions[i]->draw(window);
}


bool MenuOptionsControlsState::update(sf::Time dt)
{
    mStarfield->update(); // Update the starfield
    waveText->update();
    return true;
}


bool MenuOptionsControlsState::handleEvent(const sf::Event &event)
{
    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1))
    {
        requestStackPop();
        requestStackPush(States::MenuOptions);
    }

    // mKeys
    // 0 - plLeft       5 - p2Left
    // 1 - plRight      6 - p2Right
    // 2 - plDown       7 - p2Down
    // 3 - plRotateCW   8 - p2 CW
    // 4 - plRotateCCW  9 - p2 CCW

    // mOptions
    // 0 - save         1 - back

    if(event.type == sf::Event::MouseMoved && !keyMapInProgress)
    {
        mouseY = sf::Mouse::getPosition(*getContext().window).y;

        for(int i = 0; i < 14; i++)
        {
            double top = getContext().window->getSize().y / 2 -
                         getContext().settings->cSize * 6.0 +
                         getContext().settings->cSize * i * 1.3;

            if(mouseY > top && mouseY < top + (1.0 * getContext().settings->cSize))
            {
                if(currentSelection < 11)
                    mKeys[currentSelection]->setUnSelect();
                else
                    mOptions[currentSelection - 11]->setUnSelect();

                if(i < 11)
                    currentSelection = i;
                else
                    currentSelection = i - 1;


                if(currentSelection < 11)
                    mKeys[currentSelection]->setSelect();
                else
                    mOptions[currentSelection - 11]->setSelect();
            }
        }
    }


    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
        event.type == sf::Event::MouseButtonPressed || sf::Joystick::isButtonPressed(0, 0))
    {
        if (currentSelection == 11)
        {
            GameSettings &settings = *getContext().settings;
            settings.set("player-one-left", mKeys[0]->getValueString());
            settings.set("player-one-right", mKeys[1]->getValueString());
            settings.set("player-one-down", mKeys[2]->getValueString());
            settings.set("player-one-rotate-clockwise", mKeys[3]->getValueString());
            settings.set("player-one-rotate-counter-clockwise", mKeys[4]->getValueString());

            settings.set("player-two-left", mKeys[5]->getValueString());
            settings.set("player-two-right", mKeys[6]->getValueString());
            settings.set("player-two-down", mKeys[7]->getValueString());
            settings.set("player-two-rotate-clockwise", mKeys[8]->getValueString());
            settings.set("player-two-rotate-counter-clockwise", mKeys[9]->getValueString());

            settings.set("player-one-pause-game", mKeys[10]->getValueString());

            settings.load();

            subHeader.setString("Settings saved.");
            timerMessage.restart();
        }
        else if (currentSelection == 12)
        {
            requestStackPop();
            requestStackPush(States::MenuOptions);
        }
        else
        {
            subHeader.setString("Press the key you want for this action.");
            keyMapInProgress = true;
        }

        return true;
    }

    if (!keyMapInProgress)
    {

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down ||
            (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25))
        {
            if(currentSelection < 11)
                mKeys[currentSelection]->setUnSelect();
            else
                mOptions[currentSelection - 11]->setUnSelect();

            currentSelection++;

            if(currentSelection == 13)
                currentSelection = 0;

            if(currentSelection < 11)
                mKeys[currentSelection]->setSelect();
            else
                mOptions[currentSelection - 11]->setSelect();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up ||
            (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 25))
        {
            if(currentSelection < 11)
                mKeys[currentSelection]->setUnSelect();
            else
                mOptions[currentSelection - 11]->setUnSelect();

            currentSelection--;
            if(currentSelection == -1)
                currentSelection = 12;

            if(currentSelection < 11)
                mKeys[currentSelection]->setSelect();
            else
                mOptions[currentSelection - 11]->setSelect();
        }
    } else
    {
        if (event.type != sf::Event::KeyPressed)
            return false;

        mKeys[currentSelection]->setKey(event.key.code);

        // Check the rest of the keys in case this is a duplicate
        for(int i = 0; i < 11; i++)
        {
            if(i == currentSelection)
                continue;

            if(mKeys[i]->getValue() == event.key.code)
                mKeys[i]->setKey(sf::Keyboard::Unknown);
        }

        subHeader.setString(sMessage);
        keyMapInProgress = false;
    }
}
