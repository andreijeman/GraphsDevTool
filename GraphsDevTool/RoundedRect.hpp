#ifndef ROUNDEDRECT_HPP
#define ROUNDEDRECT_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


class RoundedRect : public sf::Drawable
{
public:
	RoundedRect();
	RoundedRect( sf::Vector2f position, sf::Vector2f size, sf::Color backgroundColor, int borderRad, float thick, sf::Color borderColor);

public:
	virtual void									draw(sf::RenderTarget& target, sf::RenderStates states) const;
	bool											containsMouse(sf::Vector2f mousePosition)const;
	sf::Vector2f									getSize();
	sf::Vector2f									getPosition();
	void											setPosition(sf::Vector2f newPosition);
	void											setSize(sf::Vector2f newSize);
	void											setColor(sf::Color&);

private:
	std::vector<sf::RectangleShape>					mLines;
	std::vector<sf::CircleShape>					mCircles;
	std::vector<sf::RectangleShape>					mRectangles;
	sf::Vector2f									size;
	sf::Vector2f									position;
	int												borderRadius;
	float											thickness;
	sf::Color										backgroundColor;
	sf::Color										borderColor;

private:
	void											validationOfParameters();
	void											setCircles(sf::Vector2f newSize);
	void											setLines(sf::Vector2f newSize);
	void											setRectangles(sf::Vector2f newSize);
};


#endif