/**
* 歴史書のページクラスの実装.
*/
#include"stdafx.h"
#include"HistoryPage.h"

#include"../HistoryBook.h"

#include"HFSM\HistoryPageStatePutIn.h"
#include"HFSM\HistoryPageStatePutOut.h"
#include"HFSM\HistoryPageStateTurn.h"
#include"HFSM\HistoryPageStateClose.h"
#include"HFSM\HistoryPageStateTakeOff.h"

/**
* コンストラクタ後の初期化.
*/
void HistoryPage::Awake()
{
	_Model = AddComponent<SkinModel>();

	SkinModelData* modelData = new SkinModelData();
	modelData->CloneModelData(SkinModelManager::LoadModel("Page.x"));

	_Material = modelData->FindMaterial("HuntingPage.png");

	_Model->SetModelData(modelData);
	_Model->SetModelEffect(ModelEffectE::CAST_SHADOW, false);

	transform->SetScale(Vector3(2.0f, 2.0f, 2.0f));
	Quaternion Rot;
	Rot.SetEuler(Vector3(-180.0f, 0.0f, 0.0f));
	transform->SetRotation(Rot);
	transform->SetPosition(Vector3(0.0f, 0.0f, 0.2f));
	transform->UpdateWolrdMatrix();

	D3DXMatrixIdentity(&_RotationMatrix);

	//状態リストの初期化.
	InitState();

}

/**
* 初期化.
*/
void HistoryPage::Start(ChipID chipID, LocationCodeE code)
{
	SkinModel* model = _HistoryBook->GetComponent<SkinModel>();
	LPD3DXFRAME frame = model->GetFrameRoot();
	D3DXFRAME_DERIVED* derived = (D3DXFRAME_DERIVED*)D3DXFrameFind(frame, "bone10_branch");
	_ParentMatrix = &derived->CombinedTransformationMatrix;

	LPD3DXFRAME pageFrame = _Model->GetFrameRoot();
	D3DXFRAME_DERIVED* pageDerived = (D3DXFRAME_DERIVED*)D3DXFrameFind(pageFrame, "bone2");
	pageDerived->RotationMatrix = &_RotationMatrix;

	TEXTURE* texture = LOADTEXTURE((char*)ChipFileName[(int)chipID].c_str());

	_Material->SetTexture(Material::TextureHandleE::DiffuseMap, texture->pTexture);

	_NowLocatuion = code;
}

/**
* 更新.
*/
void HistoryPage::Update()
{
	_StateList[_NowState]->Update();
}

/**
* 遅めの更新.
*/
void HistoryPage::LateUpdate()
{
	transform->UpdateWolrdMatrix();
	D3DXMATRIX world = transform->GetWorldMatrix();
	D3DXMatrixMultiply(&world, &world, _ParentMatrix);
	transform->SetWorldMatrix(world);
}

/**
* 状態の変化.
*/
void HistoryPage::ChangeState(StateCodeE state)
{
	if (_NowState != (int)StateCodeE::Invalid)
	{
		_StateList[_NowState]->Exit();
	}

	_NowState = (int)state;

	_StateList[_NowState]->Entry();
}

/**
* 状態リストの初期化.
*/
void HistoryPage::InitState()
{

	//挟む状態.
	_StateList.push_back(unique_ptr<HistoryPageStatePutIn>(new HistoryPageStatePutIn(this)));
	//抜く状態.
	_StateList.push_back(unique_ptr<HistoryPageStatePutOut>(new HistoryPageStatePutOut(this)));
	//捲る状態.
	_StateList.push_back(unique_ptr<HistoryPageStateTurn>(new HistoryPageStateTurn(this)));
	//本を切り離す状態.
	_StateList.push_back(unique_ptr<HistoryPageStateTakeOff>(new HistoryPageStateTakeOff(this)));
	//閉じる状態.
	_StateList.push_back(unique_ptr<HistoryPageStateClose>(new HistoryPageStateClose(this)));
	

	//初期値は挟む.
	ChangeState(StateCodeE::PutIn);

}
