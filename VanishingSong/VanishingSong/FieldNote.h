#pragma once
#include <FbxModel.h>
#include <memory>
class FieldNote
{
private:
	struct Note{
		Note(){
			_object = nullptr;
			_isUpOrDwon = false;
		}
		~Note(){
			_object = nullptr;
			_isUpOrDwon = false;
		}
		std::shared_ptr<aetherClass::FbxModel> _object;
		bool _isUpOrDwon;
	};
public:
	FieldNote();
	~FieldNote();


};

