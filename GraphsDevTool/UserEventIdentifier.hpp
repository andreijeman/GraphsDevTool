#ifndef USEREVENTIDENTIFIER_HPP
#define USEREVENTIDENTIFIER_HPP

namespace Event
{
	enum ID
	{
		None = 0,

		MouseLeftPressed,
		MouseLeftReleased,
		MouseRightPressed,
		MouseRightReleased,

		KeyLeftPressed,
		KeyRightPressed,
		
		TextEntered,
	};
}

struct UserInput
{
	Event::ID		event;
	char			character;

	UserInput() :	event(Event::None), character(' ') {};
};

#endif