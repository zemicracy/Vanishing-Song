#include "FieldNote.h"
#include <random>
#include "GameClock.h"
using namespace aetherClass;
namespace{
	const int kMaxCallCount = 60;
	const float kTranslation = 5.0f;
	const float kRotation = 2.5f;
}
FieldNote::FieldNote()
{
}


FieldNote::~FieldNote()
{
	m_object.clear();
}

//
void FieldNote::mInitialize(eMusical type, ViewCamera* view, Vector3 min, Vector3 max){
	int count = 0;
	for (auto& note : m_object[type]){
		note._object = std::make_shared<FbxModel>();
		if (count % 2 == 0){
			note._object->LoadFBX("Model\\note\\note.fbx", eAxisSystem::eAxisOpenGL);
			note._object->SetTextureDirectoryName("Model\\note\\tex\\Field");
			note._isUpOrDwon = true;
		}
		else{
			note._object->LoadFBX("Model\\note\\note2.fbx", eAxisSystem::eAxisOpenGL);
			note._object->SetTextureDirectoryName("Model\\note\\tex\\Field");
			note._isUpOrDwon = false;
		}
		
		note._object->property._transform._translation = Vector3(-3,3,3);
		note._object->SetCamera(view);
		note._object->SetModelColor(mGetColor(type));
		std::random_device randam;     // 非決定的な乱数生成器を生成
		std::mt19937 mt(randam());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
		
		std::uniform_int_distribution<> x((int)min._x, (int)max._x);
		std::uniform_int_distribution<> z((int)min._z, (int)max._z);
		std::uniform_int_distribution<> y(10, 40);
		const int randamX = x(randam);
		const int randamZ = z(randam);
		const int randamY = y(randam);
		note._object->property._transform._translation = Vector3((float)randamX, (float)randamY, (float)randamZ);
		count += 1;
	}
	return;
}

//
void FieldNote::mUpdate(){
	for (auto& note : m_object){
		for (auto& object : note.second){
			if (object._isUpOrDwon){
				object._object->property._transform._translation._y += GameClock::GetDeltaTime()*kTranslation;
			}
			else{
				object._object->property._transform._translation._y -= GameClock::GetDeltaTime()*kTranslation;
			}

			if (object._callCount > kMaxCallCount){
				object._isUpOrDwon = !object._isUpOrDwon;
				object._callCount = NULL;
			}
			object._object->property._transform._rotation._y += kRotation;
			if (object._object->property._transform._rotation._y > 360){
				object._object->property._transform._rotation._y -= 360;
			}
			object._callCount += 1;
		}
	}
	return;
}

//
void FieldNote::mRender(aetherClass::ShaderBase* tex){
	for (auto& note : m_object){
		for (auto& object : note.second){
			object._object->Render(tex);
		}
	}
	return;
}

Color FieldNote::mGetColor(eMusical type){
	switch (type)
	{
	case eMusical::eRed:
		return Color(1, 0, 0, 1);
	case eMusical::eBlue:
		return Color(0, 0, 1, 1);
	case eMusical::eYellow:
		return Color(1,1, 0, 1);
	case eMusical::eGreen:
		return Color(0, 1, 0, 1);
	default:
		break;
	}

	return Color(0, 0, 0, 1);
}