/**
* �p�����[�^�\���N���X�̎���.
*/
#include"stdafx.h"
#include"ParameterRender.h"

/**
* ������.
*/
void ParameterRender::Awake()
{
	//�p�����[�^��������.
	_ParamNameText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ParamNameText", 9);
	_ParamNameText->Initialize(L"", 40.0f);
	_ParamNameText->SetFormat(fbText::TextFormatE::LEFT);
	_ParamNameText->transform->SetParent(transform);
	_ParamNameText->transform->SetLocalPosition(Vector3(-170.0f, 0.0f, 0.0f));

	//�A�C�R���摜������.
	_IconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("IconImage", 9);
	_IconImage->transform->SetParent(_ParamNameText->transform);
	_IconImage->transform->SetLocalPosition(Vector3(-20.0f, 12.0f, 0.0f));

	//�p�����[�^������.
	_ParamText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ParamText", 9);
	_ParamText->Initialize(L"", 40.0f);
	_ParamText->SetFormat(fbText::TextFormatE::RIGHT);
	_ParamText->transform->SetParent(transform);
	_ParamText->transform->SetLocalPosition(Vector3(200.0f, 0.0f, 0.0f));
}

/**
* �X�V.
*/
void ParameterRender::Update()
{
	_ParamNameText->SetString(_ParamName);
	char param[100] = { "" };
	sprintf(param, "%d", (*_Param));
	if (_MaxParam)
	{
		strcat(param, " / ");
		char maxParam[100] = { "" };
		sprintf(maxParam, "%d", (*_MaxParam));
		strcat(param, maxParam);
	}
	_ParamText->SetString(param);
}