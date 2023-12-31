#include "stdafx.h"
#include "Player.h"


void Player::initVariables()
{
    avaibleBombs = 1;
}

void Player::initComponents()
{

}


Player::Player(float x, float y, sf::Texture& texture_sheet)
{
    initVariables();
    setPosition(x, y);
    createHitboxComponent(sprite, 0.f, 0.f, 130.f, 170.f);
    createMovementComponent(300.f, 15.f, 5.f);
    createAnimationComponent(texture_sheet);
    animationComponent->addAnimation("IDLE", 11.f, 0, 0, 2, 0, 130, 170);
    animationComponent->addAnimation("WALK_LEFT", 9.f, 0, 1, 2, 1, 120, 170);
    animationComponent->addAnimation("WALK_RIGHT", 9.f, 0, 2, 2, 2, 120, 170);
    animationComponent->addAnimation("WALK_UP", 9.f, 0, 3, 1, 3, 130, 170);
    animationComponent->addAnimation("WALK_DOWN", 9.f, 0, 4, 1, 4, 130, 170);
}

Player::~Player()
{

}


void Player::update(const float& dt, const float windowWidth, const float windowHeight)
{

    movementComponent->update(dt); // I aktualizacja ruchu

    sf::Vector2f lastPosition = sprite.getPosition(); //II zapamietanie ruchu

    // III sprawdzanie granic okna
    if (sprite.getPosition().x < 0) setPosition(0, sprite.getPosition().y);
    if (sprite.getPosition().y < 0) setPosition(sprite.getPosition().x, 0);
    if (sprite.getPosition().x + sprite.getGlobalBounds().width > windowWidth)
        setPosition(windowWidth - sprite.getGlobalBounds().width, sprite.getPosition().y);
    if (sprite.getPosition().y + sprite.getGlobalBounds().height > windowHeight)
        setPosition(sprite.getPosition().x, windowHeight - sprite.getGlobalBounds().height);

    // IV Sprawdzanie kolizji z innymi obiektami
    for (const CollisionObject& object : collisionObjects)
    {
        if (checkCollisionWithObject(object))
        {
            sf::Vector2f push;
            sf::FloatRect intersection;
            sprite.getGlobalBounds().intersects(object.rectangle, intersection); // najwazniejsze

            float intersectionWidth = intersection.width;
            float intersectionHeight = intersection.height;

            if (intersectionWidth > intersectionHeight)
            {
                if (sprite.getPosition().y > object.rectangle.top)
                    push.y = intersectionHeight;
                else
                    push.y = -intersectionHeight;
            }
            else
            {
                if (sprite.getPosition().x > object.rectangle.left)
                    push.x = intersectionWidth;
                else
                    push.x = -intersectionWidth;
            }

            sprite.setPosition(lastPosition + push);// przy kolizji powrot na ostatnie miejsce
            movementComponent->stopVelocity();

            //Je�li obiekt jest szkodliwy, zako�cz program
            if (object.entity && object.entity->harmful)
            {
                std::exit(0);
            }

            break;
        }
    }


    if (movementComponent->getState(IDLE))
        animationComponent->play("IDLE", dt);
    else if (movementComponent->getState(MOVING_LEFT))
        animationComponent->play("WALK_LEFT", dt, movementComponent->getVelocity().x, movementComponent->getMaxVelocity());
    else if (movementComponent->getState(MOVING_RIGHT))
        animationComponent->play("WALK_RIGHT", dt, movementComponent->getVelocity().x, movementComponent->getMaxVelocity());
    else if (movementComponent->getState(MOVING_UP))
        animationComponent->play("WALK_UP", dt, movementComponent->getVelocity().y, movementComponent->getMaxVelocity());
    else if (movementComponent->getState(MOVING_DOWN))
        animationComponent->play("WALK_DOWN", dt, movementComponent->getVelocity().y, movementComponent->getMaxVelocity());

    hitboxComponent->update();
}


bool Player::checkCollisionWithObject(const CollisionObject& object)
{
    return hitboxComponent->checkIntersect(object.rectangle);
}


void Player::addCollisionObject(const sf::FloatRect& object, Entity* entity = nullptr)
{
    CollisionObject collisionObject;
    collisionObject.rectangle = object;
    collisionObject.entity = entity;
    collisionObjects.push_back(collisionObject);
}

void Player::removeCollisionObject(Entity* entity)
{
    for (auto it = collisionObjects.begin(); it != collisionObjects.end(); )
    {
        if (it->entity == entity)
        {
            it = collisionObjects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
