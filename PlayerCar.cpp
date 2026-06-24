#include "PlayerCar.h"

PlayerCar::PlayerCar(sf::Texture& texture,
                     float x,
                     float y,
                     float carWidth,
                     float carHeight,
                     float leftBoundary,
                     float rightBoundary)

    : Car(texture, x, y, carWidth, carHeight)
{
    roadLeft = leftBoundary;
    roadRight = rightBoundary;
}

void PlayerCar::handleInput()
{
    float moveSpeed = 8.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (sprite.getPosition().x > roadLeft)
        {
            sprite.move(-moveSpeed, 0);
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (sprite.getPosition().x + sprite.getGlobalBounds().width < roadRight)
        {
            sprite.move(moveSpeed, 0);
        }
    }
}