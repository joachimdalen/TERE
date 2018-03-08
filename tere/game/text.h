#ifndef TEXT_H
#define TEXT_H

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Text {

public:

    Text(sf::Font& font, int tileSize, int nNumberPlayers);

    virtual ~Text();
    // Virtual destructor: Important!  If not freeing Text* pointers
    // Could leak mem if child classes allocate any

    void draw(sf::RenderWindow &window, int player, int tileSize, std::string debugText, bool bGameOver, bool bFadeIn,
              bool bFadeOut, int nScore, int nLevel, int nLines, int nFieldWidth, int nFieldHeight, int nWidth,
              int nHeight, int nWidthPadding, std::string sTime, bool bDemoMode);

    std::string loserMessage();
    std::string winnerMessage();

    sf::Text                TAdjust, TDebug, TScore[2], TScoreInt[2], TLevel[2], TLevelInt[2], TLines[2], TLinesInt[2], TGameOver[2], TDemoMessage;
};

#endif //TEXT_H
