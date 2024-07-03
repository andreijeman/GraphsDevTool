#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <vector>
#include "UIEntity.hpp"
#include "Bar.hpp"

class Container : public UIObject
{
public:
	Container(const std::shared_ptr<UIComponent>& mainComponent, const sf::Mouse::Button& openButton);

public:
	void											update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) override;
	void 											draw(sf::RenderWindow& window) const override;
	void											reset() override;
	bool											isActive() const override;
	bool											contains(const sf::Vector2f& point) const override;
	void											addComponent(std::shared_ptr<UIComponent> component);
	
private:
	std::shared_ptr<UIComponent>					mainComponent;
	std::vector<std::shared_ptr<UIComponent>>		components;

	sf::Mouse::Button								openMouseButton;
	bool											opened;

};

#endif