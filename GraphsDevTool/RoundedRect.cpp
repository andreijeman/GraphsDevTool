#include "RoundedRect.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////
RoundedRect::RoundedRect() :
	size(sf::Vector2f(1,1)),
	borderRadius(0), 
	thickness(0), 
	backgroundColor(sf::Color::Blue), 
	borderColor(sf::Color::Blue)
{
	float newSizeX = size.x - 2 * borderRadius;
	float newSizeY = size.y - 2 * borderRadius;

	setLines({ newSizeX,newSizeY });
	setCircles({ newSizeX,newSizeY });
	setRectangles({ newSizeX,newSizeY });
}
RoundedRect::RoundedRect(sf::Vector2f valuePosition, sf::Vector2f valueSize, sf::Color bkColor, int borderRad, float thick, sf::Color brColor)
	: size(valueSize)
	, position(valuePosition)
	, borderRadius(borderRad)
	, thickness(thick)
	, backgroundColor(bkColor)
	, borderColor(brColor)
{
	validationOfParameters();

	float newSizeX = size.x - 2 * borderRadius;
	float newSizeY = size.y - 2 * borderRadius;

	setLines({ newSizeX,newSizeY });
	setCircles({ newSizeX,newSizeY });
	setRectangles({ newSizeX,newSizeY });
}

//////////////////////////////// VALIDATION ///////////////////////////////////////////////////////

void RoundedRect::validationOfParameters()
{
	if (size.x - 2 * borderRadius < borderRadius || size.y - 2 * borderRadius < borderRadius)
	{
		borderRadius = std::min(size.x / 2, size.y / 2);
	}
	if (thickness > 5) thickness = 5;
}

//////////////////////////////// SET ELEMENTS ////////////////////////////////////////////////////

void RoundedRect::setLines(sf::Vector2f newSize)
{
	mLines.clear();
	mLines.push_back(sf::RectangleShape({ newSize.x,thickness }));
	mLines.push_back(sf::RectangleShape({ newSize.x,thickness }));
	mLines.push_back(sf::RectangleShape({ thickness,newSize.y }));
	mLines.push_back(sf::RectangleShape({ thickness,newSize.y }));

	mLines[0].setPosition({ position.x + borderRadius,position.y });							// side UP
	mLines[1].setPosition({ position.x + borderRadius,position.y + size.y - thickness });		// side DOWN
	mLines[2].setPosition({ position.x,position.y + borderRadius });							// side LEFT
	mLines[3].setPosition({ position.x + size.x - thickness,position.y + borderRadius });		// side RIGHT
	for (auto& line : mLines) line.setFillColor(borderColor);
}

void RoundedRect::setRectangles(sf::Vector2f newSize)
{
	mRectangles.clear();
	mRectangles.push_back(sf::RectangleShape({ size.x,newSize.y }));
	mRectangles.push_back(sf::RectangleShape({ newSize.x,size.y }));
	mRectangles[0].setPosition({ position.x,position.y + borderRadius });
	mRectangles[0].setFillColor(backgroundColor);
	mRectangles[1].setPosition({ position.x + borderRadius, position.y });
	mRectangles[1].setFillColor(backgroundColor);
}

void RoundedRect::setCircles(sf::Vector2f newSize)
{
	for (int i = 0; i < 4; i++) mCircles.push_back(sf::CircleShape(borderRadius, 30U));
	for (auto& circle : mCircles)
	{
		circle.setOutlineThickness(-thickness);
		circle.setOutlineColor(borderColor);
		circle.setRadius(borderRadius);
		circle.setFillColor(backgroundColor);
	}

	mCircles[0].setPosition({ position.x,position.y });
	mCircles[1].setPosition({ position.x + newSize.x,position.y });
	mCircles[2].setPosition({ position.x,position.y + newSize.y });
	mCircles[3].setPosition({ position.x + newSize.x,position.y + newSize.y });
}

//////////////////////////////// GET //////////////////////////////////////////////////////////////

sf::Vector2f RoundedRect::getPosition() { return position; }
sf::Vector2f RoundedRect::getSize() { return size; }

//////////////////////////////// SET //////////////////////////////////////////////////////////////

void RoundedRect::setPosition(sf::Vector2f newPosition)
{
	position = newPosition;
	float newSizeX = size.x - 2 * borderRadius;
	float newSizeY = size.y - 2 * borderRadius;

	mCircles[0].setPosition({ newPosition.x,newPosition.y });
	mCircles[1].setPosition({ newPosition.x + newSizeX,newPosition.y });
	mCircles[2].setPosition({ newPosition.x,newPosition.y + newSizeY });
	mCircles[3].setPosition({ newPosition.x + newSizeX,newPosition.y + newSizeY });

	mRectangles[0].setPosition({ newPosition.x,newPosition.y + borderRadius });
	mRectangles[1].setPosition({ newPosition.x + borderRadius, newPosition.y });

	mLines[0].setPosition({ newPosition.x + borderRadius,newPosition.y });							// side UP
	mLines[1].setPosition({ newPosition.x + borderRadius,newPosition.y + size.y - thickness });		// side DOWN
	mLines[2].setPosition({ newPosition.x,newPosition.y + borderRadius });							// side LEFT
	mLines[3].setPosition({ newPosition.x + size.x - thickness,newPosition.y + borderRadius });		// side RIGHT
}

void RoundedRect::setSize(sf::Vector2f newSize)
{
	size = newSize;
	float newSizeX = newSize.x - 2 * borderRadius;
	float newSizeY = newSize.y - 2 * borderRadius;

	mCircles[0].setPosition({ position.x,position.y });
	mCircles[1].setPosition({ position.x + newSizeX,position.y });
	mCircles[2].setPosition({ position.x,position.y + newSizeY });
	mCircles[3].setPosition({ position.x + newSizeX,position.y + newSizeY });

	setLines({ newSizeX,newSizeY });
	setRectangles({ newSizeX,newSizeY });
}

//////////////////////////////// MOUSE POSITION //////////////////////////////////////////////////

bool RoundedRect::containsMouse(sf::Vector2f mousePosition)const
{
	for (auto& shape : mRectangles)
	{
		sf::FloatRect bounds = shape.getGlobalBounds();
		//bounds.left -= 50;
		//bounds.top -= 50;
		//bounds.width += 100;
		//bounds.height += 100;
		if (bounds.contains(mousePosition))
		{
			return true;
		}
	}
	for (auto& circle : mCircles)
	{
		if (circle.getGlobalBounds().contains(mousePosition))
		{
			return true;
		}
	}
	for (auto& line : mLines)
	{
		if (line.getGlobalBounds().contains(mousePosition))
		{
			return true;
		}
	}

	return false;
}

//////////////////////////////// DRAW /////////////////////////////////////////////////////////////

void RoundedRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& circle : mCircles)
	{
		target.draw(circle, states);
	}
	for (auto& shape : mRectangles)
	{
		target.draw(shape, states);
	}
	for (auto& line : mLines)
	{
		target.draw(line, states);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void RoundedRect::setColor(sf::Color& color)
{
	backgroundColor = color;
	for (auto& iterator : mRectangles)
		iterator.setFillColor(color);
	for (auto& iterator : mCircles)
		iterator.setFillColor(color);
}