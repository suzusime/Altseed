﻿#include "asd.EffectObject2D.h"

#include <stdexcept>

namespace asd
{
	extern ObjectSystemFactory* g_objectSystemFactory;

	EffectObject2D::EffectObject2D()
		: m_coreObject(nullptr)
	{
		m_coreObject = CreateSharedPtrWithReleaseDLL(g_objectSystemFactory->CreateEffectObject2D());
	}

	EffectObject2D::~EffectObject2D()
	{
	}

	CoreObject2D* EffectObject2D::GetCoreObject() const
	{
		return m_coreObject.get();
	}

	std::shared_ptr<Effect> EffectObject2D::GetEffect()
	{
		return m_coreObject->GetEffect();
	}

	void EffectObject2D::SetEffect(std::shared_ptr<Effect> effect)
	{
		m_coreObject->SetEffect(effect.get());
	}

	int32_t EffectObject2D::Play()
	{
		return m_coreObject->Play();
	}

	void EffectObject2D::Stop()
	{
		m_coreObject->Stop();
	}

	void EffectObject2D::StopRoot()
	{
		m_coreObject->StopRoot();
	}

	void EffectObject2D::Show()
	{
		m_coreObject->Show();
	}

	void EffectObject2D::Hide()
	{
		m_coreObject->Hide();
	}

	bool EffectObject2D::GetIsPlaying()
	{
		return m_coreObject->GetIsPlaying();
	}

	bool EffectObject2D::GetSyncEffects()
	{
		return m_coreObject->GetSyncEffects();
	}

	void EffectObject2D::SetSyncEffects(bool value)
	{
		m_coreObject->SetSyncEffects(value);
	}

	float EffectObject2D::GetEffectRotationX() const
	{
		return m_coreObject->GetEffectRotationX();
	}

	void EffectObject2D::SetEffectRotationX(float value)
	{
		m_coreObject->SetEffectRotationX(value);
	}

	float EffectObject2D::GetEffectRotationY() const
	{
		return m_coreObject->GetEffectRotationY();
	}

	void EffectObject2D::SetEffectRotationY(float value)
	{
		m_coreObject->SetEffectRotationY(value);
	}

	float EffectObject2D::GetEffectRotation() const
	{
		return m_coreObject->GetEffectRotation();
	}

	void EffectObject2D::SetEffectRotation(float value)
	{
		m_coreObject->SetEffectRotation(value);
	}

	int EffectObject2D::GetDrawingPriority() const
	{
		return m_coreObject->GetDrawingPriority();
	}

	void EffectObject2D::SetDrawingPriority(int priority)
	{
		m_coreObject->SetDrawingPriority(priority);
	}
}