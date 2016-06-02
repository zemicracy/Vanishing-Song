#ifndef _ENTRYSCENE_H
#define _ENTRYSCENE_H
#include <GameScene.h>
/*
	一番最初のシーン
*/
class EntryScene :
	public aetherClass::GameScene
{
public:
	EntryScene();
	~EntryScene();

	//初期化
	bool Initialize()override;

	//解放処理
	void Finalize()override;

	//1フレームごとの更新処理
	bool Updater()override;

	//1フレームごとの描画処理
	void Render()override;

	//UIの描画処理
	void UIRender()override;

	//トランジション効果
	bool TransitionIn()override;
	bool TransitionOut()override;
};

#endif