#pragma once
#include <FbxModel.h>
#include <memory>
#include <array>
#include <unordered_map>
#include "Const.h"
class FieldNote
{
private:
	struct Note{
		Note(){
			_object = nullptr;
			_isUpOrDwon = false;
			_callCount = NULL;
		}
		~Note(){
			if (_object){
				_object->Finalize();
				_object.reset();
				_callCount = NULL;
			}
			_object = nullptr;
			_isUpOrDwon = false;
		}
		std::shared_ptr<aetherClass::FbxModel> _object;
		bool _isUpOrDwon;
		int _callCount;
	};
public:
	FieldNote();
	~FieldNote();
	void mInitialize(eMusical, aetherClass::ViewCamera*, aetherClass::Vector3, aetherClass::Vector3);
	void mUpdate();
	void mRender(aetherClass::ShaderBase*);

private:
	aetherClass::Color mGetColor(eMusical);
private:
	std::unordered_map<eMusical,std::array<Note, 10>> m_object;
};

