#ifndef RESOURCE_IDENTIFIERS_H
#define RESOURCE_IDENTIFIERS_H

// Forward declaration of SFML classes
namespace sf {
    class SoundBuffer;
    class Font;
}

namespace Fonts {
    enum ID {
        Prime,
        Fixed,
    };
}

namespace SoundEffect
{
    enum ID
    {
        Move,
        LockPiece,
        LineCompletion,
        Rotate,
        Tetris,
        Punishment,
        Lose,
        Win,
        Pause,
    };
}

namespace Music
{
    enum ID
    {
        MenuTheme,
        GameTheme,
    };
}


// Forward declaration and a few type definitions
template<typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID>	SoundBufferHolder;

#endif //RESOURCE_IDENTIFIERS_H
