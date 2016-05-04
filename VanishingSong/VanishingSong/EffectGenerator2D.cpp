#include "EffectGenerator2D.h"
#include"GameClock.h"

using namespace aetherClass;
EffectGenerator2D::EffectGenerator2D()
{
}


EffectGenerator2D::~EffectGenerator2D()
{
	mFinalize();
}

bool EffectGenerator2D::mInitialize(){
	m_pRect = std::make_shared<Rectangle2D>();
	m_pRect->Initialize();

	m_pRect->property._transform._scale = 50;


	return true;
}


void EffectGenerator2D::mLoadEffect(int max, std::string name, std::string folderpath){
	//���߉摜�Ȃ̂ŕK��PNG�@�ԍ���AE���f���o�������l���̂܂܂ɓǂݍ���
	for (int i = 0; i <= max; ++i){
		m_textureList[name].push_back(std::make_shared<Texture>());
		if (i < 10){
			m_textureList[name].rbegin()->get()->Load(folderpath + "0" + std::to_string(i) + ".png");
		}
		else
			m_textureList[name].rbegin()->get()->Load(folderpath + std::to_string(i) + ".png");
	}
}

void EffectGenerator2D::mFinalize(){
	if (m_pRect){
		m_pRect->Finalize();
		m_pRect.reset();
	}
	m_textureList.clear();
	m_effectList.clear();
}

void EffectGenerator2D::mPlay(std::string key, std::string instanceName, Transform transform){
	//���Ȃ���΃��^�[��
	if (m_textureList.find(key) == m_textureList.end())return;

	////�ǉ��ς݂Ȃ�΃��^�[��
	//if (m_effectList.find(key) != m_effectList.end())return;

	//�o�^����Ă��閼�O����T��
	m_pRect->SetTexture(m_textureList[key].begin()->get());
	EffectInfo info;
	info._transform = transform;
	info._iterator = m_textureList[key].begin();
	info._Effectname = key;

	m_effectList.insert(std::make_pair(instanceName, info));
}

void EffectGenerator2D::mRender(ShaderBase* shader){

	//���X�g���Ȃ���Ε`��͍s��Ȃ�
	if (m_effectList.empty())return;

	for (auto itr : m_effectList){
		m_pRect->property._transform = m_effectList[itr.first]._transform;
		m_pRect->SetTexture(itr.second._iterator->get());
		m_pRect->Render(shader);
	}

}

void EffectGenerator2D::mUpdate(float timescale){
	const float changeFrame = timescale * 1 / 60;
	static float durringTime = 0;

	//���X�g���Ȃ���΍Đ��͍s��Ȃ�
	if (m_effectList.empty())return;

	//�ǉ�����Ă��郊�X�g�����ɃC�e���[�^��i�߂�
	//��Ɏ��ԂƂ��^�C�~���O�Ƃ��̌v�Z�H
	if (!(durringTime >= GameClock::GetDeltaTime())){
		durringTime += changeFrame;
		return;
	}
	else{
		durringTime = 0;
	}

	auto l_eraser = std::vector<std::string>();
	for (auto itr : m_effectList){

		//�ꖇ��̉摜��������Ȃ��ꍇ�͍폜���鏀��������
		if (m_effectList.find(itr.first)->second._iterator == m_textureList[itr.second._Effectname].end() - 1){
			l_eraser.push_back(itr.first);
		}
		else
			m_effectList.find(itr.first)->second._iterator++;
	}

	//�폜�̎��s
	for (auto str : l_eraser){
		m_effectList.erase(str);
	}
}

aetherClass::Transform& EffectGenerator2D::mGetTransform(std::string key){
	Transform transform;
	if (m_effectList.find(key) == m_effectList.end())return transform;
	return m_effectList[key]._transform;
}
