/*
	ギアに関する関数をまとめたクラス
*/
#ifndef _CHARAENTITY_H
#define _CHARAENTITY_H

#include <memory>
#include "GearFrame.h"
#include "Const.h"
#include "Animation.h"
#include <WorldReader.h>
class CharaEntity
{
public:
	CharaEntity();
	~CharaEntity();

	/*
		ギアの初期化用
	*/
	 std::shared_ptr<Gear> mSetUpGear(std::string path, Gear::eType, aetherClass::ViewCamera*);

	 /*
		親子関係構築用
		内部はただポインタの代入をしているだけ
	 */
	void mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child);

	/*
		ギアを持つオブジェクトのギアの描画用
		デバッグモード時はコライダーも描画
		仕組みは引数のギアに子供がいれば再帰ループをする
	
	*/
	void mGearRender(std::shared_ptr<Gear> gear, aetherClass::ShaderBase*, aetherClass::ShaderBase*);

	/*
		ギアを持つオブジェクトの移動用
		仕組みはmGearRenderと一緒
	*/
	void mGearMove(std::shared_ptr<Gear> gear, const aetherClass::Vector3 move);

	/*
		最初に渡されたパーツを中心に公転する
	*/
	void mGearRotation(std::shared_ptr<Gear> top,std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation);

	/*
		指定パーツのみ回転
		第三引数で指定したパーツとその子供は回転をしない
	*/
	void mGearPartsRotation(std::shared_ptr<Gear> top,std::shared_ptr<Gear> gear,Gear::eType notRotaionType, const aetherClass::Vector3 rotation);

	/*
		連想配列に登録用
	*/
	void mRegisterParts(std::unordered_map<Gear::eType, std::shared_ptr<Gear>>&, Gear::eType, std::shared_ptr<Gear>&);

	/*
		値の設定用
		親と、最上位パーツの差を求め、保存する
	*/
	void mSetLoadGearValue(std::shared_ptr<Gear>& top, std::shared_ptr<Gear>& gear, ObjectInfo*);


	/*
	アニメーションの値セット用
	*/
	bool mLoadAnimation(std::vector<Animation>&animationVector,std::string startState, std::string endState);

	aetherClass::Transform mGetTransformInterpolation(aetherClass::Transform, aetherClass::Transform, const int allFrame, const int nowFrame);

private:
	Gear::eType mSetPartsValue(std::string, aetherClass::Transform* input, aetherClass::Transform value);
};


#endif