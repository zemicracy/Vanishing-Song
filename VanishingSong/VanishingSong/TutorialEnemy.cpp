#include "TutorialEnemy.h"

using namespace aetherClass;
TutorialEnemy::TutorialEnemy()
{
	m_dataPath = "data\\Battle\\Tutorial";
	m_isEnd = false;
}


TutorialEnemy::~TutorialEnemy()
{
}

//
void TutorialEnemy::mInitalize(const bool flg){
	m_isEnd = !flg;
	if (m_isEnd)return;
}


//
void TutorialEnemy::mUpdate(){
	if (m_isEnd)return;
}

//
void TutorialEnemy::mRender(ShaderBase*){
	if (m_isEnd)return;
}

void TutorialEnemy::mUIRender(ShaderBase*){
	if (m_isEnd)return;
}

//
std::string TutorialEnemy::mGetDataPath(){
	return m_dataPath;
}

void TutorialEnemy::mIsEnd(const bool flg){
	m_isEnd = flg;
}