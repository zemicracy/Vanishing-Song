#ifndef _FieldEnemy_H
#define _FieldEnemy_H
#include "CharaStatus.h"
#include "GameController.h"
#include "Cube.h"
#include "CharaEntity.h"
#include <array>
#include <Sphere.h>
class FieldEnemy
{
public:

	struct Property{
		std::shared_ptr<aetherClass::FbxModel> _pEnemy;			//Enemy本体
		std::shared_ptr<aetherClass::Sphere> _pCollider;	//コライダー
		bool _talkflag;
		int _enemyAreaNo;		//エリアナンバー
	};

	FieldEnemy();
	~FieldEnemy();

	bool mInitialize(eMusical,aetherClass::ViewCamera*,std::string dataPath);
	void mUpdate(std::string name);
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void mFinalize();	//開放処理
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

	std::shared_ptr<aetherClass::Texture>& mGetIcon();
	void mRegisterIcon(std::string);
	eMusical mGetType();

	void mIsTalking(const bool);
private:
	bool mInitializeEnemy(eMusical,aetherClass::ViewCamera*);	//敵の初期化
	void mInitializeEnemyColider(aetherClass::ViewCamera*);	//コライダーの初期化

	CharaEntity m_charaEntity;
	Property m_property;
	std::vector<std::string> m_messagePath;
	aetherClass::Transform m_initTransform;
	std::string m_cannotMessagePath;
	std::string m_dataPath;
	eMusical m_type;
	std::shared_ptr<aetherClass::Texture> m_pIcon;
	bool m_isTalking;
	int m_animationCount;
	std::string m_prevAnimationName;
};
#endif