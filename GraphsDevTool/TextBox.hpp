#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include <SFML/Graphics.hpp>
#include <string>

#include "UIObject.hpp"

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class TextBox : public UIObject
{

public:
	TextBox();
	TextBox(const sf::Vector2f& pos,int textSize, sf::Color textColor, const sf::Font& font, size_t characteLimit);

public:
	void									update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) override;
	void 									draw(sf::RenderWindow& window) const override;
	void									reset() override;
	void									setActive(bool value) override;

	void									setString(const std::string& str);
	void									setPosition(const sf::Vector2f& position) override;
	std::string								getString();

private:
	sf::Font								font;
	sf::Text								text;

	size_t									cursorID;
	std::string								str;

	int										limit;
};

#endif 
