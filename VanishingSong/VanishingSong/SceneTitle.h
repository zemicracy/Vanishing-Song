
#ifndef _SCENETITLE_H
#define _SCENETITLE_H
#include <memory>
#include <array>
#include <GameScene.h>
#include <ShaderBase.h>
#include <ViewCamera.h>
#include <Texture.h>
#include <SpriteBase.h>
#include <Skybox.h>
#include "GameManager.h"
#include"GameSound.h"
#include "FieldArea.h"
#include "Config.h"
class SceneTitle :
	public aetherClass::GameScene
{
	// �񋓌^�萔
	enum eNextMode{
		eNull = 0,
		eStart,
		eLoad,
		eCredit,
		eExit
	};

	// �J�[�\���p
	struct ModeSelect{
		ModeSelect(){

		}

		ModeSelect(const int null){
			_cursorY = 0.0f;
			_modeNumber = null;
		}
		float _cursorY;
		int _modeNumber;
	};

	// �V�[���J�ڗp
	struct SceneInfo{
		std::string _nextSceneName;
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
	SceneInfo mGetGameMode(const int index);
	void mChangeSelect(const bool isUp, const bool isDown);
	void mCursorState(const bool isStart);
	bool mMenuSelectState(const bool isReturn, const  std::pair<bool, bool>);
private:
	std::unique_ptr<aetherClass::SpriteBase> m_pLogo;
	std::unique_ptr<aetherClass::SpriteBase> m_pMenu;
	std::unique_ptr<aetherClass::SpriteBase> m_pCursor;
	std::shared_ptr<aetherClass::Texture> m_pLogoTexture;
	std::shared_ptr<aetherClass::Texture> m_pMenuTexture;
	std::shared_ptr<aetherClass::Texture> m_pPushTexture;

	std::shared_ptr<aetherClass::FbxModel> m_bluePlayer;
	std::shared_ptr<RhythmManager> m_pRhythmManager;
	std::shared_ptr<aetherClass::GameSound> m_bgm;
	aetherClass::GameSound m_returnSE;
	aetherClass::GameSound m_selectSE;
	std::array<ModeSelect, 4> m_cursorArray;
	std::unique_ptr<aetherClass::Skybox> m_pSkybox;
	FieldArea m_field;
	bool m_pushState;
	int m_nowSelectMode;
	bool m_alphaState;
	int m_nowCursor;
	bool m_isVisibleSaveData;

	aetherClass::ViewCamera m_view;

	Config m_config;
};

#endif