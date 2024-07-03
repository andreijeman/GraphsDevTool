#ifndef COMMANDHOLDER_HPP
#define COMMANDHOLDER_HPP

#include <queue>
namespace Command
{
	enum ID
	{
		Nothing = 0,
		AddNode, AddEdge, EraseGraphComponent, ClearGraph,
		ShowFileBar, ShowAlgorithmBar,
		RunMinPathAlg, RunMaxPathAlg, RunMaxFlowAlg,
		OpenLoadFileMenu, OpenSaveFileMenu, OpenTerminal
		
	};
}


class CommandHolder
{
public:
	void push(Command::ID command);
	Command::ID pop();

private:
	std::queue<Command::ID> commands;

};

#endif