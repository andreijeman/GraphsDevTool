#include "ScrollBar.hpp"

ScrollBar::ScrollBar()
{
}

ScrollBar::ScrollBar(const sf::Vector2f& position, const sf::Vector2f& sizeBar, sf::Vector2f sizeThumb, sf::Color colorBar, sf::Color colorThumb, int radBorder, float thickness, sf::Color colorBorder)
	: bar(position, sizeBar, colorBar, radBorder, thickness, colorBorder)
	, thumb(position, sizeThumb, colorThumb, radBorder, thickness, colorBorder)
	, stateColorThumb(sf::Color(colorThumb), sf::Color(153, 153, 153), sf::Color(153, 153, 153))
	, lastStateThumb(State::Idle)
	, UIComponent(position, sizeThumb)
{

}

void ScrollBar::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{
    processStates(window);

    State::ID newState = UIComponent::getState();

    if (lastStateThumb != State::Pressed)
    {
        switch (newState)
        {
        case State::Pressed:
            thumb.setColor(stateColorThumb.pressed);
            break;
        case State::Hovered:
            thumb.setColor(stateColorThumb.hovered);
            break;
        case State::Idle:
            thumb.setColor(stateColorThumb.idle);
            break;
        }

        lastStateThumb = newState;
    }
    if (lastStateThumb == State::Pressed)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            active = true;

            thumb.setPosition(sf::Vector2f(thumb.getPosition().x, mousePos.y - thumb.getSize().y / 2));
            if (thumb.getPosition().y < bar.getPosition().y)
                thumb.setPosition(sf::Vector2f(thumb.getPosition().x, bar.getPosition().y));
            if (thumb.getPosition().y + thumb.getSize().y > bar.getPosition().y + bar.getSize().y)
                thumb.setPosition(sf::Vector2f(thumb.getPosition().x, bar.getPosition().y + bar.getSize().y - thumb.getSize().y));
        }

        lastStateThumb = newState;
    }
    else active = false;
}

void ScrollBar::draw(sf::RenderWindow& window) const
{

	bar.draw(window, sf::RenderStates::Default);
	thumb.draw(window, sf::RenderStates::Default);
}

void ScrollBar::reset()
{
}

void ScrollBar::processStates(sf::RenderWindow& window)
{

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (lastState == State::Hovered)
        {
            lastState = State::Pressed;

        }
    }
    else
    {
        if (isHovered(window))
        {


            lastState = State::Hovered;
        }
        else
        {
            lastState = State::Idle;
        }
    }
}

bool ScrollBar::isHovered(const sf::RenderWindow& window) const
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
	return thumb.containsMouse(worldPos);
}

void ScrollBar::setThumbPosition(sf::Vector2f position)
{
	thumb.setPosition(position);
}

sf::Vector2f ScrollBar::getThumbPosition()
{
	return thumb.getPosition();
}
