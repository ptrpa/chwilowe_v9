#include "stdafx.h"
#include "Bomb.h"
#include "Player.h"


void Bomb::initVariables()
{
    freePass = 300;
    safe = 600;
    lifeTime = 650;
}

Bomb::Bomb(float x, float y, float hitbox_width, float hitbox_height, sf::Texture& texture_sheet, Player* owner)
{
    initVariables();
    destroyable = false;
    harmful = false;
    setPosition(x, y);
    bombOwner = owner;
    createHitboxComponent(sprite, 0.f, 0.f, hitbox_width, hitbox_height);
    //createMovementComponent(300.f, 15.f, 5.f);
    createAnimationComponent(texture_sheet);
    animationComponent->addAnimation("IDLE", 11.f, 0, 0, 2, 0, hitbox_width, hitbox_height);
    isIncrementingOwner = true;
}



Bomb::~Bomb()
{
    if (bombOwner && isIncrementingOwner)
    {
        bombOwner->avaibleBombs++;
    }
}

//Functions
void Bomb::update(const float& dt, const float windowWidth, const float windowHeight)
{
    freePass--;
    safe--;
    lifeTime--;

    // Sprawd� granice
    if (sprite.getPosition().x < 0) setPosition(0, sprite.getPosition().y);
    if (sprite.getPosition().y < 0) setPosition(sprite.getPosition().x, 0);
    if (sprite.getPosition().x + sprite.getGlobalBounds().width > windowWidth)
        setPosition(windowWidth - sprite.getGlobalBounds().width, sprite.getPosition().y);
    if (sprite.getPosition().y + sprite.getGlobalBounds().height > windowHeight)
        setPosition(sprite.getPosition().x, windowHeight - sprite.getGlobalBounds().height);

    hitboxComponent->update();
}


Explosion::Explosion(float x, float y, float width, float height ,sf::Texture& texture_sheet, Player* owner, int lifeTime,int orientation)
    : Bomb(x, y, width, height, texture_sheet, owner)
{
    harmful = true;
    this->lifeTime = lifeTime;
    isIncrementingOwner = false;
    if (orientation == 1) {
        createHitboxComponent(sprite, 0.f, 0.f, width, height);
    }
    else if (orientation == 2) {
        createHitboxComponent(sprite, 0.f, 0.f, width, height);
    }
}


void Explosion::update(const float& dt, const float windowWidth, const float windowHeight)
{
    lifeTime--;

    // Sprawd� granice
    if (sprite.getPosition().x < 0) setPosition(0, sprite.getPosition().y);
    if (sprite.getPosition().y < 0) setPosition(sprite.getPosition().x, 0);
    if (sprite.getPosition().x + sprite.getGlobalBounds().width > windowWidth)
        setPosition(windowWidth - sprite.getGlobalBounds().width, sprite.getPosition().y);
    if (sprite.getPosition().y + sprite.getGlobalBounds().height > windowHeight)
        setPosition(sprite.getPosition().x, windowHeight - sprite.getGlobalBounds().height);

    hitboxComponent->update();
}
