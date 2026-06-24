#ifndef PLAYERCAR_H
#define PLAYERCAR_H

#include "Car.h"

class PlayerCar : public Car
{
private:
    float roadLeft;
    float roadRight;

public:
    PlayerCar(sf::Texture& texture,
              float x,
              float y,
              float carWidth,
              float carHeight,
              float leftBoundary,
              float rightBoundary);

    void handleInput();
};

#endif