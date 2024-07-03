#ifndef GRAPHCOMPONENTS_HPP
#define GRAPHCOMPONENTS_HPP

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// This file will constain graph components like Nodes and Edge
/// It will be a great idea to inherit class Interactive Object 
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <list>
#include "UIObject.hpp"
#include "TextBox.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Class Nodes will inherit class CircleShape 
/// 
/////////////////////////////////////////////////////////////////////////////////////////////////////

class Node;
class Edge;
typedef std::shared_ptr<Node>       PtrNode;
typedef std::shared_ptr<Edge>        PtrEdge;


class Node :public sf::CircleShape, public UIObject {
public:
    Node();
    Node(const sf::Vector2f& position , sf::Font& font,  const sf::Color& fillColor = sf::Color(64,64,64), float radius = 30.0f, int pointCount = 30U);
public:
    virtual void			update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)override;
    void                    draw(sf::RenderWindow& window)const override;
    void                    reset() override {};

    bool		            contains(const sf::Vector2f& mousePos) const;

    void                    addEdge(const std::shared_ptr<Edge>& edge);
    void                    removeEdge(const std::shared_ptr<Edge>& edge);

    sf::Vector2f            getCenter();

    void                    setTextParameters(sf::Font& font);
    int                     getID();
    void                    setID(int value);
    static int ID;

    std::list<PtrEdge>&     getEdges() ;

    sf::Vector2f            getPosition();
    float                   getRadius();
private:
    std::list<PtrEdge>      edges;
    int idNode;
    sf::Text idNodeText;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Edge represents the line that units 2 Nodes
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////

class Edge : public UIObject
{
public:

    Edge(const sf::Vector2f &pos1, const sf::Vector2f& pos2, sf::Font& font, const sf::Color& color = sf::Color::Black);

    virtual void			update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)override;
    void                    draw(sf::RenderWindow& window)const override;
    void                    reset() override ;

    void                    setPosition(const sf::Vector2f& pos1,const sf::Vector2f& pos2);
    void                    setColor(const sf::Color& color);
    void                    setActive(bool value);

    bool		            contains(const sf::Vector2f& mousePos) const;

    void                    addNode(std::shared_ptr<Node>& node);
    void                    setValue(int value);
    int                     getValue();
    TextBox&                getTextBox();

    std::pair<PtrNode, PtrNode>& getNodes();

private:
    sf::RectangleShape      line;
    std::pair< sf::RectangleShape, sf::RectangleShape> sideLines;
    std::pair<PtrNode, PtrNode> nodes;
    int                     value;
    TextBox                 valueBox;
};

inline sf::Vector2f movePoint(const sf::Vector2f& position, float angle, float distance)
{
    auto angleRad = angle * std::acos(-1.0) / 180.0f;
    float x = position.x + distance * std::cos(angleRad);
    float y = position.y + distance * std::sin(angleRad);
    return sf::Vector2f(x, y);
}

inline float calculateAngle(const sf::Vector2f& pos1, const sf::Vector2f& pos2)
{
    return std::atan2(pos2.y - pos1.y, pos2.x - pos1.x) * 180.0 / std::acos(-1.0);
}

#endif // !GRAPHCOMPONENTS_HPP
