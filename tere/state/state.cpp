#include <settings/game_settings.h>
#include <iostream>
#include "state.h"
#include "state_stack.h"

State::Context::Context(sf::RenderWindow& window, FontHolder& fonts, GameSettings& settings, MusicPlayer& music, SoundPlayer& sounds)
        : window(&window)
        , fonts(&fonts)
        , settings(&settings)
        , music(&music)
        , sounds(&sounds)

{
}

State::State(StateStack& stack, Context context)
        : mStack(&stack)
        , mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
    mStack->pushState(stateID);
}

void State::requestStackPop()
{
    mStack->popState();
}


void State::requestStateClear()
{
    mStack->clearStates();
}

State::Context State::getContext() const
{
    return mContext;
}
