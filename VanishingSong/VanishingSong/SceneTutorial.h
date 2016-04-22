#ifndef SCENETUTORIAL_H
#define SCENETUTORIAL_H
#include <GameScene.h>
class SceneTutorial:
	public aetherClass::GameScene
{
public:
	SceneTutorial();
	~SceneTutorial();

	//初期化
	bool Initialize()override;
	void Finalize()override;

	//1フレームごとの更新処理
	bool Updater()override;

	//1フレームごとの描画処理
	void Render()override;

	//1フレームごとのui描画処理
	void UIRender()override;

	//トランジション効果を設定
	bool TransitionIn()override;
	bool TransitionOut()override;

public:
	static const std::string Name;
};

#endif