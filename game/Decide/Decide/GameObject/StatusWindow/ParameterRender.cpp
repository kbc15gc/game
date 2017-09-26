/**
* パラメータ表示クラスの実装.
*/
#include"stdafx.h"
#include"ParameterRender.h"

/**
* 初期化.
*/
void ParameterRender::Awake()
{
	//パラメータ名初期化.
	_ParamNameText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ParamNameText", 9);
	_ParamNameText->Initialize(L"", 40.0f);
	_ParamNameText->SetAnchor(fbText::TextAnchorE::MiddleLeft);
	_ParamNameText->transform->SetParent(transform);
	_ParamNameText->transform->SetLocalPosition(Vector3(-170.0f, 0.0f, 0.0f));

	//アイコン画像初期化.
	_IconImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("IconImage", 9);
	_IconImage->transform->SetParent(_ParamNameText->transform);
	_IconImage->transform->SetLocalPosition(Vector3(-20.0f, 12.0f, 0.0f));

	//パラメータ初期化.
	_ParamText = INSTANCE(GameObjectManager)->AddNew<TextObject>("ParamText", 9);
	_ParamText->Initialize(L"", 40.0f);
	_ParamText->SetAnchor(fbText::TextAnchorE::MiddleRight);
	_ParamText->transform->SetParent(transform);
	_ParamText->transform->SetLocalPosition(Vector3(200.0f, 0.0f, 0.0f));
}

/**
* 更新.
*/
void ParameterRender::Update()
{
	_ParamNameText->SetText(_ParamName);
	char param[100] = { "" };
	sprintf(param, "%d", (*_Param));
	if (_MaxParam)
	{
		strcat(param, " / ");
		char maxParam[100] = { "" };
		sprintf(maxParam, "%d", (*_MaxParam));
		strcat(param, maxParam);
	}
	_ParamText->SetText(param);
}
