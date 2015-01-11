﻿//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#include "ace.Chip2D_Imp.h"
#include "../ace.Graphics_Imp.h"

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace ace {

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Chip2D_Imp::Chip2D_Imp(Graphics* graphics)
		: DeviceObject(graphics)
		, m_graphics(graphics)
		, m_src(RectF())
		, m_color(Color())
		, m_alphaBlend(AlphaBlend::Blend)
		, m_texture(nullptr)
		, m_turnLR(false)
		, m_turnUL(false)
	{

	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Chip2D_Imp::~Chip2D_Imp()
	{
		SafeRelease(m_texture);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Texture2D* Chip2D_Imp::GetTexture() const
	{
		return m_texture;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Chip2D_Imp::SetTexture(Texture2D* texture)
	{
		SafeSubstitute(m_texture, texture);
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	RectF Chip2D_Imp::GetSrc() const
	{
		return m_src;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Chip2D_Imp::SetSrc(RectF src)
	{
		m_src = src;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	Color Chip2D_Imp::GetColor() const
	{
		return m_color;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Chip2D_Imp::SetColor(Color color)
	{
		m_color = color;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Chip2D_Imp::GetTurnLR() const
	{
		return m_turnLR;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Chip2D_Imp::SetTurnLR(bool turnLR)
	{
		m_turnLR = turnLR;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	bool Chip2D_Imp::GetTurnUL() const
	{
		return m_turnUL;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Chip2D_Imp::SetTurnUL(bool turnUL)
	{
		m_turnUL = turnUL;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	AlphaBlend Chip2D_Imp::GetAlphaBlendMode() const
	{
		return m_alphaBlend;
	}

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	void Chip2D_Imp::SetAlphaBlendMode(AlphaBlend alphaBlend)
	{
		m_alphaBlend = alphaBlend;
	}
}