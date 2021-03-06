/**
* 歴史書のページの挟む状態クラスの実装.
*/
#include"stdafx.h"
#include"HistoryPageStatePutIn.h"

#include"..\HistoryPage.h"
#include"../../../HistoryManager.h"

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryPageStatePutIn::Entry()
{
	_LerpRate = 0.0f;

	_MoveSpeed = 2.0f;

	_MovePosition = _HistoryPage->GetInitPos();
	_InitialPosition = _MovePosition + Vector3(0, 0, 1);
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
	
	_HistoryPage->transform->SetLocalPosition(pos);

	if (_LerpRate >= 1.0f)
	{
		INSTANCE(HistoryManager)->ChangeLocation(_HistoryPage->GetNowLocation());
		_HistoryPage->ChangeState(HistoryPage::StateCodeE::Turn);
	}

}

/**
* 他の状態に変わるとき呼ばれる.
*/
void HistoryPageStatePutIn::Exit()
{
	_HistoryPage->SetRotAngle(-90.0f);
}
