#include "UIEntity.hpp"

UIEntity::UIEntity() : 
	orizontalPos(FixedPosition::Left), verticalPos(FixedPosition::Top), view(sf::FloatRect(0,0,1100,600))
{
}

UIEntity::UIEntity(const sf::Vector2f& position, const sf::Vector2f& size) :
	UIObject(position, size), 
	orizontalPos(FixedPosition::Left), verticalPos(FixedPosition::Top), view(sf::FloatRect(0, 0, 1100, 600))
{
}

void UIEntity::resize(sf::RenderWindow& window)
{
	sf::Vector2f windowSize = sf::Vector2f(window.getSize());
	sf::Vector2f pos;
	if (orizontalPos == FixedPosition::Left && verticalPos == FixedPosition::Top)
	{
		pos = margin;
	}
	else if (orizontalPos == FixedPosition::Middle && verticalPos == FixedPosition::Down)
	{
		pos.x = margin.x + (windowSize.x - size.x)/2;
		pos.y = windowSize.y - margin.y - size.y;
	}
	else if (orizontalPos == FixedPosition::Left && verticalPos == FixedPosition::Down)
	{
		pos.x = margin.x;
		pos.y = windowSize.y - size.y - margin.y;
	}
	else if (orizontalPos == FixedPosition::Middle && verticalPos == FixedPosition::Middle)
	{
		pos.x = margin.x + (windowSize.x - size.x) / 2;
		pos.y = margin.y + (windowSize.y - size.y) / 2;;
	}

	view.setSize(windowSize);
	view.setCenter(windowSize.x / 2 - pos.x, windowSize.y / 2 - pos.y);

	position = pos;
}

void UIEntity::setFixedPosition(sf::RenderWindow& window, FixedPosition::ID orizontalPos, FixedPosition::ID verticalPos, const sf::Vector2f& margin)
{
	this->orizontalPos = orizontalPos;
	this->verticalPos = verticalPos;
	this->margin = margin;

	resize(window);
}

void UIEntity::setPosition(const sf::Vector2f& position)
{
	this->position = position;
	margin = position;
	view.move(-position);
}

sf::Vector2f UIEntity::getLocalPos(sf::RenderWindow& window, const sf::Vector2f& globalPos)
{	 
	return window.mapPixelToCoords(sf::Vector2i(globalPos));
}

sf::View UIEntity::getView()
{
	return view;
}
