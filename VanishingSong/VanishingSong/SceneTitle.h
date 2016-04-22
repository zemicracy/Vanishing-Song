
#ifndef _SCENETITLE_H
#define _SCENETITLE_H
#include <memory>
#include <array>
#include <GameScene.h>
#include <ShaderBase.h>
#include <ViewCamera.h>
#include <Texture.h>
#include <SpriteBase.h>
#include "GameManager.h"
class SceneTitle :
	public aetherClass::GameScene
{
	// �񋓌^�萔
	enum eNextMode{
		eNull = 0,
		eSurvival,
		eBattle,
		ePractice,
		eTutorial,
		eExit
	};
	struct ModeSelect{
		float _cursorY;
		int _modeNumber;
	};

public:
	SceneTitle();

	~SceneTitle();

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

public:
	static const std::string Name;
private:
	std::string mGetGameMode(const int index);
	void mChangeSelect(aetherClass::Vector2);
	void mClickState();
	bool mMenuSelectState();
private:
	std::unique_ptr<aetherClass::SpriteBase> m_pLogo;
	std::shared_ptr<aetherClass::ShaderBase> m_pTextureShader;
	std::shared_ptr<aetherClass::ShaderBase> m_pColorShader;
	std::shared_ptr<aetherClass::Texture> m_pLogoTexture;
	std::shared_ptr<aetherClass::Texture> m_pMenuTexture;
	std::shared_ptr<aetherClass::Texture> m_pPushTexture;
	std::unique_ptr<aetherClass::SpriteBase> m_pMenu;
	std::unique_ptr<aetherClass::SpriteBase> m_pCursor;
	std::array<ModeSelect, 5> m_cursorArray;
	bool m_pushState;
	int m_nowSelectMode;
	bool m_alphaState;
};

#endif