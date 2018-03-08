#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

#include "settings_parser.h"
#include "player_key_map.h"

class GameSettings {
public:
    GameSettings();
    bool load();
    bool fileExists();
    bool reset();
    void set(std::string key, std::string value);
    void set(std::string key, bool value);
    void set(std::string key, int value);
    void set(std::string key, float value);

    bool musicEnabled;
    bool soundEffectsEnabled;
    bool antiAliasEnabled;
    bool starfieldEnabled;
    float soundVolume;
    float musicVolume;
    bool trailsEnabled;
    int trailsDelay;
    int trailsIntensity;
    float screenHeight;
    float screenWidth;

    PlayerKeyMapPtr playerOneKeyMap;
    PlayerKeyMapPtr playerTwoKeyMap;

    //In memory settings
    enum Mode {
        SinglePlayer = 1, MultiPlayer = 2,
    };

    enum GameMode {
        Marathon = 0, LineClear = 1, Battle = 2, Sprint = 3
    };

    GameSettings::GameMode gameMode = GameMode::Marathon;
    GameSettings::Mode playMode = Mode::SinglePlayer;

    int level = 0;
    int height = 0;
    int lines = 25;
    int players = 1;
    int gameType = 0;
    bool vsCpu = false;
    int aiLevel = 0;
    int map = -1;
    std::string randomBag = "0123456";
    int randomBagIndex = 0;

    static float cSize;
    static float cSizeFixed;

    static int hPadding;
    static int currentScore;
    static int currentLevel;

    static bool bDemoMode;

protected:
    SettingsParser settingsParser;
    bool writeDefaultsToFile();
};

#endif //GAME_SETTINGS_H
