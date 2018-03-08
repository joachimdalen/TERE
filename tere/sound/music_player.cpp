#include <iostream>
#include "music_player.h"

MusicPlayer::MusicPlayer()
        : mMusic()
        , mFilenames()
        , mVolume(100.f)
{
    mFilenames[Music::MenuTheme] = "media/music/menu.ogg";
    mFilenames[Music::GameTheme] = "media/music/game.ogg";
}


void MusicPlayer::play(Music::ID theme)
{
    std::string filename = mFilenames[theme];

    // Keep it from restarting the menu track when returning to the Main Menu from sub menus
    if(mCurrentFile != filename)
    {
        if (!mMusic.openFromFile(filename))
            throw std::runtime_error("Music " + filename + " could not be loaded.");

        mMusic.setLoop(true);
        mMusic.play();

        mCurrentFile = filename;
    }
}


void MusicPlayer::stop()
{
    mMusic.stop();
}


// TODO Music Volume needs to be implemented in settings
void MusicPlayer::setVolume(float volume)
{
    mVolume = volume * 2.f; // Max volume is 50%, but it is still plenty
    mMusic.setVolume(mVolume);
}


void MusicPlayer::setPaused(bool paused)
{
    if (paused)
        mMusic.pause();
    else
        mMusic.play();
}
