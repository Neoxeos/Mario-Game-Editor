#pragma once

#include "Components.h"
#include <memory>
#include <string>
#include <tuple>

using ComponentTuple = std::tuple<
	CTransform,
	CShape,	
	CCollision,
	CInput,
	CAnimation,
	CGravity,
	CBoundingBox
	>;

class Entity
{
	friend class EntityManager;

	bool m_active = true;
	size_t m_id = 0;
	std::string m_tag = "default";

	ComponentTuple m_components;

	// constructor and destructor
	Entity(const size_t id, const std::string& tag);

public:

	void destroy();
	size_t id();
	bool isActive();
	const std::string& tag();

	template<typename T> bool hasComponent() const
	{
		return getComponent<T>().has;
	}

	template<typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		auto& component = getComponent<T>();
		component = T(std::forward<TArgs>(mArgs)...);
		component.has = true;
		return component;
	}

	template<typename T>
	T& getComponent()
	{
		return std::get<T>(m_components);
	}

	template<typename T>
	const T& getComponent() const
	{
		return std::get<T>(m_components);
	}

	// private member access functions
	bool isActive() const;
	const std::string& tag() const;
	const size_t id() const;
	void destroy();
};