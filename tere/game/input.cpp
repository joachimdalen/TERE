#include <settings/game_settings.h>
#include "input.h"
#include "game_state.h"

Input::Input()
{
}

Input::~Input() {}


/*

 TETRIS Keyboard (Joystick) Controls  (Should be possible to reconfigure in menu):

    Hold           - Left Shift, C, Right Shift (Shoulder buttons)      -- N/A
    Rotate CCW     - Left CTRL, Right CTRL, Z (PS: X, Xbox: A)          -- SFML Button 0
    Rotate CW      - X, Up (PS: Circle, Xbox: B)                        -- SFML Button 1
    Move Left      - Left (Left DPad)
    Move Right     - Right (Right Dpad)
    Soft Drop      - Down (Down Dpad)
    Hard Drop      - Space (Up Dpad)

    Pause - Esc, F1 (PS/Xbox: Start)                                    -- SFML Button 7

 XBOX Controller in SFML:
 A - button 0, B = button 1, X = button 2, Y = button 3
 LB - button 4, RB = button 5, Select = button 6, Start = button 7
 L3 - button 8, R3 - button 9

*/


bool Input::handleEvent(GameState &GameState, const sf::Event& event, sf::RenderWindow &window, int cSize)
{
    if( (GameState.bGameOver[0] && !GameState.bFadeOut) || GameState.bDemoMode )
    {
        // If it is Game Over, or demo mode, any input from keyboard, mouse or joypad should make the game
        // go to the high score entry state to check if the high score is valid

        if(event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed ||
                event.type == sf::Event::JoystickButtonPressed )
        {
            return true;
        }

        return false;
    }


    if( (event.type == sf::Event::KeyPressed && event.key.code == GameState.kPause) ||
        (sf::Joystick::isButtonPressed(0, 7)))
    {
        GameState.bGamePaused = !GameState.bGamePaused;
        GameState.bPauseMusic = true;

        if(GameState.bGamePaused)
        {
            // Turn off everything for both players
            GameState.bMovingLeft[0] = false; GameState.bMovingRight[0] = false; GameState.bMovingDown[0] = false; GameState.bRotatingCCW[0] = false; GameState.bRotatingCW[0] = false;
            GameState.bMovingLeft[1] = false; GameState.bMovingRight[1] = false; GameState.bMovingDown[1] = false; GameState.bRotatingCCW[1] = false; GameState.bRotatingCW[1] = false;

            GameState.bPauseSound = true;
            GameState.pauseButton[1]->setSelect();
            GameState.currentSelection = 1;

            if(GameState.nGameType == 3)
                GameState.pauseTime += GameState.timerSprint.getElapsedTime().asMilliseconds();
        }
        else
        {
            if(GameState.nGameType == 3)
                GameState.timerSprint.restart();
        }
        return false;
    }


    if(GameState.bGamePaused)
    {

        // Find the mouse location
        if(event.type == sf::Event::MouseMoved)
        {
            GameState.mouseY = sf::Mouse::getPosition(window).y;

            for(int i = 1; i < 3; i++)
            {
                double top = window.getSize().y / 2 + cSize * (i - 1) * 1.3;

                if(GameState.mouseY > top && GameState.mouseY < top + (1.0 * cSize))
                {
                    GameState.pauseButton[GameState.currentSelection]->setUnSelect();
                    GameState.currentSelection = i;
                    GameState.pauseButton[GameState.currentSelection]->setSelect();
                }
            }
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down ||
            (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25))
        {
            GameState.pauseButton[GameState.currentSelection]->setUnSelect();
            GameState.currentSelection++;
            if(GameState.currentSelection == 3)
                GameState.currentSelection = 1;

            GameState.pauseButton[GameState.currentSelection]->setSelect();
        }

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up ||
            (sf::Joystick::isConnected(0) && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 25))
        {
            GameState.pauseButton[GameState.currentSelection]->setUnSelect();
            GameState.currentSelection--;
            if(GameState.currentSelection == 0)
                GameState.currentSelection = 2;

            GameState.pauseButton[GameState.currentSelection]->setSelect();
        }

        if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) ||
                event.type == sf::Event::MouseButtonPressed || sf::Joystick::isButtonPressed(0, 0))
        {
            switch (GameState.currentSelection)
            {
                case 1: // Return to Game
                    GameState.bGamePaused = false;
                    GameState.bPauseMusic = true;
                    break;

                case 2: // Exit to Main Menu
                    GameState.bActiveQuit = true;
                    return true;
            }
        }
        return false;
    }


    // ================================================================================================================


    if(!joystickUtilization) // If the joystick has not been used, or player is currently using keyboard
        GameState.bJoystick[0] = false;


    if(event.type == sf::Event::KeyPressed)
    {
        // Check for keyboard input in case the player switches from joystick to keyboard

        for (int i = 0; i < 5; i++)
        {
            if(event.key.code == GameState.kInput[0][i])
            {
                GameState.bJoystick[0] = false;
                joystickUtilization = false;
            }
        }
    }

    if(sf::Joystick::isConnected(0) && !joystickUtilization)
    {
        // Check for joystick input if joystick utilization is turned off

        if(sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -25 ||
           sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > 25 ||
           sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25 ||
           sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) > 25 ||
           sf::Joystick::isButtonPressed(0, 0) ||
           sf::Joystick::isButtonPressed(0, 1))
        {
            GameState.bJoystick[0] = true;
            joystickUtilization = true;
        }
    }


    if(!GameState.bAI[0]) // Does not apply for AI player 1
    {
        // Player 1 - Moving Left

        if( (event.type == sf::Event::KeyPressed && event.key.code == GameState.kInput[0][0]) ||
            (GameState.bJoystick[0] && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -25))
        {
            GameState.bMovingLeft[0] = true;

            if(GameState.nEntryDelay[0] == 0) // Allow for manipulation during the ARE entry delay (can change direction)
                GameState.nDASCounter[0] = 0; // reset the DAS counter when pressing left
        }

        // Player 1 - Moving Right
        if( (event.type == sf::Event::KeyPressed && event.key.code == GameState.kInput[0][1]) ||
            (GameState.bJoystick[0] && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > 25))
        {
            GameState.bMovingRight[0] = true;

            if(GameState.nEntryDelay[0] == 0) // Allow for manipulation during the ARE entry delay (can change direction)
                GameState.nDASCounter[0] = 0; // reset the DAS counter when pressing right
        }

        // Player 1 - Soft Drop
        if( (event.type == sf::Event::KeyPressed && event.key.code == GameState.kInput[0][2]) ||
            (GameState.bJoystick[0] && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) < -25))
        {
            GameState.bMovingDown[0] = true;
            GameState.nSoftDropCounter[0] = 0; // reset the Soft Drop counter when pressing down
        }

        // Player 1 - Rotate CCW
        if( (event.type == sf::Event::KeyPressed && event.key.code == GameState.kInput[0][3]) ||
           (GameState.bJoystick[0] && sf::Joystick::isButtonPressed(0, 0)))
            GameState.bRotatingCCW[0] = true;

        // Player 1 - Rotate CW
        if( (event.type == sf::Event::KeyPressed && event.key.code == GameState.kInput[0][4]) ||
           (GameState.bJoystick[0] && sf::Joystick::isButtonPressed(0, 1)))
            GameState.bRotatingCW[0] = true;

        if(GameState.bMovingDown[0] + GameState.bMovingLeft[0] + GameState.bMovingRight[0] > 1)
        {
            // Turn off everything if Player 1 moves in multiple directions at once (except rotation)

            GameState.bMovingDown[0] = false;
            GameState.bMovingLeft[0] = false;
            GameState.bMovingRight[0] = false;
            GameState.nSoftDropPoints[0] = 0;
        }
    }

    if(GameState.nNumberPlayers > 1 && !GameState.bAI[1]) // Does not apply for AI player 2
    {
        // Player 2 - Moving Left
        if(event.type == sf::Event::KeyPressed && event.key.code == GameState.kInput[1][0])
        {
            GameState.bMovingLeft[1] = true;

            if(GameState.nEntryDelay[1] == 0) // Allow for manipulation during the ARE entry delay (can change direction)
                GameState.nDASCounter[1] = 0; // reset the DAS counter when pressing right
        }

        // Player 2 - Moving Right
        if(event.type == sf::Event::KeyPressed && event.key.code == GameState.kInput[1][1])
        {
            GameState.bMovingRight[1] = true;

            if(GameState.nEntryDelay[1] == 0) // Allow for manipulation during the ARE entry delay (can change direction)
                GameState.nDASCounter[1] = 0; // reset the DAS counter when pressing right
        }

        // Player 2 - Soft Drop
        if(event.type == sf::Event::KeyPressed && event.key.code == GameState.kInput[1][2])
        {
            GameState.bMovingDown[1] = true;
            GameState.nSoftDropCounter[1] = 0; // reset the Soft Drop counter when pressing down
        }

        // Player 2 - Rotate CCW
        if(event.type == sf::Event::KeyPressed && event.key.code == GameState.kInput[1][3])
            GameState.bRotatingCCW[1] = true;

        // Player 2 - Rotate CW
        if(event.type == sf::Event::KeyPressed && event.key.code == GameState.kInput[1][4])
            GameState.bRotatingCW[1] = true;

        if(GameState.bMovingDown[1] + GameState.bMovingLeft[1] + GameState.bMovingRight[1] > 1)
        {
            // Turn off everything if Player 2 moves in multiple directions at once (except rotation)

            GameState.bMovingDown[1] = false;
            GameState.bMovingLeft[1] = false;
            GameState.bMovingRight[1] = false;
            GameState.nSoftDropPoints[1] = 0;
        }
    }


    if(!GameState.bAI[0]) // Does not apply for AI player 1
    {
        // Player 1 - Stop Move Left
        if( (event.type == sf::Event::KeyReleased && event.key.code == GameState.kInput[0][0]) ||
           (GameState.bJoystick[0] && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) >= -25))
            GameState.bMovingLeft[0] = false;

        // Player 1 - Stop Move Right
        if( (event.type == sf::Event::KeyReleased && event.key.code == GameState.kInput[0][1]) ||
           (GameState.bJoystick[0] && sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) <= 25))
            GameState.bMovingRight[0] = false;

        // Player 1 - Stop Soft Drop
        if( (event.type == sf::Event::KeyReleased && event.key.code == GameState.kInput[0][2]) ||
           (GameState.bJoystick[0] && sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) >= -25))
        {
            GameState.bMovingDown[0] = false;
            GameState.nSoftDropPoints[0] = 0;
        }

        // Player 1 - Stop Rotating CCW
        if( (event.type == sf::Event::KeyReleased && event.key.code == GameState.kInput[0][3]) ||
           (GameState.bJoystick[0] && !sf::Joystick::isButtonPressed(0, 0)))
            GameState.bRotatingCCW[0] = false;

        // Player 1 - Stop Rotating CW
        if( (event.type == sf::Event::KeyReleased && event.key.code == GameState.kInput[0][4]) ||
           (GameState.bJoystick[0] && !sf::Joystick::isButtonPressed(0, 1)))
            GameState.bRotatingCW[0] = false;
    }


    if(GameState.nNumberPlayers > 1 && !GameState.bAI[1]) // Does not apply for AI player 2
    {
        // Player 2 - Stop Move Left

        if(event.type == sf::Event::KeyReleased && event.key.code == GameState.kInput[1][0])
            GameState.bMovingLeft[1] = false;

        // Player 2 - Stop Move Right
        if(event.type == sf::Event::KeyReleased && event.key.code == GameState.kInput[1][1])
            GameState.bMovingRight[1] = false;

        // Player 2 - Stop Soft Drop
        if(event.type == sf::Event::KeyReleased && event.key.code == GameState.kInput[1][2])
        {
            GameState.bMovingDown[1] = false;
            GameState.nSoftDropPoints[1] = 0;
        }

        // Player 2 - Stop Rotating CCW
        if(event.type == sf::Event::KeyReleased && event.key.code == GameState.kInput[1][3])
            GameState.bRotatingCCW[1] = false;

        // Player 2 - Stop Rotating CW
        if(event.type == sf::Event::KeyReleased && event.key.code == GameState.kInput[1][4])
            GameState.bRotatingCW[1] = false;
    }

    return false;
}
