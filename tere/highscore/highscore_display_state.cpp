#include "highscore_display_state.h"
#include <utility/utility.h>
#include "resource/resource_holder.h"

HighScoreDisplayState::HighScoreDisplayState(StateStack& stack, Context context) : State(stack, context)
{
    sf::RenderWindow &window = *getContext().window;

    sf::Font &mainFont = context.fonts->get(Fonts::Prime);
    sf::Font &fixedFont = context.fonts->get(Fonts::Fixed);

    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    waveText = new WaveText(window, "HIGH SCORES");
    highScore = new HighScore();

    selection[0] = TextButtonPtr(new TextButton( sMode[0], mainFont));
    selection[0]->setPosition((context.window->getView().getSize() / 2.f) + sf::Vector2f(0.f, -context.settings->cSize * 6.0));

    selection[1] = TextButtonPtr(new TextButton("Back", mainFont));
    selection[1]->setPosition(selection[0]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f * 12));

    selection[0]->setSelect();

    float posX = (context.settings->hPadding + (window.getSize().y / 3 * 4) * 0.15f );
    float columnWidth = (window.getSize().x - 2 * posX) / 5;

    for(int i = 0; i < 9; i++)
    {
        if(i == 0)
        {
            TNames[i].setString("Name");
            TScore[i].setString("Score");
            TLevel[i].setString("Level");
        }
        else
        {
            TNames[i].setString(highScore->tempScoreName[0][i - 1]);
            TScore[i].setString(thousandDelimiter(highScore->tempScore[0][i - 1]));
            TLevel[i].setString(toString(highScore->tempScoreLevel[0][i - 1]));
        }

        TNames[i].setFont(mainFont);
        TScore[i].setFont(fixedFont);
        TLevel[i].setFont(mainFont);

        TNames[i].setCharacterSize(context.settings->cSize);
        TScore[i].setCharacterSize(context.settings->cSize);
        TLevel[i].setCharacterSize(context.settings->cSize);

        TNames[i].setFillColor(sf::Color::White);
        TScore[i].setFillColor(sf::Color::White);
        TLevel[i].setFillColor(sf::Color::White);

        TNames[i].setPosition(sf::Vector2f(posX, window.getSize().y / 2 - context.settings->cSize * 4.f) +
                                      sf::Vector2f(0.f, context.settings->cSize * 1.3f * i));
        leftOrigin(TNames[i]);

        TScore[i].setPosition(sf::Vector2f(window.getSize().x - posX - columnWidth, window.getSize().y / 2 - context.settings->cSize * 4.f) +
                              sf::Vector2f(0.f, context.settings->cSize * 1.3f * i));
        rightOrigin(TScore[i]);

        TLevel[i].setPosition(sf::Vector2f(window.getSize().x - posX, window.getSize().y / 2 - context.settings->cSize * 4.f) +
                              sf::Vector2f(0.f, context.settings->cSize * 1.3f * i));
        rightOrigin(TLevel[i]);
    }

    for(int i = 0; i < 2; i++)
    {
        TDirection[i].setFont(mainFont);
        TDirection[i].setCharacterSize(context.settings->cSize);
        TDirection[i].setFillColor(sf::Color::White);
        TDirection[i].setString("O");
    }

    TDirection[0].setPosition(sf::Vector2f(posX - context.settings->cSize * 3,
                                           window.getSize().y / 2 - context.settings->cSize * 4.0) +
                                      sf::Vector2f(0.f, context.settings->cSize * 1.3f * 4));
    TDirection[1].setPosition(sf::Vector2f(window.getSize().x - posX + context.settings->cSize * 3,
                                           window.getSize().y / 2 - context.settings->cSize * 4.0) +
                                      sf::Vector2f(0.f, context.settings->cSize * 1.3f * 4));
    leftOrigin(TDirection[0]);
    rightOrigin(TDirection[1]);
    TDirection[0].setString("<");
    TDirection[1].setString(">");

    TDebug.setFont(mainFont);
    TDebug.setCharacterSize(context.settings->cSize / 2);
    TDebug.setFillColor(sf::Color::White);
    TDebug.setPosition(100, 150);

}


void HighScoreDisplayState::updateMode(int mode)
{
    selection[0]->updateString(sMode[mode]);

    if(mode == 2)
        TScore[0].setString("Time");
    else TScore[0].setString("Score");

    rightOrigin(TScore[0]);

    for(int i = 0; i < 8; i++)
    {
        TNames[i + 1].setString(highScore->tempScoreName[mode][i]);

        if(mode < 2)
            TScore[i + 1].setString(thousandDelimiter(highScore->tempScore[mode][i]));
        else
            TScore[i + 1].setString(timerString(highScore->tempScore[mode][i], 0));

        TLevel[i + 1].setString(toString(highScore->tempScoreLevel[mode][i]));

        leftOrigin(TNames[i + 1]);
        rightOrigin(TScore[i + 1]);
        rightOrigin(TLevel[i + 1]);
    }
}


void HighScoreDisplayState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // draw the starfield first
    waveText->draw(window, getContext().settings->cSize * 2);

    for(int i = 0; i < 2; i++)
        selection[i]->draw(window);

    for(int i = 0; i < 9; i++)
    {
        window.draw(TNames[i]);
        window.draw(TScore[i]);
        window.draw(TLevel[i]);
    }

    for(int i = 0; i < 2; i++)
        window.draw(TDirection[i]);

    //TDebug.setString(debugText);
    //window.draw(TDebug);
}


bool HighScoreDisplayState::update(sf::Time)
{
    mStarfield->update(); // Update the starfield
    waveText->update();

    return true;
}


void HighScoreDisplayState::selectHighScore()
{
    for(int i = 0; i < 2; i++)
        TDirection[i].setFillColor(sf::Color::White);
}


void HighScoreDisplayState::deselectHighScore()
{
    for(int i = 0; i < 2; i++)
        TDirection[i].setFillColor(sf::Color(127, 127, 127));
}


void HighScoreDisplayState::horizontalAction(int direction)
{
    switch(direction)
    {
        case -1 :
            modeSelected--;

            if(modeSelected < 0)
                modeSelected = 2;

            updateMode(modeSelected);
            break;

        case 1 :
            modeSelected++;

            if(modeSelected > 2)
                modeSelected = 0;

            updateMode(modeSelected);
            break;

        default : break;
    }
}


bool HighScoreDisplayState::handleEvent(const sf::Event& event)
{
    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1))
    {
        requestStackPop();
        requestStackPush(States::MainMenu);
    }


    if(event.type == sf::Event::MouseMoved)
    {
        mouseX = sf::Mouse::getPosition(*getContext().window).x;
        mouseY = sf::Mouse::getPosition(*getContext().window).y;

        double top = getContext().window->getSize().y / 2 - getContext().settings->cSize * 6.0;
        hoverX = 0;

        if(mouseY > top + getContext().settings->cSize * 1.3 * 12.0)
        {
            currentSelection = 1;
            selection[0]->setUnSelect();
            selection[1]->setSelect();
            deselectHighScore();
        }
        else if(mouseY < top + getContext().settings->cSize * 1.3 * 10.0)
        {
            currentSelection = 0;
            selection[0]->setSelect();
            selection[1]->setUnSelect();
            selectHighScore();

            if(mouseX < getContext().window->getSize().x / 2)
                hoverX = -1;
            else
                hoverX = 1;
        }

        debugText = "mouseX: " + toString(mouseX) + "  mouseY: " + toString(mouseY) + "  currentSelection: " + toString(currentSelection) + "\n";
    }


    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
        event.type == sf::Event::MouseButtonPressed || sf::Joystick::isButtonPressed(0, 0))
    {
        if(currentSelection == 1)
        {
            requestStackPop();
            requestStackPush(States::MainMenu);
            return true;
        }

        horizontalAction(hoverX);
        return true;
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down ||
    (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25))
    {
        selection[currentSelection]->setUnSelect();

        currentSelection++;

        if (currentSelection > 1)
            currentSelection = 0;

        if(currentSelection == 0)
            selectHighScore();
        else
            deselectHighScore();

        selection[currentSelection]->setSelect();
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 25))
    {
        selection[currentSelection]->setUnSelect();

        currentSelection--;

        if (currentSelection < 0)
            currentSelection = 1;

        if(currentSelection == 0)
            selectHighScore();
        else
            deselectHighScore();

        selection[currentSelection]->setSelect();
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -25))
    {
        if(currentSelection == 0)
            horizontalAction(-1);
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > 25))
    {
        if(currentSelection == 0)
            horizontalAction(1);
    }

    return true;
}
