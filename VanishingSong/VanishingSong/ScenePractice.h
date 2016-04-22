#ifndef _SCENEPRACTICE_H
#define _SCENEPRACTICE_H
#include <GameScene.h>
class ScenePractice :
	public aetherClass::GameScene
{
public:
	ScenePractice();
	~ScenePractice();

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