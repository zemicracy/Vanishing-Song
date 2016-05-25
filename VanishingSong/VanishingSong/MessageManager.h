#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H
#include "MessageWindow.h"
#include "FieldEnemyManager.h"
class MessageManager
{
	enum class eState{
		eNext,
		eSelect,
		eEnd,
		eNull
	};
	enum class eSelectType{
		eYes,
		eNo,
		eNull
	};
public:
	MessageManager(std::shared_ptr<FieldEnemyManager>&);
	~MessageManager();
	void mUpdate(const std::pair<int,bool>,const bool isPressButton,const bool isCursor,aetherClass::Vector3 position);
	
	void mChangeMessage(aetherClass::Texture*);
	bool mGetIsChangeScene()const;
	void mRender(aetherClass::ShaderBase*, aetherClass::ShaderBase*);
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
	bool m_select;
	bool m_isChangeScene;
};

#endif