#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"

#include "Animation.h"
#include "Apple.h"
#include "Coin.h"
#include "Item.h"
#include "Key.h"
#include "Platform.h"
#include "Player.h"
#include "Potion.h"
#include "Portal.h"
#include "Trap.h"

#include "TextureLoadException.h"
#include "FontLoadException.h"
#include "SoundLoadException.h"
#include "OutOfBoundsException.h"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <vector>

static const float VIEW_HEIGHT{512.0f};
const std::string ResourcePath{"C:/Users/Brian/29c++/29c++ final/resources/"};
const std::string Sounds{"Sounds/"};
const std::string Sprites{"Sprites/"};
const std::string Fonts{"Font/"};
const std::string Texts{"Text/"};

void resizeView(const sf::RenderWindow &window, sf::View &view)
{
    float aspectRatio{static_cast<float>(window.getSize().x / static_cast<float>(window.getSize().y))};
    view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT);
}

sf::String gameStatsDisplay(Player &player, sf::Time time)
{
    std::ostringstream sout;
    sout << "Time left: " << static_cast<int>(time.asSeconds()) << "   Score: " << player.getScore();
    return sout.str();
}

sf::String livesDisplay(Player &player)
{
    std::ostringstream sout;
    sout << "Lives: x" << player.getLives();
    return sout.str();
}

sf::String finishDisplay(Player &player, int timeTaken)
{
    std::ostringstream sout;
    int coinsCollected{player.getScore() / 100};
    sout << "   Coins collected : " << coinsCollected << "/20" << std::endl;
    sout << "   Time taken      : " << timeTaken << std::endl << std::endl;

    int timeBonus{500 - timeTaken};
    if(player.getLives() == 0)
    {
        timeBonus = 0;
    }
    int livesBonus{player.getLives() * 100};
    if(player.getScore() < 1000)
    {
        sout << "\t Coin bonus  :  " << player.getScore() << std::endl;
    } else
    {
        sout << "\t Coin bonus  : " << player.getScore() << std::endl;
    }
    sout << "\t Time bonus  :  " << timeBonus << std::endl;
    sout << "\t Lives bonus :  " << livesBonus << std::endl;
    int finalScore = player.getScore() + timeBonus + livesBonus;
    sout << "\t--------------------" << std::endl;
    sout << "\t Final score : ";
    if(finalScore == 0)
    {
        sout << " " << finalScore;
    } else
    {
        sout << finalScore;
    }
    sout << std::endl << std::endl;
    sout << "Press Esc to view highscores" << std::endl;
    return sout.str();
}

struct Score
{
public:
    std::string name;
    std::string date;
    int score;
    Score(std::string name, std::string date, int score) : name(name), date(date), score(score) { }
    Score() { }

    bool operator>(const Score& other) const
    {
        if(score > other.score)
            return true;
        return false;
    }
    friend std::ostream& operator<<(std::ostream& os, const Score& score) // Insertion operator
    {
        os << std::endl  << "         " << score.name << std::string(13 - (score.name).length(), ' ') << score.date << std::string(10 - (score.date).length(), ' ') << score.score;
        return os;
    }
};

sf::String scoresDisplay(Player &player, int timeTaken, std::string user)
{
    std::multiset<Score, std::greater <Score> > scores; // STL container

    int timeBonus{500 - timeTaken};
    int livesBonus{player.getLives() * 100};
    int finalScore{player.getScore() + timeBonus + livesBonus};

    std::ifstream ifs;
    ifs.open(ResourcePath + Texts + "scores.txt");
    if (!ifs)
    {
        std::cerr << "Cannot open file!!" << std::endl;
        return "";
    }

    std::string name;
    std::string date;
    int score;
    while(ifs >> score)
    {
        ifs.ignore(1, ';');
        getline(ifs, name, ';');
        ifs >> date;
        scores.insert(Score(name, date, score));
    }
    ifs.close();

    std::time_t now{std::time(0)};
    std::ostringstream os;
    tm* a = localtime(&now);
    int d{a->tm_mday}, m{a->tm_mon + 1}, y{a->tm_year - 100};
    if (m < 10)
    {
        os << '0';
    }
    os << m << '/';
    if (d < 10)
    {
        os << '0';
    }
    os << d << '/';
    if (y < 10)
    {
        os << '0';
    }
    os << y;
    scores.insert(Score(user, os.str(), finalScore));

    std::ofstream ofs;
    ofs.open(ResourcePath + Texts + "scores.txt");

    std::multiset<Score, std::greater <Score> >::iterator itr = scores.begin();
    int numPrinted{0};
    while (itr != scores.end() && numPrinted < 100)
    {
        ofs << itr->score << ';' << itr->name << ';' << itr->date << std::endl;
        numPrinted++;
        std::advance(itr, 1);
    }
    ofs.close();

    std::ostringstream sout;
    std::ostringstream fout;
    itr = scores.begin();
    numPrinted = 0;

    sout << "\t\t\t Final score : " << finalScore << std::endl << std::endl;
    sout << "\t\t\t\tTop scores : " << std::endl;
    fout << "Name           Date           Score" << std::endl;

    while (itr != scores.end() && numPrinted < 100)
    {
        if (numPrinted < 5)
            sout << *itr;
        fout << itr->name << std::string(15-(itr->name).length(), ' ') << itr->date << std::string(15-(itr->date).length(), ' ') << itr->score << std::endl;
        numPrinted++;
        std::advance(itr, 1);
    }
    sout << std::endl << std::endl << "\t\t\tPress Enter to exit" << std::endl;

    ofs.open(ResourcePath + Texts + "TopScores.txt");
    ofs << fout.str();
    ofs.close();
    return sout.str();
}

std::string getWelcomeText(const std::string filename)
{
    std::ifstream fin(filename);
    std::string text, buffer;

    if (!fin)
    {
        std::cerr << "Cannot open file!!" << std::endl;
        return "";
    }

    while (getline(fin, buffer))
    {
        text += buffer += '\n';
    }

    text += "Please enter the name used for your highscore and press Enter ===> \n";

    fin.close();
    return text;
}

char getKey()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        return  'a';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        return  'b';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        return  'c';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        return  'd';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        return  'e';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        return  'f';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        return  'g';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        return  'h';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        return  'i';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        return  'j';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        return  'k';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        return  'l';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        return  'm';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
        return  'n';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
        return  'o';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        return  'p';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        return  'q';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        return  'r';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        return  's';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        return  't';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
        return  'u';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
        return  'v';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        return  'w';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        return  'x';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
        return  'y';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        return  'z';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        return  ' ';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        return  '\n';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
        return  '\b';
    return ' ';
}

void welcomeScreen(std::string &name)
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Welcome To Pink Bean's Adventure!", sf::Style::Close | sf::Style::Resize);
    std::string text;
    sf::Font font;

    sf::RectangleShape player(v2f(200.0f, 200.0f));
    player.setPosition(100.0f, 600.0f);

    sf::Texture playerTexture;
    playerTexture.loadFromFile(ResourcePath + Sprites + "Pink_Bean_intro.png");
    player.setTexture(&playerTexture);

    text = getWelcomeText(ResourcePath + Texts + "welcometext.txt");
    sf::Text contents;

    contents.setString(text.c_str());
    contents.setFont(font);
    contents.setCharacterSize(20);
    contents.setFillColor(sf::Color::Cyan);
    contents.setStyle(sf::Text::Bold);
    contents.setPosition(32.0f, 32.0f);
    try
    {
        if(!font.loadFromFile(ResourcePath + Fonts + "Arial.ttf"))
            throw Font::Error("Font Error");
    } catch (const Font::Error& error)
    {
        std::cerr << error << std::endl;
        exit(0);
    }
    char input{' '};
    while (window.isOpen())
    {
        sf::Event event;
        sf::Text title;
        title.setString("Welcome to Pink Bean's Adventure!");
        title.setStyle(sf::Text::Bold);

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    input = getKey();
                    if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z') || input == ' ')
                    {
                        name += input;
                        player.move(8.0f, 0.0f);
                    }
                    if (input == '\b')
                    {
                        name = name.substr(0, name.length() - 1);
                        if (name.length() != 0)
                        {
                            player.move(-8.0f, 0.0f);
                        }
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                        window.close();
                    break;
                default:
                    break;
            }
        }

        window.clear();
        contents.setString((text + name + '_').c_str());
        window.draw(contents);
        window.draw(player);
        window.display();
    }
}

void checkBounds(Player &player)
{
    try
    {
        if((player.GetPosition().x < -3000.0f || player.GetPosition().x > 12000.0f) && (player.GetPosition().x != 13500.0f && player.GetPosition().x != 15000.0f))
            throw Bound::Error("Out of Bounds Error");
        if(player.GetPosition().y < -2500.0f || player.GetPosition().y > 6000.0f)
            throw Bound::Error("Out of Bounds Error");
    } catch(const Bound::Error& error)
    {
        std::cerr << error << std::endl;
        exit(0);
    }
}

int main()
{

    /**Welcome page**/

    std::string name;
    welcomeScreen(name);



    /** Window **/

    sf::RenderWindow window(sf::VideoMode(512, 512), "Test");
    sf::View view(v2f(0.0f, 0.0f), v2f(VIEW_HEIGHT, VIEW_HEIGHT));



    /** Time **/

    float deltaTime{0.0f};
    const sf::Time timeLimit{sf::seconds(500)};
    int timeTaken;
    sf::Time timeRemaining;



    /** Clocks **/

    sf::Clock clock;
    sf::Clock gameTimer;
    sf::Clock appleTimer;



    /** Player **/

    sf::Texture playerTexture;
    try
    {
        if(!playerTexture.loadFromFile(ResourcePath + Sprites + "PBs.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    Player player(&playerTexture, v2u(4, 6), 0.25f, 300.0f, 150.0f);



    /** Platforms **/

    sf::Texture platformTexture;
    try
    {
        if(!platformTexture.loadFromFile(ResourcePath + Sprites + "Green.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<Platform> platforms;
    // Part 1
    platforms.push_back(Platform(&platformTexture, v2f(30.0f, 50.0f), v2f(-1600.0f, 0.0f))); // Startwall
    platforms.push_back(Platform(&platformTexture, v2f(500.0f, 20.0f), v2f(0.0f, 1100.0f))); // Floor
    platforms.push_back(Platform(&platformTexture, v2f(500.0f, 10.0f), v2f(0.0f, -3000.0f))); // Ceiling
    platforms.push_back(Platform(&platformTexture, v2f(3.0f, 20.0f), v2f(750.0f, 1000.0f))); // S1
    platforms.push_back(Platform(&platformTexture, v2f(3.0f, 20.0f), v2f(1000.0f, 900.0f))); // S2
    platforms.push_back(Platform(&platformTexture, v2f(3.0f, 20.0f), v2f(1250.0f, 800.0f))); // S3
    platforms.push_back(Platform(&platformTexture, v2f(3.0f, 20.0f), v2f(1500.0f, 700.0f))); // S4
    platforms.push_back(Platform(&platformTexture, v2f(3.0f, 20.0f), v2f(2200.0f, 900.0f))); // S5
    platforms.push_back(Platform(&platformTexture, v2f(3.0f, 5.0f), v2f(3000.0f, -500.0f))); // A1
    platforms.push_back(Platform(&platformTexture, v2f(3.0f, 28.0f), v2f(3650.0f, 900.0f))); // S6
    platforms.push_back(Platform(&platformTexture, v2f(5.0f, 3.0f), v2f(4350.0f, -750.0f))); // A2
    platforms.push_back(Platform(&platformTexture, v2f(3.0f, 7.0f), v2f(4500.0f, -750.0f))); // S7
    platforms.push_back(Platform(&platformTexture, v2f(3.0f, 20.0f), v2f(5150.0f, 700.0f))); // S8
    platforms.push_back(Platform(&platformTexture, v2f(10.0f, 5.0f), v2f(7500.0f, -500.0f))); // A3
    platforms.push_back(Platform(&platformTexture, v2f(10.0f, 15.0f), v2f(7500.0f, -2000.0f))); // A4
    platforms.push_back(Platform(&platformTexture, v2f(5.0f, 5.0f), v2f(7750.0f, 0.0f))); // Door
    // Part 2
    platforms.push_back(Platform(&platformTexture, v2f(5.0f, 20.0f), v2f(-400.0f, 3000.0f))); // A1.1
    platforms.push_back(Platform(&platformTexture, v2f(8.0f, 5.0f), v2f(0.0f, 3500.0f))); // A1.1
    platforms.push_back(Platform(&platformTexture, v2f(5.0f, 10.0f), v2f(-2250.0f, 4000.0f))); // A1.2
    platforms.push_back(Platform(&platformTexture, v2f(5.0f, 3.0f), v2f(950.0f, 4500.0f))); // A2
    platforms.push_back(Platform(&platformTexture, v2f(15.0f, 5.0f), v2f(1450.0f, 5250.0f))); // A3.1
    platforms.push_back(Platform(&platformTexture, v2f(10.0f, 75.0f), v2f(2000.0f, 5250.0f))); // A3.2
    platforms.push_back(Platform(&platformTexture, v2f(10.0f, 6.0f), v2f(-150.0f, 5050.0f))); // A4
    platforms.push_back(Platform(&platformTexture, v2f(50.0f, 50.0f), v2f(2350.0f, 8250.0f))); // Floor1
    platforms.push_back(Platform(&platformTexture, v2f(10.0f, 50.0f), v2f(-1000.0f, 8250.0f))); // Floor2
    platforms.push_back(Platform(&platformTexture, v2f(10.0f, 50.0f), v2f(-1500.0f, 8000.0f))); // S1
    platforms.push_back(Platform(&platformTexture, v2f(10.0f, 50.0f), v2f(-2000.0f, 7750.0f))); // S2
    platforms.push_back(Platform(&platformTexture, v2f(5.0f, 50.0f), v2f(-2250.0f, 7500.0f))); // S3
    platforms.push_back(Platform(&platformTexture, v2f(500.0f, 5.0f), v2f(0.0f, 6500.0f))); // ActualFloor
    platforms.push_back(Platform(&platformTexture, v2f(5.0f, 200.0f), v2f(-3250.0f, 7500.0f))); // Backwall
    // Part 3
    platforms.push_back(Platform(&platformTexture, v2f(5.0f, 10.0f), v2f(7250.0f, -850.0f))); // Startwall P2
    platforms.push_back(Platform(&platformTexture, v2f(10.0f, 35.0f), v2f(9000.0f, 0.0f))); // S9
    platforms.push_back(Platform(&platformTexture, v2f(10.0f, 35.0f), v2f(10500.0f, -2250.0f))); // A5
    platforms.push_back(Platform(&platformTexture, v2f(10.0f, 200.0f), v2f(12500.0f, 1100.0f))); // Endwall



    /** Items **/

    /* Coins (20) */
    sf::Texture coinTexture;
    try
    {
        if(!coinTexture.loadFromFile(ResourcePath + Sprites + "Coin.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<Coin> coins;
    // Part 1
    coins.push_back(Coin(&coinTexture, v2f(975.0f, -200.0f)));
    coins.push_back(Coin(&coinTexture, v2f(1485.0f, -400.0f)));
    coins.push_back(Coin(&coinTexture, v2f(2175.0f, -200.0f)));
    coins.push_back(Coin(&coinTexture, v2f(2990.0f, -850.0f)));
    coins.push_back(Coin(&coinTexture, v2f(3650.0f, -600.0f)));
    coins.push_back(Coin(&coinTexture, v2f(4500.0f, -1200.0f)));
    coins.push_back(Coin(&coinTexture, v2f(5165.0f, -400.0f)));
    coins.push_back(Coin(&coinTexture, v2f(6500.0f, 0.0f)));
    // Part 2
    coins.push_back(Coin(&coinTexture, v2f(300.0f, 3150.0f)));
    coins.push_back(Coin(&coinTexture, v2f(950.0f, 4150.0f)));
    coins.push_back(Coin(&coinTexture, v2f(1350.0f, 4900.0f)));
    coins.push_back(Coin(&coinTexture, v2f(1000.0f, 5650.0f)));
    coins.push_back(Coin(&coinTexture, v2f(-100.0f, 4650.0f)));
    coins.push_back(Coin(&coinTexture, v2f(-350.0f, 5600.0f)));
    coins.push_back(Coin(&coinTexture, v2f(-1300.0f, 5400.0f)));
    // Part 3
    coins.push_back(Coin(&coinTexture, v2f(8250.0f, -1000.0f)));
    coins.push_back(Coin(&coinTexture, v2f(9200.0f, -1850.0f)));
    coins.push_back(Coin(&coinTexture, v2f(9900.0f, -2300.0f)));
    coins.push_back(Coin(&coinTexture, v2f(10500.0f, 0.0f)));
    coins.push_back(Coin(&coinTexture, v2f(11500.0f, 0.0f)));

    /* Apples (4) */
    sf::Texture appleTexture;
    try
    {
        if(!appleTexture.loadFromFile(ResourcePath + Sprites + "Apple.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<Apple> apples;
    // Part 1
    apples.push_back(Apple(&appleTexture, v2f(4000.0f, 0.0f)));
    // Part 2
    apples.push_back(Apple(&appleTexture, v2f(300.0f, 5650.0f)));
    apples.push_back(Apple(&appleTexture, v2f(-2200.0f, 4900.0f)));
    // Part 3
    apples.push_back(Apple(&appleTexture, v2f(7900.0f, -850.0f)));

    /* Key (1) */
    sf::Texture keyTexture;
    try
    {
        if(!keyTexture.loadFromFile(ResourcePath + Sprites + "Key.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    Key key(&keyTexture, v2f(1300.0f, 5650.0f));


    /* Potion (3) */
    sf::Texture potionTexture;
    try
    {
        if(!potionTexture.loadFromFile(ResourcePath + Sprites + "Potion.png"))
        throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<Potion> potions;

    // Part 1
    potions.push_back(Potion(&potionTexture, v2f(5500.0f, 0.0f)));
    // Part 2
    potions.push_back(Potion(&potionTexture, v2f(-350.0f, 5800.0f)));
    // Part 3
    potions.push_back(Potion(&potionTexture, v2f(8600.0f, -1850.0f)));



    /** Portals **/

    /* Portals (2) */
    sf::Texture portalTexture;
    try
    {
        if(!portalTexture.loadFromFile(ResourcePath + Sprites + "Portal.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<Portal> portals;
    // Part 1
    portals.push_back(Portal(&portalTexture, v2f(2.0f, 2.0f), v2f(7000.0f, 0.0f), v2f(-50.0f, 3150.0f)));
    // Part 2
    portals.push_back(Portal(&portalTexture, v2f(2.0f, 2.0f), v2f(-2300.0f, 3400.0f), v2f(7000.0f, 0.0f)));

    /* Door (1) */
    sf::Texture doorTexture;
    try
    {
        if(!doorTexture.loadFromFile(ResourcePath + Sprites + "Door.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<Portal> doors;

    doors.push_back(Portal(&doorTexture, v2f(2.5f, 3.0f), v2f(7300.0f, -50.0f), v2f(7600.0f, -900.0f)));



    /** Traps **/

    /* Spikes (6) */
    sf::Texture spikeTexture;
    try
    {
        if(!spikeTexture.loadFromFile(ResourcePath + Sprites + "Spike.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<Trap> traps;
    // Part 1
    traps.push_back(Trap(&spikeTexture, v2f(4.0f, 1.0f), v2f(1850.0f, 50.0f), 0));
    traps.push_back(Trap(&spikeTexture, v2f(2.0f, 1.0f), v2f(2450.0f, 50.0f), 0));
    traps.push_back(Trap(&spikeTexture, v2f(2.0f, 1.0f), v2f(4900.0f, 50.0f), 0));
    // Part 2
    traps.push_back(Trap(&spikeTexture, v2f(3.0f, 4.0f), v2f(0.0f, 5650.0f), 0));
    traps.push_back(Trap(&spikeTexture, v2f(3.5f, 2.0f), v2f(-325.0f, 6150.0f), 0));
    traps.push_back(Trap(&spikeTexture, v2f(5.0f, 4.0f), v2f(-2750.0f, 6050.0f), 0));

    /* Thorns (8) */
    sf::Texture thornTexture;
    thornTexture.loadFromFile(ResourcePath + Sprites + "Thorn.png");

    // Part 1
    traps.push_back(Trap(&thornTexture, v2f(0.3f, 0.5f), v2f(1665.0f, -250.0f), 90));
    traps.push_back(Trap(&thornTexture, v2f(0.3f, 0.5f), v2f(3000.0f, -235.0f), 180));
    traps.push_back(Trap(&thornTexture, v2f(0.3f, 0.5f), v2f(5750.0f, 85.0f), 0));
    traps.push_back(Trap(&thornTexture, v2f(0.3f, 0.5f), v2f(6250.0f, 85.0f), 0));
    // Part 2
    traps.push_back(Trap(&thornTexture, v2f(0.3f, 0.5f), v2f(1485.0f, 4950.0f), -90));
    traps.push_back(Trap(&thornTexture, v2f(0.3f, 0.5f), v2f(-1950.0f, 5250.0f), 0));
    traps.push_back(Trap(&thornTexture, v2f(0.3f, 0.5f), v2f(0.0f, 4750.0f), 0));
    // Part 3
    traps.push_back(Trap(&thornTexture, v2f(0.3f, 0.5f), v2f(8000.0f, -2000.0f), 90));
    traps.push_back(Trap(&thornTexture, v2f(0.3f, 0.5f), v2f(9515.0f, -1700.0f), 90));


    /* Fire (3) */
    sf::Texture fireTexture;
    try
    {
        if(!fireTexture.loadFromFile(ResourcePath + Sprites + "Fire.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    // Part 2
    traps.push_back(Trap(&fireTexture, v2f(5.0f, 2.0f), v2f(8250.0f, 0.0f), 0));
    traps.push_back(Trap(&fireTexture, v2f(3.0f, 2.0f), v2f(9650.0f, 0.0f), 0));
    // Part 3
    traps.push_back(Trap(&fireTexture, v2f(4.0f, 3.0f), v2f(900.0f, 4850.0f), 0));



    /** Sprites **/

    /* Checkpoint Flags */
    std::vector<sf::Sprite> checkpointFlags;
    //Part 1
    sf::Texture checkpointFlagTexture1;
    try
    {
        if(!checkpointFlagTexture1.loadFromFile(ResourcePath + Sprites + "CheckpointFlag1.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite checkpointFlag1;
    checkpointFlag1.setTexture(checkpointFlagTexture1);
    checkpointFlag1.setScale(v2f(2.0f, 2.0f));
    checkpointFlag1.setPosition(v2f(100.0f, -100.0f));
    checkpointFlags.push_back(checkpointFlag1);
    // Part 2
    sf::Texture checkpointFlagTexture2;
    try
    {
        if(!checkpointFlagTexture2.loadFromFile(ResourcePath + Sprites + "CheckpointFlag2.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite checkpointFlag2;
    checkpointFlag2.setTexture(checkpointFlagTexture2);
    checkpointFlag2.setScale(v2f(2.0f, 2.0f));
    checkpointFlag2.setPosition(v2f(50.0f, 3050.0f));
    checkpointFlags.push_back(checkpointFlag2);
    // Part 3
    sf::Texture checkpointFlagTexture3;
    try
    {
        if(!checkpointFlagTexture3.loadFromFile(ResourcePath + Sprites + "CheckpointFlag3.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite checkpointFlag3;
    checkpointFlag3.setTexture(checkpointFlagTexture3);
    checkpointFlag3.setScale(v2f(2.0f, 2.0f));
    checkpointFlag3.setPosition(v2f(7700.0f, -950.0f));
    checkpointFlags.push_back(checkpointFlag3);

    /* End Flag */
    sf::Texture endFlagTexture;
    try
    {
        if(!endFlagTexture.loadFromFile(ResourcePath + Sprites + "EndFlag.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite endFlag;
    endFlag.setTexture(endFlagTexture);
    endFlag.setScale(v2f(2.0f, 2.0f));
    endFlag.setPosition(v2f(11800.0f, -100.0f));

    /* Clouds */
    srand(time(0));
    sf::Texture cloudTexture;
    try
    {
        if(!cloudTexture.loadFromFile(ResourcePath + Sprites + "Cloud.png"))
            throw Textures::Error("Texture Error");
    } catch(const Textures::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<sf::Sprite> clouds;

    unsigned numClouds{static_cast<unsigned>(rand() % 50 + 150)};
    for(auto i = 0u; i < numClouds; i++)
    {
        sf::Sprite tempCloud;
        float randomScale{static_cast<float>(rand() % 4 + 1)};
        float randomX{static_cast<float>(rand() % 12000)};
        float randomY{static_cast<float>((rand() % 9000) - 3000)};
        tempCloud.setOrigin(v2f(50.0f, 50.0f));
        tempCloud.setTexture(cloudTexture);
        tempCloud.setScale(v2f(randomScale, randomScale));
        tempCloud.setPosition(v2f(randomX, randomY));
        clouds.push_back(tempCloud);
    }

    /* Key Icon & Key Shadow*/
    sf::Sprite keyIcon;
    keyIcon.setTexture(keyTexture);
    keyIcon.setOrigin(-175.0f, -150.0f);

    sf::Sprite keyShadow;
    keyShadow.setTexture(keyTexture);
    keyShadow.setColor(sf::Color::Black);
    keyShadow.setOrigin(-175.0f, -150.0f);



    /** Text **/

    sf::Font font;
    try
    {
        if(!font.loadFromFile(ResourcePath + Fonts + "joystix.ttf"))
            throw Font::Error("Font Error");
    } catch(const Font::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }

    /* Game Stats (Time & Score) */
    sf::Text gameStats(gameStatsDisplay(player, timeRemaining), font);
    gameStats.setCharacterSize(20);
    gameStats.setStyle(sf::Text::Bold);
    gameStats.setFillColor(sf::Color::White);
    gameStats.setOrigin(v2f(250.0f, -225.0f));
    sf::RectangleShape blackBar(v2f(512.0f, 36.0f));
    blackBar.setFillColor(sf::Color::Black);
    blackBar.setOrigin(v2f(256.0f, -221.0f));

    /* Lives Remaining */
    sf::Text lives(livesDisplay(player), font);
    lives.setCharacterSize(20);
    lives.setStyle(sf::Text::Bold);
    lives.setFillColor(sf::Color::Black);
    lives.setOrigin(v2f(235.0f, 250.0f));

    /* Score Summary */
    sf::Text finish(finishDisplay(player, 0), font);
    finish.setCharacterSize(20);
    finish.setStyle(sf::Text::Bold);
    finish.setFillColor(sf::Color::Black);
    finish.setOrigin(100.0f, 100.0f);

    /* Door Message */
    sf::Text doorMessage("Obtain\nkey\nto\nproceed!", font);
    doorMessage.setCharacterSize(20);
    doorMessage.setStyle(sf::Text::Bold);
    doorMessage.setFillColor(sf::Color::Black);
    doorMessage.setPosition(v2f(7230.0f, -180.0f));

    /* Top Scores */
    sf::Text scores(finishDisplay(player, 0), font);
    scores.setCharacterSize(21);
    scores.setStyle(sf::Text::Bold);
    scores.setFillColor(sf::Color::Black);
    scores.setOrigin(100.0f, 100.0f);



    /** Music & Sounds **/

    /* Background music */
    sf::Music music;
    try
    {
        if(!music.openFromFile(ResourcePath + Sounds + "Erev.wav")) //RaindropFlower
            throw Sound::Error("Sound Error");
    } catch(const Sound::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    music.setVolume(10);
    music.play();
    music.setLoop(true);

    /* Coin sound */
    sf::SoundBuffer coinSoundBuffer;
    try
    {
        if(!coinSoundBuffer.loadFromFile(ResourcePath + Sounds + "Ding.wav"))
            throw Sound::Error("Sound Error");
    } catch(const Sound::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound coinSound;
    coinSound.setBuffer(coinSoundBuffer);
    coinSound.setVolume(10);

    /* Apple sound */
    sf::SoundBuffer appleSoundBuffer;
    try
    {
        if(!appleSoundBuffer.loadFromFile(ResourcePath + Sounds + "Running.wav"))
            throw Sound::Error("Sound Error");
    } catch(const Sound::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound appleSound;
    appleSound.setBuffer(appleSoundBuffer);
    appleSound.setVolume(5);

    /* Potion sound */
    sf::SoundBuffer potionSoundBuffer;
    try
    {
        if(!potionSoundBuffer.loadFromFile(ResourcePath + Sounds + "Tuturu.wav"))
            throw Sound::Error("Sound Error");
    } catch(const Sound::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound potionSound;
    potionSound.setBuffer(potionSoundBuffer);
    potionSound.setVolume(20);

    /* Key obtained sound */
    sf::SoundBuffer keySoundBuffer;
    try
    {
        if(!keySoundBuffer.loadFromFile(ResourcePath + Sounds + "Key.wav"))
            throw Sound::Error("Sound Error");
    } catch(const Sound::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound keySound;
    keySound.setBuffer(keySoundBuffer);
    keySound.setVolume(70);

    /* Door unlocked sound */
    sf::SoundBuffer doorSoundBuffer;
    try
    {
        if(!doorSoundBuffer.loadFromFile(ResourcePath + Sounds + "Unlocked.wav"))
            throw Sound::Error("Sound Error");
    } catch(const Sound::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound doorSound;
    doorSound.setBuffer(doorSoundBuffer);
    doorSound.setVolume(100);

    /* Death sound */
    sf::SoundBuffer deathSoundBuffer;
    try
    {
        if(!deathSoundBuffer.loadFromFile(ResourcePath + Sounds + "Oof.wav"))
            throw Sound::Error("Sound Error");
    } catch(const Sound::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sound deathSound;
    deathSound.setBuffer(deathSoundBuffer);
    deathSound.setVolume(50);

    /* Win sound */
    sf::Music winSound;
    try
    {
        if(!winSound.openFromFile(ResourcePath + Sounds + "Victory.wav"))
            throw Sound::Error("Sound Error");
    } catch(const Sound::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    winSound.setVolume(30);

    /* Lose sound */
    sf::Music loseSound;
    try
    {
        if(!loseSound.openFromFile(ResourcePath + Sounds + "Sad Violin.wav"))
            throw Sound::Error("Sound Error");
    } catch(const Sound::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    loseSound.setVolume(30);

    /* Wow sound */
    sf::Music wowSound;
    try
    {
        if(!wowSound.openFromFile(ResourcePath + Sounds + "Wow.wav"))
            throw Sound::Error("Sound Error");
    } catch(const Sound::Error& error)
    {
        std::cerr << error << std::endl;
        return EXIT_FAILURE;
    }
    wowSound.setVolume(50);



    /** ETC **/

    bool upPressed{false};
    bool statsRetrieved{false};
    bool doorLocked{true};



    /** Game Loop **/

    while(window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

        sf::Event event;

        while(window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                resizeView(window, view);
                break;
            case sf::Event::MouseButtonPressed:
                if(sf::Mouse::Button::Left == event.mouseButton.button && player.getKeyStatus() && doorLocked)
                {
                    doorLocked = false;
                    doorMessage.setString("Door\nunlocked!\nPress Up");
                    doorSound.play();
                }
                break;
            case sf::Event::KeyReleased:
                if(sf::Keyboard::Key::R == event.key.code && !player.getFinishStatus())
                {
                    player.TPCheckpoint();
                    player.loseLives();
                    music.play();
                    appleSound.stop();
                }
                if(sf::Keyboard::Key::Up == event.key.code)
                {
                    for(auto i = 0u; i < portals.size(); i++)
                    {
                        if(portals.at(i).GetGlobalBounds().intersects(player.GetGlobalBounds()))
                        {
                            portals.at(i).UpdatePlayer(player);
                        }
                    }
                    for(auto i = 0u; i < doors.size(); i++)
                    {
                        if(doors.at(i).GetGlobalBounds().intersects(player.GetGlobalBounds()))
                        {
                            if(doorLocked && upPressed && player.getKeyStatus())
                            {
                                doorMessage.setString("Left\nclick\non door\nto unlock!");
                            }
                            if(!doorLocked)
                            {
                                doors.at(i).UpdatePlayer(player);
                            } else
                            {
                                upPressed = true;
                            }
                        }
                    }
                }
                if(sf::Keyboard::Key::Escape == event.key.code && player.getFinishStatus() && !player.getAtHighscore())
                {
                    player.TPHighscore();
                    if(loseSound.getStatus() == sf::Music::Status::Playing)
                    {
                        loseSound.stop();
                    }
                    if(winSound.getStatus() == sf::Music::Status::Playing)
                    {
                        winSound.stop();
                    }
                    wowSound.play();
                }
                if(sf::Keyboard::Key::Enter == event.key.code && player.getAtHighscore())
                {
                    std::cout << "Top scores file written." << std::endl;
                    return 0;
                }
                //Back door commands
                if(sf::Keyboard::Key::Q == event.key.code)
                {
                    player.Boost(); // 2x speed and jump
                }
                if(sf::Keyboard::Key::W == event.key.code)
                {
                    player.Revert(); // normal speed and jump
                }
                if(sf::Keyboard::Key::Num1 == event.key.code)
                {
                    player.TPPart1();
                }
                if(sf::Keyboard::Key::Num2 == event.key.code)
                {
                    player.TPPart2();
                }
                if(sf::Keyboard::Key::Num3 == event.key.code)
                {
                    player.TPPart3();
                }
                if(sf::Keyboard::Key::Num4 == event.key.code)
                {
                    player.TPEnd();
                }
                if(sf::Keyboard::Key::Num5 == event.key.code)
                {
                    player.TP(v2f(7200.0f, 0.0f)); // Front of door
                }
                break;
            default:
                break;
            }
        }

        /* Back door for checking player's position
        std::cout << static_cast<int>(player.GetPosition().x) << " " << static_cast<int>(player.GetPosition().y) << std::endl;
        */

        player.Update(deltaTime);



        /** Bounds Check **/
        checkBounds(player);



        /** Collision Detection **/

        v2f direction;
        for(Platform &platform : platforms)
        {
            if(platform.GetCollider().CheckCollision(player.GetCollider(), direction, 1.0f))
            {
                player.OnCollision(direction);
            }
        }

        for(auto i = 0u; i < coins.size(); i++)
        {
            if(coins.at(i).GetGlobalBounds().intersects(player.GetGlobalBounds()))
            {
                coins.at(i).UpdatePlayer(player);
                coinSound.play();
            }
        }

        sf::Time appleDuration{sf::seconds(10)};
        for(auto i = 0u; i < apples.size(); i++)
        {
            if(apples.at(i).GetGlobalBounds().intersects(player.GetGlobalBounds()))
            {
                apples.at(i).UpdatePlayer(player);
                music.pause();
                appleSound.play();
                appleTimer.restart();
            }
        }

        if(appleSound.getStatus() != sf::Music::Status::Playing && music.getStatus() == sf::Music::Status::Paused)
        {
            music.play();
        }
        if(appleTimer.getElapsedTime() > appleDuration)
        {
            player.Revert();
        }

        for(auto i = 0u; i < traps.size(); i++)
        {
            if(traps.at(i).GetGlobalBounds().intersects(player.GetGlobalBounds()))
            {
                traps.at(i).UpdatePlayer(player);
                deathSound.play();
                if(appleSound.getStatus() == sf::Music::Status::Playing)
                {
                    appleSound.stop();
                }
                player.Revert();
            }
        }

        for(auto i = 0u; i < potions.size(); i++)
        {
            if(potions.at(i).GetGlobalBounds().intersects(player.GetGlobalBounds()))
            {
                potions.at(i).UpdatePlayer(player);
                potionSound.play();
            }
        }

        if(key.GetGlobalBounds().intersects(player.GetGlobalBounds()))
        {
            key.UpdatePlayer(player);
            keySound.play();
        }



        /** Status **/

        sf::Time elapsed{gameTimer.getElapsedTime()};
        timeRemaining = timeLimit - elapsed;

        if(!(static_cast<int>(timeRemaining.asSeconds()) % 10))
        {
            for(Apple &apple : apples)
                apple.Refresh();
        }

        if(endFlag.getGlobalBounds().intersects(player.GetGlobalBounds()))
        {
            player.setWin(true);
            player.setFinished(true);
            appleSound.stop();
            music.stop();
            winSound.play();
            timeTaken = static_cast<int>(gameTimer.getElapsedTime().asSeconds());
            view.setCenter(v2f(13500.0f, -100.0f));
        }

        if(player.getLives() == 0 || elapsed.asSeconds() > 500)
        {
            player.setLose(true);
            player.setFinished(true);
            appleSound.stop();
            music.stop();
            if(player.GetPosition() != v2f(13500.0f, 0.0f))
            {
                timeTaken = static_cast<int>(gameTimer.getElapsedTime().asSeconds());
                loseSound.play();
            }
            view.setCenter(v2f(13500.0f, -100.0f));
        }

        if(!player.getFinishStatus())
        {
            view.setCenter(player.GetPosition());
        }



        /** Text and Icon Settings **/

        /* Default size (512, 512) */
        gameStats.setString(gameStatsDisplay(player, timeRemaining));
        gameStats.setPosition(player.GetPosition());
        blackBar.setPosition(player.GetPosition());

        lives.setString(livesDisplay(player));
        lives.setPosition(player.GetPosition());

        finish.setString(finishDisplay(player, timeTaken));
        finish.setPosition(v2f(13348.0f, -250.0f));

        keyIcon.setPosition(v2f(player.GetPosition()));
        keyShadow.setPosition(v2f(player.GetPosition()));

        /* Resized */
        if(view.getSize() != (v2f(512.0f, 512.0f)))
        {
            gameStats.setString(gameStatsDisplay(player, timeRemaining));
            gameStats.setPosition(player.GetPosition());
            blackBar.setPosition(player.GetPosition());
            blackBar.setScale(v2f(4.0f, 1.0f));

            lives.setString(livesDisplay(player));
            lives.setPosition(player.GetPosition());
            float rescaleX{static_cast<float>(window.getSize().x / 8)};
            lives.move(v2f(-rescaleX, 0.0f));

            finish.setString(finishDisplay(player, timeTaken));
            finish.setPosition(v2f(13348.0f, -250.0f));

            keyIcon.setPosition(v2f(player.GetPosition()));
            keyIcon.move(v2f(window.getSize().x / 8, 0.0f));
            keyShadow.setPosition(v2f(player.GetPosition()));
            keyShadow.move(v2f(window.getSize().x / 8, 0.0f));
        }

        /* Retrieve stats */
        if(!statsRetrieved && player.getFinishStatus())
        {
            scores.setString(scoresDisplay(player, timeTaken, name));
            scores.setPosition(v2f(14700.0f, -250.0f));
            statsRetrieved = true;
        }

        if(player.getAtHighscore())
        {
            view.setCenter(v2f(player.GetPosition().x, -100.0f));
        }



        /** Background **/

        for(sf::Sprite &cloud: clouds)
        {
            cloud.move(v2f(0.01f, 0.0f));
        }

        window.clear(sf::Color(180, 246, 255, 40));
        window.setView(view);

        for(sf::Sprite &cloud: clouds)
        {
            window.draw(cloud);
        }



        /** Draw **/

        for(auto &checkpointFlag: checkpointFlags)
        {
            window.draw(checkpointFlag);
        }
        for(auto &portal : portals)
        {
            portal.Draw(window);
        }
        for(auto &door : doors)
        {
            door.Draw(window);
        }
        if(upPressed)
        {
            window.draw(doorMessage);
        }
        for(auto &trap : traps)
        {
            trap.Draw(window);
        }
        for(auto &platform : platforms)
        {
            platform.Draw(window);
        }
        for(auto &coin : coins)
        {
            coin.Draw(window);
        }
        for(auto &apple : apples)
        {
            apple.Draw(window);
        }
        for(auto &potion : potions)
        {
            potion.Draw(window);
        }
        key.Draw(window);
        player.Draw(window);
        window.draw(endFlag);
        if(!player.getFinishStatus() && !player.getAtHighscore())
        {
            window.draw(blackBar);
            window.draw(gameStats);
            window.draw(lives);
        } else
        {
            window.draw(finish);
        }
        if(player.getFinishStatus() && player.getAtHighscore())
        {
            window.draw(scores);
        }
        window.draw(keyShadow);
        if(player.getKeyStatus())
        {
            window.draw(keyIcon);
        }
        window.display();
    }
}

/** C++ 11 features
1. auto keyword (in for loops)
2. brace/list initialization (used in all variable initializations except first line of for loops)
3. range-based for loop (for drawing)
4. default keyword (in class destructors)
5. override specifier (in inherited functions through polymorphism)
6. using keyword (in Player.h variables)
**/
