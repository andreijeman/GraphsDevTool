#ifndef PATHANIM_HPP
#define PATHANIM_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

#include <list>

class EdgeAnim
{
public:
	EdgeAnim(const sf::Vector2f& point1, const sf::Vector2f& point2, float distBetweenParticle, float particleRadius);

	void							draw(sf::RenderWindow& window) const;
	void							update();


private:
	sf::Vector2f					point1;
	sf::Vector2f					point2;

	sf::Vector2f					step;

	float							dist;

	std::list<sf::CircleShape>		mList;
};

class PathAnim
{
public:
	PathAnim();
	PathAnim(std::list<std::pair<sf::Vector2f, sf::Vector2f>>);

	void							draw(sf::RenderWindow& window) const;
	void							update();
	std::list<EdgeAnim>&			getList();
	const std::list<EdgeAnim>&		getList() const;

private:
	std::list<EdgeAnim>				edgesAnim;
	sf::Clock						clock;

};

#endif 
