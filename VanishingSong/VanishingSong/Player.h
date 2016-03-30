#ifndef _PLAYER_H
#define _PLAYER_H

#include "GearFrame.h"
#include "CharaStatus.h"
#include <ShaderBase.h>
#include <unordered_map>
class ActionCommand;
class Player
{
public:
	Player();
	~Player();
	bool mInitialize(aetherClass::ViewCamera*);
	void mUpdate(const float timeScale);
	void mRender(aetherClass::ShaderBase*);
	void mAction(std::shared_ptr<ActionCommand>);

private:

	/*
		�p�[�c�쐬�p�֐�
		�������FFBX���f���̃p�X
		�������F�p�[�c�̎��
		��O�����F���e����J�����̃|�C���^
	*/
	std::shared_ptr<Gear> mSetUpGear(std::string path, Gear::eType,aetherClass::ViewCamera*);

	/*
	�e�q�֌W���쐬���邽�߂̊֐�
	�������F�e�̃|�C���^
	�������F�q���ɂ���|�C���^
	*/
	void mCreateRelationship(std::shared_ptr<Gear> parentGear, std::shared_ptr<Gear> child);

	/*
		�����I�ɕ`�悷�邽�߂̂���
		�������F�`�悷��p�[�c
		�������F�V�F�[�_�[�̃|�C���^
	*/
	void mGearRender(std::shared_ptr<Gear> gear,aetherClass::ShaderBase*);

	
private:
	std::shared_ptr<GearFrame> m_pGearFrame;
	std::unordered_map<eActionType, int> m_actionHash;
	std::shared_ptr<ActionCommand> m_pAction;
	CharaStatus m_status;
};

#endif