#ifndef _ATTACKPARTICLE_H
#define _ATTACKPARTICLE_H
#include <vector>
#include <ViewCamera.h>
#include <ShaderBase.h>
#include <Rectangle3D.h>
#include <Vector3.h>
class AttackParticle
{
public:
	struct ParticleDesc{
		ParticleDesc(){
			_spawnRate = NULL;
			_rangeMax = NULL;
			_rangeMin = NULL;
			_startPoint = 0.0f;
			_endPoint = 0.0f;
			_texturePath = "NULL";
		}

		~ParticleDesc(){
			_spawnRate = NULL;
			_rangeMax = NULL;
			_rangeMin = NULL;
			_startPoint = 0.0f;
			_endPoint = 0.0f;
			_texturePath = "NULL";
		}
		int _spawnRate;		//出現量
		aetherClass::Vector3 _rangeMax;		//出現位置の最大範囲
		aetherClass::Vector3 _rangeMin;		//出現位置の最小範囲
		aetherClass::Vector3 _startPoint;
		aetherClass::Vector3 _endPoint;
		aetherClass::Vector3 _scale;
		std::string _texturePath;
	};
private:
	struct Particle{
		std::shared_ptr<aetherClass::ModelBase> _object;
		aetherClass::Vector3 _vector;
		aetherClass::Vector3 _endPoint;
		bool _active;
		bool _deth;

	};
public:
	AttackParticle(ParticleDesc, aetherClass::ViewCamera*);
	~AttackParticle();
	void mUpdate(float timeScale);

	void mRender(aetherClass::ShaderBase* shader);

	void mReset();
	void mReset(ParticleDesc);
private:
	void mInitialize(ParticleDesc,aetherClass::ViewCamera*);

	aetherClass::Vector3 mRandam(aetherClass::Vector3 min, aetherClass::Vector3 max);
private:
	std::vector<Particle> m_particle;
	float m_endPointLength;
	std::shared_ptr<aetherClass::Texture> m_texture;
	int m_callCount;
	aetherClass::Vector3 m_minRange,m_maxRange;
	aetherClass::DirectXEntity m_entity;
};

#endif