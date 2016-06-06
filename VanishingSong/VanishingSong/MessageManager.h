#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H
#include "MessageWindow.h"
#include "FieldEnemyManager.h"
#include "Rectangle3D.h"
class MessageManager
{
	enum class eState{
		eNext,
		eSelect,
		eCannotSelect,
		eEnd,
		eNull
	};
	enum class eSelectType{
		eYes,
		eNo,
		eNull
	};
public:
	MessageManager(std::shared_ptr<FieldEnemyManager>&,aetherClass::ViewCamera*);
	~MessageManager();
	void mUpdate(const std::pair<int, bool>, const bool isPressButton, const bool isCursor, aetherClass::Vector3 position, aetherClass::Vector3 enemy,const int nowStage);
	
	void mChangeMessage(aetherClass::Texture*);
	bool mGetIsChangeScene()const;
	void m2DRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	void m3DRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
	bool mIsView();
private:
	void mSetState(const int);
	void mCursorUpdate(const bool);
private:
	std::shared_ptr<aetherClass::Rectangle2D> m_pCursor;
	MessageWindow m_message;
	bool m_isView;
	int m_counter;
	std::shared_ptr<FieldEnemyManager> m_enemy;
	std::unordered_map<eState, aetherClass::Texture> m_buttonTexture;
	eState m_state;
	std::unordered_map<eSelectType, float> m_cursorPosition;
	eSelectType m_selectType;

	std::shared_ptr<aetherClass::Rectangle3D>m_messageFlame;
	std::shared_ptr<aetherClass::Texture>m_messageFlameTexture;
	std::shared_ptr<aetherClass::Texture>m_messageFlameTexture2;
	bool m_select;
	bool m_isChangeScene;
	bool m_viewMessageFlame;
	aetherClass::ViewCamera* m_camera;
	float m_messageFlameTime;
	bool m_changeMessageFlame;
};

#endif