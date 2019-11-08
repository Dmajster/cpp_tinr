#pragma once

#include "entt/entt.hpp"

class System
{
public:
	virtual ~System() = default;

	explicit System(const entt::registry& t_registry): m_registry(t_registry) {}

	virtual void Update() const;
	
	const entt::registry& m_registry;
};