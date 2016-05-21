#include "MessageManager.h"


MessageManager::MessageManager()
{
	m_message.mInitialize();
	m_isView = false;
	m_counter = NULL;
}


MessageManager::~MessageManager()
{
	m_message.mFinalize();
}

//
void MessageManager::mUpdate(const std::pair<int, bool> pair, const bool isPressButton){
	bool isEnd = false;
	if (pair.second)return;

	if (isPressButton){
		m_isView = true;
		if (m_counter > 2){
			isEnd = true;
		}
		m_counter += 1;
	}

	if (isEnd){
		m_isView = false;
		m_counter = NULL;
	}
}

//
void MessageManager::mChangeMessage(aetherClass::Texture* tex){
	m_message.mSetText(tex);
}

void MessageManager::mRender(aetherClass::ShaderBase* shader){
	if (!m_isView)return;
	m_message.mRender(shader);

}

bool MessageManager::mIsView(){
	return m_isView;
}