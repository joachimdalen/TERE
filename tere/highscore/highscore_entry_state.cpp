#include "highscore_entry_state.h"

#include <utility/utility.h>
#include "resource/resource_holder.h"
#include "sound/music_player.h"

HighScoreEntryState::HighScoreEntryState(StateStack& stack, Context context) : State(stack, context)
{
    sf::RenderWindow &window = *getContext().window;
    sf::Font &mainFont = context.fonts->get(Fonts::Prime);
    sf::Font &fixedFont = context.fonts->get(Fonts::Fixed);



    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    waveText = new WaveText(window, "CONGRATULATIONS");
    highScore = new HighScore();


    // Check if this is a valid high score, or if we should move on to the main menu instead

    nGameMode = getContext().settings->gameMode;
    nScore = getContext().settings->currentScore;
    nLevel = getContext().settings->currentLevel;

/*
    // Debug
    nGameMode = 1; // Sprint
    nScore = 600000;
    nLevel = 4;
*/

    if(nGameMode == 3) // Sprint Mode is Game Mode 3 in-game, but mode 2 for the high score list
        nGameMode = 2;

    nPosition = highScore->validHighScore(nGameMode, nScore);

    if(nPosition != -1)
    {
        nPosition = highScore->getPosition(nGameMode, nScore);
        highScore->shiftList(nGameMode, nPosition, "", nScore, nLevel);
    }
    else
        bNameEntryMode = false;


    // Set up the components of the state, if we are staying

    if(bNameEntryMode)
    {

        TDescription[0].setString("You are a");
        TDescription[1].setString("Tetris Master.");
        TDescription[2].setString("Please enter your name");

        for(int i = 0; i < 3; i++)
        {
            TDescription[i].setFont(mainFont);
            TDescription[i].setCharacterSize(context.settings->cSize);
            TDescription[i].setFillColor(sf::Color::White);
            centerOriginHorizontal(TDescription[i]);
        }

        float top = window.getSize().y / 2 - context.settings->cSize * 7.f;

        TDescription[0].setPosition( window.getSize().x / 2, top );
        TDescription[1].setPosition( TDescription[0].getPosition() + sf::Vector2f(0.f, context.settings->cSize * 1.3f ));
        TDescription[2].setPosition( TDescription[0].getPosition() + sf::Vector2f(0.f, context.settings->cSize * 3.f * 1.3f ));


        float posX = (context.settings->hPadding + (window.getSize().y / 3 * 4) * 0.15f );
        float columnWidth = (window.getSize().x - 2 * posX) / 5;

        for(int i = 0; i < 9; i++)
        {
            TNames[i].setFont(mainFont);
            TScore[i].setFont(fixedFont);
            TLevel[i].setFont(mainFont);

            TNames[i].setCharacterSize(context.settings->cSize);
            TScore[i].setCharacterSize(context.settings->cSize);
            TLevel[i].setCharacterSize(context.settings->cSize);

            TNames[i].setFillColor(sf::Color::White);
            TScore[i].setFillColor(sf::Color::White);
            TLevel[i].setFillColor(sf::Color::White);

            if(i == 0)
            {
                TNames[i].setString("Name");
                TScore[i].setString("Score");
                TLevel[i].setString("Level");
            }
            else
            {
                TNames[i].setString(highScore->tempScoreName[nGameMode][i - 1]);

                if(nGameMode < 2)
                    TScore[i].setString(thousandDelimiter(highScore->tempScore[nGameMode][i - 1]));
                else
                    TScore[i].setString(timerString(highScore->tempScore[nGameMode][i - 1], 0));

                TLevel[i].setString(toString(highScore->tempScoreLevel[nGameMode][i - 1]));
            }

            TNames[i].setPosition(posX, top + context.settings->cSize * 1.3f * (i + 5));
            leftOrigin(TNames[i]);

            TScore[i].setPosition(window.getSize().x - posX - columnWidth,
                                  top + context.settings->cSize * 1.3f * (i + 5));
            rightOrigin(TScore[i]);

            TLevel[i].setPosition(window.getSize().x - posX, top + context.settings->cSize * 1.3f * (i + 5));
            rightOrigin(TLevel[i]);
        }

        if(getContext().settings->musicEnabled)
            getContext().music->setVolume( getContext().settings->musicVolume );

        context.music->play(Music::MenuTheme); // Play menu theme
    }
}


void HighScoreEntryState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    if(nPosition == -1) // Heading back to the main menu momentarily, no need to draw anything
        return;

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // draw the starfield first
    waveText->draw(window, getContext().settings->cSize * 2);

    for(int i = 0; i < 3; i++)
        window.draw(TDescription[i]);

    // Adjust the origin of the name being changed
    TNames[nPosition + 1].setString("O");
    leftOrigin(TNames[nPosition + 1]);

    if(bNameEntryMode)
        TNames[nPosition + 1].setString(sNameEntry + (bCursor ? "_" : "") );
    else
        TNames[nPosition + 1].setString(sNameEntry);

    for(int i = 0; i < 9; i++)
    {
        window.draw(TNames[i]);
        window.draw(TScore[i]);
        window.draw(TLevel[i]);
    }
}


bool HighScoreEntryState::update(sf::Time)
{
    if(nPosition == -1) // Not a valid high score, moving on to the main menu
    {
        requestStackPop();
        requestStackPush(States::MainMenu);
        return true;
    }

    mStarfield->update(); // Update the starfield
    waveText->update();

    if(cursorTime.getElapsedTime().asMilliseconds() > nCursorToggle)
    {
        bCursor = !bCursor;
        cursorTime.restart();
    }

    return true;
}


bool HighScoreEntryState::handleEvent(const sf::Event& event)
{
    if(bNameEntryMode)
    {
        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace)
        {
            if(sNameEntry.length() > 0)
                sNameEntry.erase(sNameEntry.length() - 1, 1);

            return true;
        }

        if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return)
        {
            bNameEntryMode = false;

            highScore->tempScoreName[nGameMode][nPosition] = sNameEntry;
            highScore->createSaveFile(false); // Update the save file with new data

            return true;
        }

        if( event.type == sf::Event::KeyPressed )
        {
            bool shifted = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

            if(sNameEntry.length() < 16)
            {
                switch( event.key.code )
                {
                    // Letters

                    case sf::Keyboard::A :
                        if(shifted) sNameEntry += "A";
                        else sNameEntry += "a";
                        break;

                    case sf::Keyboard::B :
                        if(shifted) sNameEntry += "B";
                        else sNameEntry += "b";
                        break;

                    case sf::Keyboard::C :
                        if(shifted) sNameEntry += "C";
                        else sNameEntry += "c";
                        break;

                    case sf::Keyboard::D :
                        if(shifted) sNameEntry += "D";
                        else sNameEntry += "d";
                        break;

                    case sf::Keyboard::E :
                        if(shifted) sNameEntry += "E";
                        else sNameEntry += "e";
                        break;

                    case sf::Keyboard::F :
                        if(shifted) sNameEntry += "F";
                        else sNameEntry += "f";
                        break;

                    case sf::Keyboard::G :
                        if(shifted) sNameEntry += "G";
                        else sNameEntry += "g";
                        break;

                    case sf::Keyboard::H :
                        if(shifted) sNameEntry += "H";
                        else sNameEntry += "h";
                        break;

                    case sf::Keyboard::I :
                        if(shifted) sNameEntry += "I";
                        else sNameEntry += "i";
                        break;

                    case sf::Keyboard::J :
                        if(shifted) sNameEntry += "J";
                        else sNameEntry += "j";
                        break;

                    case sf::Keyboard::K :
                        if(shifted) sNameEntry += "K";
                        else sNameEntry += "k";
                        break;

                    case sf::Keyboard::L :
                        if(shifted) sNameEntry += "L";
                        else sNameEntry += "l";
                        break;

                    case sf::Keyboard::M :
                        if(shifted) sNameEntry += "M";
                        else sNameEntry += "m";
                        break;

                    case sf::Keyboard::N :
                        if(shifted) sNameEntry += "N";
                        else sNameEntry += "n";
                        break;

                    case sf::Keyboard::O :
                        if(shifted) sNameEntry += "O";
                        else sNameEntry += "o";
                        break;

                    case sf::Keyboard::P :
                        if(shifted) sNameEntry += "P";
                        else sNameEntry += "p";
                        break;

                    case sf::Keyboard::Q :
                        if(shifted) sNameEntry += "Q";
                        else sNameEntry += "q";
                        break;

                    case sf::Keyboard::R :
                        if(shifted) sNameEntry += "R";
                        else sNameEntry += "r";
                        break;

                    case sf::Keyboard::S :
                        if(shifted) sNameEntry += "S";
                        else sNameEntry += "s";
                        break;

                    case sf::Keyboard::T :
                        if(shifted) sNameEntry += "T";
                        else sNameEntry += "t";
                        break;

                    case sf::Keyboard::U :
                        if(shifted) sNameEntry += "U";
                        else sNameEntry += "u";
                        break;

                    case sf::Keyboard::V :
                        if(shifted) sNameEntry += "v";
                        else sNameEntry += "v";
                        break;

                    case sf::Keyboard::W :
                        if(shifted) sNameEntry += "w";
                        else sNameEntry += "w";
                        break;

                    case sf::Keyboard::X :
                        if(shifted) sNameEntry += "X";
                        else sNameEntry += "x";
                        break;

                    case sf::Keyboard::Y :
                        if(shifted) sNameEntry += "Y";
                        else sNameEntry += "y";
                        break;

                    case sf::Keyboard::Z :
                        if(shifted) sNameEntry += "Z";
                        else sNameEntry += "z";
                        break;


                    // Symbols

                    case sf::Keyboard::Comma :
                        sNameEntry += ",";
                        break;

                    case sf::Keyboard::Slash :
                        sNameEntry += "/";
                        break;

                    case sf::Keyboard::LBracket :
                        sNameEntry += "[";
                        break;

                    case sf::Keyboard::RBracket :
                        sNameEntry += "]";
                        break;

                    case sf::Keyboard::Quote :
                        sNameEntry += "\"";
                        break;

                    case sf::Keyboard::Period :
                        sNameEntry += ".";
                        break;

                    case sf::Keyboard::Dash :
                        if(shifted)
                            sNameEntry += "_";
                        else
                            sNameEntry += "-";
                        break;

                    case sf::Keyboard::Space :
                        sNameEntry += " ";
                        break;


                        // Numbers

                    case sf::Keyboard::Num1 :
                        sNameEntry += "1";
                        break;

                    case sf::Keyboard::Num2 :
                        sNameEntry += "2";
                        break;

                    case sf::Keyboard::Num3 :
                        sNameEntry += "3";
                        break;

                    case sf::Keyboard::Num4 :
                        sNameEntry += "4";
                        break;

                    case sf::Keyboard::Num5 :
                        sNameEntry += "5";
                        break;

                    case sf::Keyboard::Num6 :
                        sNameEntry += "6";
                        break;

                    case sf::Keyboard::Num7 :
                        sNameEntry += "7";
                        break;

                    case sf::Keyboard::Num8 :
                        sNameEntry += "8";
                        break;

                    case sf::Keyboard::Num9 :
                        sNameEntry += "9";
                        break;

                    case sf::Keyboard::Num0 :
                        sNameEntry += "0";
                        break;
                }
            }
        }
    }
    else
    {
        if ((event.type == sf::Event::KeyPressed) || event.type == sf::Event::MouseButtonPressed ||
            sf::Joystick::isButtonPressed(0, 0) || sf::Joystick::isButtonPressed(0, 1))
        {
            // Since we are done filling in the name, any interaction will return to the main menu
            requestStackPop();
            requestStackPush(States::MainMenu);
        }
    }

    return true;
}
