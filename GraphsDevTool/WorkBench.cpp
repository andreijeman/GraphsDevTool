#include "WorkBench.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

WorkBench::WorkBench() : move(false), zoomCount(1), currentState(WorkBenchState::Repaus), fonts(nullptr)
{
}

WorkBench::WorkBench(const sf::Vector2f& position, const sf::Vector2f& size, FontHolder* fonts) :
	UIEntity(position, size), move(false), zoomCount(1), currentState(WorkBenchState::Repaus), fonts(fonts)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WorkBench::update(sf::RenderWindow& window, const sf::Vector2f& mousePos, const UserInput& userInput)
{
	window.setView(view);
	sf::Vector2f localPos = getLocalPos(window, mousePos);

	if (currentState == WorkBenchState::MinPathAlg)
	{
		if (!pathAnim.getList().empty())
		{
			pathAnim.update();
		}
		else
		{
			minPath(localPos,userInput);
		}
	}
	if (currentState == WorkBenchState::MaxPathAlg)
	{
		if (!pathAnim.getList().empty())
		{
			pathAnim.update();
		}
		else
		{
			maxPath(localPos,userInput);
		}
	}

	if (userInput.event == Event::MouseLeftPressed)
	{
		if (currentState == WorkBenchState::MakeNode)
		{
			if (!searchNode(localPos))
			{
				this->makeNode(localPos);
			}
		}
		else if (currentState == WorkBenchState::MakeEdge)
		{
			this->makeEdge(localPos);
		}
		else if (currentState == WorkBenchState::EraseElement)
		{
			this->eraseGraphElement(localPos);
		}
		else if (currentState == WorkBenchState::Repaus)
		{
			if (focusedObject)
			{
				focusedObject->setActive(false);
			}
			focusedObject = searchNode(localPos);
			if (!focusedObject)
			{
				focusedObject = searchEdge(localPos);
			}
			if (focusedObject)
			{
				focusedObject->setActive(true);
			}
		}
	}
	else if (userInput.event == Event::MouseRightPressed)
	{
		move = true;
		lastMousePos = mousePos;
	}
	else if (userInput.event == Event::MouseRightReleased)
	{
		move = false;
	}

	if (buildingEdge)
	{
		if (userInput.event == Event::MouseRightPressed)
		{
			buildingEdge = nullptr;
		}
		else
		{
			float alpha = calculateAngle(buildingEdge->getNodes().first->getCenter(), localPos);
			sf::Vector2f pos1 = movePoint(buildingEdge->getNodes().first->getCenter(), alpha, 30);
			buildingEdge->setPosition(pos1, localPos);
		}
	}
	if (focusedObject)
	{
		if (focusedObject->isActive())
		{
			focusedObject->update(window, localPos, userInput);
		}
		else
		{
			focusedObject = nullptr;
		}
	}
	if (move)
	{
		margin += sf::Vector2f((mousePos.x - lastMousePos.x), (mousePos.y - lastMousePos.y));
		view.move((lastMousePos.x - mousePos.x) * zoomCount, (lastMousePos.y - mousePos.y) * zoomCount);
		lastMousePos = mousePos;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WorkBench::setState(WorkBenchState::ID state)
{
	this->reset();
	if (currentState != state)
	{
		currentState = state;
	}
	else
	{
		currentState = WorkBenchState::Repaus;
	}
}

void WorkBench::setFont(FontHolder* fonts)
{
	if (fonts)
	{
		this->fonts = fonts;
	}
}

void WorkBench::draw(sf::RenderWindow& window) const
{
	window.setView(view);
	if (buildingEdge)
	{
		buildingEdge->draw(window);
	}

	for (const auto& edgePtr : mEdges)
	{
		edgePtr->draw(window);
	}

	if (!pathAnim.getList().empty())
	{
		pathAnim.draw(window);
	}

	for (const auto& nodePtr : mNodes)
	{
		nodePtr->draw(window);
	}
}

bool WorkBench::isActive() const
{
	return false;
}

void WorkBench::reset()
{
	currentState = WorkBenchState::Repaus;
	if (buildingEdge)
	{
		buildingEdge->reset();
		buildingEdge.reset();
	}
	if (focusedObject)
	{
		focusedObject->setActive(false);
		focusedObject = nullptr;
	}
	pathAnim.getList().clear();
	pathAnimEdge = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

PtrNode& WorkBench::searchNode(const sf::Vector2f& mousePos)
{
	for (auto& node : mNodes)
	{
		if (node->contains(mousePos))
			return node;
	}
	static PtrNode nullNode;
	return nullNode;
}

PtrEdge& WorkBench::searchEdge(const sf::Vector2f& mousePos)
{
	for (auto& edge : mEdges)
	{
		if (edge->contains(mousePos))
			return edge;
	}
	static PtrEdge nullEdge;
	return nullEdge;
}

bool WorkBench::verificationDuplicatEdge(const PtrNode& node1, const PtrNode& node2)
{
	for (PtrEdge edge : mEdges)
	{
		if (edge->getNodes().first == node1 && edge->getNodes().second == node2)
		{
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////-COMMANDS-/////////////////////////////////////////////////////////

void WorkBench::makeNode(const sf::Vector2f& mousePos)
{
	std::shared_ptr<Node> newNode = std::make_shared<Node>(mousePos, fonts->get(Fonts::Main), sf::Color(60, 60, 60));

	mNodes.push_back(newNode);
}

void WorkBench::makeEdge(const sf::Vector2f& mousePos)
{
	if (!buildingEdge)
	{
		if (PtrNode tempNode = searchNode(mousePos))
		{
			buildingEdge = std::make_shared<Edge>(tempNode->getCenter(), mousePos, fonts->get(Fonts::Main));
			buildingEdge->addNode(tempNode);
			tempNode->addEdge(buildingEdge);
		}
	}
	else if (PtrNode tempNode = searchNode(mousePos))
	{
		if (tempNode != buildingEdge->getNodes().first && !verificationDuplicatEdge(buildingEdge->getNodes().first, tempNode))
		{
			float alpha = calculateAngle(buildingEdge->getNodes().first->getCenter(), tempNode->getCenter());
			sf::Vector2f pos1 = movePoint(buildingEdge->getNodes().first->getCenter(), alpha, tempNode->getRadius());
			sf::Vector2f pos2 = movePoint(tempNode->getCenter(), alpha, -tempNode->getRadius());
			buildingEdge->setPosition(pos1, pos2);
			mEdges.push_back(buildingEdge);
			tempNode->addEdge(buildingEdge);
			buildingEdge->addNode(tempNode);
			buildingEdge = nullptr;
		}
	}
}

void WorkBench::eraseGraphElement(const sf::Vector2f& mousePos)
{
	std::shared_ptr<UIObject> elem = this->searchNode(mousePos);

	if (elem)
	{
		if (PtrNode deleteNode = std::dynamic_pointer_cast<Node>(elem))
		{
			for (PtrEdge& deleteNodeEdge : deleteNode->getEdges())
			{
				if (deleteNodeEdge->getNodes().first != deleteNode && deleteNodeEdge->getNodes().first)
				{
					deleteNodeEdge->getNodes().first->removeEdge(deleteNodeEdge);
				}
				else if (deleteNodeEdge->getNodes().second)
				{
					deleteNodeEdge->getNodes().second->removeEdge(deleteNodeEdge);
				}
				mEdges.remove_if([deleteNodeEdge](const PtrEdge& sptr) { return sptr == deleteNodeEdge;});
			}
			mNodes.remove_if([deleteNode](const PtrNode& sptr) { return sptr == deleteNode;});
		}
		Node::ID = 0;
		for (std::list<PtrNode>::iterator::value_type node : mNodes)
		{
			node->setID(Node::ID++);
		}
	}
	else
	{
		elem = this->searchEdge(mousePos);
		if (elem)
		{
			if (PtrEdge deleteEdge = std::dynamic_pointer_cast<Edge>(elem))
			{
				deleteEdge->getNodes().first->removeEdge(deleteEdge);
				deleteEdge->getNodes().second->removeEdge(deleteEdge);
				mEdges.remove_if([deleteEdge](const PtrEdge& sptr) { return sptr == deleteEdge;});
			}
		}
	}
}


void WorkBench::clear()
{
	std::vector<std::vector<int>> temp = getAdjacencyMatrix();
	this->setState(WorkBenchState::Repaus);
	Node::ID = 0;
	mEdges.clear();
	mNodes.clear();
}

std::vector<std::vector<int>> WorkBench::getAdjacencyMatrix()
{
	std::vector<std::vector<int>> graph(mNodes.size(), std::vector<int>(mNodes.size(), 0));

	auto it = mNodes.begin();
	for (int i = 0; it != mNodes.end(); ++i, ++it)
	{
		for (auto& edge : (*it)->getEdges())
		{
			auto& nodes = edge->getNodes();
			if (nodes.first == *it)
			{
				auto& secondNode = nodes.second;
				if (secondNode && secondNode->getID() < mNodes.size())
				{
					graph[i][secondNode->getID()] = edge->getValue();
				}
			}
		}
	}
	return graph;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WorkBench::zoom(const sf::Event& event)
{
	if (event.mouseWheel.x < 0)
	{
		if (zoomCount < 5.f)
		{
			zoomCount *= 1.1f;
			view.zoom(1.1f);
		}
	}
	else if (zoomCount > 0.5f)
	{
		zoomCount *= 10.0f / 11.0f;
		view.zoom(10.0f / 11.0f);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WorkBench::setPathAnim(std::list<std::pair<sf::Vector2f, sf::Vector2f>> edges)
{
	pathAnim = PathAnim(edges);
}

std::list<PtrNode> WorkBench::getNodeList()
{
	return mNodes;
}

std::list<PtrEdge> WorkBench::getEdgeList()
{
	return mEdges;
}

PtrEdge WorkBench::getLastAddedEdge()
{
	return mEdges.back();
}

void WorkBench::minPath(const sf::Vector2f& localPos, const UserInput& userInput)
{
	if (userInput.event == Event::MouseLeftPressed)
	{
		if (!pathAnimEdge)
		{
			if (auto& node = searchNode(localPos))
			{
				pathAnimEdge = std::make_shared<Edge>(node->getCenter(), node->getCenter(), fonts->get(Fonts::Main));
				pathAnimEdge->addNode(node);
			}
		}
		else
		{
			if (auto& node = searchNode(localPos))
			{
				std::list<int> l = bellmanCallabaMin(getAdjacencyMatrix(), mNodes.size(), pathAnimEdge->getNodes().first->getID(), node->getID());
				std::list<std::pair<sf::Vector2f, sf::Vector2f>> lh;
				auto it1 = mNodes.begin();
				auto it2 = mNodes.begin();

				std::advance(it1, l.front());
				l.pop_front();

				for (auto intIt : l)
				{

					auto it2 = mNodes.begin();
					std::advance(it2, intIt);

					if (it1 != mNodes.end() && it2 != mNodes.end())
					{
						lh.push_back(std::pair<sf::Vector2f, sf::Vector2f>(it1->get()->getCenter(), it2->get()->getCenter()));
						it1 = it2;
					}
					else
					{
						break;
					}
				}
				pathAnim = lh;
			}
		}
	}
}

void WorkBench::maxPath(const sf::Vector2f& localPos, const UserInput& userInput)
{
	if (userInput.event == Event::MouseLeftPressed)
	{
		if (!pathAnimEdge)
		{
			if (auto& node = searchNode(localPos))
			{
				pathAnimEdge = std::make_shared<Edge>(node->getCenter(), node->getCenter(), fonts->get(Fonts::Main));
				pathAnimEdge->addNode(node);
			}
		}
		else
		{
			if (auto& node = searchNode(localPos))
			{
				std::list<int> l = bellmanCallabaMax(getAdjacencyMatrix(), mNodes.size(), pathAnimEdge->getNodes().first->getID(), node->getID());
				std::list<std::pair<sf::Vector2f, sf::Vector2f>> lh;
				auto it1 = mNodes.begin();
				auto it2 = mNodes.begin();

				std::advance(it1, l.front());
				l.pop_front();

				for (auto intIt : l)
				{

					auto it2 = mNodes.begin();
					std::advance(it2, intIt);

					if (it1 != mNodes.end() && it2 != mNodes.end())
					{
						lh.push_back(std::pair<sf::Vector2f, sf::Vector2f>(it1->get()->getCenter(), it2->get()->getCenter()));
						it1 = it2;
					}
					else
					{
						break;
					}
				}
				pathAnim = lh;
			}
		}
	}
}