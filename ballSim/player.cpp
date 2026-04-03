#include "player.h"

Player::Player()
{
    points = 20000000;
}

void Player::addPoints(int points)
{
    this->points += points;
}

void Player::subtractPoints(int points)
{
    this->points -= points;
}