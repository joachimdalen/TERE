#include <fstream>
#include <utility/key_converter.h>
#include <iostream>
#include "game_settings.h"

GameSettings::GameSettings() {
    playerOneKeyMap = PlayerKeyMapPtr(new PlayerKeyMap());
    playerTwoKeyMap = PlayerKeyMapPtr(new PlayerKeyMap());
    this->musicEnabled = true;
    this->soundEffectsEnabled = true;
    this->antiAliasEnabled = true;
    this->trailsEnabled = true;
    this->starfieldEnabled = true;
    this->musicVolume = 5;
    this->soundVolume = 5;
    this->trailsIntensity = 5;
    this->trailsDelay = 5;
    this->randomBag = "0123456";
}

bool GameSettings::fileExists() {
    std::ifstream infile("save/settings.ini");

    if (!infile.good())
        return false;

    return true;
}

bool GameSettings::reset() {
    writeDefaultsToFile();
    load();
}

bool GameSettings::load()
{
    if (settingsParser.loadFromFile("save/settings.ini"))
    {
        settingsParser.get("music-enabled", this->musicEnabled);
        settingsParser.get("sound-effects-enabled", this->soundEffectsEnabled);
        settingsParser.get("sound-volume", this->soundVolume);
        settingsParser.get("music-volume", this->musicVolume);
        settingsParser.get("anti-alias-enabled", this->antiAliasEnabled);
        settingsParser.get("starfield-enabled", this->starfieldEnabled);
        settingsParser.get("trails-enabled", this->trailsEnabled);
        settingsParser.get("trails-intensity", this->trailsIntensity);
        settingsParser.get("trails-delay", this->trailsDelay);
        settingsParser.get("screen-height", this->screenHeight);
        settingsParser.get("screen-width", this->screenWidth);

        settingsParser.get("random-bag", this->randomBag);

        std::string oneLeft = "A";
        std::string oneRight = "D";
        std::string oneDown = "S";
        std::string oneRotateCounterClockwise = "Left Control";
        std::string oneRotateClockwise = "W";
        std::string onePause = "Escape";

        settingsParser.get("player-one-left", oneLeft);
        settingsParser.get("player-one-right", oneRight);
        settingsParser.get("player-one-down", oneDown);
        settingsParser.get("player-one-rotate-counter-clockwise", oneRotateCounterClockwise);
        settingsParser.get("player-one-rotate-clockwise", oneRotateClockwise);
        settingsParser.get("player-one-pause-game", onePause);

        playerOneKeyMap->Left = stringToKey(oneLeft);
        playerOneKeyMap->Right = stringToKey(oneRight);
        playerOneKeyMap->Down = stringToKey(oneDown);
        playerOneKeyMap->RotateClockwise = stringToKey(oneRotateClockwise);
        playerOneKeyMap->RotateCounterClockwise = stringToKey(oneRotateCounterClockwise);
        playerOneKeyMap->PauseGame = stringToKey(onePause);


        std::string twoLeft = "Left";
        std::string twoRight = "Right";
        std::string twoDown = "Down";
        std::string twoRotateCounterClockwise = "Right Control";
        std::string twoRotateClockwise = "Up";

        settingsParser.get("player-two-left", twoLeft);
        settingsParser.get("player-two-right", twoRight);
        settingsParser.get("player-two-down", twoDown);
        settingsParser.get("player-two-rotate-counter-clockwise", twoRotateCounterClockwise);
        settingsParser.get("player-two-rotate-clockwise", twoRotateClockwise);

        playerTwoKeyMap->Left = stringToKey(twoLeft);
        playerTwoKeyMap->Right = stringToKey(twoRight);
        playerTwoKeyMap->Down = stringToKey(twoDown);
        playerTwoKeyMap->RotateCounterClockwise = stringToKey(twoRotateCounterClockwise);
        playerTwoKeyMap->RotateClockwise = stringToKey(twoRotateClockwise);

        settingsParser.saveToFile();
    } else
        this->reset();

    return true;
}

void GameSettings::set(std::string key, std::string value) {
    this->settingsParser.set(key, value);
    this->settingsParser.saveToFile();
}

void GameSettings::set(std::string key, bool value) {
    this->settingsParser.set(key, value);
    this->settingsParser.saveToFile();
}

void GameSettings::set(std::string key, int value) {
    this->settingsParser.set(key, value);
    this->settingsParser.saveToFile();
}

void GameSettings::set(std::string key, float value) {
    this->settingsParser.set(key, value);
    this->settingsParser.saveToFile();
}

bool GameSettings::writeDefaultsToFile()
{
    std::ofstream writeToFile;
    writeToFile.open("save/settings.ini");
    writeToFile << "#Global\nsound-effects-enabled = TRUE\nmusic-enabled = TRUE\nsound-volume = 5\nmusic-volume = 5\nscreen-height = 600\nscreen-width = 800\nstarfield-enabled = TRUE";
    writeToFile << "\n#Player 1\nplayer-one-down = S\nplayer-one-left = A\nplayer-one-right = D\nplayer-one-rotate-clockwise = W\nplayer-one-rotate-counter-clockwise = Left Control\nplayer-one-pause-game = Escape";
    writeToFile << "\n#Player 2\nplayer-two-down = Down\nplayer-two-left = Left\nplayer-two-right = Right\nplayer-two-rotate-clockwise = Right Control\nplayer-two-rotate-counter-clockwise = Up";
    writeToFile << "\n#Gameplay\ntrails-enabled = TRUE\ntrails-intensity = 1\ntrails-delay = 1\nanti-alias-enabled = TRUE\nrandom-bag = 0123456";
    writeToFile.close();
}


float GameSettings::cSize = 0.f;
float GameSettings::cSizeFixed = 0.f;

int GameSettings::hPadding = 0;
int GameSettings::currentScore = 0; // Used for high-score entry
int GameSettings::currentLevel = 0; // Used for high-score entry
bool GameSettings::bDemoMode = false;
