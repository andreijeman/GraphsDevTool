#ifndef WORKBENCH_HPP
#define WORKBENCH_HPP

#include "UIEntity.hpp"
#include "GraphComponents.hpp"
#include "ResourceIdentifier.hpp"
#include "ResourceHolder.hpp"
#include "PathAnim.hpp"
#include "GraphAlgorithms.hpp"

namespace WorkBenchState
{
	enum ID
	{
		Repaus,
		MakeNode,
		MakeEdge,
		EraseElement,
		Clear,
		MinPathAlg,
		MaxPathAlg
	};
}


class WorkBench : public UIEntity
{
public:
	WorkBench();
	WorkBench(const sf::Vector2f& position, const sf::Vector2f& size,FontHolder * fonts);

	void								update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput) override;
	void 								draw(sf::RenderWindow& window) const override;
	bool								isActive() const override;
	void								reset() override;

	PtrNode&						   	searchNode(const sf::Vector2f& mousePos);
	PtrEdge&							searchEdge(const sf::Vector2f& mousePos);
	bool								verificationDuplicatEdge(const PtrNode& node1, const PtrNode& node2);

	void								setFont(FontHolder * fonts);

	void								setState(WorkBenchState::ID state);
	void                                zoom(const sf::Event& event);
	void								clear();

	std::vector<std::vector<int>>		getAdjacencyMatrix();

	void								setPathAnim(std::list<std::pair<sf::Vector2f, sf::Vector2f>>);

	void								minPath(const sf::Vector2f& localPos, const UserInput& userInput);
	void								maxPath(const sf::Vector2f& localPos, const UserInput& userInput);

public:

	void                                makeNode(const sf::Vector2f& mousePos);
	void								makeEdge(const sf::Vector2f& mousePos);
	void								eraseGraphElement(const sf::Vector2f& mousePos);

	std::list<PtrNode>					getNodeList();
	std::list<PtrEdge>					getEdgeList();
	PtrEdge								getLastAddedEdge();

private:

	WorkBenchState::ID					currentState;
	sf::Vector2f						lastMousePos;
	bool								move;

	std::list<PtrNode>					mNodes;
	std::list<PtrEdge>					mEdges;

	PtrEdge								buildingEdge;
	PtrEdge								pathAnimEdge;
	std::shared_ptr<UIObject>			focusedObject;

	float                               zoomCount;
	FontHolder*							fonts;

private:
	PathAnim							pathAnim;
};

#endif 
