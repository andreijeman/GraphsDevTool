#ifndef BUTTONBOX_HPP
#define BUTTONBOX_HPP

#include <vector>
#include "UIEntity.hpp"
#include "Bar.hpp"

class ButtonBox : public UIObject
{
public:
	ButtonBox(const std::shared_ptr<Button>& mainButton);

public:
	void											update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) override;
	void 											draw(sf::RenderWindow& window) const override;
	void											reset() override;
	bool											isActive() const override;
	bool											contains(const sf::Vector2f& point) const override;
	void											addButton(std::shared_ptr<Button> button);
	
private:
	std::shared_ptr<Button>							mainButton;
	std::vector<std::shared_ptr<Button>>			buttons;

	bool											opened;

};

#endif