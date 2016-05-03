#include "EffectGenerator.h"
#include"GameClock.h"

using namespace aetherClass;
EffectGenerator::EffectGenerator()
{
}


EffectGenerator::~EffectGenerator()
{
	mFinalize();
}

bool EffectGenerator::mInitialize(ViewCamera* camera){
	m_pRect = std::make_shared<Rectangle3D>();
	m_pRect->Initialize();
	m_pRect->SetCamera(camera);
	m_camera = camera;

	m_pRect->property._transform._scale = 50;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	SecureZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_pRect->GetDirect3DManager()->GetDevice()->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);

	return true;
}


void EffectGenerator::mLoadEffect(int max, std::string name, std::string folderpath){
	//透過画像なので必ずPNG　番号はAEが吐き出した数値そのままに読み込む
	for (int i = 0; i <= max; ++i){
		m_textureList[name].push_back(std::make_shared<Texture>());
		if (i < 10){
			m_textureList[name].rbegin()->get()->Load(folderpath + "0" + std::to_string(i) + ".png");
		}
		else
			m_textureList[name].rbegin()->get()->Load(folderpath + std::to_string(i) + ".png");
	}
}

void EffectGenerator::mFinalize(){
	if (m_pRect){
		m_pRect->Finalize();
		m_pRect.reset();
	}
	m_textureList.clear();
	m_effectList.clear();
}

void EffectGenerator::mPlay(std::string key, std::string instanceName, Transform transform, Vector3 flg){
	//見つなければリターン
	if (m_textureList.find(key) == m_textureList.end())return;
	
	////追加済みならばリターン
	//if (m_effectList.find(key) != m_effectList.end())return;

	//登録されている名前から探す
	m_pRect->SetTexture(m_textureList[key].begin()->get());
	EffectInfo info;
	info._transform = transform;
	info._iterator = m_textureList[key].begin();
	info._flgBillboard = flg;
	info._Effectname = key;

	m_effectList.insert(std::make_pair(instanceName, info));
}

void EffectGenerator::mRender(ShaderBase* shader){

	//リストがなければ描画は行わない
	if (m_effectList.empty())return;
	m_pRect->GetDirect3DManager()->GetDeviceContext()->OMSetDepthStencilState(m_depthStencilState,1);

	for (auto itr : m_effectList){
		//billboard
		m_pRect->property._transform = m_effectList[itr.first]._transform;
		m_pRect->property._transform._rotation._x += m_camera->property._rotation._x * m_effectList[itr.first]._flgBillboard._x;
		m_pRect->property._transform._rotation._y += m_camera->property._rotation._y * m_effectList[itr.first]._flgBillboard._y;
		m_pRect->property._transform._rotation._z += m_camera->property._rotation._z * m_effectList[itr.first]._flgBillboard._z;

		m_pRect->SetTexture(itr.second._iterator->get());
		m_pRect->Render(shader);
	}
	m_pRect->GetDirect3DManager()->Change3DMode();

}

void EffectGenerator::mUpdate(float timescale){
	const float changeFrame = timescale * 1 / 60;
	static float durringTime = 0;

	//リストがなければ再生は行わない
	if (m_effectList.empty())return;

	//追加されているリストを元にイテレータを進める
	//主に時間とかタイミングとかの計算？
	if (!(durringTime >= GameClock::GetDeltaTime())){
		durringTime += changeFrame;
		return;
	}
	else{
		durringTime = 0;
	}
		
	auto l_eraser = std::vector<std::string>();
	for (auto itr : m_effectList){

		//一枚先の画像が見つからない場合は削除する準備をする
		if (m_effectList.find(itr.first)->second._iterator == m_textureList[itr.second._Effectname].end()-1){
			l_eraser.push_back(itr.first);
		}else
			m_effectList.find(itr.first)->second._iterator++;
	}

	//削除の実行
	for (auto str : l_eraser){
		m_effectList.erase(str);
	}
}

aetherClass::Transform& EffectGenerator::mGetTransform(std::string key){
	Transform transform;
	if (m_effectList.find(key) == m_effectList.end())return transform;
	return m_effectList[key]._transform;
}
