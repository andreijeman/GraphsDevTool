#ifndef  SCROLLBARR_HPP
#define SCROLLBARR_HPP

#include <SFML/Graphics.hpp>
#include "RoundedRect.hpp"
#include "UIComponent.hpp"

class ScrollBar :public UIComponent
{

public:
	ScrollBar();
	ScrollBar(const sf::Vector2f& position, const sf::Vector2f& sizeBar, sf::Vector2f sizeThumb, sf::Color colorBar, sf::Color colorThumb, int radBorder, float thickness, sf::Color colorBorder);

public:
	void			update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) override;
	void 			draw(sf::RenderWindow& window) const override;
	void			reset() override;

	void											processStates(sf::RenderWindow& window);

	bool											isHovered(const sf::RenderWindow& window)const;
	void											setThumbPosition(sf::Vector2f position);
	sf::Vector2f									getThumbPosition();

private:
	RoundedRect										bar;
	RoundedRect										thumb;
	StateLook<sf::Color>							stateColorThumb;
	State::ID										lastStateThumb;

};

#endif // !SCROLLBAR_HPP
