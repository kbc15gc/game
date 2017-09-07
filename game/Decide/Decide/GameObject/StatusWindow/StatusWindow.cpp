/**
* �X�e�[�^�X��ʃN���X�̎���.
*/
#include"stdafx.h"
#include"StatusWindow.h"

#include"GameObject\Inventory\Inventory.h"

/**
* ������.
*/
void StatusWindow::Start()
{
	//�v���C���[�N���X���擾.
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	//���W�𒆐S�ɐݒ�.
	transform->SetPosition(Vector3(g_WindowSize.x / 2, g_WindowSize.y / 2, 0.0f));

	//�E�B���h�E�\���R���|�[�l���g��ǉ�.
	_WindowSprite = AddComponent<Sprite>();
	//�e�N�X�`����ݒ�.
	_WindowSprite->SetTexture(LOADTEXTURE("UI/Panel 5.png"));
	Vector2 size = _WindowSprite->GetSize();
	_WindowSprite->SetSize(size * 1.3f);

	int ParamCount = 7;
	for (int i = 0; i < ParamCount; i++)
	{
		ParameterRender* pr = INSTANCE(GameObjectManager)->AddNew<ParameterRender>("ParamParameterRender", 9);
		pr->transform->SetParent(transform);
		pr->transform->SetLocalPosition(Vector3(-280.0f, -230.0f + (i * 40.0f), 0.0f));
		_ParameterRenderList.push_back(pr);
	}
	_ParameterRenderList[0]->SetParam("LV", _Player->GetParamPt(CharacterParameter::Param::LV));
	_ParameterRenderList[1]->SetParam("EXP", _Player->GetParamPt(CharacterParameter::Param::EXP));
	_ParameterRenderList[2]->SetParam("HP", _Player->GetParamPt(CharacterParameter::Param::HP), _Player->GetParamPt(CharacterParameter::Param::MAXHP));
	_ParameterRenderList[3]->SetParam("MP", _Player->GetParamPt(CharacterParameter::Param::MP), _Player->GetParamPt(CharacterParameter::Param::MAXMP));
	_ParameterRenderList[4]->SetParam("ATK", _Player->GetParamPt(CharacterParameter::Param::ATK));
	_ParameterRenderList[5]->SetParam("DEF", _Player->GetParamPt(CharacterParameter::Param::DEF));
	_ParameterRenderList[6]->SetParam("MONEY", INSTANCE(Inventory)->GetPlayerMoneyPt());

	//�n�߂͔�\��.
	this->SetActive(false, true);
}

/**
* �X�V.
*/
void StatusWindow::Update()
{
}
