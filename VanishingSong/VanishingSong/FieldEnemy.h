#ifndef _FieldEnemy_H
#define _FieldEnemy_H
#include "CharaStatus.h"
#include "GameController.h"
#include "Cube.h"
#include "CharaEntity.h"
#include <array>

class FieldEnemy
{
public:

	struct Property{
		std::shared_ptr<aetherClass::FbxModel> _penemy;			//Enemy�{��
		std::shared_ptr<aetherClass::ModelBase> _pCollider;	//�R���C�_�[
		bool _talkflag;
		int _enemyAreaNo;		//�G���A�i���o�[
	};

	FieldEnemy();
	~FieldEnemy();

	bool mInitialize(eMusical,aetherClass::ViewCamera*,std::string dataPath);
	void mUpdate();
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mFinalize();	//�J������
	void mResetTransform();
	void mSetTransform(aetherClass::Transform);
	Property &mGetProperty();
	void mFaceToPlayer(aetherClass::Vector3);
	void mRegisterMessage(std::string);
	void mRegisterCannnotMessage(std::string);
	int mGetMessageNum()const;
	std::string mGetMessage(const int id);
	std::string mGetCannotMessga();
	std::string mGetBattleDataPath();
private:
	bool mInitializeEnemy(eMusical,aetherClass::ViewCamera*);	//�G�̏�����
	void mInitializeEnemyColider(aetherClass::ViewCamera*);	//�R���C�_�[�̏�����

	CharaEntity m_charaEntity;
	Property m_property;
	std::vector<std::string> m_messagePath;
	aetherClass::Transform m_initTransform;
	std::string m_cannotMessagePath;
	std::string m_dataPath;
};
#endif