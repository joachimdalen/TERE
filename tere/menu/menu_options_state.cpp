#include "menu_options_state.h"
#include "resource/resource_holder.h"

MenuOptionState::MenuOptionState(StateStack &stack, const State::Context &context) : State(stack, context)
{
    sf::RenderWindow &window = *getContext().window;

    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    waveText = new WaveText(window, "OPTIONS");

    sf::Font &mainFont = context.fonts->get(Fonts::Prime);



    selection[0] = TextButtonPtr(new TextButton("General Options", mainFont));
    selection[0]->setPosition((context.window->getView().getSize() / 2.f) + sf::Vector2f(0.f, context.settings->cSize * 1.0));

    selection[1] = TextButtonPtr(new TextButton("Reconfigure Controls", mainFont));
    selection[1]->setPosition(selection[0]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));

    selection[2] = TextButtonPtr(new TextButton("Back", mainFont));
    selection[2]->setPosition(selection[1]->getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f));

    selection[0]->setSelect();

}


void MenuOptionState::draw()
{
    sf::RenderWindow &window = *getContext().window;
    window.setView(window.getDefaultView());

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // draw the starfield first
    waveText->draw(window, getContext().settings->cSize * 2);

    for(int i = 0; i < 3; i++)
        selection[i]->draw(window);

 }


bool MenuOptionState::update(sf::Time dt)
{
    mStarfield->update(); // Update the starfield
    waveText->update();

    return true;
}


bool MenuOptionState::handleEvent(const sf::Event &event)
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
                         getContext().settings->cSize * 1.0 +
                         getContext().settings->cSize * i * 1.3;

            if(mouseY > top && mouseY < top + (1.0 * getContext().settings->cSize))
            {
                selection[currentSelection]->setUnSelect();
                currentSelection = i;
                selection[currentSelection]->setSelect();
            }
        }
    }


    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25))
    {
        selection[currentSelection]->setUnSelect();
        currentSelection++;
        if(currentSelection == 3)
            currentSelection = 0;

        selection[currentSelection]->setSelect();

    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up ||
        (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 25))
    {
        selection[currentSelection]->setUnSelect();
        currentSelection--;
        if(currentSelection == -1)
            currentSelection = 2;

        selection[currentSelection]->setSelect();
    }


    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
        event.type == sf::Event::MouseButtonPressed || sf::Joystick::isButtonPressed(0, 0))
    {
        switch (currentSelection)
        {
            case 0:
                requestStackPop();
                requestStackPush(States::MenuGeneral);
                break;

            case 1:
                requestStackPop();
                requestStackPush(States::MenuControls);
                break;

            case 2:
                requestStackPop();
                requestStackPush(States::MainMenu);
                break;

            default:
                return false;
        }
    }
}
