#include "credits_state.h"
#include <utility/utility.h>
#include <iostream>
#include "resource/resource_holder.h"

CreditsState::CreditsState(StateStack& stack, Context context) : State(stack, context)
{
    sf::RenderWindow &window = *getContext().window;
    sf::Font &mainFont = context.fonts->get(Fonts::Prime);

    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    mCube = new Cube();

    TScroller.setFont(mainFont);
    TScroller.setFillColor(sf::Color(255, 255, 255));
    TScroller.setCharacterSize(context.settings->cSize);

    // Keep the text at maximum 40 characters per line

/*
    std::string scrollText =
                    "\n\nLorem ipsum dolor sit amet, consectetur\n"
                    "adipiscing elit, sed do eiusmod tempor\n"
                    "incididunt ut labore et dolore magna\n"
                    "aliqua. Fermentum iaculis eu non diam\n"
                    "phasellus. Elementum curabitur vitae\n"
                    "nunc sed velit dignissim. Et ligula\n"
                    "ullamcorper malesuada proin libero nunc\n"
                    "consequat. Augue lacus viverra vitae\n"
                    "congue eu consequat ac felis. Natoque\n"
                    "penatibus et magnis dis parturient\n"
                    "montes. Quis imperdiet massa tincidunt\n"
                    "nunc pulvinar sapien et ligula. Tortor\n"
                    "vitae purus faucibus ornare suspendisse\n"
                    "sed nisi lacus sed. Mi in nulla posuere\n"
                    "sollicitudin aliquam ultrices. Diam\n"
                    "phasellus vestibulum lorem sed.\n"
                    "Pellentesque habitant morbi tristique\n"
                    "senectus et netus. Leo duis ut diam quam\n"
                    "nulla porttitor massa. Sagittis aliquam\n"
                    "malesuada bibendum arcu vitae elementum\n"
                    "curabitur. Non sodales neque sodales ut\n"
                    "etiam. Aenean euismod elementum nisi\n"
                    "quis. Ultricies lacus sed turpis\n"
                    "tincidunt id aliquet risus.\n"
                    "\n"
                    "Sit amet venenatis urna cursus eget nunc\n"
                    "scelerisque viverra. A scelerisque purus\n"
                    "semper eget duis. Sodales ut etiam sit\n"
                    "amet nisl purus in mollis nunc. Semper\n"
                    "eget duis at tellus at urna condimentum\n"
                    "mattis. Consectetur adipiscing elit\n"
                    "pellentesque habitant morbi tristique.\n"
                    "Diam vel quam elementum pulvinar etiam.\n"
                    "Sit amet luctus venenatis lectus magna\n"
                    "fringilla. Nibh praesent tristique magna\n"
                    "sit amet purus gravida quis. Gravida\n"
                    "quis blandit turpis cursus. Et malesuada\n"
                    "fames ac turpis egestas maecenas. Ut\n"
                    "morbi tincidunt augue interdum. Id\n"
                    "ornare arcu odio ut sem nulla. Eu\n"
                    "lobortis elementum nibh tellus molestie\n"
                    "nunc. Tristique et egestas quis ipsum\n"
                    "suspendisse ultrices gravida dictum\n"
                    "fusce. Pharetra massa massa ultricies mi\n"
                    "quis hendrerit dolor magna. Parturient\n"
                    "montes nascetur ridiculus mus mauris\n"
                    "vitae. Pellentesque pulvinar pellentesque\n"
                    "habitant morbi tristique senectus et\n"
                    "netus et. Et ultrices neque ornare aenean\n"
                    "euismod elementum nisi quis eleifend.\n"
                    "Lacus vestibulum sed arcu non odio\n"
                    "euismod lacinia at. Pretium aenean\n"
                    "pharetra magna ac placerat vestibulum.\n\n";
*/

    std::wstring scrollText =
L"\n"
        "Tetris - Original concept, design and\n"
        "program by Alexey Pajitnov\n"
        "\n"
        "\n"
        "TETRIS REVISITED\n"
        "\n"
        "\n"
        "Programming\n"
        "Joachim Andreas Dalen\n"
        "Jan Øyvind Kruse\n"
        "Erik Tokheim\n"
        "\n"
        "\n"
        "Supervisor\n"
        "Christian Auby\n"
        "\n"
        "\n"
        "Design, Art and Sound Direction\n"
        "Jan Øyvind Kruse\n"
        "\n"
        "\n"
        "Menu Design\n"
        "Joachim Andreas Dalen\n"
        "\n"
        "\n"
        "Game Testing\n"
        "Joachim Andreas Dalen\n"
        "Jan Øyvind Kruse\n"
        "Erik Tokheim\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "Credits\n"
        "\n"
        "\n"
        "Code\n"
        "\n"
        "Original Starfield code in part by\n"
        "Daniel Shiffman and Draugr\n"
        "Licensed under Creative Commons Zero \n"
        "No Rights Reserved\n"
        "\n"
        "\n"
        "Original Tetris Game in part by\n"
        "One Lone Coder\n"
        "Licensed under Creative Commons Zero\n"
        "No Rights Reserved\n"
        "\n"
        "\n"
        "Original AI code in part by Yiyuan Lee\n"
        "Used under the MIT License\n"
        "\n"
        "\n"
        "\n"
        "Graphics\n"
        "\n"
        "4x4 Pixel Font by Swamp Design\n"
        "Licenced under Creative Commons BY 3.0\n"
        "\n"
        "\n"
        "Prime Font by Max Pirsky\n"
        "Licensed under Fontfabric\n"
        "Free Font EULA License\n"
        "\n"
        "\n"
        "Controller Button Graphics by\n"
        "Nicolae Berbece (Xelu)\n"
        "Licensed under Creative Commons Zero\n"
        "No Rights Reserved\n"
        "\n"
        "\n"
        "\n"
        "Sound and Music\n"
        "\n"
        "No. 10 Scène:\n"
        "Moderato from Swan Lake (Act II)\n"
        "Remixed by Millstones\n"
        "Written by P.I. Tchaikovsky\n"
        "Licenced under Creative Commons BY 3.0\n"
        "\n"
        "\n"
        "Dance of the Sugar Plum Fairy\n"
        "Remixed by Millstones\n"
        "Written by P.I. Tchaikovsky\n"
        "Licenced under Creative Commons BY 3.0\n"
        "\n"
        "\n"
        "Sound Effects by\n"
        "Christopher Barrett (Oryx Design Lab)\n"
        "Royalty free, commercial license\n"
        "\n"
        "\n"
        "Thanks to our beta testers;\n"
        "Drazule, Infomastr, twitchy_v2 and Pequisto\n"
        "\n"
        "Our friends and family\n"
        "\n"
        "You for playing the game.\n"
        "";


    yPos = window.getSize().y;

    TScroller.setString(scrollText);
    centerOriginHorizontal(TScroller);
}


void CreditsState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // draw the starfield first
    mCube->draw(window, getContext().settings->hPadding);


    TScroller.setPosition(window.getSize().x / 2, yPos);
    window.draw(TScroller);

/*
    TScroller.setPosition(window.getSize().x / 2, 400);
    TScroller.setString("ØÆLLLLØÅÅÅÅ");
    window.draw(TScroller);
*/


    int nWidth = window.getSize().x, nHeight = window.getSize().y;

    sf::RectangleShape mRectangle; // Used for fading in/out
    mRectangle.setSize(sf::Vector2f(nWidth, nHeight));
    mRectangle.setPosition(0, 0);


    if(fadeInTimer.getElapsedTime().asSeconds() < 1) // Fade-in
    {
        float factor = (fadeInTimer.getElapsedTime().asMilliseconds() - (exitTime - fadeTime) * 1000.f) / (fadeTime * 1000.f) ;
        if(factor > 1.f) factor = 1.f;

        mRectangle.setFillColor(sf::Color(0, 0, 0, 255 - factor * 255));
        window.draw(mRectangle);
    }


    if(fadeOutTimer.getElapsedTime().asSeconds() > exitTime - fadeTime) // Fade-out
    {
        float factor = (fadeOutTimer.getElapsedTime().asMilliseconds() - (exitTime - fadeTime) * 1000.f) / (fadeTime * 1000.f) ;
        if(factor > 1.f) factor = 1.f;

        mRectangle.setFillColor(sf::Color(0, 0, 0, factor * 255));
        window.draw(mRectangle);
    }
}


bool CreditsState::update(sf::Time)
{
    mStarfield->update(); // Update the starfield
    mCube->update();

    float adjustedSpeed = scrollSpeed / (1000.f / 60) * (Timer.getElapsedTime().asMicroseconds() / 1000.f);

    //adjustedSpeed = adjustedSpeed / 720.f * getContext().window->getSize().y;


    yPos -= adjustedSpeed;

    Timer.restart();

    if(yPos < TScroller.getGlobalBounds().height * -1.f)
    {
        if(fadeOutTimer.getElapsedTime().asSeconds() > 1)
        {
            requestStackPop();
            requestStackPush(States::MainMenu);
        }
        return true;
    }

    fadeOutTimer.restart();
    return true;
}


bool CreditsState::handleEvent(const sf::Event& event)
{
    if ((event.type == sf::Event::KeyPressed) || event.type == sf::Event::MouseButtonPressed ||
            sf::Joystick::isButtonPressed(0, 0) || sf::Joystick::isButtonPressed(0, 1))
    {
        requestStackPop();
        requestStackPush(States::MainMenu);
    }

    return true;
}
