#include "AttackParticle.h"
#include <random>
using namespace aetherClass;
namespace{
	const int kActiveNum = 5; // 一度にアクティブにする数
	const int kCallCount = 10; // アクティブにする間隔
}
AttackParticle::AttackParticle(ParticleDesc desc, ViewCamera* camera)
{
	mInitialize(desc,camera);
}


AttackParticle::~AttackParticle()
{
	for (auto& index : m_particle){
		index._object->Finalize();
		index._object.reset();
		index._active = false;
	}
}

void AttackParticle::mInitialize(ParticleDesc desc, ViewCamera* camera){
	m_particle.resize(desc._spawnRate);
	m_texture = std::make_shared<Texture>();
	m_texture->Load(desc._texturePath);
	for (auto& index : m_particle){
		index._object = std::make_shared<Rectangle3D>();
		index._object->Initialize();
		index._object->SetCamera(camera);
		index._object->SetTexture(m_texture.get());
		// 初期位置決め
		index._object->property._transform._translation = desc._startPoint;
		index._object->property._transform._scale = desc._scale;

		// 進むベクトルの算出
		index._vector = (desc._endPoint - index._object->property._transform._translation).Normalize();
		index._endPoint = desc._endPoint;
		index._active = false;
		index._deth = false;
	}
	m_endPointLength = desc._endPoint.GetVectorLength();
	m_callCount = 0;
	m_minRange = desc._rangeMin;
	m_maxRange = desc._rangeMax;
	return;
}

//
void AttackParticle::mUpdate(float timeScale){
	m_callCount += 1;

	// アクティブにする
	if (m_callCount > kCallCount){
		int counter = NULL;
		for (auto& index : m_particle){
			if (index._active)continue;
			index._active = true;
			index._object->property._transform._translation += mRandam(m_minRange, m_maxRange);
			index._vector = (index._endPoint - index._object->property._transform._translation).Normalize();
			counter += 1;
			if (counter > kActiveNum){
				break;
			}
		}
		m_callCount = NULL;
		counter = NULL;
	}

	// 更新処理
	for (auto& index : m_particle){
		if (!index._active || index._deth)continue;
		index._object->property._transform._translation += index._vector*timeScale;

		const float length = index._object->property._transform._translation.GetVectorLength();
		if (length>m_endPointLength){
			index._deth = true;
		}
	}
	return;
}

//
void AttackParticle::mRender(aetherClass::ShaderBase* shader){

	for (auto& index : m_particle){
		if (!index._active||index._deth)continue;
		index._object->Render(shader);
	}
	return;
}

//
Vector3 AttackParticle::mRandam(Vector3 min, Vector3 max){
	std::random_device randam;
	std::mt19937 mt(randam());
	std::uniform_int_distribution<> xCreate(min._x, max._x);
	std::uniform_int_distribution<> yCreate(min._y, max._y);
	std::uniform_int_distribution<> zCreate(min._z, max._z);

	const float x = xCreate(randam);
	const float y = yCreate(randam);
	const float z = zCreate(randam);

	return Vector3(x, y, z);
}

//
void AttackParticle::mReset(){
	for (auto& index : m_particle){
		index._active = false;
		index._deth = false;
	}
	m_callCount = 0;
}


//
void AttackParticle::mReset(ParticleDesc desc){
	for (auto& index : m_particle){
		// 初期位置決め
		index._object->property._transform._translation = desc._startPoint;
		index._object->property._transform._scale = desc._scale;
		index._endPoint = desc._endPoint;
		index._active = false;
		index._deth = false;
	}
	m_endPointLength = desc._endPoint.GetVectorLength();
	m_callCount = 0;
	m_minRange = desc._rangeMin;
	m_maxRange = desc._rangeMax;
}