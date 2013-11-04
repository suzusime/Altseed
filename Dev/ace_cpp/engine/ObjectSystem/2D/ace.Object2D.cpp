﻿#include <list>
#include "ace.Object2D.h"
using namespace std;

namespace ace
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Object2D::Object2D()
		: m_owner(nullptr)
		, m_children(list<Object2DBasePtr>())
		, m_components(map<astring, ComponentPtr>())
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Object2D::~Object2D()
	{
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Object2D::Start()
	{
		OnStart();
	}

	void Object2D::Update()
	{
		OnUpdate();
		for (auto& x : m_components)
		{
			x.second->Update();
		}
	}

	void Object2D::SetLayer(Layer2D* layer)
	{
		m_owner = layer;
	}

#pragma region Get/Set
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Layer2D* Object2D::GetLayer() const
	{
		return m_owner;
	}

	Vector2DF Object2D::GetPosition() const
	{
		return GetCoreObject()->GetPosition();
	}

	void Object2D::SetPosition(Vector2DF value)
	{
		GetCoreObject()->SetPosition(value);
	}

	Vector2DF Object2D::GetGlobalPosition()
	{
		return GetCoreObject()->GetGlobalPosition();
	}

	float Object2D::GetAngle() const
	{
		return GetCoreObject()->GetAngle();
	}

	void Object2D::SetAngle(float value)
	{
		GetCoreObject()->SetAngle(value);
	}

	Vector2DF Object2D::GetScale() const
	{
		return GetCoreObject()->GetScale();
	}

	void Object2D::SetScale(Vector2DF value)
	{
		GetCoreObject()->SetScale(value);
	}

#pragma endregion

	void Object2D::AddChild(const Object2DBasePtr& child, eChildMode mode)
	{
		GetCoreObject()->AddChild(*(child->GetCoreObject()), mode);
		m_children.push_back(child);
	}

	void Object2D::RemoveChild(const Object2DBasePtr& child)
	{
		GetCoreObject()->RemoveChild(*(child->GetCoreObject()));
		m_children.remove(child);
	}

	const std::list<Object2D::Object2DBasePtr>& Object2D::GetChildren() const
	{
		return m_children;
	}


	void Object2D::AddComponent(const ComponentPtr& component, astring key)
	{
		m_components[key] = component;
		component->SetOwner(this);
	}

	Object2D::ComponentPtr& Object2D::GetComponent(astring key)
	{
		return m_components[key];
	}

	void Object2D::RemoveComponent(astring key)
	{
		auto it = m_components.find(key);
		m_components.erase(it);
		it->second->SetOwner(nullptr);
	}
}