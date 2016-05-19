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
	void mRender(aetherClass::ShaderBase*);
	void mUpdate(float);
	std::shared_ptr<ActionCommand> mSelectType();

	//AccessorMethod
	std::shared_ptr<ActionCommand> mGetCommand(eCommandType type);
	std::shared_ptr<ActionCommand> mGetCommandTexture(eCommandType type);

	private:

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
	template<class T>
	void mInitializer(Color color, std::string path);

	void mFinalize();
private:

	std::unordered_map<eCommandType, ActionCommandType>m_actionList;

};

