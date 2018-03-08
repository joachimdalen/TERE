#include "text.h"
#include <utility/utility.h>
#include <iostream>

Text::Text(sf::Font& font, int tileSize, int nNumberPlayers)
{
    float cSize = 2.f;

    TAdjust.setFont(font);
    TAdjust.setString("999,999,999");

    float width = 0.0f;

    while(width < (tileSize * 4))
    {
        cSize += 0.1f;
        TAdjust.setCharacterSize(cSize);
        width = TAdjust.getGlobalBounds().width;
    }

    TDebug.setFont(font);
    TDebug.setFillColor(sf::Color(127, 127, 127));
    TDebug.setCharacterSize(14);
    TDebug.setString("Empty");

    TDemoMessage.setFont(font);
    TDemoMessage.setFillColor(sf::Color(255, 255, 255));
    TDemoMessage.setCharacterSize(cSize);
    //TDemoMessage.setCharacterSize(cSize / 2);
    TDemoMessage.setString("Demo Mode");
    centerOrigin(TDemoMessage);

    for(int player = 0; player < 2; player++)
    {
        TScore[player].setFont(font);
        TScore[player].setFillColor(sf::Color(127, 127, 127));
        TScore[player].setCharacterSize(cSize);
        TScore[player].setString("Score");

        TScore[player].getGlobalBounds();

        TScoreInt[player].setFont(font);
        TScoreInt[player].setFillColor(sf::Color::White);
        TScoreInt[player].setCharacterSize(cSize);
        TScoreInt[player].setString("0");

        TLevel[player].setFont(font);
        TLevel[player].setFillColor(sf::Color(127, 127, 127));
        TLevel[player].setCharacterSize(cSize);
        TLevel[player].setString("Level");

        TLevelInt[player].setFont(font);
        TLevelInt[player].setFillColor(sf::Color::White);
        TLevelInt[player].setCharacterSize(cSize);
        TLevelInt[player].setString("0");

        TLines[player].setFont(font);
        TLines[player].setFillColor(sf::Color(127, 127, 127));
        TLines[player].setCharacterSize(cSize);
        TLines[player].setString("Lines");

        TLinesInt[player].setFont(font);
        TLinesInt[player].setFillColor(sf::Color::White);
        TLinesInt[player].setCharacterSize(cSize);
        TLinesInt[player].setString("0");

        TGameOver[player].setFont(font);
        TGameOver[player].setFillColor(sf::Color::White);
        TGameOver[player].setCharacterSize(cSize);
        TGameOver[player].setString("Get\nReady!");

        switch(player)
        {
            default :
                leftOrigin(TScore[player]);
                leftOrigin(TLevel[player]);
                leftOrigin(TLines[player]);
                break;
            case 1 :
                rightOrigin(TScore[player]);
                rightOrigin(TLevel[player]);
                rightOrigin(TLines[player]);
                break;
        }
    }
}


Text::~Text() {
}


void Text::draw(sf::RenderWindow &window, int player, int tileSize, std::string debugText, bool bGameOver,
                bool bFadeIn, bool bFadeOut, int nScore, int nLevel, int nLines, int nFieldWidth, int nFieldHeight,
                int nWidth, int nHeight, int nWidthPadding, std::string sTime, bool bDemoMode)
{
    // Draw the text for Score, Level, Lines

    if(sTime != "") // Change "Score" to "Time" for Sprint Mode
    {
        TScore[player].setString("Time");
        TScoreInt[player].setString(sTime);
    }
    else
        TScoreInt[player].setString(thousandDelimiter(nScore));

    switch(player)
    {
        default :
            leftOrigin(TScoreInt[player]);
            leftOrigin(TLevelInt[player]);
            leftOrigin(TLinesInt[player]);
            break;
        case 1 :
            rightOrigin(TScoreInt[player]);
            rightOrigin(TLevelInt[player]);
            rightOrigin(TLinesInt[player]);
            break;
    }

    int x;

    if(player == 0)
        x = tileSize * (nFieldWidth) + nWidthPadding;
    else x = (nWidth - nWidthPadding - tileSize * (nFieldWidth + 4));

    TScore[player].setPosition( x + (3.85f * tileSize) * player,
                                      (nHeight - (nFieldHeight - 2) * tileSize) / 2 +
                                      tileSize * 4.2f +
                                      player * tileSize * (nFieldHeight / 2 - 5));
    window.draw(TScore[player]);

    TScoreInt[player].setPosition( x + (3.85f * tileSize) * player,
                                         (nHeight - (nFieldHeight - 2) * tileSize) / 2 +
                                         tileSize * 5.2f +
                                         player * tileSize * (nFieldHeight / 2 - 5));
    //TScoreInt[player].setString(thousandDelimiter(nScore));
    window.draw(TScoreInt[player]);

    TLevel[player].setPosition( x + (3.85f * tileSize) * player,
                                      (nHeight - (nFieldHeight - 2) * tileSize) / 2 +
                                      tileSize * 6.2f +
                                      player * tileSize * (nFieldHeight / 2 - 5));
    window.draw(TLevel[player]);

    TLevelInt[player].setPosition( x + (3.85f * tileSize) * player,
                                         (nHeight - (nFieldHeight - 2) * tileSize) / 2 +
                                         tileSize * 7.2f +
                                         player * tileSize * (nFieldHeight / 2 - 5));
    TLevelInt[player].setString(thousandDelimiter(nLevel));  // tho reaching level 1,000 is unlikely
    window.draw(TLevelInt[player]);

    TLines[player].setPosition( x + (3.85f * tileSize) * player,
                                      (nHeight - (nFieldHeight - 2) * tileSize) / 2 +
                                      tileSize * 8.2f +
                                      player * tileSize * (nFieldHeight / 2 - 5));
    window.draw(TLines[player]);

    TLinesInt[player].setPosition( x + (3.85f * tileSize) * player,
                                         (nHeight - (nFieldHeight - 2) * tileSize) / 2 +
                                         tileSize * 9.2f +
                                         player * tileSize * (nFieldHeight / 2 - 5));
    TLinesInt[player].setString(thousandDelimiter(nLines));
    window.draw(TLinesInt[player]);

    //  Game Over
    if(  (bGameOver && !bFadeOut) || bFadeIn )
    {
        sf::RectangleShape mBox;  // First draw a black box
        mBox.setSize( sf::Vector2f(4 * tileSize, 4 * tileSize));
        mBox.setFillColor(sf::Color(0, 0, 0, 128));  // 50% transparent black box

        mBox.setPosition( 4 * tileSize, 8 * tileSize );
        mBox.move( (nWidth - nWidthPadding - tileSize * nFieldWidth) * player +
                   nWidthPadding * ((player + 1) % 2),
                   (nHeight - (nFieldHeight - 2) * tileSize) / 2);
        window.draw(mBox);

        centerOrigin(TGameOver[player]);
        TGameOver[player].setPosition(
                (nWidth - nWidthPadding - tileSize * nFieldWidth) * player +
                nWidthPadding * ((player + 1) % 2) + tileSize * 6,
                (nHeight - (nFieldHeight - 2) * tileSize) / 2 + tileSize * 10);
        window.draw(TGameOver[player]);
    }

    //  Debug messages
/*
    TDebug.setString(debugText);
    TDebug.setPosition(50, 50);
    window.draw(TDebug);
*/

    if(bDemoMode) // Should alternate every second
    {
        TDemoMessage.setPosition( window.getSize().x / 2, window.getSize().y - tileSize / 2 );
        //TDemoMessage.setPosition( window.getSize().x / 2, window.getSize().y / 2 - tileSize / 2 );

        window.draw(TDemoMessage);
    }
}


// A to Z,  0 to 9,   !&%^*[]

std::string Text::loserMessage()
{
    std::vector<std::string> messages;

    messages.emplace_back(" NEXT TIME ");
    messages.emplace_back(" WOE ");
    messages.emplace_back(" OH NO! ");
    messages.emplace_back(" COMFORT  ");
    messages.emplace_back(" !&%^*[] ");

    return messages.at(rand() % messages.size());
}


std::string Text::winnerMessage()
{
    std::vector<std::string> messages;

    messages.emplace_back(" WAY TO GO ");
    messages.emplace_back(" OH YEAH ");
    messages.emplace_back(" CHAMP ");
    messages.emplace_back(" AWESOME ");
    messages.emplace_back(" GREAT ");
    messages.emplace_back(" GOOD ");
    messages.emplace_back(" YES! ");

    return messages.at(rand() % messages.size());
}
