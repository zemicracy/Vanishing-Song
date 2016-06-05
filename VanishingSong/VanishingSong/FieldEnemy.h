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
	enum class eType{
		Air,
		Ground,
		Blue,
		Yellow,
		Null
	};

	struct Property{
		std::shared_ptr<GearFrame> _penemy;			//Enemy�{��
		std::shared_ptr<aetherClass::ModelBase> _pCollider;	//�R���C�_�[
		bool _talkflag;
		int _enemyAreaNo;		//�G���A�i���o�[
	};

	FieldEnemy();
	~FieldEnemy();

	bool mInitialize(eType,aetherClass::ViewCamera*,std::string dataPath);
	void SetLoadModelValue(std::shared_ptr<Gear>&, ObjectInfo*);
	void mUpdate();
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mFinalize();	//�J������
	void mEnemyOnHit();	//�����������ǂ���
	Property &mGetProperty();
	void mFaceToPlayer(aetherClass::Vector3);
	void mRegisterMessage(std::string);
	int mGetMessageNum()const;
	std::shared_ptr<aetherClass::Texture> mGetMessage(const int id);
	std::string mGetBattleDataPath();
private:
	bool mInitializeGround(aetherClass::ViewCamera*);				//�G������(�n��)
	bool mInitializeAir(aetherClass::ViewCamera*);				//�G������(��)
	bool mInitializeYellow(aetherClass::ViewCamera*);				//�G������(�n��)
	bool mInitializeBlue(aetherClass::ViewCamera*);				//�G������(��)
	void mInitializeEnemyColider(aetherClass::ViewCamera*);	//�R���C�_�[�̏�����

	CharaEntity m_charaEntity;
	std::shared_ptr<Gear> m_pTopGear;
	Property m_property;
	std::vector<std::shared_ptr<aetherClass::Texture>> m_message;
	std::string m_dataPath;
};
#endif