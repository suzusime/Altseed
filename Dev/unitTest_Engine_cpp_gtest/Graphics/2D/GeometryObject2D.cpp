﻿
#include <gtest/gtest.h>
#include <ace.h>
#include "../../EngineTest.h"

using namespace std;
using namespace ace;

class Graphics_GeometryObject2D : public EngineTest
{
public:
	Graphics_GeometryObject2D(bool isOpenGLMode)
		: EngineTest(ace::ToAString("GeometryObject2D"), isOpenGLMode, 20)
	{
	}

private:

protected:
	void OnStart()
	{
		auto scene = make_shared<Scene>();
		auto layer = make_shared<Layer2D>();
		auto texture = ace::Engine::GetGraphics()->CreateTexture2D(ace::ToAString("Data/Texture/Sample1.png").c_str());
		auto geometryObj1 = make_shared<GeometryObject2D>();
		auto geometryObj2 = make_shared<GeometryObject2D>();
		auto geometryObj3 = make_shared<GeometryObject2D>();
		auto geometryObj4 = make_shared<GeometryObject2D>();
		auto geometryObj5 = make_shared<GeometryObject2D>();

		layer->AddObject(geometryObj1);
		layer->AddObject(geometryObj2);
		layer->AddObject(geometryObj3);
		layer->AddObject(geometryObj4);
		layer->AddObject(geometryObj5);

		scene->AddLayer(layer);
		ace::Engine::ChangeScene(scene);

		{
			auto circle = make_shared<Circle>();
			circle->SetOuterDiameter(100);
			circle->SetInnerDiameter(10);
			circle->SetNumberOfCorners(96);
			circle->SetPosition(Vector2DF(100, 50));

			geometryObj1->SetShape(circle);
			geometryObj1->SetTexture(texture);
			geometryObj1->SetPosition(ace::Vector2DF(0, 0));
		}

		{
			auto arc = make_shared<ace::Arc>();
			arc->SetOuterDiameter(100);
			arc->SetInnerDiameter(10);
			arc->SetNumberOfCorners(96);
			arc->SetPosition(Vector2DF(300, 50));
			arc->SetStartingCorner(90);
			arc->SetEndingCorner(5);

			geometryObj2->SetShape(arc);
			geometryObj2->SetTexture(texture);
			geometryObj2->SetPosition(ace::Vector2DF(0, 0));
		}

		{
			auto line = make_shared<Line>();
			line->SetStartingPosition(Vector2DF(410, 50));
			line->SetEndingPosition(Vector2DF(630, 50));
			line->SetThickness(5.0f);

			geometryObj3->SetShape(line);
			geometryObj3->SetPosition(ace::Vector2DF(0, 0));
		}

		{
			auto rect = make_shared<ace::Rectangle>();
			rect->SetDrawingArea(ace::RectF(10, 110, 300, 200));
			rect->SetUV(ace::RectF(0, 0, 0.5, 0.5));

			geometryObj4->SetShape(rect);
			geometryObj4->SetTexture(texture);
			geometryObj4->SetPosition(ace::Vector2DF(0, 0));
		}

		{
			auto triangle = make_shared<ace::Triangle>();
			triangle->SetPointByIndex(ace::Vector2DF(320, 350), 0);
			triangle->SetPointByIndex(ace::Vector2DF(100, 450), 1);
			triangle->SetPointByIndex(ace::Vector2DF(540, 450), 2);

			triangle->SetUVByIndex(ace::Vector2DF(0.5, 0.2), 0);
			triangle->SetUVByIndex(ace::Vector2DF(0.1, 0.5), 1);
			triangle->SetUVByIndex(ace::Vector2DF(0.9, 0.7), 2);

			geometryObj5->SetShape(triangle);
			geometryObj5->SetTexture(texture);
			geometryObj5->SetPosition(ace::Vector2DF(0, 0));
		}
	}

	void OnUpdating()
	{
	}
};


ENGINE_TEST(Graphics, GeometryObject2D)
