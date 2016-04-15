#ifndef _UTILITY_H
#define _UTILITY_H

#include <vector>
#include "GearFrame.h"
#include <Matrix4x4.h>
template<typename Type>
static void ReleaseVector(std::vector<Type>& vector)
{
	if (!vector.empty())
	{
		
		for (auto object : vector)
		{
			if (object)
			{
				delete object;
				object = nullptr;
			}
		}
	}
	vector.clear();
}

/*
	補間用関数
	第一引数：初期状態
	第二引数：最大の状態
	第三引数：全フレーム数
	第四引数：今のフレーム数
*/
template<class type>
static type gInterpolation(type first, type last, const int allFrameCount, const int nowCount){
	
	// 最大フレーム数を越えそうなら最後の状態を返す
	if (allFrameCount <= nowCount) return last;
	

	type output;
	float allFrame = static_cast<float>(allFrameCount);
	float nowFrame = static_cast<float>(nowCount);
	float coefficient = (float)(nowFrame / allFrame);
	output = (type)(first + (last - first)*coefficient);
	return output;
}

/*
	キーフレームアニメーション用
*/
static void gGearFrameAnimation(std::shared_ptr<GearFrame> gearFrame, Gear::eType type, aetherClass::Transform trans){
	static aetherClass::Matrix4x4 animation;

	switch (type)
	{
	case Gear::eType::eBody:
		animation = trans.Transmatrix();
		
		gearFrame->m_pBody->_pGear->property._transform._translation.TransformCoordNormal(animation);
		gearFrame->m_pBody->_pGear->property._transform._rotation.TransformCoordNormal(animation);
		break;
	case Gear::eType::eRightUpperArm:
		gearFrame->m_pRightUpperArm->_pGear->property._transform._translation.TransformCoordNormal(animation);
		gearFrame->m_pRightUpperArm->_pGear->property._transform._rotation.TransformCoordNormal(animation);
		break;
	case Gear::eType::eLeftUpperArm:
		gearFrame->m_pRightUpperArm->_pGear->property._transform._translation.TransformCoordNormal(animation);
		gearFrame->m_pRightUpperArm->_pGear->property._transform._rotation.TransformCoordNormal(animation);
		break;
	case Gear::eType::eRightLowerArm:
		gearFrame->m_pRightUpperArm->_pGear->property._transform._translation.TransformCoordNormal(animation);
		gearFrame->m_pRightUpperArm->_pGear->property._transform._rotation.TransformCoordNormal(animation);

		break;
	case Gear::eType::eLeftLowerArm:
		gearFrame->m_pRightUpperArm->_pGear->property._transform._translation.TransformCoordNormal(animation);
		gearFrame->m_pRightUpperArm->_pGear->property._transform._rotation.TransformCoordNormal(animation);

		break;
	case Gear::eType::eWaist:
		gearFrame->m_pRightUpperArm->_pGear->property._transform._translation.TransformCoordNormal(animation);
		gearFrame->m_pRightUpperArm->_pGear->property._transform._rotation.TransformCoordNormal(animation);

		break;
	case Gear::eType::eRightUpperLeg:
		gearFrame->m_pRightUpperArm->_pGear->property._transform._translation.TransformCoordNormal(animation);
		gearFrame->m_pRightUpperArm->_pGear->property._transform._rotation.TransformCoordNormal(animation);

		break;
	case Gear::eType::eLeftUpperLeg:
		gearFrame->m_pRightUpperArm->_pGear->property._transform._translation.TransformCoordNormal(animation);
		gearFrame->m_pRightUpperArm->_pGear->property._transform._rotation.TransformCoordNormal(animation);

		break;
	case Gear::eType::eRightLowerLeg:
		gearFrame->m_pRightLowerLeg->_pGear->property._transform = trans;

		break;
	case Gear::eType::eLeftLowerLeg:
		gearFrame->m_pLeftLowerLeg->_pGear->property._transform = trans;

		break;
	case Gear::eType::eRightHand:
		gearFrame->m_pRightHand->_pGear->property._transform = trans;

		break;
	case Gear::eType::eLeftHand:
		gearFrame->m_pLeftHand->_pGear->property._transform = trans;

		break;
	case Gear::eType::eNull:
		break;
	default:
		break;
	}
}


#endif