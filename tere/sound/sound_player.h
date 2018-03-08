#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include <list>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include "resource/resource_holder.h"
#include "resource/resource_identifiers.h"


class SoundPlayer : private sf::NonCopyable
{
public:
    SoundPlayer();

    void						play(SoundEffect::ID effect);
    void						play(SoundEffect::ID effect, sf::Vector2f position);

    void						removeStoppedSounds();
    void						setListenerPosition(sf::Vector2f position);
    sf::Vector2f				getListenerPosition() const;

    void						setVolume(float volume);


private:
    SoundBufferHolder			mSoundBuffers;
    std::list<sf::Sound>		mSounds;
    float						mVolume;
};

#endif //SOUND_PLAYER_H
