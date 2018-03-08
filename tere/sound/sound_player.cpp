#include <cmath>
#include <SFML/Audio/Listener.hpp>

#include "sound_player.h"

namespace
{
    // Sound coordinate system, point of view of a player in front of the screen:
    // X = left; Y = up; Z = back (out of the screen)
    const float ListenerZ = 300.f;
    const float Attenuation = 8.f;
    const float MinDistance2D = 200.f;
    const float MinDistance3D = std::sqrt(MinDistance2D*MinDistance2D + ListenerZ*ListenerZ);
}

SoundPlayer::SoundPlayer()
        : mSoundBuffers()
        , mSounds()
{
    mSoundBuffers.load(SoundEffect::Move, "media/sound/move.wav");
    mSoundBuffers.load(SoundEffect::LockPiece, "media/sound/lock_piece.wav");
    mSoundBuffers.load(SoundEffect::LineCompletion, "media/sound/line_completion.wav");
    mSoundBuffers.load(SoundEffect::Rotate, "media/sound/rotate.wav");
    mSoundBuffers.load(SoundEffect::Tetris, "media/sound/tetris.wav");
    mSoundBuffers.load(SoundEffect::Punishment, "media/sound/punishment.wav");
    mSoundBuffers.load(SoundEffect::Lose, "media/sound/lose.wav");
    mSoundBuffers.load(SoundEffect::Win, "media/sound/win.wav");
    mSoundBuffers.load(SoundEffect::Pause, "media/sound/pause.wav");

    // Listener points towards the screen (default in SFML)
    sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(SoundEffect::ID effect)
{
    play(effect, getListenerPosition());
}


void SoundPlayer::setVolume(float volume)
{
//    mVolume = volume * 2.f; // Max volume is 50%, but it is still plenty
    mVolume = volume * 5.f; // Max volume is 50%, but it is still plenty
}


void SoundPlayer::play(SoundEffect::ID effect, sf::Vector2f position)
{
    mSounds.push_back(sf::Sound());
    sf::Sound& sound = mSounds.back();

    sound.setBuffer(mSoundBuffers.get(effect));
    sound.setPosition(position.x, -position.y, 0.f);
    sound.setAttenuation(Attenuation);
    sound.setMinDistance(MinDistance3D);

    //sound.setVolume(10.0f);
    sound.setVolume(mVolume);

    sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
    mSounds.remove_if([] (const sf::Sound& s)
                      {
                          return s.getStatus() == sf::Sound::Stopped;
                      });
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
    sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    sf::Vector3f position = sf::Listener::getPosition();
    return sf::Vector2f(position.x, -position.y);
}
