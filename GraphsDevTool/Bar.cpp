#include "Bar.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////

Bar::Bar() : focusedObject(nullptr)
{
}

Bar::Bar(const sf::Vector2f& position, const sf::Vector2f& size) :
	UIEntity(position, size)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////
// BASE BAR

void Bar::draw(sf::RenderWindow& window) const
{

    window.setView(view);
    window.draw(background);
    for (std::shared_ptr<UIObject> object : objects)
    {
		object->draw(window);
    }
}

void Bar::reset()
{
	if(focusedObject) focusedObject->reset();
}

void Bar::addObject(std::shared_ptr<UIObject> object)
{
    objects.push_back(object);
}

void Bar::setBackground(const RoundedRect& background)
{
	this->background = background;
}

void Bar::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{
	window.setView(view);
	sf::Vector2f localPos = getLocalPos(window, mousePos);

	if (this->contains(mousePos))
	{
		this->active = true;
		
		if (focusedObject == nullptr)
		{
			for (std::shared_ptr<UIObject> object : objects)
			{
				if (object->contains(localPos))
				{
					focusedObject = object;
					break;
				}
			}
		}
	}

	if (focusedObject)
	{
		focusedObject->update(window, localPos, userInput);
		if (!focusedObject->isActive())
		{
			focusedObject = nullptr;
		}
	}
	else
	{
		this->active = false;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////

