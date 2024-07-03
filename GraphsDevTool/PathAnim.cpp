#include "PathAnim.hpp"

EdgeAnim::EdgeAnim(const sf::Vector2f& point1, const sf::Vector2f& point2, float distBetweenParticle, float particleRadius) :
	point1(point1 - sf::Vector2f(particleRadius, particleRadius)), point2(point2 - sf::Vector2f(particleRadius, particleRadius))
{
	dist = std::sqrtf(std::powf((point2.x - point1.x), 2) + std::powf((point2.y - point1.y), 2));
	float segments = dist / (distBetweenParticle + 2 * particleRadius);

	step.x = (point2.x - point1.x) / segments;
	step.y = (point2.y - point1.y) / segments;


	sf::Vector2f pos = this->point1;
	for (int i = 1; i < segments; i++)
	{
		sf::CircleShape circle;
		circle.setRadius(particleRadius);
		circle.setFillColor(sf::Color::White);
		circle.setOutlineThickness(2);
		circle.setOutlineColor(sf::Color(0, 51, 102));
		circle.setPosition(pos);

		mList.push_back(circle);

		pos += step;
	}

	step.x /= 20;
	step.y /= 20;
}

void EdgeAnim::update()
{
	for(sf::CircleShape& circle : mList)
	{
		circle.setPosition(circle.getPosition() + step);
	}

	sf::CircleShape circle = mList.back();

	float dist2 = std::sqrtf(std::powf((circle.getPosition().x - point1.x), 2) + std::powf((circle.getPosition().y - point1.y), 2));

	if (dist2 >= dist)
	{
		mList.pop_back();
		circle.setPosition(point1);
		mList.push_front(circle);
	}
}

void EdgeAnim::draw(sf::RenderWindow& window) const
{
	for (const sf::CircleShape& circle : mList)
	{
		window.draw(circle);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

PathAnim::PathAnim()
{
}

PathAnim::PathAnim(std::list<std::pair<sf::Vector2f, sf::Vector2f>> edges)
{
	for (std::pair<sf::Vector2f, sf::Vector2f>& edge : edges)
	{
		edgesAnim.push_back(EdgeAnim(edge.first, edge.second, 16, 8));
	}
}

void PathAnim::draw(sf::RenderWindow& window) const
{
	for (const EdgeAnim& e : edgesAnim)
	{
		e.draw(window);
	}
}

void PathAnim::update()
{
	float time = clock.getElapsedTime().asMilliseconds();
	while (time > 10)
	{
		clock.restart();
		for (EdgeAnim& e : edgesAnim)
		{
			e.update();
		}
		time -= 10;
	}
}

std::list<EdgeAnim>& PathAnim::getList()
{
	return this->edgesAnim;
}

const std::list<EdgeAnim>& PathAnim::getList()const
{
	return this->edgesAnim;
}