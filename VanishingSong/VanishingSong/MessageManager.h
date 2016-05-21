#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H
#include "MessageWindow.h"
class MessageManager
{
public:
	MessageManager();
	~MessageManager();
	void mUpdate(const std::pair<int,bool>,const bool isPressButton);
	void mChangeMessage(aetherClass::Texture*);

	void mRender(aetherClass::ShaderBase*);

	bool mIsView();
private:
	MessageWindow m_message;
	bool m_isView;
	int m_counter;
};

#endif