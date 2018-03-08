#include <utility/utility.h>
#include "custom_map_state.h"
#include "sound/sound_player.h"

CustomMapState::CustomMapState(StateStack& stack, Context context) : State(stack, context)
        , mSounds(*context.sounds)
{
    clearField(); // Clear the map

    sf::RenderWindow &window = *getContext().window;

    // Find the padding for this window, used to position the various text objects
    int dx = window.getSize().x;
    int dy = window.getSize().y;

    double aspect = dx * 1.0 / dy * 1.0;

    if( aspect > 1.33334)
        padding = (dx - (dy * 4 / 3)) / 2;


    mStarfield = new StarfieldHandler(window.getSize().x, window.getSize().y);
    tile = new Tile(window, 28, false);
    tetromino = new Tetromino();

    waveText = new WaveText(window, "CUSTOMIZE MAP"); // Wavy heading

    sf::Font &mainFont = context.fonts->get(Fonts::Prime);

    cSizeTiny = 9;
    TDescription.setFont(mainFont);
    TDescription.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    float height = 0.0f;

    while(height < tile->tileSize ) // The tiny font size should fit within one of the pField tiles
    {
        cSizeTiny++;
        TDescription.setCharacterSize(cSizeTiny);
        height = TDescription.getGlobalBounds().height;
    }

    height = 0.0f;
    cSizeSmall = 9;

    while(height < (window.getSize().y / 25)) // In this case, we want to be able to fit around 25 lines in the window
    {
        cSizeSmall++;
        TDescription.setCharacterSize(cSizeSmall);
        height = TDescription.getGlobalBounds().height;
    }

    TDescription.setFont(mainFont);
    TDescription.setFillColor(sf::Color(255, 255, 255));
    TDescription.setCharacterSize(cSizeTiny);
    std::string description = "Design your own\nmap to play on.\n\nUse the mouse to select\na color from the palette\n\
and draw on the playing field.\n\nSave your masterpiece to\none of the four map slots\nwhen you are done.\n";
    TDescription.setString(description);
    TDescription.setPosition(padding + tile->tileSize * 4, tile->tileSize * 13.5); // 7.5

    TDebug.setFont(mainFont);
    TDebug.setFillColor(sf::Color::White);
    TDebug.setCharacterSize(12);
    TDebug.setPosition(padding + cSizeSmall * 19.2, cSizeSmall * 5.2);

    std::string sOptions[4] = { "Clear", "Load", "Save", "Quit" };
    std::string sCustom[4] = { "MapA", "MapB", "MapC", "MapD" };

    for(int i = 0; i < 4; i++)
    {

        TOption[i].setFont(mainFont);
        TOption[i].setFillColor(sf::Color::White);
        TOption[i].setCharacterSize(cSizeTiny);
        TOption[i].setString(sOptions[i]);

        //TOption[i].setPosition(padding + tile->tileSize * 31, tile->tileSize * (12 + i));
        TOption[i].setPosition(padding + tile->tileSize * 31, tile->tileSize * (12.5 + i));


        TNumber[i].setFont(mainFont);
        TNumber[i].setFillColor(sf::Color::White);
        TNumber[i].setCharacterSize(cSizeTiny);
        TNumber[i].setString(sCustom[i]);
        TNumber[i].setPosition(padding + tile->tileSize * 31, tile->tileSize * (17.5 + i));

    }

    TMessage.setFont(mainFont);
    TMessage.setFillColor(sf::Color::White);
    TMessage.setCharacterSize(cSizeTiny);
    TMessage.setString("");
    TMessage.setPosition(padding + tile->tileSize * 31, tile->tileSize * (24.5));



    if(fexists("save/custom.map")) // Does the save file exist?
    {
        loadFile(); // It does, load it into memory
    }
    else { // The file does not exist, make a new one
        createSaveFile(true); // Clear the save file
    }

    mSounds.setVolume(context.settings->soundVolume);
}


bool CustomMapState::fexists(const std::string& filename)
{
    std::ifstream ifile(filename.c_str());
    return (bool)ifile;
}


void CustomMapState::createSaveFile(bool clear) // Create a new save file, or update the current one
{
    char c;

    saveToFile.open("save/custom.map");

    for(int i = 0; i < 800; i++)
    {
        c = '.';

        if(!clear)
        {
            if(nMap[i] > -1 && nMap[i] < 7)
                c = nMap[i] + 48;
        }

        saveToFile << c;

        if((i+1) % 10 == 0) saveToFile << std::endl;
        if((i+1) % 200 == 0) saveToFile << std::endl;

    }
    saveToFile.close();
}


void CustomMapState::loadFile()
{
    loadFromFile.open("save/custom.map");

    for(int i = 0; i < 800; i++)
    {
        char c;

        int value;

        loadFromFile >> c;

        if(c == '\n')
            loadFromFile >> c;

        value = (int)c - 48;

        if(value == -2) value = 7;

        if(value < 0 || value > 7)
        {
            // Something is wrong with the file, make a new one
            loadFromFile.close();

            createSaveFile(true); // Clear the save file
            return;
        }

        nMap[i] = value;
    }
    loadFromFile.close();
}



void CustomMapState::clearField()
{
    for(int y = 0; y < 20; y++) // Clear the working field
        for(int x = 0; x < 10; x++)
            nField[y][x] = 7;
}


void CustomMapState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    mStarfield->draw(window, getContext().settings->starfieldEnabled); // Draw the starfield first
    waveText->draw(window, cSizeSmall * 2);

    window.draw(TDescription);

    if(timerMessage.getElapsedTime().asSeconds() < 2) // Message when user load or save a map
        window.draw(TMessage);

    for(int i = 0; i < 4; i++) // Options and save slots
    {
        // If the mouse is hovering above this option, color is white
        if( (hoverX == 300 && hoverY - 300 == i) )
            TOption[i].setFillColor(sf::Color::White);
        else
            TOption[i].setFillColor(sf::Color(127, 127, 127)); // grey otherwise

        window.draw(TOption[i]);

        // If this map is either selected or the mouse is hovering above, color is white
        if(bMapSelected[i] || (hoverX == 300 && hoverY - 300 - 5 == i))
            TNumber[i].setFillColor(sf::Color::White);
        else
            TNumber[i].setFillColor(sf::Color(127, 127, 127)); // grey otherwise

        window.draw(TNumber[i]);
    }

    sf::RectangleShape mRectangle; // Used for making the starfield less visible
    mRectangle.setFillColor(sf::Color(0, 0, 0, 223));
    mRectangle.setSize(sf::Vector2f(tile->tileSize * 10, tile->tileSize * 20));
    mRectangle.setPosition( padding + 20 * tile->tileSize, 6.5f * tile->tileSize );
    window.draw(mRectangle);

    for(int y = 0; y < 20; y++) // The working field
    {
        for(int x = 0; x < 10; x++)
        {
            if(hoverX == x && hoverY == y)
                tile->mTile.setFillColor(tile->mColors[nSelectedColor]);
            else
                tile->mTile.setFillColor(tile->mColors[ nField[y][x] ]);  // Should be the actual current set color of the field

            tile->mTile.setPosition(padding + (x + 20) * tile->tileSize, (y + 6.5f) * tile->tileSize);
            window.draw(tile->mTile);
        }
    }

    mRectangle.setSize(sf::Vector2f(tile->tileSize * 2, tile->tileSize * 4));
    mRectangle.setPosition( padding + 31 * tile->tileSize, 7.5f * tile->tileSize );
    window.draw(mRectangle);

    for(int y = 0; y < 4; y++) // The palette
    {
        for(int x = 0; x < 2; x++)
        {
            tile->mTile.setFillColor(tile->mColors[ mCol[y][x] ]);
            tile->mTile.setPosition(padding + (x + 31) * tile->tileSize, (y + 7.5f) * tile->tileSize);
            window.draw(tile->mTile);
        }
    }
}


bool CustomMapState::update(sf::Time)
{
    mStarfield->update(); // Update the starfield
    waveText->update();

    // Draw on the playing field
    if(hoverX > -1 && hoverX < 10 && hoverY > -1 && hoverY < 20 && bLeftMouseButton)
        nField[hoverY][hoverX] = nSelectedColor;

    LastUpdate.restart(); // Ugly fix for pausing updates when the window is moved, instead of having separate threads etc.
    return true;
}


bool CustomMapState::handleEvent(const sf::Event& event)
{
    if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) || sf::Joystick::isButtonPressed(0, 1))
    {
        requestStackPop();
        requestStackPush(States::CustomizeMenu);
    }

    if(event.type == sf::Event::MouseMoved)
    {
        mouseX = sf::Mouse::getPosition(*getContext().window).x;
        mouseY = sf::Mouse::getPosition(*getContext().window).y;

        debugText = "MouseX: " + toString(mouseX) + "  MouseY: " + toString(mouseY) + "\n";

        hoverX = -1;
        hoverY = -1;

        // Is the mouse hovering above the field?
        if(mouseX >= padding + 20 * tile->tileSize && mouseX < padding + 30 * tile->tileSize)
        {
            if(mouseY >= 6.5 * tile->tileSize && mouseY < 26.5 * tile->tileSize)
            {
                hoverX = (mouseX - padding - 20 * tile->tileSize) / tile->tileSize;
                hoverY = (mouseY - 6.5 * tile->tileSize) / tile->tileSize;
            }
        }


        // Is the mouse hovering above the palette?
        if(mouseX >= padding + 31 * tile->tileSize && mouseX < padding + 33 * tile->tileSize)
        {
            if(mouseY >= 7.5 * tile->tileSize && mouseY < 11.5 * tile->tileSize)
            {
                hoverX = (mouseX - padding - 31 * tile->tileSize) / tile->tileSize + 100;
                hoverY = (mouseY - 7.5 * tile->tileSize) / tile->tileSize + 100;
                //debugText += "hoverX: " + toString(hoverX) + "  hoverY: " + toString(hoverY) + "\n"; //The XCoords are within range, the ycoords are within range?\n";
            }
        }

        // Is the mouse hovering above the options?
        if(mouseX >= padding + 31 * tile->tileSize &&
                mouseX < padding + 31 * tile->tileSize + TNumber[0].getGlobalBounds().width)
        {
            if(mouseY >= 12.5 * tile->tileSize && mouseY < 21.5 * tile->tileSize)
            {
                hoverX = 300;
                hoverY = (mouseY - 12.5 * tile->tileSize) / tile->tileSize + 300;
                debugText += "hoverX: " + toString(hoverX) + "  hoverY: " + toString(hoverY) + "\n"; //The XCoords are within range, the ycoords are within range?\n";
            }
        }
    }

    // Let the user hold down the mouse button to draw on the field
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && hoverX > -1 && hoverX < 10 && hoverY > -1 && hoverY < 20)
        bLeftMouseButton = true;
    else
        bLeftMouseButton = false;


    if (event.type != sf::Event::MouseButtonPressed)
        return false;

    // The event was clicking the mouse button, check palette and everything else

    // Select a color from the palette
    if(hoverX > 99 && hoverX < 102 && hoverY > 99 && hoverY < 104)
        nSelectedColor = mCol[hoverY - 100][hoverX - 100];

    // ============================================================================================

    // Clear button
    if(hoverX == 300 && hoverY == 300)
        clearField();

    // Load button
    if(hoverX == 300 && hoverY == 301)
    {
        // We already have the save file in memory in the nMap[] array,
        // let us copy it over to the nField[][] array
        for(int y = 0; y < 20; y++)
            for(int x = 0; x < 10; x++)
                nField[y][x] = nMap[ nMapSelected * 200 + y * 10 + x ];

        // Give some indication to user that map has been loaded
        TMessage.setString("Load ok.");
        timerMessage.restart();
    }

    // Save button
    if(hoverX == 300 && hoverY == 302)
    {
        for(int y = 0; y < 20; y++)
            for(int x = 0; x < 10; x++)
                nMap[ nMapSelected * 200 + y * 10 + x ] = nField[y][x];

        createSaveFile(false); // Save the contents of nMap[] to the save file

        // Give some indication to user that file has been saved
        TMessage.setString("Save ok.");
        timerMessage.restart();
    }

    // Quit button
    if(hoverX == 300 && hoverY == 303)
    {
        //Temp to get menu to reset the state
     //   getContext().settings->gameMode = GameSettings::GameMode::NoGameMode;
        requestStackPop();
        requestStackPush(States::CustomizeMenu);
    }

    // ============================================================================================

    // Select which map to save to or load from
    if(hoverX == 300 && (hoverY > 304 && hoverY < 309))
    {
        bMapSelected[nMapSelected] = false;
        bMapSelected[ hoverY - 305 ] = true;
        nMapSelected = hoverY - 305;
    }

    return true;
}
