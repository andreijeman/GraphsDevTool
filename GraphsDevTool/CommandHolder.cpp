#include "CommandHolder.hpp"

void CommandHolder::push(Command::ID command)
{
	if (command)
	{
		commands.push(command);
	}
}

Command::ID CommandHolder::pop()
{
	if (commands.empty()) return Command::Nothing;

	Command::ID temp = commands.front();
	commands.pop();
	return temp;
}
