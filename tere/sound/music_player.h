#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <map>
#include <string>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Music.hpp>

#include "resource/resource_holder.h"
#include "resource/resource_identifiers.h"

class MusicPlayer : private sf::NonCopyable
{
public:
    MusicPlayer();

    void						play(Music::ID theme);
    void						stop();

    void						setPaused(bool paused);
    void						setVolume(float volume);


private:
    sf::Music							mMusic;
    std::map<Music::ID, std::string>	mFilenames;
    float								mVolume;

    std::string                         mCurrentFile;

};

#endif //MUSIC_PLAYER_H
