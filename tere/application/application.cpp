#include <SFML/OpenGL.hpp>

#include "application.h"
#include <utility/utility.h>
#include <game/game_state.h>
//#include <SFML/Audio.hpp>
#include <menu/main_menu_state.h>
#include <menu/menu_options_state.h>
#include <menu/menu_options_control_state.h>
#include <menu/menu_options_general_state.h>
#include <menu/game_selection_state.h>

#include <custom/custom_bag_state.h>
#include <custom/custom_map_state.h>
#include <custom/custom_menu_state.h>
#include <credits/credits_state.h>
#include <highscore/highscore_display_state.h>
#include <highscore/highscore_entry_state.h>

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application(GameSettings &gameSettings)
        //: mWindow(sf::VideoMode(320, 200), "Tetris Revisited", sf::Style::None, sf::ContextSettings(24, 8, gameSettings.antiAliasEnabled ? 4 : 0))
        //: mWindow(sf::VideoMode(3840, 2160), "Tetris Revisited", sf::Style::None, sf::ContextSettings(24, 8, gameSettings.antiAliasEnabled ? 4 : 0))
        : mWindow(sf::VideoMode(gameSettings.screenWidth, gameSettings.screenHeight), "Tetris Revisited", sf::Style::None, sf::ContextSettings(24, 8, gameSettings.antiAliasEnabled ? 4 : 0))
        , mFonts()
        , mMusic()
        , mSounds()
        , mSettings(gameSettings)
        , mStateStack(State::Context(mWindow, mFonts, mSettings, mMusic, mSounds))
        , mStatisticsText()
        , mStatisticsUpdateTime()
        , mStatisticsNumFrames(0)
{
    mWindow.setKeyRepeatEnabled(false);

/*
    // The release will have an icon built in for the .exe file, making this code obsolete

    // Load an icon for the app
    sf::Image icon;
    icon.loadFromFile("media/icon.png");
    mWindow.setIcon(128, 128, icon.getPixelsPtr());
*/

    // Limit the framerate to 60 frames per second (this step is optional) -- These settings do not play well with our timestep method below
    //mWindow.setFramerateLimit(60);
    //mWindow.setVerticalSyncEnabled(true);

    mFonts.load(Fonts::Prime, "media/prime_regular.otf");
    mFonts.load(Fonts::Fixed, "media/prime_fixed.otf");
    //mFonts.load(Fonts::Fixed, "media/inconsolata.otf");

    mStatisticsText.setFont(mFonts.get(Fonts::Prime));
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(20);

    mSettings.load();


    GameSettings::cSize = 2.f;

    TAdjust.setFont(mFonts.get(Fonts::Prime));
    TAdjust.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    float height = 0.0f;

    while (height < (mWindow.getSize().y / 25)) // In this case, we want to be able to fit around 25 lines in the window
    {
        GameSettings::cSize += 0.1f;
        TAdjust.setCharacterSize(GameSettings::cSize);
        height = TAdjust.getGlobalBounds().height;
    }

    TAdjust.setFont(mFonts.get(Fonts::Fixed));

    height = 0.0f;

    while (height < (mWindow.getSize().y / 25)) // In this case, we want to be able to fit around 25 lines in the window
    {
        GameSettings::cSizeFixed += 0.1f;
        TAdjust.setCharacterSize(GameSettings::cSizeFixed);
        height = TAdjust.getGlobalBounds().height;
    }


    double aspect = mWindow.getSize().x * 1.0 / mWindow.getSize().y * 1.0;
    GameSettings::hPadding = 0;

    if( aspect > 1.33334)
        GameSettings::hPadding = (mWindow.getSize().x - (mWindow.getSize().y * 4 / 3)) / 2;

    if(gameSettings.soundEffectsEnabled)
        mSounds.setVolume( gameSettings.soundVolume );
    else
        mSounds.setVolume( 0.f );

    if(gameSettings.musicEnabled)
        mMusic.setVolume( gameSettings.musicVolume );
    else
        mMusic.setVolume( 0.f );

    registerStates();
    mStateStack.pushState(States::MainMenu);
}


void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;

        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            // Check inside this loop, because stack might be empty before update() call
            if (mStateStack.isEmpty())
                mWindow.close();
        }

        updateStatistics(dt);
        render();
    }
}


void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}


void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}


void Application::render()
{
    mWindow.clear();

    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);

    mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
    mStatisticsUpdateTime += dt;
    mStatisticsNumFrames += 1;

    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        // mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));
        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}


void Application::registerStates()
{
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<MainMenuState>(States::MainMenu);
    mStateStack.registerState<MenuOptionState>(States::MenuOptions);
    mStateStack.registerState<MenuOptionsControlsState>(States::MenuControls);
    mStateStack.registerState<MenuOptionsGeneralState>(States::MenuGeneral);
    mStateStack.registerState<GameSelectionState>(States::MenuSelection);
    mStateStack.registerState<CustomMenuState>(States::CustomizeMenu);
    mStateStack.registerState<CustomBagState>(States::CustomizeBag);
    mStateStack.registerState<CustomMapState>(States::CustomizeMap);
    mStateStack.registerState<CreditsState>(States::Credits);
    mStateStack.registerState<HighScoreDisplayState>(States::HighScoreDisplay);
    mStateStack.registerState<HighScoreEntryState>(States::HighScoreEntry);
}
