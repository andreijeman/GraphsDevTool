#include "ButtonBox.hpp"

ButtonBox::ButtonBox(const std::shared_ptr<Button>& mainButton):
	mainButton(mainButton), opened(false)
{
	setPosition(mainButton->getPosition());
}

void ButtonBox::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{
	mainButton->update(window, mousePos, userInput);
	if (mainButton->wasClicked())
	{
		opened = !opened;
	}
	
	if (opened)
	{
		for (std::shared_ptr<UIComponent> button : buttons)
		{
			button->update(window, mousePos, userInput);
		}
	}
	
	if (!UIObject::contains(mousePos)) opened = false;
}

void ButtonBox::draw(sf::RenderWindow& window) const
{
	mainButton->draw(window);
	if (opened)
	{
		for (std::shared_ptr<UIComponent> button : buttons)
		{
			button->draw(window);
		}
	}
}

void ButtonBox::reset()
{
	opened = false;
}

bool ButtonBox::isActive() const
{
	if (mainButton->isActive()) return true;
	if (opened)
	{
		for (std::shared_ptr<UIComponent> button : buttons)
		{
			if (button->isActive()) return true;
		}
	}
	return false;
}

bool ButtonBox::contains(const sf::Vector2f& point) const
{
	if (opened) return UIObject::contains(point);
	else return mainButton->contains(point);
}

void ButtonBox::addButton(std::shared_ptr<Button> button)
{
	buttons.push_back(button);
}
