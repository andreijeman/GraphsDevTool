#ifndef FILEMENU_HPP
#define FILEMENU_HPP

#include "UIEntity.hpp"
#include "Button.hpp"
#include "RoundedRect.hpp"
#include "TextBox.hpp"
#include "ResourceIdentifier.hpp"
#include "ResourceHolder.hpp"
#include "WorkBench.hpp"

#include <list>
#include <windows.h>
#include <fstream>

class LoadFileMenu : public UIEntity
{
public:
	LoadFileMenu(FontHolder* fonts, WorkBench* workBench);

	void															update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) override;
	void 															draw(sf::RenderWindow& window) const override;
	void															reset() override;
	void															setFileButtons(std::list<std::string>);

private:
	std::list<std::string>											ListFilesInDirectory(const std::string& directory);
	std::string														GetExecutablePath();

	void															loadFile(const std::string& path);

private:
	TextBox															textBox;

	Button															backButton;

	std::list<std::pair<std::shared_ptr<Button>, std::string>>		fileButtons;

	FontHolder*														fonts;

	RoundedRect														background;
	RoundedRect														textBg;

	std::string														path;

	WorkBench*														workBench;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SaveFileMenu : public UIEntity
{
public:
	SaveFileMenu(FontHolder* fonts, WorkBench* workBench);

	void															update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) override;
	void 															draw(sf::RenderWindow& window) const override;
	void															reset() override;
	void															setFileButtons(std::list<std::string>);

private:
	std::list<std::string>											ListFilesInDirectory(const std::string& directory);
	std::string														GetExecutablePath();
	void															saveFile(const std::string& path);

private:
	TextBox															textBox;
	TextBox															textBoxName;

	Button															backButton;
	Button															saveButton;

	std::list<std::pair<std::shared_ptr<Button>, std::string>>		fileButtons;

	FontHolder*														fonts;

	RoundedRect														background;
	RoundedRect														nameBg;
	RoundedRect														textBg;

	std::string														path;

	WorkBench*														workBench;
};

// p.s: da, cod dublat, timpul nu asteapta.

#endif 
