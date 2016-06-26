#pragma once

#include<Rectangle2D.h>
#include<GameScene.h>
#include<GameSound.h>

class SceneLogoView : public aetherClass::GameScene
{
	enum eState{
		eAether, eZemicracy, eFin
	};
public:
	static const std::string Name;

public:
	SceneLogoView();
	~SceneLogoView();

	//������
	bool Initialize()override;
	void Finalize()override;

	//1�t���[�����Ƃ̍X�V����
	bool Updater()override;

	//1�t���[�����Ƃ̕`�揈��
	void Render()override;

	//1�t���[�����Ƃ�ui�`�揈��
	void UIRender()override;

	//�g�����W�V�������ʂ�ݒ�
	bool TransitionIn()override;
	bool TransitionOut()override;

private:
	bool mChangeTexture(int);
	void mLoadingTexture(std::string, int);


	float m_FrameTime;

	int m_MaxImage;
	int m_State;
	int m_PrevState;
	bool m_isEndTransition;
	bool m_triggerOnFade;

	std::unordered_map<int,std::shared_ptr<aetherClass::Texture>>m_pTextureList;
	std::shared_ptr<aetherClass::SpriteBase>m_pSprite;
	std::shared_ptr<aetherClass::GameSound>m_pSoundDevice;
};

