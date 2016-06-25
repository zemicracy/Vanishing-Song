
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

	struct  TitlePlayer
	{
		std::shared_ptr<aetherClass::FbxModel> _model;
		std::string _animationName;
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
	void mSetPlayer(eMusical);
private:
	std::shared_ptr<aetherClass::SpriteBase> m_pLogo;
	std::shared_ptr<aetherClass::SpriteBase> m_pMenu;
	std::shared_ptr<aetherClass::SpriteBase> m_pCursor;
	std::shared_ptr<aetherClass::Texture> m_pLogoTexture;
	std::shared_ptr<aetherClass::Texture> m_pMenuTexture;
	std::shared_ptr<aetherClass::Texture> m_pPushTexture;
	
	std::unordered_map<eMusical, TitlePlayer> m_players;
	std::shared_ptr<RhythmManager> m_pRhythmManager;
	std::shared_ptr<aetherClass::GameSound> m_pBGM;
	std::shared_ptr<aetherClass::GameSound> m_returnSE;
	std::shared_ptr<aetherClass::GameSound> m_selectSE;
	std::array<ModeSelect, 4> m_cursorArray;
	std::shared_ptr<FieldArea>  m_pField;
	FieldNote m_fieldNote;

	bool m_pushState;
	int m_nowSelectMode;
	bool m_alphaState;
	int m_nowCursor;
	bool m_isVisibleSaveData;

	aetherClass::ViewCamera m_view;

	Config m_config;
};

#endif