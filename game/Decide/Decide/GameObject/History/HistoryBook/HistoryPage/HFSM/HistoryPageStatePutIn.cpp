/**
* 歴史書のページの挟む状態クラスの実装.
*/
#include"stdafx.h"
#include"HistoryPageStatePutIn.h"

#include"..\HistoryPage.h"

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryPageStatePutIn::Entry()
{
	_LerpRate = 0.0f;
}

/**
* 更新.
*/
void HistoryPageStatePutIn::Update()
{
	_LerpRate += _MoveSpeed * Time::DeltaTime();
	
	float lerpRate = min(1.0f, _LerpRate);

	Vector3 pos = Vector3::zero;
	Vector3 movePos = _MovePosition;
	movePos.Scale(lerpRate);
	Vector3 initPos = _InitialPosition;
	initPos.Scale((1.0f - lerpRate));

	pos.Add(movePos, initPos);
	
	_HistoryPage->transform->SetPosition(pos);

	if (_LerpRate >= 1.0f)
	{
		_HistoryPage->ChangeState(HistoryPage::StateCodeE::Turn);
	}

}

/**
* 他の状態に変わるとき呼ばれる.
*/
void HistoryPageStatePutIn::Exit()
{
}
