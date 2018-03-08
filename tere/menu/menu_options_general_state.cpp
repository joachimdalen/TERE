#include <iostream>

#include "menu_options_general_state.h"
#include "resource/resource_holder.h"
#include "sound/music_player.h"
#include "sound/sound_player.h"

MenuOptionsGeneralState::MenuOptionsGeneralState(StateStack &stack, State::Context &context) : State(stack, context)
{
    sf::Font &mainFont = context.fonts->get(Fonts::Prime);
    sf::RenderWindow &window = *getContext().window;
    sf::Vector2f size = getContext().window->getView().getSize();

    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    waveText = new WaveText(window, "GENERAL OPTIONS");

    std::vector<std::string> onOff = {"On", "Off"};
    std::vector<std::string> zeroToTen = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    std::vector<std::string> oneToFour = {"1", "2", "3", "4"};
    std::vector<std::string> res;
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

    for (std::size_t i = 0; i < modes.size(); ++i)
    {
        sf::VideoMode mode = modes[i];
        res.emplace_back(toString(mode.width) + toString("x") + toString(mode.height));
    }

    std::reverse(res.begin(), res.end());
    GameSettings &settings = *getContext().settings;

    spacing = sf::Vector2f(0.f, context.settings->cSize * 1.3f);

    // Toggle
    // 0 - sound effects,   1 - music,      2 - sfx volume,     3 - music volume,       4 - resolution,
    // 5 - anti-alias,      6 - trails,     7 - tr. intensity,  8 - tr. delay,          9 - starfield
    // Buttons
    // 0 - save,            1 - back

    selectionToggle[0] = TextTogglePtr(new TextToggle("Sound Effects", mainFont, onOff));
    selectionToggle[0]->setSelectedByValue(boolToText(settings.soundEffectsEnabled));
    selectionToggle[0]->setPosition(sf::Vector2f(
            (context.settings->hPadding + (window.getSize().y / 3 * 4) * 0.15f ),
            window.getSize().y / 2 - context.settings->cSize * 5.0  ), window);

    selectionToggle[1] = TextTogglePtr(new TextToggle("Music", mainFont, onOff));
    selectionToggle[1]->setSelectedByValue(boolToText(settings.musicEnabled));
    selectionToggle[1]->setPosition(selectionToggle[0]->getPosition() + spacing, window);

    selectionToggle[2] = TextTogglePtr(new TextToggle("Sound Volume", mainFont, zeroToTen));
    selectionToggle[2]->setSelectedByValue(toString(settings.soundVolume));
    selectionToggle[2]->setPosition(selectionToggle[1]->getPosition() + spacing, window);

    selectionToggle[3] = TextTogglePtr(new TextToggle("Music Volume", mainFont, zeroToTen));
    selectionToggle[3]->setSelectedByValue(toString(settings.musicVolume));
    selectionToggle[3]->setPosition(selectionToggle[2]->getPosition() + spacing, window);

    selectionToggle[4] = TextTogglePtr(new TextToggle("Screen Resolution", mainFont, res));
    selectionToggle[4]->setSelectedByValue(toString(settings.screenWidth) + toString("x") + toString(settings.screenHeight));
    selectionToggle[4]->setPosition(selectionToggle[3]->getPosition() + spacing, window);

    selectionToggle[5] = TextTogglePtr(new TextToggle("Anti-alias", mainFont, onOff));
    selectionToggle[5]->setSelectedByValue(boolToText(settings.antiAliasEnabled));
    selectionToggle[5]->setPosition(selectionToggle[4]->getPosition() + spacing, window);

    selectionToggle[6] = TextTogglePtr(new TextToggle("Trails", mainFont, onOff));
    selectionToggle[6]->setSelectedByValue(boolToText(settings.trailsEnabled));
    selectionToggle[6]->setPosition(selectionToggle[5]->getPosition() + spacing, window);

    selectionToggle[7] = TextTogglePtr(new TextToggle("Trails Intensity", mainFont, oneToFour));
    selectionToggle[7]->setSelectedByValue(toString(settings.trailsIntensity));
    selectionToggle[7]->setPosition(selectionToggle[6]->getPosition() + spacing, window);

    selectionToggle[8] = TextTogglePtr(new TextToggle("Trails Delay", mainFont, oneToFour));
    selectionToggle[8]->setSelectedByValue(toString(settings.trailsDelay));
    selectionToggle[8]->setPosition(selectionToggle[7]->getPosition() + spacing, window);

    selectionToggle[9] = TextTogglePtr(new TextToggle("Starfield", mainFont, onOff));
    selectionToggle[9]->setSelectedByValue(boolToText(settings.starfieldEnabled));
    selectionToggle[9]->setPosition(selectionToggle[8]->getPosition() + spacing, window);

    selectionToggle[0]->setSelected();


    selectionButton[0] = TextButtonPtr(new TextButton("Apply Changes", mainFont));
    selectionButton[0]->setPosition(sf::Vector2f( window.getSize().x / 2, selectionToggle[9]->getPosition().y) + spacing + spacing);

    selectionButton[1] = TextButtonPtr(new TextButton("Back", mainFont));
    selectionButton[1]->setPosition(selectionButton[0]->getPosition() + spacing);

    TMessage.setFont(mainFont);
    TMessage.setCharacterSize(context.settings->cSize / 2);
    TMessage.setString("");
    TMessage.setPosition( window.getSize().x / 2, context.settings->cSize * 6);
}


void MenuOptionsGeneralState::draw()
{
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // draw the starfield first
    waveText->draw(window, getContext().settings->cSize * 2);

    if(timerMessage.getElapsedTime().asSeconds() > 1)
    {
        sMessage = "";

        if(currentSelection == 4)
        {
            int nWidth = stoi( split(selectionToggle[4]->getSelectedValue(), 'x')[0] );
            int nHeight = stoi( split(selectionToggle[4]->getSelectedValue(), 'x')[1] );

            sMessage = "Note: A restart is required after applying changes to the screen resolution.";

            if(nWidth <= nHeight) // Quick fix if user attempts to set a portrait resolution
            {
                sMessage = "Note: Portrait resolutions are not yet supported. Defaulting to 800x600.";
                bInvalidResolution = true;
            }
        }

        if(currentSelection == 5)
            sMessage = "Note: A restart is required after applying changes to the anti-alias settings.";

    }

    TMessage.setString(sMessage);
    centerOriginHorizontal(TMessage);
    window.draw(TMessage);

    for(int i = 0; i < 10; i++)
        selectionToggle[i]->draw(window);

    for(int i = 0; i < 2; i++)
        selectionButton[i]->draw(window);
}


bool MenuOptionsGeneralState::update(sf::Time dt)
{
    mStarfield->update(); // Update the starfield
    waveText->update();

    return true;
}


bool MenuOptionsGeneralState::handleEvent(const sf::Event &event)
{
    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1))
    {
        requestStackPop();
        requestStackPush(States::MenuOptions);
    }


    if(event.type == sf::Event::MouseMoved)
    {
        mouseX = sf::Mouse::getPosition(*getContext().window).x;
        mouseY = sf::Mouse::getPosition(*getContext().window).y;

        directionX = -1;

        for(int i = 0; i < 13; i++)
        {
            double top = getContext().window->getSize().y / 2 -
                    getContext().settings->cSize * 5.0 +
                         getContext().settings->cSize * i * 1.3;

            if(mouseY > top && mouseY < top + (1.0 * getContext().settings->cSize))
            {
                if(currentSelection < 10)
                    selectionToggle[currentSelection]->setUnSelected();
                else
                    selectionButton[currentSelection - 10]->setUnSelect();

                if(i < 10)
                    currentSelection = i;
                else
                    currentSelection = i - 1;


                if(currentSelection < 10)
                    selectionToggle[currentSelection]->setSelected();
                else
                    selectionButton[currentSelection - 10]->setSelect();


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
        if(currentSelection < 10 && event.type == sf::Event::MouseButtonPressed)
        {
            if(directionX == 0)
                selectionToggle[currentSelection]->goPrevious();
            else if(directionX == 1)
                selectionToggle[currentSelection]->goNext();

        }

        if (currentSelection == 11) // Back
        {
            requestStackPop();
            requestStackPush(States::MenuOptions);
        }

        if (currentSelection == 10) // Apply settings
        {
            // Toggle
            // 0 - sound effects,   1 - music,      2 - sfx volume,     3 - music volume,       4 - resolution,
            // 5 - anti-alias,      6 - trails,     7 - tr. intensity,  8 - tr. delay,          9 - starfield

            // Buttons
            // 0 - save,            1 - back

            GameSettings &settings = *getContext().settings;
            settings.set("sound-effects-enabled", textToBool(selectionToggle[0]->getSelectedValue()));
            settings.set("music-enabled", textToBool(selectionToggle[1]->getSelectedValue()));
            settings.set("sound-volume", selectionToggle[2]->getSelectedValue());
            settings.set("music-volume", selectionToggle[3]->getSelectedValue());

            if(!bInvalidResolution)
            {
                settings.set("screen-width", split(selectionToggle[4]->getSelectedValue(), 'x')[0]);
                settings.set("screen-height", split(selectionToggle[4]->getSelectedValue(), 'x')[1]);
            }
            else // Quick fix if user attempts to set a portrait resolution
            {
                std::string nWidth = "800", nHeight = "600";
                settings.set("screen-width", nWidth);
                settings.set("screen-height", nHeight);
            }

            settings.set("anti-alias-enabled", textToBool(selectionToggle[5]->getSelectedValue()));
            settings.set("trails-enabled", textToBool(selectionToggle[6]->getSelectedValue()));
            settings.set("trails-intensity", selectionToggle[7]->getSelectedValue());
            settings.set("trails-delay", selectionToggle[8]->getSelectedValue());
            settings.set("starfield-enabled", textToBool(selectionToggle[9]->getSelectedValue()));
            settings.load();

            // Set the music and sound volume now
            //getContext().sounds->setVolume( getContext().settings->soundVolume );
            //getContext().music->setVolume( getContext().settings->musicVolume );

            if(!getContext().settings->soundEffectsEnabled)
                getContext().sounds->setVolume(0.f);
            else
                getContext().sounds->setVolume( getContext().settings->soundVolume );


            if(!getContext().settings->musicEnabled)
                getContext().music->setVolume(0.f);
            else
                getContext().music->setVolume(getContext().settings->musicVolume);





            sMessage = "Settings saved.";
            timerMessage.restart();




        }
    }

    // Toggle
    // 0 - sound effects,   1 - music,      2 - sfx volume,     3 - music volume,       4 - resolution,
    // 5 - anti-alias,      6 - trails,     7 - tr. intensity,  8 - tr. delay,          9 - starfield
    // Buttons
    // 0 - save,            1 - back

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25))
    {
        if(currentSelection < 10)
            selectionToggle[currentSelection]->setUnSelected();
        else
            selectionButton[currentSelection - 10]->setUnSelect();

        currentSelection++;

        if(currentSelection == 12)
            currentSelection = 0;

        if(currentSelection < 10)
            selectionToggle[currentSelection]->setSelected();
        else
            selectionButton[currentSelection - 10]->setSelect();
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 25))
    {
        if(currentSelection < 10)
            selectionToggle[currentSelection]->setUnSelected();
        else
            selectionButton[currentSelection - 10]->setUnSelect();

        currentSelection--;
        if(currentSelection == -1)
            currentSelection = 11;

        if(currentSelection < 10)
            selectionToggle[currentSelection]->setSelected();
        else
            selectionButton[currentSelection - 10]->setSelect();
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -25))
    {
        if(currentSelection < 10)
            selectionToggle[currentSelection]->goPrevious();
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > 25))
    {
        if(currentSelection < 10)
            selectionToggle[currentSelection]->goNext();
    }
}
