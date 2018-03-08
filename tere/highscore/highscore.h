#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <iostream>
#include <fstream>

class HighScore
{
public:
    HighScore();
    virtual ~HighScore();

    void createSaveFile(bool clear);
    void loadHighScores();
    bool fexists(const std::string& filename);
    int validHighScore(int mode, int value);
    int getPosition(int mode, int value);
    //void replaceHighScore();
    //void shuffleList();
    void shiftList(int mode, int pos, std::string name, int value, int level);

    // This is the key for the simple XOR encryption
    char key[1041] = "TetrisIsATileMatchingPuzzleVideoGameOriginallyDesignedAndProgrammedByRussianGameDesignerAlexeyPajitnovItWasReleasedOnJune61984WhileHeWasWorkingForTheDorodnitsynComputingCentreOfTheAcademyOfScienceOfTheSovietUnionInMoscowHeDerivedItsNameFromTheGreekNumericalPrefixTetraAllOfTheGamesPiecesContainFourSegmentsAndTennisPajitnovsFavoriteSportTetrisWasTheFirstEntertainmentSoftwareToBeExportedFromTheSovietUnionToTheUsWhereItWasPublishedBySpectrumHolobyteForCommodore64AndIbmPcTheTetrisGameIsAPopularUseOfTetrominoesTheFourElementSpecialCaseOfPolyominoesPolyominoesHaveBeenUsedInPopularPuzzlesSinceAtLeast1907AndTheNameWasGivenByTheMathematicianSolomonWGolombIn1953HoweverEvenTheEnumerationOfPentominoesIsDatedToAntiquityTheGameOrOneOfItsManyVariantsIsAvailableForNearlyEveryVideoGameConsoleAndComputerOperatingSystemAsWellAsOnDevicesSuchAsGraphingCalculatorsMobilePhonesPortableMediaPlayersPdasNetworkMusicPlayersAndEvenAsAnEasterEggOnNonMediaProductsLikeOscilloscopesItHasEvenInspiredTetrisServingDishesAndBeenPlayedOnTheSidesOfVariousBuildings";

    // We keep three sets of scores,    0 - Marathon, 1 - Line Clear, 2 - Sprint (Sprint is GameMode 3 in-game)


    // The names are top Tetris players who were finalists in the documentary "Ecstacy of Order - The Tetris Masters"

    std::string scoreName[3][8] = {
            { "Jonas", "Harry", "Matt", "Dana", "Thor", "Ben", "Trey", "Jesse" },
            { "Thor", "Trey", "Jonas", "Matt", "Ben", "Jesse", "Harry", "Dana" },
            { "Jesse", "Trey", "Ben", "Thor", "Dana", "Matt", "Harry", "Jonas" },
    };

    //{ 20000, 17500, 15000, 12500, 10000, 7500, 5000, 2500 }, // Marathon


    /*
    int score[3][8] = {
            { 20000, 17500, 15000, 12500, 10000, 7500, 5000, 2500 }, // Marathon
            { 4000, 3500, 3000, 2500, 2000, 1500, 1000, 500 }, // Line Clear
            { 300000, 330000, 360000, 390000, 420000, 450000, 480000, 510000 }, // Sprint (in milliseconds), from 3 min up to 8.5 min
    };
     */

    int score[3][8] = {
            { 10000, 9500, 8750, 8000, 7250, 6500, 5750, 5000 }, // Marathon
            { 2000, 1800, 1600, 1400, 1200, 1000, 800, 500 }, // Line Clear
            { 600000, 720000, 840000, 960000, 1080000, 1200000, 1320000, 1500000 }, // Sprint (in milliseconds), from 10 min up to 25 min
    };

    int scoreLevel[3][8] = {
            { 9, 8, 7, 6, 4, 2, 1, 0 },
            { 9, 8, 7, 6, 4, 2, 1, 0 },
            { 9, 8, 7, 6, 4, 2, 1, 0 },
    };

    std::string tempScoreName[3][8];
    int tempScore[3][8];
    int tempScoreLevel[3][8];

    std::ofstream saveToFile;
    std::ifstream loadFromFile;
};

#endif //HIGHSCORE_H
