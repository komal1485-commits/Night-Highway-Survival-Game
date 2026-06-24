#include "EnemyCar.h"

EnemyCar::EnemyCar(sf::Texture& texture,
                   float x,
                   float y,
                   float carWidth,
                   float carHeight,
                   float sp)

    : Car(texture, x, y, carWidth, carHeight)
{
    speed = sp;
}

void EnemyCar::update()
{
    sprite.move(0, speed);
}