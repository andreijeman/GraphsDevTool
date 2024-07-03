#include "GraphComponents.hpp"
/////////////////////////////////////////// NODES /////////////////////////////////////////////////////

int Node::ID = 0;

Node::Node(): idNode(0)
{}

Node::Node(const sf::Vector2f& position,sf::Font &font, const sf::Color& fillColor,float radius, int pointCount)
    : sf::CircleShape(radius, pointCount)
    , UIObject(sf::Vector2f(position.x - radius, position.y - radius), sf::Vector2f(radius * 2, radius * 2))
{
    setFillColor(sf::Color::White);
    setOutlineColor(sf::Color(60, 60, 60));
    setOutlineThickness(3);
    sf::CircleShape::setPosition(sf::Vector2f(position.x - radius, position.y - radius));

    idNode = ID;
    setTextParameters(font);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

int Node::getID()
{
    return idNode;
}

void Node::setID(int value)
{
    this->idNode = value;
    this->idNodeText.setString(std::to_string(value+1));
}

void Node::setTextParameters(sf::Font &font)
{
    idNodeText.setFont(font);
    idNodeText.setCharacterSize(20);
    idNodeText.setString(std::to_string(++ID));
    idNodeText.setOrigin(idNodeText.getGlobalBounds().getSize() / 2.f + idNodeText.getLocalBounds().getPosition());
    idNodeText.setPosition(position + (size / 2.f));
    idNodeText.setFillColor(sf::Color::Black);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void Node::draw(sf::RenderWindow& window) const
{
    window.draw(*this);
    window.draw(idNodeText);
}

void Node::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{

}

sf::Vector2f Node::getPosition()
{
    return UIObject::getPosition();
}


sf::Vector2f Node::getCenter()
{
    return this->getPosition() + sf::Vector2f(30.0,30.0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

float Node::getRadius() 
{
    return sf::CircleShape::getRadius();
}

bool Node::contains(const sf::Vector2f& mousePos) const
{
    return this->getGlobalBounds().contains(mousePos);
}

void Node:: addEdge(const std::shared_ptr<Edge>& edge) 
{
    this->edges.push_back(edge);
}

void Node::removeEdge(const std::shared_ptr<Edge>& edge)
{
    edges.remove_if([edge](const std::shared_ptr<Edge>& sptr) {
        return sptr && sptr == edge;
        });
}

std::list<PtrEdge>&  Node::getEdges()
{
    return edges;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

Edge::Edge(const sf::Vector2f& pos1, const sf::Vector2f& pos2,sf::Font &font, const sf::Color& color)
    : line(), value(0)
    , valueBox(sf::Vector2f(0,0),15,sf::Color::Black,font,7)
{
    this->setColor(color);
    setPosition(pos1,pos2);
    UIObject::setPosition(pos1);
    sideLines.first.setSize(sf::Vector2f(20, 3));
    sideLines.second.setSize(sf::Vector2f(20, 3));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

void Edge::draw(sf::RenderWindow& window) const
{
    window.draw(line);
    window.draw(sideLines.first);
    window.draw(sideLines.second);
    valueBox.draw(window);
}

void Edge::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{
    if (userInput.event == Event::TextEntered)
    {
        if (userInput.character > 47 && userInput.character < 58 || userInput.character == 8 || userInput.character == 27)
        {
            valueBox.update(window, mousePos, userInput);
            if (!valueBox.getString().empty())
            {
                value = std::stoi(valueBox.getString());
            }
            else
            {
                value = 0;
            }
        }
        else if (userInput.character == 13)
        {
            this->setActive(false);
        }
    }
}

bool Edge::contains(const sf::Vector2f& mousePos) const 
{
    sf::Transform transform = line.getTransform();
    sf::FloatRect localBounds = line.getLocalBounds();

    sf::Transform transformSideLine1 = sideLines.first.getTransform();
    sf::FloatRect localBoundsSideLine1 = sideLines.first.getLocalBounds();

    sf::Transform transformSideLine2 = sideLines.second.getTransform();
    sf::FloatRect localBoundsSideLine2 = sideLines.second.getLocalBounds();

    localBounds.top -= 7;
    localBounds.height += 14;
    localBoundsSideLine1.top -= 7;
    localBoundsSideLine1.height += 14;
    localBoundsSideLine2.top -= 7;
    localBoundsSideLine2.height += 14;

    sf::Vector2f localMousePos1 = transform.getInverse().transformPoint(mousePos);
    sf::Vector2f localMousePos2 = transformSideLine1.getInverse().transformPoint(mousePos);
    sf::Vector2f localMousePos3 = transformSideLine2.getInverse().transformPoint(mousePos);
    return localBounds.contains(localMousePos1) || localBoundsSideLine1.contains(localMousePos2) || localBoundsSideLine2.contains(localMousePos3);
}

void Edge::setPosition(const sf::Vector2f& pos1,const sf::Vector2f& pos2)
{
    float alpha = calculateAngle(pos1, pos2);

    sf::Vector2f distance = sf::Vector2f(pos2.x - pos1.x, pos2.y - pos1.y);

    line.setPosition(movePoint(pos1, alpha - 90, 1.5));
    UIObject::setPosition(line.getPosition());
    line.setSize(sf::Vector2f(std::sqrt(distance.x * distance.x + distance.y * distance.y), 3));
    line.setRotation(alpha);

    sideLines.first.setPosition(movePoint(pos2, alpha + 150, 1.5));
    sideLines.second.setPosition(movePoint(pos2, alpha -90, -1.5));

    sideLines.first.setRotation(alpha - 150.0);
    sideLines.second.setRotation(alpha + 150.0);

    valueBox.setPosition(movePoint(pos1,alpha,line.getSize().x/2));

    if (alpha > -180 && alpha < -90)
    {
        valueBox.setPosition(valueBox.getPosition() + sf::Vector2f(0, -20));
    }
    else if (alpha < 90 && alpha > 0)
    {
        valueBox.setPosition(valueBox.getPosition() + sf::Vector2f(0, -20));
    }
}

void Edge::addNode(std::shared_ptr<Node>& node)
{
    if (!nodes.first)
    {
        nodes.first = node;
    }
    else if(!nodes.second)
    {
        nodes.second = node;
    }
}

std::pair<PtrNode, PtrNode >& Edge::getNodes()
{
    return nodes;
}

void Edge::setColor(const sf::Color& color)
{
    line.setFillColor(color);
    sideLines.first.setFillColor(color);
    sideLines.second.setFillColor(color);
}

void Edge::setValue(int value)
{
    this->value = value;
    valueBox.setString(std::to_string(value));
}

void Edge::setActive(bool value)
{
    UIObject::setActive(value);
    valueBox.setActive(value);
    if (value)
    {
        valueBox.setString(valueBox.getString());
    }
}

int Edge::getValue()
{
    return value;
}

TextBox& Edge::getTextBox()
{
    return valueBox;
}

void Edge::reset()
{
    if (nodes.first)
    {
        nodes.first->removeEdge( std::make_shared<Edge>(*this));
    }
    if (nodes.second)
    {
        nodes.second->removeEdge(std::make_shared<Edge>(*this));
    }
}