#include "TextBox.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////

TextBox::TextBox() : limit(100), cursorID(0), str("|")
{
}

TextBox::TextBox(const sf::Vector2f& pos, int textSize, sf::Color textColor, const sf::Font& font, size_t characteLimit) : 
	limit(characteLimit), font(font), cursorID(0), str("|")
{
	setPosition(pos);
	text.setFont(font);
	text.setCharacterSize(textSize);
	text.setFillColor(textColor);
	text.setPosition(pos);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


void TextBox::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{
	if (active)
	{
		if (userInput.event == Event::TextEntered)
		{
			char ch = userInput.character;
			if (ch > 31 && ch < 127)
			{
				if (str.length() < limit)
				{
					str = str.substr(0, cursorID) + ch + '|' + str.substr(cursorID + 1, str.length());
					cursorID++;
				}
			}
			else if (ch == DELETE_KEY)
			{
				if (cursorID > 0)
				{
					str = str.substr(0, cursorID - 1) + '|' + str.substr(cursorID + 1, str.length());
					cursorID--;
				}
			}
		}

		if (userInput.event == Event::KeyLeftPressed)
		{
			if (cursorID > 0)
			{
				str[cursorID] = str[cursorID - 1];
				str[--cursorID] = '|';
			}
		}
		else if (userInput.event == Event::KeyRightPressed)
		{
			if (cursorID < str.length() - 1)
			{
				str[cursorID] = str[cursorID + 1];
				str[++cursorID] = '|';
			}
		}
		text.setString(str);
	}
}

void TextBox::draw(sf::RenderWindow& window) const
{
	window.draw(text);
}

void TextBox::reset()
{
	str.clear();
	setActive(false);
}

void TextBox::setActive(bool value)
{
	active = value;
	if (!active)
	{
		text.setString(getString());
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void TextBox::setString(const std::string& str)
{
	cursorID = str.length();
	this->str = str + '|';
	if (active) text.setString(this->str);
	else text.setString(str);
}

void TextBox::setPosition(const sf::Vector2f& position)
{
	text.setPosition(position);
	this->position = position;
}


std::string TextBox::getString()
{
	return str.substr(0, cursorID) + str.substr(cursorID + 1, str.length());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////