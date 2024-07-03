#include "Container.hpp"

Container::Container(const std::shared_ptr<UIComponent>& mainComponent, const sf::Mouse::Button& openMouseButton):
	mainComponent(mainComponent), openMouseButton(openMouseButton), opened(false)
{
	setPosition(mainComponent->getPosition());
}

void Container::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{
	if (mainComponent->contains(mousePos))
	{
		if (userInput.event == Event::MouseLeftReleased && mainComponent->getState() == State::Pressed)
		{
			opened = !opened;
		}
	}

	mainComponent->update(window, mousePos, userInput);
	
	if (opened)
	{
		for (std::shared_ptr<UIComponent> comp : components)
		{
			comp->update(window, mousePos, userInput);
		}
	}
	
	if (!UIObject::contains(mousePos)) opened = false;
}

void Container::draw(sf::RenderWindow& window) const
{
	mainComponent->draw(window);
	if (opened)
	{
		for (std::shared_ptr<UIComponent> comp : components)
		{
			comp->draw(window);
		}
	}
}

void Container::reset()
{
}

bool Container::isActive() const
{
	if (mainComponent->isActive()) return true;
	if (opened)
	{
		for (std::shared_ptr<UIComponent> comp : components)
		{
			if (comp->isActive()) return true;
		}
	}
	return false;
}

bool Container::contains(const sf::Vector2f& point) const
{
	if (opened) return UIObject::contains(point);
	else return mainComponent->contains(point);
}

void Container::addComponent(std::shared_ptr<UIComponent> component)
{
	components.push_back(component);
}
