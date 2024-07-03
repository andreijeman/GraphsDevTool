#include "Application.hpp"
#include <iostream>
#include <memory>

Application::Application() :
	window(sf::VideoMode(1200, 720), "App", sf::Style::Default, sf::ContextSettings(0u, 0u, 8u, 1u, 0u, false, false)),
	focusedEntity(nullptr), loadFileMenu(nullptr), showTerminal(false)
{
}

void Application::run()
{
	window.setFramerateLimit(60);
	while (window.isOpen())
	{
		processEvents();
		update();
		render();
	}
}


void Application::processEvents()
{
	userInput.event = Event::None;
	userInput.character = 0;
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::Resized:
			resize();
			break;

		case sf::Event::TextEntered:
			userInput.event = Event::TextEntered;
			userInput.character = static_cast<char>(event.key.code);
			break;

		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) userInput.event = Event::MouseLeftPressed;
			if (event.mouseButton.button == sf::Mouse::Right) { userInput.event = Event::MouseRightPressed; reset(); }
			break;

		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left) userInput.event = Event::MouseLeftReleased;
			if (event.mouseButton.button == sf::Mouse::Right) { userInput.event = Event::MouseRightReleased; cursor.setTexture(textures.get(Textures::DefaultCursor)); }
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Left) userInput.event = Event::KeyLeftPressed;
			if (event.key.code == sf::Keyboard::Right) userInput.event = Event::KeyRightPressed;		
			if (event.key.code == sf::Keyboard::Escape) { cursor.setTexture(textures.get(Textures::DefaultCursor));	reset(); }
			break;

		case sf::Event::MouseWheelScrolled:
			window.setView(workBench->getView());
			workBench->zoom(event);
			break;
		}

	}
	
	mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
}

void Application::reset()
{
	workBench->reset();
	toolBar->reset();
	menuBar->reset();
	loadFileMenu->reset();
	saveFileMenu->reset();
	terminal->reset();
}

void Application::resize()
{
	defaultView.setSize(sf::Vector2f(window.getSize()));
	defaultView.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
	
	menuBar->resize(window);
	toolBar->resize(window);
	workBench->resize(window);
	loadFileMenu->resize(window);
	saveFileMenu->resize(window);
	terminal->resize(window);
}

void Application::processCommands()
{
	Command::ID mCommand;
	while (mCommand = commandHolder.pop())
	{
		switch (mCommand)
		{
		case Command::AddNode:
			cursor.setTexture(textures.get(Textures::AddNodeCursor));
			workBench->setState(WorkBenchState::MakeNode);
			break;

		case Command::AddEdge:
			cursor.setTexture(textures.get(Textures::AddEdgeCursor));
			workBench->setState(WorkBenchState::MakeEdge);
			break;

		case Command::EraseGraphComponent:
			cursor.setTexture(textures.get(Textures::EraseCursor));
			workBench->setState(WorkBenchState::EraseElement);
			break;

		case Command::ClearGraph:
			workBench->clear();
			break;

		case Command::OpenLoadFileMenu:
			if (focusedEntity) focusedEntity->reset();
			loadFileMenu->setActive(true);
			focusedEntity = loadFileMenu;
			break;

		case Command::OpenSaveFileMenu:
			if (focusedEntity) focusedEntity->reset();
			saveFileMenu->setActive(true);
			focusedEntity = saveFileMenu;
			break;

		case Command::RunMinPathAlg:
			workBench->setState(WorkBenchState::MinPathAlg);
			break;
		case Command::RunMaxPathAlg:
			workBench->setState(WorkBenchState::MaxPathAlg);
			break;

		case Command::OpenTerminal:
			showTerminal = !showTerminal;
			break;

		default: break;
		}
	}
}

void Application::update()
{
	cursor.setPosition(mousePos);

	if (focusedEntity == nullptr)
	{
		if (menuBar->contains(mousePos)) focusedEntity = menuBar;
		else if (toolBar->contains(mousePos)) focusedEntity = toolBar;
		else if (showTerminal && terminal->contains(mousePos)) focusedEntity = terminal;
		else focusedEntity = workBench;
	}

	if (focusedEntity)
	{
		focusedEntity->update(window, mousePos, userInput);
		if (!focusedEntity->isActive())
		{
			focusedEntity = nullptr;
		}
	}

	processCommands();
}

void Application::render()
{
	window.clear(sf::Color::White);

	workBench->draw(window);
	toolBar->draw(window);
	menuBar->draw(window);
	if(showTerminal) terminal->draw(window);

	if(loadFileMenu->isActive()) loadFileMenu->draw(window);
	if(saveFileMenu->isActive()) saveFileMenu->draw(window);

	window.setView(defaultView);

	window.draw(cursor);

	window.display();
}

void Application::initialize()
{	
	// FONT
	fonts.load(Fonts::Main, "Fonts/ArialBold.ttf");
	fonts.load(Fonts::Terminal, "Fonts/Terminal.ttf");
	fonts.load(Fonts::FileMenu, "Fonts/FileMenu.ttf");

	// TEXTURES
	textures.load(Textures::AddNodeButton, "Textures/node.png");
	textures.load(Textures::AddEdgeButton, "Textures/edge.png");
	textures.load(Textures::EraseButton, "Textures/erase.png");
	textures.load(Textures::ClearButton, "Textures/clear.png");
	
	textures.load(Textures::DefaultCursor, "Textures/default_cursor.png");
	textures.load(Textures::AddNodeCursor, "Textures/node_cursor.png");
	textures.load(Textures::AddEdgeCursor, "Textures/edge_cursor.png");
	textures.load(Textures::EraseCursor, "Textures/erase_cursor.png");


	// BUTTONS
	std::shared_ptr<Button> file_button(new Button);
	std::shared_ptr<Button> load_button(new Button);
	std::shared_ptr<Button> save_button(new Button);

	std::shared_ptr<Button> algorithm_button(new Button);
	std::shared_ptr<Button> min_path_button(new Button);
	std::shared_ptr<Button> max_path_button(new Button);

	std::shared_ptr<Button> terminal_button(new Button);


	std::shared_ptr<Button> node_button(new Button);
	std::shared_ptr<Button> edge_button(new Button);
	std::shared_ptr<Button> erase_button(new Button);
	std::shared_ptr<Button> clear_button(new Button);


	file_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
	load_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
	save_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));

	algorithm_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
	min_path_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
	max_path_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));

	terminal_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
	
	node_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
	edge_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
	erase_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
	clear_button->setColors(sf::Color(60, 60, 60), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
	

	file_button->setSize(sf::Vector2f(100, 50));
	load_button->setSize(sf::Vector2f(100, 50));
	save_button->setSize(sf::Vector2f(100, 50));

	algorithm_button->setSize(sf::Vector2f(200, 50));
	min_path_button->setSize(sf::Vector2f(200, 50));
	max_path_button->setSize(sf::Vector2f(200, 50));

	terminal_button->setSize(sf::Vector2f(200, 50));

	node_button->setSize(sf::Vector2f(50, 50));
	edge_button->setSize(sf::Vector2f(50, 50));
	erase_button->setSize(sf::Vector2f(50, 50));
	clear_button->setSize(sf::Vector2f(50, 50));


	file_button->setPosition(sf::Vector2f(0, 0));
	load_button->setPosition(sf::Vector2f(0, 50));
	save_button->setPosition(sf::Vector2f(0, 100));

	terminal_button->setPosition(sf::Vector2f(300, 0));

	algorithm_button->setPosition(sf::Vector2f(100, 0));
	min_path_button->setPosition(sf::Vector2f(100, 50));
	max_path_button->setPosition(sf::Vector2f(100, 100));


	node_button->setPosition(sf::Vector2f(10, 10));
	edge_button->setPosition(sf::Vector2f(70, 10));
	erase_button->setPosition(sf::Vector2f(130, 10));
	clear_button->setPosition(sf::Vector2f(190, 10));

	save_button->setCommand(&commandHolder, Command::OpenSaveFileMenu);
	load_button->setCommand(&commandHolder, Command::OpenLoadFileMenu);

	min_path_button->setCommand(&commandHolder, Command::RunMinPathAlg);
	max_path_button->setCommand(&commandHolder, Command::RunMaxPathAlg);

	terminal_button->setCommand(&commandHolder, Command::OpenTerminal);

	node_button->setCommand(&commandHolder, Command::AddNode);
	edge_button->setCommand(&commandHolder, Command::AddEdge);
	erase_button->setCommand(&commandHolder, Command::EraseGraphComponent);
	clear_button->setCommand(&commandHolder, Command::ClearGraph);


	if (fonts.contains(Fonts::Main))
	{
		file_button->setText(fonts.get(Fonts::Main), 20, sf::Color::White, sf::Text::Bold, "FILE");
		load_button->setText(fonts.get(Fonts::Main), 20, sf::Color::White, sf::Text::Bold, "LOAD");
		save_button->setText(fonts.get(Fonts::Main), 20, sf::Color::White, sf::Text::Bold, "SAVE");

		terminal_button->setText(fonts.get(Fonts::Main), 20, sf::Color::White, sf::Text::Bold, "TERMINAL");

		algorithm_button->setText(fonts.get(Fonts::Main), 20, sf::Color::White, sf::Text::Bold, "ALGORITHM");
		min_path_button->setText(fonts.get(Fonts::Main), 20, sf::Color::White, sf::Text::Bold, "MIN PATH");
		max_path_button->setText(fonts.get(Fonts::Main), 20, sf::Color::White, sf::Text::Bold, "MAX PATH");
	}

	if (textures.contains(Textures::AddNodeButton))
	{
		textures.get(Textures::AddNodeButton).setSmooth(true);
		node_button->setTextures(textures.get(Textures::AddNodeButton), textures.get(Textures::AddNodeButton), textures.get(Textures::AddNodeButton));
	}

	if (textures.contains(Textures::AddEdgeButton))
	{
		textures.get(Textures::AddEdgeButton).setSmooth(true);
		edge_button->setTextures(textures.get(Textures::AddEdgeButton), textures.get(Textures::AddEdgeButton), textures.get(Textures::AddEdgeButton));
	}

	if (textures.contains(Textures::EraseButton))
	{
		textures.get(Textures::EraseButton).setSmooth(true);
		erase_button->setTextures(textures.get(Textures::EraseButton), textures.get(Textures::EraseButton), textures.get(Textures::EraseButton));
	}
	if (textures.contains(Textures::ClearButton))
	{
		textures.get(Textures::ClearButton).setSmooth(true);
		clear_button->setTextures(textures.get(Textures::ClearButton), textures.get(Textures::ClearButton), textures.get(Textures::ClearButton));
	}


	// MENU BAR
	menuBar = std::shared_ptr<Bar>(new Bar);

	RoundedRect background(sf::Vector2f(0, 0), sf::Vector2f(2000, 50), sf::Color(60, 60, 60), 0, 0, sf::Color(55, 55, 55));
	
	menuBar->setBackground(background);
	menuBar->setSize(sf::Vector2f(2000, 50));
	menuBar->setFixedPosition(window, FixedPosition::Left, FixedPosition::Top, sf::Vector2f(0, 0));

		// FILE BUTTON BOX
		std::shared_ptr<ButtonBox> fileButtonBox(new ButtonBox(file_button));

		fileButtonBox->setSize(sf::Vector2f(100, 150));
		fileButtonBox->addButton(load_button);
		fileButtonBox->addButton(save_button);


		// ALGORITHM BUTTON BOX
		std::shared_ptr<ButtonBox> algorithmButtonBox(new ButtonBox(algorithm_button));

		algorithmButtonBox->setSize(sf::Vector2f(200, 150));
		algorithmButtonBox->addButton(min_path_button);
		algorithmButtonBox->addButton(max_path_button);


	menuBar->addObject(fileButtonBox);
	menuBar->addObject(algorithmButtonBox);
	menuBar->addObject(terminal_button);

	
	// TOOL BAR
	toolBar = std::shared_ptr<Bar>(new Bar);

	background = RoundedRect(sf::Vector2f(0, 0), sf::Vector2f(250, 70), sf::Color(60, 60, 60), 15, 3, sf::Color(45, 45, 45));

	toolBar->setBackground(background);
	toolBar->setSize(sf::Vector2f(250, 70));
	toolBar->setFixedPosition(window, FixedPosition::Middle, FixedPosition::Down, sf::Vector2f(0, 50));

	toolBar->addObject(node_button);
	toolBar->addObject(edge_button);
	toolBar->addObject(erase_button);
	toolBar->addObject(clear_button);

	// WORK BENCH
	workBench = std::shared_ptr<WorkBench>(new WorkBench(sf::Vector2f(0, 0), sf::Vector2f(window.getSize()), &fonts));
	workBench->setFont(&fonts);

	// CURSOR
	cursor.setTexture(textures.get(Textures::DefaultCursor));
	window.setMouseCursorVisible(false);

	// LOAD FILE MENU
	loadFileMenu = std::shared_ptr<LoadFileMenu>(new LoadFileMenu(&fonts, workBench.get()));
	loadFileMenu->setFixedPosition(window, FixedPosition::Middle, FixedPosition::Middle, sf::Vector2f(0, 0));

	// SAVE FILE MENU
	saveFileMenu = std::shared_ptr<SaveFileMenu>(new SaveFileMenu(&fonts, workBench.get()));
	saveFileMenu->setFixedPosition(window, FixedPosition::Middle, FixedPosition::Middle, sf::Vector2f(0, 0));


	// TERMINAL
	background = RoundedRect(sf::Vector2f(0, 0), sf::Vector2f(350, 500), sf::Color(64, 64, 64), 20, 2, sf::Color::Black);
	terminal = std::shared_ptr<Terminal>(new Terminal(sf::Vector2f(350, 500), &fonts, background));
	terminal->setFixedPosition(window, FixedPosition::Left, FixedPosition::Down, sf::Vector2f(50, 50));
	terminal->setCommands(&commandHolder);

	resize();
}