#include "player.h"

Player::Player()
{
    points = 0;
}

void Player::addPoints(int points)
{
    this->points += points;
}

void Player::subtractPoints(int points)
{
    this->points -= points;
}