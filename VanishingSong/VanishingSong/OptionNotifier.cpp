#include"OptionNotifier.h"

OptionNotifier::OptionNotifier()
{
	m_active = false;
}


OptionNotifier::~OptionNotifier()
{
	mFinalize();
}

void OptionNotifier::mInitialize(aetherClass::ViewCamera* camera, aetherClass::Vector3& pos){
	using namespace aetherClass;
	m_pRect = std::make_shared<Rectangle3D>();
	m_pRect->Initialize();

	m_pRect->property._transform._translation = pos;
	m_pRect->property._transform._translation._y += 30;

	m_pRect->property._transform._rotation = camera->property._rotation;
	m_pRect->property._transform._scale = Vector3(4, 3, 1);
	m_pRect->property._transform._scale *= 3;

	m_pRect->SetCamera(camera);

	std::string folder = "Texture\\Battle\\";
	m_textureList.insert(std::make_pair(OrderList::eAppendOption::eBlack, std::make_shared<Texture>()));
	m_textureList[OrderList::eAppendOption::eBlack]->Load(folder + "black.png");
	m_textureList.insert(std::make_pair(OrderList::eAppendOption::eReverce, std::make_shared<Texture>()));
	m_textureList[OrderList::eAppendOption::eReverce]->Load(folder + "reverce.png");


}
void OptionNotifier::mChangeTexture(OrderList::eAppendOption state){
	if (m_textureList.find(state) == m_textureList.end())return;
	m_pRect->SetTexture(m_textureList.at(state).get());
}


void OptionNotifier::mSetActive(bool flg){
	if (flg){
		m_pRect->property._color._alpha = 1;
	}
	m_active = flg;
}

void OptionNotifier::mRender(aetherClass::ShaderBase* shader){
	if (!m_active){
		m_pRect->property._color._alpha -= 0.1;
	}
	m_pRect->Render(shader);

}
void OptionNotifier::mFinalize(){
	m_pRect->Finalize();
	m_pRect.reset();
	m_textureList.clear();
}

