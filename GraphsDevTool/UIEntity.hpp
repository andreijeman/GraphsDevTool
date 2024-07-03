#ifndef UIENTITY_HPP
#define UIENTITY_HPP

#include <SFML/Graphics.hpp>

#include "UIObject.hpp"

namespace FixedPosition
{
	enum ID
	{
		Left, 
		Top,
		Middle,
		Right,
		Down
	};
}



class UIEntity : public UIObject
{
public:
	UIEntity();
	UIEntity(const sf::Vector2f& position, const sf::Vector2f& size);

public:
	virtual void			resize(sf::RenderWindow& window);
	virtual void			setFixedPosition(sf::RenderWindow& window, FixedPosition::ID orizontalPos, FixedPosition::ID verticalPos, const sf::Vector2f& margin);
	virtual void			setPosition(const sf::Vector2f& position) override;
	sf::Vector2f			getLocalPos(sf::RenderWindow& window, const sf::Vector2f& globalPos);
	sf::View				getView();

protected: 
	sf::View				view;

	FixedPosition::ID		orizontalPos;
	FixedPosition::ID		verticalPos;

	sf::Vector2f			margin;
};


#endif