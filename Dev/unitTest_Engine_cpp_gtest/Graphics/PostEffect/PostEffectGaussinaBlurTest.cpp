﻿#include <ace.h>
#include <gtest/gtest.h>
#include <memory>


void Graphics_PostEffectGaussianBlur(bool isOpenGLMode)
{
	ace::EngineOption option;
	option.GraphicsType = isOpenGLMode ? ace::GRAPHICS_TYPE_GL : ace::GRAPHICS_TYPE_DX11;

	auto engine = ace::GetEngine();
	engine->Initialize(ace::ToAString("PostEffectGaussianBlur").c_str(), 640, 480, option);

	{
		auto scene = std::make_shared<ace::Scene>();
		auto layer = std::make_shared<ace::Layer2D>();
		auto object = std::make_shared<ace::TextureObject2D>();
		scene->AddLayer(layer);
		layer->AddObject(object);
		engine->ChangeScene(scene);

		auto g = ace::GetGraphics();
		auto texture = g->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		object->SetTexture(texture);
		object->SetScale(ace::Vector2DF(2, 2));

		auto pe = std::make_shared<ace::PostEffectGaussianBlur>(g);
		layer->AddPostEffect(pe);

		while (engine->DoEvents())
		{
			engine->Update();
		}
	}

	engine->Terminate();

	auto ref = ace::GetGlobalReferenceCount();
	ASSERT_TRUE(ref == 0);

}


TEST(Graphics, PostEffectGaussianBlur_GL)
{
	Graphics_PostEffectGaussianBlur(true);
}

#if _WIN32
TEST(Graphics, PostEffectGaussianBlur_DX)
{
	Graphics_PostEffectGaussianBlur(false);
}
#endif
