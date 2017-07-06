/**
* ���j���̃y�[�W�N���X�̎���.
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
* �R���X�g���N�^��̏�����.
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

	//��ԃ��X�g�̏�����.
	InitState();

}

/**
* ������.
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
* �X�V.
*/
void HistoryPage::Update()
{
	_StateList[_NowState]->Update();
}

/**
* �x�߂̍X�V.
*/
void HistoryPage::LateUpdate()
{
	transform->UpdateWolrdMatrix();
	D3DXMATRIX world = transform->GetWorldMatrix();
	D3DXMatrixMultiply(&world, &world, _ParentMatrix);
	transform->SetWorldMatrix(world);
}

/**
* ��Ԃ̕ω�.
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
* ��ԃ��X�g�̏�����.
*/
void HistoryPage::InitState()
{

	//���ޏ��.
	_StateList.push_back(unique_ptr<HistoryPageStatePutIn>(new HistoryPageStatePutIn(this)));
	//�������.
	_StateList.push_back(unique_ptr<HistoryPageStatePutOut>(new HistoryPageStatePutOut(this)));
	//������.
	_StateList.push_back(unique_ptr<HistoryPageStateTurn>(new HistoryPageStateTurn(this)));
	//�{��؂藣�����.
	_StateList.push_back(unique_ptr<HistoryPageStateTakeOff>(new HistoryPageStateTakeOff(this)));
	//������.
	_StateList.push_back(unique_ptr<HistoryPageStateClose>(new HistoryPageStateClose(this)));
	

	//�����l�͋���.
	ChangeState(StateCodeE::PutIn);

}
