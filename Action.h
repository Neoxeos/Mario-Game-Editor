#pragma once

#include "Entity.h"

#include <string>
#include <sstream>

class Action
{
	std::string m_name = "NONE";
	std::string m_type = "NONE";

public:
	Action() = default;

	Action(const std::string& name, const std::string& type)
		: m_name(name)
		, m_type(type) {}

	const std::string& getName() const { return m_name; }
	const std::string& getType() const { return m_type; }

	static Action parse(const std::string& actionStr)
	{
		std::istringstream iss(actionStr);
		std::string name, type;
		if (!(iss >> name >> type))
		{
			throw std::runtime_error("Invalid action string: " + actionStr);
		}
		return Action(name, type);
	}

	std::string toString() const
	{
		return m_name + " " + m_type;
	}
};