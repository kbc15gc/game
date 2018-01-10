/**
* 歴史書の状態の基底クラスの実装.
*/
#include"stdafx.h"
#include"..\HistoryBook.h"
#include"HistoryBookStateUnused.h"

#include"GameObject\Player\Player.h"

/**
* コンストラクタ.
*/
HistoryBookStateUnused::HistoryBookStateUnused(HistoryBook * historybook) :
	IHistoryBookState(historybook)
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	_PlayerCamera = (PlayerCamera*)INSTANCE(GameObjectManager)->FindObject("PlayerCamera");
}

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryBookStateUnused::Entry()
{
	_HistoryBook->PageInactive();
	_HistoryBook->SetActiveGameObject(false);
	_HistoryBook->PlayerStopDisable();
}

/**
* 更新.
*/
void HistoryBookStateUnused::Update()
{
}

/**
* 他の状態に変わるとき呼ばれる.
*/
void HistoryBookStateUnused::Exit()
{
	Camera* camera = _PlayerCamera->GetComponent<Camera>();


	//プレイヤーからカメラへのベクトル.
	Vector3 pToc = _PlayerCamera->transform->GetPosition() - _Player->transform->GetPosition();
	pToc.y = 0.0f;
	pToc.Normalize();

	_HistoryBook->transform->SetLocalPosition(_Player->transform->GetPosition() + pToc + Vector3(0.0f, 1.0f, 0.0f));

	//カメラの前方向.
	Vector3 cForward = _PlayerCamera->transform->GetForward();
	//カメラの下方向.
	Vector3 cDown = _PlayerCamera->transform->GetUp() * -1;
	cDown.Scale(0.4f);

	_HistoryBook->SetDestPos(_PlayerCamera->transform->GetPosition() + cForward + cDown);
	
	_HistoryBook->transform->SetRotation(_PlayerCamera->transform->GetRotation());
}