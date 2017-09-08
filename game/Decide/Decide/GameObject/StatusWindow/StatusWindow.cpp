/**
* ステータス画面クラスの実装.
*/
#include"stdafx.h"
#include"StatusWindow.h"

#include"GameObject\Inventory\Inventory.h"
#include"ItemWindow.h"

/**
* 初期化.
*/
void StatusWindow::Start()
{
	//プレイヤークラスを取得.
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");

	//座標を中心に設定.
	transform->SetPosition(Vector3(g_WindowSize.x / 2, g_WindowSize.y / 2, 0.0f));

	//バックの背景.
	ImageObject* backWindow = INSTANCE(GameObjectManager)->AddNew<ImageObject>("StatusWindow", 9);
	backWindow->SetTexture(LOADTEXTURE("UI/Panel 5.png"));
	backWindow->SetSize(backWindow->GetSize() * 1.3f);
	backWindow->transform->SetParent(transform);

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

	ItemWindow* iw = INSTANCE(GameObjectManager)->AddNew<ItemWindow>("ItemWindow", 9);
	iw->transform->SetParent(transform);
	_ItemWindowList.push_back(iw);

	//始めは非表示.
	this->SetActive(false, true);
}

/**
* 更新.
*/
void StatusWindow::Update()
{

}
