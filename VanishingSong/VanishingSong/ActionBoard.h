#pragma once

#include<array>
#include<ShaderBase.h>
#include"ActionCommand.h"
#include"GameManager.h"
class ActionBoard
{
public:
	ActionBoard();
	~ActionBoard();

	bool mInitialize();

	/*void mRender(aetherClass::ShaderBase*);
	void mUpdate(float);*/

	std::shared_ptr<ActionCommand> mSelectType();

	//AccessorMethod
	std::shared_ptr<ActionCommand> mGetCommand(eMusical type);
	std::shared_ptr<aetherClass::Texture> mGetCommandTexture(eMusical type);

	public:

	struct ActionCommandType{
		std::shared_ptr<ActionCommand> _command;
		std::shared_ptr<aetherClass::Texture> _texture;
		ActionCommandType(){
			_command = nullptr;
			_texture = nullptr;
		}
		~ActionCommandType(){
			_command.reset();
			_command = nullptr;
			_texture.reset();
			_texture = nullptr;
		}
	};

private:

	void mFinalize();
private:

	std::unordered_map<eMusical, bool>m_playerOrder;
	std::unordered_map<eMusical, ActionCommandType>m_actionList;

};

