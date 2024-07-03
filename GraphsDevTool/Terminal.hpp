#ifndef UITERMINAL_HPP
#define UITERMINAL_HPP

#include <SFML/Graphics.hpp>
#include "ResourceIdentifier.hpp"
#include "ResourceHolder.hpp"
#include "UIEntity.hpp"
#include "RoundedRect.hpp"
#include "ScrollBar.hpp"
#include "TextBox.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include "CommandHolder.hpp"

class Terminal : public UIEntity
{
public:
	Terminal();
	Terminal(const sf::Vector2f& size, FontHolder* fonts,RoundedRect background);

public:
	void											update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) override;
	void 											draw(sf::RenderWindow& window) const override;
	void											reset() override;
	void											setCommands(CommandHolder* commandHolder);
	bool											isActive() const override;



private:
	RoundedRect										background;
	RoundedRect										writeBox;
	ScrollBar										scrollBar;
	TextBox											textBox;
	sf::Text										defaultText;
	std::vector<std::string>						outputCommands;
	
	FontHolder*										fonts;

	CommandHolder*									commandHolder;


	int												possibleRowsToShow;
	int												maxRows;
	int												rowToStartShowing;



private:
	bool									validationOutputText(std::string str);
	bool									typeCommand(std::string command) const;
	void									showOutputCommands(sf::RenderWindow& window) const;
	void									processOutputCommands(std::string text);



};

#endif // !UITERMINAL_HPP