#include "FileMenu.hpp"

LoadFileMenu::LoadFileMenu(FontHolder* fonts, WorkBench* workBench) :
    fonts(fonts), workBench(workBench)
{
    size = sf::Vector2f(1000, 290);
    background = RoundedRect(sf::Vector2f(0, 0), size, sf::Color(60, 60, 60), 15, 3, sf::Color(40, 40, 40));
    textBg = RoundedRect(sf::Vector2f(10, 15), sf::Vector2f(860, 40), sf::Color(100, 100, 100), 11, 3, sf::Color(40, 40, 40));

    backButton.setPosition(sf::Vector2f(880, 15));
    backButton.setSize(sf::Vector2f(100, 40));
    backButton.setColors(sf::Color(215, 55, 23), sf::Color(180, 55, 23), sf::Color(180, 55, 23));
    backButton.setBox(RoundedRect(backButton.getPosition(), backButton.getSize(), sf::Color(215, 55, 23), 10, 3, sf::Color(40, 40, 40)));

    if (fonts->contains(Fonts::FileMenu))
    {
        backButton.setText(fonts->get(Fonts::FileMenu), 20, sf::Color::White, sf::Text::Bold, "Back");
        textBox = TextBox(sf::Vector2f(15, 15), 20, sf::Color::White, fonts->get(Fonts::FileMenu), 70);
        textBox.setPosition(sf::Vector2f(20, 22));
        path = GetExecutablePath();
        textBox.setString(path);
        setFileButtons(ListFilesInDirectory(path));
    }
}

void LoadFileMenu::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{
    if (!contains(mousePos) && userInput.event == Event::MouseLeftPressed) active = false;

    window.setView(view);
    sf::Vector2f localPos = getLocalPos(window, mousePos);
    backButton.update(window, localPos, userInput);
   
    if (backButton.wasClicked())
    {
        int i = path.length() - 1;
        while (i > 0 && path[i] != '\\') i--;
        path = path.substr(0, i);
        fileButtons.clear();
        setFileButtons(ListFilesInDirectory(path));
        textBox.setString(path);
    }

    for (std::pair<std::shared_ptr<Button>, std::string> a : fileButtons)
    {
        a.first->update(window, localPos, userInput);
        if (a.first->wasClicked())
        {
            if (a.second.substr(a.second.length() - 3, a.second.length()) == ".gh")
            {
                loadFile(path + "\\" + a.second);
                reset();
            }
            else
            {
                path += '\\' + a.second;
                textBox.setString(path);
                fileButtons.clear();
                setFileButtons(ListFilesInDirectory(path));
            }

            break;

        }

    }
    
    if (textBg.containsMouse(localPos) && userInput.event == Event::MouseLeftPressed)
    {
        textBox.setActive(true);
    }

    if (textBox.isActive())
    {
        textBox.update(window, localPos, userInput);

        if (userInput.character == ENTER_KEY && textBox.isActive())
        {
            textBox.setActive(false);
            fileButtons.clear();
            path = textBox.getString();
            setFileButtons(ListFilesInDirectory(path));
        }
    }
}

void LoadFileMenu::draw(sf::RenderWindow& window) const
{
    window.setView(view);
    window.draw(background);
    window.draw(textBg);
    textBox.draw(window);
    backButton.draw(window);
    for (auto& a : fileButtons) a.first->draw(window);

}

void LoadFileMenu::reset()
{
    textBox.setActive(false);
    active = false;
}

void LoadFileMenu::setFileButtons(std::list<std::string> files)
{
    sf::Vector2f pos(15, 80);

    for (auto& f : files)
    {
        if ((pos.x + f.size() * 15) > (size.x - 30))
        {
            pos.x = 20;
            pos.y += 50;
        }
        
        std::shared_ptr<Button> btn(new Button(pos, sf::Vector2f(f.size() * 15, 40)));
        pos.x += btn->getSize().x + 10;

        btn->setColors(sf::Color(45, 45, 45), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
        btn->setText(fonts->get(Fonts::FileMenu), 18, sf::Color::White, sf::Text::Italic, f);
        btn->setBox(RoundedRect(btn->getPosition(), btn->getSize(), sf::Color(45, 45, 45), 10, 2, sf::Color(150, 150, 150)));

        fileButtons.push_back(std::pair<std::shared_ptr<Button>, std::string>(btn, f));
    }
}

std::list<std::string> LoadFileMenu::ListFilesInDirectory(const std::string& directory)
{
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((directory + "\\*.").c_str(), &findFileData);

    std::list<std::string> files;

    if (hFind == INVALID_HANDLE_VALUE) {
        return files;
    }


    do 
    {
        files.push_back(findFileData.cFileName);
    } while (FindNextFileA(hFind, &findFileData) != 0);

    files.pop_front();
    files.pop_front();

    /////

    hFind = FindFirstFileA((directory + "\\*.gh").c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return files;
    }

    do
    {
        files.push_back(findFileData.cFileName);
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);

    return files;
}

std::string LoadFileMenu::GetExecutablePath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string executablePath(buffer);
    size_t lastSlash = executablePath.find_last_of("\\");
    return executablePath.substr(0, lastSlash);
}

void LoadFileMenu::loadFile(const std::string& path)
{
    workBench->clear();

    std::ifstream file;
    file.open(path);

    float s, x, y;
    file >> s;

    std::vector<PtrNode> nodes;

    for (int i = 0; i < s; i++)
    {
        file >> x >> y;
        workBench->makeNode(sf::Vector2f(x, y));
    }

    file >> s;


    for (int i = 0; i < s; i++)
    {
        file >> x >> y;
        workBench->makeEdge(sf::Vector2f(x, y));
        file >> x >> y;
        workBench->makeEdge(sf::Vector2f(x, y));
        file >> x;
        workBench->getLastAddedEdge()->setValue(x);
    }

    file.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

SaveFileMenu::SaveFileMenu(FontHolder* fonts, WorkBench* workBench) :
    fonts(fonts), workBench(workBench)
{
    size = sf::Vector2f(1000, 290);
    background = RoundedRect(sf::Vector2f(0, 0), size, sf::Color(60, 60, 60), 15, 3, sf::Color(40, 40, 40));
    nameBg = RoundedRect(sf::Vector2f(10, 15), sf::Vector2f(860, 40), sf::Color(100, 100, 100), 11, 3, sf::Color(40, 40, 40));
    textBg = RoundedRect(sf::Vector2f(10, 65), sf::Vector2f(860, 40), sf::Color(100, 100, 100), 11, 3, sf::Color(40, 40, 40));

    backButton.setPosition(sf::Vector2f(880, 65));
    backButton.setSize(sf::Vector2f(100, 40));
    backButton.setColors(sf::Color(215, 55, 23), sf::Color(240, 55, 23), sf::Color(180, 55, 23));
    backButton.setBox(RoundedRect(backButton.getPosition(), backButton.getSize(), sf::Color(215, 55, 23), 10, 3, sf::Color(40, 40, 40)));

    saveButton.setPosition(sf::Vector2f(880, 15));
    saveButton.setSize(sf::Vector2f(100, 40));
    saveButton.setColors(sf::Color(215, 55, 23), sf::Color(240, 55, 23), sf::Color(180, 55, 23));
    saveButton.setBox(RoundedRect(saveButton.getPosition(), saveButton.getSize(), sf::Color(215, 55, 23), 10, 3, sf::Color(40, 40, 40)));

    if (fonts->contains(Fonts::FileMenu))
    {
        backButton.setText(fonts->get(Fonts::FileMenu), 20, sf::Color::White, sf::Text::Bold, "Back");
        saveButton.setText(fonts->get(Fonts::FileMenu), 20, sf::Color::White, sf::Text::Bold, "Save");
        textBox = TextBox(sf::Vector2f(15, 72), 20, sf::Color::White, fonts->get(Fonts::FileMenu), 70);

        textBoxName = TextBox(sf::Vector2f(20, 22), 20, sf::Color::White, fonts->get(Fonts::FileMenu), 70);
        textBoxName.setString("filename");

        path = GetExecutablePath();
        textBox.setString(path);
        setFileButtons(ListFilesInDirectory(path));
    }
}

void SaveFileMenu::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{
    if (!contains(mousePos) && userInput.event == Event::MouseLeftPressed) active = false;

    window.setView(view);
    sf::Vector2f localPos = getLocalPos(window, mousePos);
    backButton.update(window, localPos, userInput);
    saveButton.update(window, localPos, userInput);

    if (backButton.wasClicked())
    {
        int i = path.length() - 1;
        while (i > 0 && path[i] != '\\') i--;
        path = path.substr(0, i);
        fileButtons.clear();
        setFileButtons(ListFilesInDirectory(path));
        textBox.setString(path);
    }

    for (std::pair<std::shared_ptr<Button>, std::string> a : fileButtons)
    {
        a.first->update(window, localPos, userInput);
        if (a.first->wasClicked())
        {
            path += '\\' + a.second;
            textBox.setString(path);
            fileButtons.clear();
            setFileButtons(ListFilesInDirectory(path));
            break;

        }
    }

    if (textBg.containsMouse(localPos) && userInput.event == Event::MouseLeftPressed)
    {
        textBox.setActive(true);
        textBoxName.setActive(false);
    }

    if (textBox.isActive())
    {
        textBox.update(window, localPos, userInput);

        if (userInput.character == ENTER_KEY && textBox.isActive())
        {
            textBox.setActive(false);
            fileButtons.clear();
            path = textBox.getString();
            setFileButtons(ListFilesInDirectory(path));
        }
    }

    if (nameBg.containsMouse(localPos) && userInput.event == Event::MouseLeftPressed)
    {
        textBoxName.setActive(true);
        textBox.setActive(false);
    }

    if (textBoxName.isActive())
    {
        textBoxName.update(window, localPos, userInput);

        if (userInput.character == ENTER_KEY && textBoxName.isActive())
        {
            textBoxName.setActive(false);
            fileButtons.clear();
            path = textBox.getString();
            setFileButtons(ListFilesInDirectory(path));
        }
    }

    if (saveButton.wasClicked() || (userInput.event == Event::TextEntered && userInput.character == ESCAPE_KEY))
    {
        std::string name = textBoxName.getString();
        if (!name.empty())
        {
            saveFile(path + "\\" + name + ".gh");
            reset();
        }
    }
}

void SaveFileMenu::draw(sf::RenderWindow& window) const
{
    window.setView(view);
    window.draw(background);
    window.draw(textBg);
    window.draw(nameBg);
    textBox.draw(window);
    textBoxName.draw(window);
    backButton.draw(window);
    saveButton.draw(window);
    for (auto& a : fileButtons) a.first->draw(window);

}

void SaveFileMenu::setFileButtons(std::list<std::string> files)
{
    sf::Vector2f pos(15, 115);

    for (auto& f : files)
    {
        if ((pos.x + f.size() * 15) > (size.x - 30))
        {
            pos.x = 20;
            pos.y += 50;
        }

        std::shared_ptr<Button> btn(new Button(pos, sf::Vector2f(f.size() * 15, 40)));
        pos.x += btn->getSize().x + 10;

        btn->setColors(sf::Color(45, 45, 45), sf::Color(90, 90, 90), sf::Color(45, 45, 45));
        btn->setText(fonts->get(Fonts::FileMenu), 18, sf::Color::White, sf::Text::Italic, f);
        btn->setBox(RoundedRect(btn->getPosition(), btn->getSize(), sf::Color(45, 45, 45), 10, 2, sf::Color(150, 150, 150)));

        fileButtons.push_back(std::pair<std::shared_ptr<Button>, std::string>(btn, f));
    }
}

std::list<std::string> SaveFileMenu::ListFilesInDirectory(const std::string& directory)
{
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((directory + "\\*.").c_str(), &findFileData);

    std::list<std::string> files;

    if (hFind == INVALID_HANDLE_VALUE) {
        return files;
    }

    do
    {
        files.push_back(findFileData.cFileName);
    } while (FindNextFileA(hFind, &findFileData) != 0);

    files.pop_front();
    files.pop_front();

    FindClose(hFind);

    return files;
}

std::string SaveFileMenu::GetExecutablePath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string executablePath(buffer);
    size_t lastSlash = executablePath.find_last_of("\\");
    return executablePath.substr(0, lastSlash);
}

void SaveFileMenu::reset()
{
    textBox.setActive(false);
    active = false;
}

void SaveFileMenu::saveFile(const std::string& path)
{
    std::ofstream file;
    file.open(path);
    
    std::list<PtrNode> nodes = workBench->getNodeList();
    std::list<PtrEdge> edges = workBench->getEdgeList();

    file << nodes.size() << " ";

    for (PtrNode node : nodes)
    {
        file << node->getPosition().x << " " << node->getPosition().y << " ";
    }

    file << std::endl << edges.size() << " ";

    sf::Vector2f pos1, pos2;
    for (PtrEdge edge : edges)
    {
        pos1 = edge->getNodes().first->getPosition();
        pos2 = edge->getNodes().second->getPosition();
        file << pos1.x << " " << pos1.y << " " << pos2.x << " " << pos2.y << " " << edge->getValue() << " ";
    }

    file.close();
}

