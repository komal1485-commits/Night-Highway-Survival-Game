#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>

class Car
{
protected:
    sf::Sprite sprite;
    float speed;

public:
    Car(sf::Texture& texture, float x, float y, float desiredWidth, float desiredHeight);

    virtual void move(float dx, float dy);

    sf::Sprite& getSprite();

    sf::FloatRect getBounds();
};

#endif