#ifndef TEXT_TOGGLE_H
#define TEXT_TOGGLE_H

#include <memory>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <utility/utility.h>

class TextToggle
{
public:

    TextToggle(std::string text, sf::Font &font, std::vector<std::string> &values)
    {
        this->values = values;

        originalText = text;
        optionText.setFont(font);
        optionText.setCharacterSize(GameSettings::cSize);
        optionText.setFillColor(sf::Color(127, 127, 127));

        optionTextSelection.setString(text);
        optionTextSelection.setFont(font);
        optionTextSelection.setCharacterSize(GameSettings::cSize);
        optionTextSelection.setFillColor(sf::Color(127, 127, 127));

        previousText.setString("O");
        previousText.setFont(font);
        previousText.setCharacterSize(GameSettings::cSize);
        previousText.setFillColor(sf::Color(127, 127, 127));

        nextText.setString("O");
        nextText.setFont(font);
        nextText.setCharacterSize(GameSettings::cSize);
        nextText.setFillColor(sf::Color(127, 127, 127));

        optionText.setString(originalText);
        optionTextSelection.setString(values[selectedIndex]);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(optionText);

        window.draw(previousText);
        window.draw(optionTextSelection);
        window.draw(nextText);

    }

    void setPosition(sf::Vector2f position, sf::RenderWindow &window)
    {
        leftOrigin(optionText);
        optionText.setPosition(position);


        previousText.setString("O");
        leftOrigin(previousText);
        previousText.setString("<");
        previousText.setPosition( window.getSize().x / 2, position.y );

        nextText.setString("O");
        rightOrigin(nextText);
        nextText.setString(">");
        nextText.setPosition( window.getSize().x - position.x, position.y );

        optionTextSelection.setPosition( window.getSize().x / 2 + (nextText.getPosition().x - previousText.getPosition().x) / 2, position.y );
        centerOriginHorizontal(optionTextSelection);

    }

    sf::Vector2f getPosition()
    {
        return optionText.getPosition();
    }

    void goNext()
    {
        if (selectedIndex == values.size() - 1)
            selectedIndex = 0;
        else
            selectedIndex++;

        optionTextSelection.setString(values[selectedIndex]);
        centerOriginHorizontal(optionTextSelection); // Reset positioning
    }

    void goPrevious()
    {
        if (selectedIndex == 0)
            selectedIndex = values.size() - 1;
        else
            selectedIndex--;

        optionTextSelection.setString(values[selectedIndex]);
        centerOriginHorizontal(optionTextSelection); // Reset positioning
    }

    int getSelectedIndex()
    {
        return selectedIndex;
    }

    std::string getSelectedValue()
    {
        return values[selectedIndex];
    }

    void setSelected()
    {
        optionText.setFillColor(sf::Color::White);
        previousText.setFillColor(sf::Color::White);
        nextText.setFillColor(sf::Color::White);
        optionTextSelection.setFillColor(sf::Color::White);

        selected = true;
    }

    void setUnSelected()
    {
        optionText.setFillColor(sf::Color(127, 127, 127));
        previousText.setFillColor(sf::Color(127, 127, 127));
        nextText.setFillColor(sf::Color(127, 127, 127));
        optionTextSelection.setFillColor(sf::Color(127, 127, 127));
        selected = false;
    }

    void setSelectedIndex(int index)
    {
        selectedIndex = index;
        optionTextSelection.setString(values[selectedIndex]);
    }


    bool isSelected()
    {
        return selected;
    }

    void setSelectedByValue(std::string value)
    {
        for (std::size_t i = 0; i < values.size(); ++i)
        {
            if (values[i] == value)
            {
                setSelectedIndex(i);
                break;
            }
        }
    }

    void setCharacterSize(unsigned int size)
    {
        optionText.setCharacterSize(size);
    }


protected:

    sf::Text optionText, optionTextSelection, previousText, nextText;
    std::string originalText;
    std::vector<std::string> values;
    int selectedIndex = 0;
    bool selected = false;
};

typedef std::shared_ptr<TextToggle> TextTogglePtr;
#endif //TEXT_TOGGLE_H
