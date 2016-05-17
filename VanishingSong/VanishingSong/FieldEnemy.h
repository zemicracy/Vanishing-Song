#ifndef _FieldEnemy_H
#define _FieldEnemy_H
#include "CharaStatus.h"
#include "GameController.h"
#include "GearFrame.h"
#include "Cube.h"
#include "CharaEntity.h"
#include <array>

class FieldEnemy
{
public:

	struct Property{
		std::shared_ptr<GearFrame> _penemy;			//Enemy�{��
		std::shared_ptr<aetherClass::Cube> _pCollider;	//�R���C�_�[
		int _enemyAreaNo;		//�G���A�i���o�[
	};

	FieldEnemy();
	~FieldEnemy();

	bool mInitializeGround(aetherClass::ViewCamera*);				//�G������(�n��)
	bool mInitializeAir(aetherClass::ViewCamera*);				//�G������(��)
	void mInitializeEnemyColider(aetherClass::ViewCamera*);	//�R���C�_�[�̏�����
	void SetLoadModelValue(std::shared_ptr<Gear>&, ObjectInfo*);
	void mUpdate();
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mFinalize();	//�J������
	void mEnemyOnHit();	//�����������ǂ���
	Property &mGetProperty();

private:
	CharaEntity m_charaEntity;
	std::shared_ptr<Gear> m_pTopGear;
	Property m_property;
};
#endif