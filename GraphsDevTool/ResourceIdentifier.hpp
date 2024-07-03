#ifndef RESOURCEIDENTIFIERS_HPP
#define RESOURCEIDENTIFIERS_HPP

#include <windows.h>

namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	enum ID
	{
		AddNodeButton, AddEdgeButton, EraseButton, ClearButton,
		DefaultCursor, AddNodeCursor, AddEdgeCursor, EraseCursor
	};
}

namespace Fonts
{
	enum ID
	{
		Main, Terminal, FileMenu
	};
}


template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>	TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>			FontHolder;

#endif // RESOURCEIDENTIFIERS_HPP