#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <cmath>

#include "PlayerCar.h"
#include "EnemyCar.h"

using namespace sf;
using namespace std;

enum GameState
{
    START_SCREEN,
    PLAYING,
    GAME_OVER
};

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    const float ROAD_LEFT = 150.0f;
    const float ROAD_WIDTH = 500.0f;
    const float ROAD_RIGHT = ROAD_LEFT + ROAD_WIDTH;

    const float CAR_WIDTH = WINDOW_WIDTH * 0.085f;
    const float CAR_HEIGHT = WINDOW_HEIGHT * 0.20f;

    RenderWindow window(
        VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Car Dodging Game"
    );

    window.setFramerateLimit(60);

    GameState gameState = START_SCREEN;

    // TEXTURES
    Texture playerTexture;
    Texture enemyTextures[5];

    if (!playerTexture.loadFromFile("Assets/WhiteCar.png"))
        return -1;

    if (!enemyTextures[0].loadFromFile("Assets/RedCar1.png"))
        return -1;

    enemyTextures[1].loadFromFile("Assets/RedCar2.png");
    enemyTextures[2].loadFromFile("Assets/YellowCar1.png");
    enemyTextures[3].loadFromFile("Assets/YellowCar2.png");
    enemyTextures[4].loadFromFile("Assets/YellowCar3.png");

    // FONT
    Font font;

    bool fontLoaded =
        font.loadFromFile("C:/Windows/Fonts/arial.ttf");

    if (!fontLoaded)
    {
        fontLoaded =
            font.loadFromFile(
                "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
            );
    }

    if (!fontLoaded)
        return -1;

    // PLAYER
    float playerX =
        ROAD_LEFT + ROAD_WIDTH / 2.0f - CAR_WIDTH / 2.0f;

    float playerY =
        WINDOW_HEIGHT - CAR_HEIGHT - 35.0f;

    PlayerCar player(
        playerTexture,
        playerX,
        playerY,
        CAR_WIDTH,
        CAR_HEIGHT,
        ROAD_LEFT,
        ROAD_RIGHT
    );

    // ENEMIES
    vector<EnemyCar> enemies;

    // CLOCKS
    Clock spawnClock;
    Clock gameClock;
    Clock roadClock;

    // GAME VARIABLES
    float elapsedTime = 0.0f;
    float gameSpeed = 4.0f;
    int score = 0;

    // ROAD
    RectangleShape leftSide(
        Vector2f(ROAD_LEFT, WINDOW_HEIGHT)
    );

    leftSide.setPosition(0, 0);
    leftSide.setFillColor(Color(10, 15, 25));

    RectangleShape road(
        Vector2f(ROAD_WIDTH, WINDOW_HEIGHT)
    );

    road.setPosition(ROAD_LEFT, 0);
    road.setFillColor(Color(32, 32, 32));

    RectangleShape rightSide(
        Vector2f(WINDOW_WIDTH - ROAD_RIGHT, WINDOW_HEIGHT)
    );

    rightSide.setPosition(ROAD_RIGHT, 0);
    rightSide.setFillColor(Color(10, 15, 25));

    // TEXT
    Text titleText;
    Text startText;
    Text hudText;
    Text gameOverText;

    titleText.setFont(font);
    startText.setFont(font);
    hudText.setFont(font);
    gameOverText.setFont(font);

    titleText.setCharacterSize(42);
    titleText.setFillColor(Color::White);

    titleText.setString(
        "Car Dodging Game"
    );

    titleText.setPosition(120, 120);

    startText.setCharacterSize(28);
    startText.setFillColor(Color::White);

    startText.setString(
        "Press ENTER to Start\n\n"
        "Press R to Restart\n\n"
        "Press ESC to Quit"
    );

    startText.setPosition(250, 240);

    hudText.setCharacterSize(22);
    hudText.setFillColor(Color::White);
    hudText.setPosition(20, 20);

    gameOverText.setCharacterSize(28);
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition(185, 245);

    while (window.isOpen())
    {
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        // ESC TO QUIT
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // START SCREEN
        if (gameState == START_SCREEN)
        {
            if (Keyboard::isKeyPressed(Keyboard::Enter))
            {
                enemies.clear();

                score = 0;

                elapsedTime = 0.0f;

                gameSpeed = 4.0f;

                player.getSprite().setPosition(
                    playerX,
                    playerY
                );

                gameClock.restart();
                spawnClock.restart();
                roadClock.restart();

                gameState = PLAYING;
            }
        }

        // PLAYING
        else if (gameState == PLAYING)
        {
            elapsedTime =
                gameClock.getElapsedTime().asSeconds();

            score =
                static_cast<int>(elapsedTime * 10);

            gameSpeed =
                4.0f + elapsedTime * 0.15f;

            player.handleInput();

            float spawnDelay =
                1.1f - elapsedTime * 0.01f;

            if (spawnDelay < 0.45f)
            {
                spawnDelay = 0.45f;
            }

            // SPAWN ENEMIES
            if (spawnClock.getElapsedTime().asSeconds()
                > spawnDelay)
            {
                float laneWidth =
                    ROAD_WIDTH / 4.0f;

                int randomLane =
                    rand() % 4;

                int randomTexture =
                    rand() % 5;

                float enemyX =
                    ROAD_LEFT +
                    randomLane * laneWidth +
                    laneWidth / 2.0f -
                    CAR_WIDTH / 2.0f;

                float enemyY =
                    -CAR_HEIGHT - 20.0f;

                float enemySpeed =
                    gameSpeed +
                    static_cast<float>(rand() % 3);

                enemies.push_back(
                    EnemyCar(
                        enemyTextures[randomTexture],
                        enemyX,
                        enemyY,
                        CAR_WIDTH,
                        CAR_HEIGHT,
                        enemySpeed
                    )
                );

                spawnClock.restart();
            }

            // UPDATE ENEMIES
            for (int i = 0;
                 i < static_cast<int>(enemies.size());
                 i++)
            {
                enemies[i].update();

                // COLLISION
                if (enemies[i].getBounds().intersects(
                        player.getBounds()))
                {
                    gameState = GAME_OVER;
                }
            }

            // REMOVE OFFSCREEN ENEMIES
            for (int i = 0;
                 i < static_cast<int>(enemies.size());
                 i++)
            {
                if (enemies[i].getSprite()
                        .getPosition().y
                    > WINDOW_HEIGHT + 100)
                {
                    enemies.erase(
                        enemies.begin() + i
                    );

                    i--;
                }
            }
        }

        // GAME OVER
        else if (gameState == GAME_OVER)
        {
            if (Keyboard::isKeyPressed(
                    Keyboard::R))
            {
                enemies.clear();

                score = 0;

                elapsedTime = 0.0f;

                gameSpeed = 4.0f;

                player.getSprite().setPosition(
                    playerX,
                    playerY
                );

                gameClock.restart();
                spawnClock.restart();
                roadClock.restart();

                gameState = START_SCREEN;
            }
        }

        // DRAW
        window.clear(Color::Black);

        window.draw(leftSide);
        window.draw(rightSide);
        window.draw(road);

        // MOVING ROAD LINES
        float offset;

        if (gameState == PLAYING)
        {
            offset =
                fmod(
                    roadClock.getElapsedTime()
                            .asSeconds()
                        * gameSpeed * 25.0f,
                    80.0f
                );
        }
        else
        {
            offset =
                fmod(
                    roadClock.getElapsedTime()
                            .asSeconds()
                        * 120.0f,
                    80.0f
                );
        }

        for (int i = -1; i < 10; i++)
        {
            RectangleShape line(
                Vector2f(10, 50)
            );

            line.setFillColor(Color::White);

            line.setPosition(
                WINDOW_WIDTH / 2.0f - 5.0f,
                i * 80.0f + offset
            );

            window.draw(line);
        }

        // START SCREEN DRAW
        if (gameState == START_SCREEN)
        {
            window.draw(titleText);
            window.draw(startText);
        }

        else
        {
            // DRAW PLAYER
            window.draw(player.getSprite());

            // DRAW ENEMIES
            for (int i = 0;
                 i < static_cast<int>(enemies.size());
                 i++)
            {
                window.draw(
                    enemies[i].getSprite()
                );
            }



            // HUD
            stringstream hud;

            hud << "Time: "
                << static_cast<int>(elapsedTime)
                << "s   Score: "
                << score
                << "   Speed: "
                << static_cast<int>(gameSpeed);

            hudText.setString(hud.str());

            window.draw(hudText);

            // GAME OVER SCREEN
            if (gameState == GAME_OVER)
            {
                RectangleShape overBox(
                    Vector2f(470, 220)
                );

                overBox.setPosition(165, 230);

                overBox.setFillColor(
                    Color(25, 25, 35)
                );

                overBox.setOutlineColor(
                    Color::Red
                );

                overBox.setOutlineThickness(3);

                stringstream over;

                over << "GAME OVER!\n\n"
                     << "Collision Detected\n\n"
                     << "Final Time: "
                     << static_cast<int>(elapsedTime)
                     << " seconds\n"
                     << "Final Score: "
                     << score
                     << "\n\n"
                     << "Press R to Restart\n"
                     << "Press ESC to Quit";

                gameOverText.setString(
                    over.str()
                );

                window.draw(overBox);

                window.draw(gameOverText);
            }
        }

        window.display();
    }

    return 0;
}