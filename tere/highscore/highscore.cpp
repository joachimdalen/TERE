#include "highscore.h"
#include "utility/utility.h"

HighScore::HighScore()
{
    if(fexists("save/highscores.dat"))  // Does the save file exist?
        loadHighScores();               // It does, load it into memory
    else                                // The file does not exist, make a new one
        createSaveFile(true);           // Clear the save file
}


HighScore::~HighScore() {}


bool HighScore::fexists(const std::string& filename)
{
    std::ifstream ifile(filename.c_str());
    return (bool)ifile;
}


void HighScore::createSaveFile(bool clear) // Create a new save file, or update the current one
{
    saveToFile.open("save/highscores.dat");

    std::string temp[16], tmp, tmp2;

    //char temp;
    int keyIndex = 0;

    for(int mode = 0; mode < 3; mode++ )
    {
        for(int entry = 0; entry < 8; entry++)
        {
            // Name takes 16 chars
            tmp = "";

            if(clear)
            {
                for(int x = 0; x < 16 - scoreName[mode][entry].length(); x++)
                    tmp += "#";

                tmp = scoreName[mode][entry] + tmp;
            }
            else
            {
                for(int x = 0; x < 16 - tempScoreName[mode][entry].length(); x++)
                    tmp += "#";

                tmp = tempScoreName[mode][entry] + tmp;
            }

            for(int x = 0; x < tmp.length(); x++)
            {
                temp[x] = tmp[x]^(key[keyIndex] + 100);
                keyIndex++;
                saveToFile << temp[x];
            }

            // Score takes 9 chars
            tmp = "";

            if(clear)
                tmp2 = toString(score[mode][entry]);
            else
                tmp2 = toString(tempScore[mode][entry]);


            for(int x = 0; x < 9 - tmp2.length(); x++)
                tmp += "#";

            tmp = tmp2 + tmp;

            for(int x = 0; x < tmp.length(); x++)
            {
                temp[x] = tmp[x]^(key[keyIndex] + 100);
                keyIndex++;
                saveToFile << temp[x];
            }

            // Level takes 3 chars
            tmp = "";

            if(clear)
                tmp2 = toString(scoreLevel[mode][entry]);
            else
                tmp2 = toString(tempScoreLevel[mode][entry]);

            for(int x = 0; x < 3 - tmp2.length(); x++)
                tmp += "#";

            tmp = tmp2 + tmp;

            for(int x = 0; x < tmp.length(); x++)
            {
                temp[x] = tmp[x]^(key[keyIndex] + 100);
                keyIndex++;
                saveToFile << temp[x];
            }
        }
    }
    saveToFile.close();
}


inline bool isInteger(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        return false;

    char * p;
    strtol(s.c_str(), &p, 10);

    return (*p == 0);
}


void HighScore::loadHighScores()
{
    bool problem = false;
    std::string tempString, tmpString, tmp, tmp2;
    char c;

    int keyIndex = 0;

    loadFromFile.open("save/highscores.dat");

    for(int mode = 0; mode < 3; mode++)
    {
        for(int entry = 0; entry < 8; entry++)
        {
            //  Name
            tmp = "";

            for(int x = 0; x < 16; x++)
            {
                loadFromFile >> c;
                tmp += c;
            }

            tempScoreName[mode][entry] = "";

            for(int x = 0; x < 16; x++)
            {
                tmp2 = tmp[x]^(key[keyIndex] + 100);

                if(tmp2 != "#")
                    tempScoreName[mode][entry] += tmp2;

                keyIndex++;
            }

            //  Score
            tmp = "";

            for(int x = 0; x < 9; x++)
            {
                loadFromFile >> c;
                tmp += c;
            }

            tempString = "";

            for(int x = 0; x < 9; x++)
            {
                tmp2 = tmp[x]^(key[keyIndex] + 100);

                if(tmp2 != "#")
                    tempString += tmp2;

                keyIndex++;
            }

            if(!isInteger(tempString)) // This is not the expected integer
                problem = true;
            else
                tempScore[mode][entry] = std::stoi(tempString);

            //  Level
            tmp = "";

            for(int x = 0; x < 3; x++)
            {
                loadFromFile >> c;
                tmp += c;
            }

            tempString = "";

            for(int x = 0; x < 3; x++)
            {
                tmp2 = tmp[x]^(key[keyIndex] + 100);

                if(tmp2 != "#")
                    tempString += tmp2;

                keyIndex++;
            }

            if(!isInteger(tempString)) // This is not the expected integer
                problem = true;
            else
                tempScoreLevel[mode][entry] = std::stoi(tempString);

            if (loadFromFile.eof()) // reached end-of-file prematurely (file is shorter than it should be)
                problem = true;

            if(problem)
                break;
        }

        if(problem)
            break;
    }

    if(problem) // Problem encountered, recreate the high score list
    {
        //std::cout << "Oops, problem!\n";
        loadFromFile.close();
        createSaveFile(true);
        loadHighScores();
    }

    loadFromFile.close();

    // The entries are now in tempScoreName (strings) and tempScore (int)
}


int HighScore::validHighScore(int mode, int value)
{
    // Mode 0 - Marathon
    // Mode 1 - Line Clear
    // Mode 2 - Sprint (Sprint Mode is mode 3 in-game, less is better)

    if(mode < 2)
    {
        if(value >= tempScore[mode][7])
            return getPosition(mode, value);
        else
            return -1;
    }
    else // Sprint mode
    {
        if(value <= tempScore[mode][7])
            return getPosition(mode, value);
        else
            return -1;
    }
}


void HighScore::shiftList(int mode, int pos, std::string name, int value, int level)
{
    for(int index = 7; index > pos; index--)
    {
        tempScoreName[mode][index] = tempScoreName[mode][index - 1];
        tempScore[mode][index] = tempScore[mode][index - 1];
        tempScoreLevel[mode][index] = tempScoreLevel[mode][index - 1];
    }

    tempScoreName[mode][pos] = name;
    tempScore[mode][pos] = value;
    tempScoreLevel[mode][pos] = level;
}


int HighScore::getPosition(int mode, int value)
{
    int pos = 7;

    if(mode < 2)
    {
        for(int i = 7; i > -1; i--)
        {
            if(tempScore[mode][i] <= value)
                pos = i;
        }
    }
    else
    {
        for(int i = 7; i > -1; i--)
        {
            if(tempScore[mode][i] >= value)
                pos = i;
        }
    }
    return pos;
}
