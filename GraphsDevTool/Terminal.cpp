#include "Terminal.hpp"

Terminal::Terminal() : maxRows(100), possibleRowsToShow(0), rowToStartShowing(0),fonts(NULL), commandHolder(nullptr)
{
}

Terminal::Terminal(const sf::Vector2f& size, FontHolder* fonts, RoundedRect background)
    : scrollBar(sf::Vector2f(size.x - 30, 10), sf::Vector2f(20.0f, size.y - 20), sf::Vector2f(20.0f, 50.0f), sf::Color(46, 46, 46), sf::Color(77, 77, 77), 20, 2, sf::Color(64, 64, 64))
    , writeBox(sf::Vector2f(10, size.y - size.y / 10 - 10), sf::Vector2f(size.x - 45, size.y / 10), sf::Color(74, 74, 74), 20, 1, sf::Color::Black)
    , background(background)
    , fonts(fonts)
    , maxRows(100)
    , possibleRowsToShow(0)
    , rowToStartShowing(0)
    , commandHolder(nullptr)
{
    textBox = TextBox(sf::Vector2f(30, size.y - size.y / 10 + 5), 15, sf::Color(0, 255, 204), fonts->get(Fonts::Terminal), (size.x - 45 - 40) / 10);
    defaultText.setFont(fonts->get(Fonts::Terminal));
    defaultText.setString("Write HELP here...");
    defaultText.setPosition({ 30,size.y - size.y / 10 + 5 });
    defaultText.setFillColor(sf::Color(209, 209, 224));
    defaultText.setCharacterSize(15);
    

    if (size.y <= 300)
    {
        writeBox.setSize(sf::Vector2f(size.x - 45, 50));
        writeBox.setPosition(sf::Vector2f(10, size.y - 60));
        textBox.setPosition(sf::Vector2f(30, size.y - 45));
        defaultText.setPosition(sf::Vector2f(30, size.y - 45));
    }
    setSize(size);
}

void Terminal::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{
    window.setView(view);
    sf::Vector2f localPos = getLocalPos(window, mousePos);

    scrollBar.update(window, localPos, userInput);

    if (scrollBar.isActive()) this->active = true;
    else this->active = false;

    if (userInput.event == Event::MouseLeftPressed && !contains(mousePos)) textBox.setActive(false);


    rowToStartShowing = (scrollBar.getThumbPosition().y - 10) * maxRows / (size.y - 20 - 50);
    possibleRowsToShow = std::min(static_cast<int>((size.y - 30 - 30 - size.y / 10) / 30), static_cast<int>(outputCommands.size()));

    if (rowToStartShowing > maxRows - (size.y - 30 - 30 - size.y / 10) / 30)  // x > 87 -> x = 87
        rowToStartShowing = maxRows - ((size.y - 30 - 30 - size.y / 10) / 30);

    if (rowToStartShowing + possibleRowsToShow > outputCommands.size())
    {
        rowToStartShowing = std::max(0, static_cast<int>(outputCommands.size()) - possibleRowsToShow);
    }

 

    if (userInput.event == Event::MouseLeftPressed)
    {
        if (writeBox.containsMouse(localPos))
        {
            textBox.setActive(true);   
            defaultText.setString("");

        }
    }

    if (textBox.isActive())
    {
        textBox.update(window, localPos, userInput);

        if (userInput.event == Event::TextEntered)
        {
            if (userInput.character == ENTER_KEY)
            {
                if (validationOutputText(textBox.getString()))
                { 
                    processOutputCommands(textBox.getString());
                    textBox.setActive(false);
                    defaultText.setString("Write HELP here...");                  
                }

            }
            else if (userInput.character == ESCAPE_KEY) textBox.setActive(false);
        }
    }
}



void Terminal::setCommands(CommandHolder* commandHolder)
{
    this->commandHolder = commandHolder;
}

bool Terminal::isActive() const
{
    if (textBox.isActive()) return true;
    return active;
}

bool Terminal::validationOutputText(std::string str)
{
    if (str == "") return false;
    for (char c : str)
    {
        if (!std::isspace(static_cast<unsigned char>(c)))
        {
            return true;
        }
    }
    return false;
}

bool Terminal::typeCommand(std::string command) const
{
    if (command == "SAVE GRAPH" || command == "OPEN GRAPH" || command == "DELETE ALL EDGES" ||
        command == "CLEAR GRAPH" || command == "SHOW GRAPH" || command == "SHOW FLUX" ||
        command == "SHOW WAYS")
        return true;

    return false;
}

void Terminal::showOutputCommands(sf::RenderWindow& window) const
{
    sf::Text text;
    text.setFont(fonts->get(Fonts::Terminal));
    text.setFillColor(sf::Color(0, 255, 204));
    text.setCharacterSize(15);

    int k = 0;

    if ((rowToStartShowing + possibleRowsToShow) <= outputCommands.size())
    {
        for (int i = rowToStartShowing; i < rowToStartShowing + possibleRowsToShow; i++)
        {
            text.setPosition(sf::Vector2f(15, 30 * k + 30));
            if (typeCommand(outputCommands[i]))
            {
                text.setString("   " + outputCommands[i]);
            }
            else text.setString(">> " + outputCommands[i]);
            k++;
            window.draw(text);
        }
    }
}

void Terminal::processOutputCommands(std::string text)
{
    int numOfCommands = 7;
    std::cout << text<<std::endl;

    //if(text == "help")
    if (text.find("help") != std::string::npos || text.find("HELP") != std::string::npos)
    {
        
        if (outputCommands.size() + (numOfCommands + 1) > maxRows)
        {
            for (int i = 0; i < numOfCommands + 1; i++)
                outputCommands.erase(outputCommands.begin());
        }

        outputCommands.push_back(text);
        outputCommands.push_back("SAVE GRAPH");
        outputCommands.push_back("OPEN GRAPH");
        outputCommands.push_back("CLEAR GRAPH");
        outputCommands.push_back("DELETE ALL EDGES");
        outputCommands.push_back("SHOW GRAPH");
        outputCommands.push_back("SHOW FLUX");
        outputCommands.push_back("SHOW WAYS");

        if (scrollBar.getThumbPosition().y + (numOfCommands + 1) * (size.y / 100) > size.y - 20 - 50)
        {
            scrollBar.setThumbPosition(sf::Vector2f(scrollBar.getThumbPosition().x, size.y - 10 - 50));
        }
        else scrollBar.setThumbPosition({ scrollBar.getThumbPosition().x,scrollBar.getThumbPosition().y + (numOfCommands + 1) * (size.y / 100) });
    }
    else
    {
        outputCommands.push_back(text);
        if (text.find("clear") != std::string::npos && text.find("graph") != std::string::npos)
        {
            commandHolder->push(Command::ClearGraph);
        }
        if (scrollBar.getThumbPosition().y < size.y - 20 - 50)
        {
            scrollBar.setThumbPosition({ scrollBar.getThumbPosition().x,scrollBar.getThumbPosition().y + size.y / 100 });
        }

        if (outputCommands.size() > maxRows)
        {
            outputCommands.erase(outputCommands.begin());
        }
    }


    textBox.setString("");
    textBox.setActive(false);
}


void Terminal::draw(sf::RenderWindow& window) const
{
    window.setView(view);

    window.draw(background);
    scrollBar.draw(window);
    window.draw(writeBox);
    textBox.draw(window);
    window.draw(defaultText);
    showOutputCommands(window);

}

void Terminal::reset()
{

}