#ifndef UICOMPONENT_HPP
#define UICOMPONENT_HPP

#include <SFML/Graphics.hpp>
#include "UIObject.hpp"

namespace State
{
	enum ID
	{
		Idle,
		Hovered,
		Pressed
	};
}


class UIComponent : public UIObject
{
public:
	UIComponent();
	UIComponent(const sf::Vector2f& position, const sf::Vector2f& size);

public:
	State::ID				getState();

protected:
	State::ID				lastState;
};


//////////////////////////////////////////////////////////////////////////////////////

template<class Type>
struct StateLook
{
public:
	Type idle;
	Type hovered;
	Type pressed;

	StateLook() {};
	StateLook(Type idle, Type hovered, Type pressed) : idle(idle), hovered(hovered), pressed(pressed) {};
};

#endif
