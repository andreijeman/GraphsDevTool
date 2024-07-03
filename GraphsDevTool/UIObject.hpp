#ifndef UIOBJECT_HPP
#define UIOBJECT_HPP

#include <SFML/Graphics.hpp>
#include "UserEventIdentifier.hpp"


class UIObject
{
public:
	UIObject();
	UIObject(const sf::Vector2f& position, const sf::Vector2f& size);

public:
	virtual void			update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) = 0;
	virtual void 			draw(sf::RenderWindow& window) const = 0;
	virtual void			reset() = 0;

	virtual bool			contains(const sf::Vector2f& point) const;


	virtual void			setPosition(const sf::Vector2f& position);
	virtual void			setSize(const sf::Vector2f& size);

	virtual bool			isActive() const;
	virtual void			setActive(bool value);

	sf::Vector2f			getPosition();
	sf::Vector2f			getSize();


protected:
	sf::Vector2f			position;
	sf::Vector2f			size;

	bool					active;

};

#endif 
