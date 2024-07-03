#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <windows.h>

#include "UIEntity.hpp"
#include "Button.hpp"
#include "Bar.hpp"
#include "CommandHolder.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifier.hpp"
#include "FileMenu.hpp"
#include "ButtonBox.hpp"
#include "WorkBench.hpp"
#include "Terminal.hpp"
#include "GraphAlgorithms.hpp"

class Application
{
public:
	Application();

	void										run();
	void										initialize();

private:
	void										processEvents();
	void										processCommands();
	void										update();
	void										render();

	void										reset();
	void										resize();


private:
	std::shared_ptr<UIEntity>					focusedEntity;

	UserInput									userInput;
	sf::Vector2f								mousePos;

	CommandHolder								commandHolder;

private:
	std::shared_ptr<WorkBench>					workBench;
	std::shared_ptr<Bar>						menuBar;
	std::shared_ptr<Bar>						toolBar;
	std::shared_ptr<LoadFileMenu>				loadFileMenu;
	std::shared_ptr<SaveFileMenu>				saveFileMenu;
	std::shared_ptr<Terminal>					terminal;
	bool										showTerminal;



private:
	FontHolder									fonts;
	TextureHolder								textures;

	sf::Sprite									cursor;

	sf::View									defaultView;


private:
	sf::RenderWindow window;	
};

#endif

