#ifndef APPLICATION_H
#define APPLICATION_H

#include <list>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include <resource/resource_holder.h>
#include <resource/resource_identifiers.h>
#include <state/state_stack.h>
#include "sound/music_player.h"
#include "sound/sound_player.h"
#include <settings/game_settings.h>

class Application
{
public:
    Application(GameSettings &gameSettings);

    void run();

private:
    void processInput();

    void update(sf::Time dt);

    void render();

    void updateStatistics(sf::Time dt);

    void registerStates();

private:
    static const sf::Time TimePerFrame;

    sf::RenderWindow mWindow;
    FontHolder mFonts;
    GameSettings mSettings;

    MusicPlayer mMusic;
    SoundPlayer mSounds;

    StateStack mStateStack;

    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    std::size_t mStatisticsNumFrames;

    sf::Text TAdjust;
    int cSize, padding;
    double nXShift = 0.0;

};

#endif //APPLICATION_H
