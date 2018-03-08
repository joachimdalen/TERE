#include "custom_menu_state.h"
#include <utility/utility.h>
#include "resource/resource_holder.h"

CustomMenuState::CustomMenuState(StateStack& stack, Context context) : State(stack, context)
{
    sf::RenderWindow &window = *getContext().window;
    sf::Font &mainFont = context.fonts->get(Fonts::Prime);

    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    waveText = new WaveText(window, "CUSTOMIZATION");

    TDescription.setFont(mainFont);
    TDescription.setFillColor(sf::Color(255, 255, 255));
    TDescription.setCharacterSize(context.settings->cSize);

    std::string description = "\
Customize the pieces you want to play with,\nor design a unique map to play on.";

    TDescription.setString(description);
    centerOriginHorizontal(TDescription);
    TDescription.setPosition(window.getSize().x / 2, context.settings->cSize * 10);


    mOptions[0] = TextButtonPtr(new TextButton("Customize Random Bag", mainFont));
    mOptions[0]->setPosition((context.window->getView().getSize() / 2.f) + sf::Vector2f(0.f, context.settings->cSize * 5.0));

    mOptions[1] = TextButtonPtr(new TextButton("Customize Map", mainFont));
    mOptions[1]->setPosition(mOptions[0]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));

    mOptions[2] = TextButtonPtr(new TextButton("Back", mainFont));
    mOptions[2]->setPosition(mOptions[1]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));

    mOptions[0]->setSelect();
}


void CustomMenuState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // draw the starfield first
    waveText->draw(window, getContext().settings->cSize * 2);

    window.draw(TDescription);

    for(int i = 0; i < 3; i++) // Draw option texts
        mOptions[i]->draw(window);
}


bool CustomMenuState::update(sf::Time)
{
    mStarfield->update(); // Update the starfield
    waveText->update();

    LastUpdate.restart(); // Ugly fix for pausing updates when the window is moved, instead of having separate threads etc.


    return true;
}


bool CustomMenuState::handleEvent(const sf::Event& event)
{
    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1))
    {
        requestStackPop();
        requestStackPush(States::MainMenu);
    }


    // Find the mouse location
    if(event.type == sf::Event::MouseMoved)
    {
        mouseY = sf::Mouse::getPosition(*getContext().window).y;

        for(int i = 0; i < 3; i++)
        {
            double top = getContext().window->getSize().y / 2 +
                         getContext().settings->cSize * 5.0 +
                         getContext().settings->cSize * i * 1.3;

            if(mouseY > top && mouseY < top + (1.0 * getContext().settings->cSize))
            {
                mOptions[currentSelection]->setUnSelect();
                currentSelection = i;
                mOptions[currentSelection]->setSelect();
            }
        }
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25))
    {
        mOptions[currentSelection]->setUnSelect();
        currentSelection++;
        if(currentSelection == 3)
            currentSelection = 0;

        mOptions[currentSelection]->setSelect();

    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 25))
    {
        mOptions[currentSelection]->setUnSelect();
        currentSelection--;
        if(currentSelection == -1)
            currentSelection = 2;

        mOptions[currentSelection]->setSelect();
    }


    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
        event.type == sf::Event::MouseButtonPressed || sf::Joystick::isButtonPressed(0, 0))
    {
        switch (currentSelection)
        {
            case 0:
                requestStackPop();
                requestStackPush(States::CustomizeBag);
                break;

            case 1:
                requestStackPop();
                requestStackPush(States::CustomizeMap);
                break;

            case 2:
                requestStackPop();
                requestStackPush(States::MainMenu);
                break;

            default:
                return false;
        }
    }

    return true;
}
