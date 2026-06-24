#ifndef ENEMYCAR_H
#define ENEMYCAR_H

#include "Car.h"

class EnemyCar : public Car
{
public:
    EnemyCar(sf::Texture& texture,
             float x,
             float y,
             float carWidth,
             float carHeight,
             float speed);

    void update();
};

#endif