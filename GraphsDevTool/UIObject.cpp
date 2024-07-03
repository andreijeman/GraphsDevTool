#include "UIObject.hpp"

UIObject::UIObject() : active(false)
{
}

UIObject::UIObject(const sf::Vector2f& position, const sf::Vector2f& size) :
    position(position), size(size), active(false)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////
// CONTAINS

bool UIObject::contains(const sf::Vector2f& point) const
{
    return point.x >= position.x && point.x < (position.x + size.x) &&
        point.y >= position.y && point.y < (position.y + size.y);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// SET && GET 

void UIObject::setPosition(const sf::Vector2f& position)
{
    this->position = position;
}

void UIObject::setSize(const sf::Vector2f& size)
{
    this->size = size;
}

sf::Vector2f UIObject::getPosition()
{
    return position;
}

sf::Vector2f UIObject::getSize()
{
    return size;
}

/////////////////////////////////////////////////////////////////////////////////////////////

bool UIObject::isActive() const
{
    return active;
}

void UIObject::setActive(bool value)
{
    active = value;
}

/////////////////////////////////////////////////////////////////////////////////////////////