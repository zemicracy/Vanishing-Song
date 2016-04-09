#ifndef _CHARAENTITY_H
#define _CHARAENTITY_H

#include <memory>
#include "GearFrame.h"
#include "Const.h"
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
		ギアを持つオブジェクトの回転用
		体全体を均一に回転する
		仕組みはmGearRenderと一緒
	*/
	void mBodyGearRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation);

	/*
		ギアを持つオブジェクトの回転用
		部分部分を回転できる
		仕組みはmGearRenderと一緒
	*/
	void mPartsGearRotation(std::shared_ptr<Gear> gear, const aetherClass::Vector3 rotation);

	aetherClass::Transform mGetTransformInterpolation(aetherClass::Transform, aetherClass::Transform, const int allFrame, const int nowFrame);
};


#endif