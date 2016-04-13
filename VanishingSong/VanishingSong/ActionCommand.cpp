#include "ActionCommand.h"
#include <WorldReader.h>
using namespace aetherClass;
ActionCommand::ActionCommand(eCommandType type){
	m_type = type;
}
ActionCommand::~ActionCommand(){
	if (m_pSprite)
	{
		m_pSprite->Finalize();
		m_pSprite.reset();
	}
	m_type = eCommandType::eNull;
}

eCommandType ActionCommand::mGetType(){
	return m_type;
}


aetherClass::SpriteBase::Property& ActionCommand::mGetProperty(){
	return m_pSprite->property;
}

void ActionCommand::mCreate(){
	m_pSprite = std::make_shared<aetherClass::Rectangle2D>();
	m_pSprite->Initialize();
}

void ActionCommand::mRender(aetherClass::ShaderBase *shader){
	m_pSprite->Render(shader);
}
void ActionCommand::mSetTexture(aetherClass::Texture *texture){
	m_pSprite->SetTexture(texture);
}


CharaEntity ActionCommand::mGetCharaEntity(){
	return m_entity;
}


bool ActionCommand::LoadAnimation(std::string startState, std::string endState){
	WorldReader read;
	bool result;
	result = read.Load(startState);
	if (!result)
	{
		return false;
	}
	Animation animation;
	for (auto index : read.GetInputWorldInfo()._object)
	{
		animation._name =SetPartsValue(index->_name, &animation._start, index->_transform);
		m_animation.push_back(animation);
	}
	read.UnLoad();

	result = read.Load(endState);
	if (!result)
	{
		return false;
	}

	for (auto index : read.GetInputWorldInfo()._object)
	{
		for (auto& endIndex : m_animation)
		{
			Gear::eType type;
			type = SetPartsValue(index->_name, &animation._end, index->_transform);

			if (endIndex._name == type)
			{
				endIndex._end = animation._end;
			}
		}
	}

	read.UnLoad();

	return true;
}

std::vector<ActionCommand::Animation>& ActionCommand::GetAnimationValue(){
	return m_animation;
}



Gear::eType ActionCommand::SetPartsValue(std::string partsName, Transform* input, Transform value){
	/*	体	*/
	if (partsName == "Body"){

		*input = value;
		return Gear::eType::eBody;
	}
	else
	if (partsName == "Waist"){
		*input = value;
		return Gear::eType::eWaist;
	}
	else
	/*	左上半身*/
	if (partsName == "LeftUpperArm"){
		*input = value;

		return Gear::eType::eLeftUpperArm;
	}
	else
	if (partsName == "LeftLowerArm"){
		*input = value;

		return Gear::eType::eLeftLowerArm;
	}
	else
	if (partsName == "LeftHand"){
		*input = value;

		return Gear::eType::eLeftHand;
	}
	else
	/*	右上半身	*/
	if (partsName == "RightUpperArm"){
		*input = value;
		return Gear::eType::eRightUpperArm;
	}
	else
	if (partsName == "RightLowerArm"){
		*input = value;
		return Gear::eType::eRightLowerArm;
	}
	else
	if (partsName == "RightHand"){
		*input = value;
		return Gear::eType::eRightHand;
	}
	else
	/*	右足	*/
	if (partsName == "RightUpperLeg"){
		*input = value;
		return Gear::eType::eRightUpperLeg;
	}
	else
	if (partsName == "RightLowerLeg"){
		*input = value;
		return Gear::eType::eRightLowerLeg;
	}
	else
	/*	左足	*/
	if (partsName == "LeftUpperLeg"){
		*input = value;
		return Gear::eType::eLeftUpperLeg;
	}
	else
	if (partsName == "LeftLowerLeg"){
		*input = value;
		return Gear::eType::eLeftLowerLeg;
	}
	else{
		*input = value;
		return Gear::eType::eNull;
	}
	return Gear::eType::eNull;
}


void ActionCommand::CallCount(const int count){
	m_callCount = count;
}
int ActionCommand::CallCount()const{
	return m_callCount;
}