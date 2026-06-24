#include "Car.h"

Car::Car(sf::Texture& texture, float x, float y, float desiredWidth, float desiredHeight)
{
    sprite.setTexture(texture);

    sprite.setPosition(x, y);

    float textureWidth = static_cast<float>(texture.getSize().x);
    float textureHeight = static_cast<float>(texture.getSize().y);

    float scaleX = desiredWidth / textureWidth;
    float scaleY = desiredHeight / textureHeight;

    sprite.setScale(scaleX, scaleY);

    speed = 0;
}

void Car::move(float dx, float dy)
{
    sprite.move(dx, dy);
}

sf::Sprite& Car::getSprite()
{
    return sprite;
}

sf::FloatRect Car::getBounds()
{
    return sprite.getGlobalBounds();
}