/**
* 歴史書のページの抜く状態クラスの実装.
*/
#include"stdafx.h"
#include"HistoryPageStatePutOut.h"

#include"..\HistoryPage.h"

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryPageStatePutOut::Entry()
{
	_BefAngle = _HistoryPage->GetAngle();

	_MoveSpeed = 2.0f;
	_Angle = 0.0f;

	_LerpRate = 0.0f;
}

/**
* 更新.
*/
void HistoryPageStatePutOut::Update()
{
	_LerpRate += _MoveSpeed * Time::DeltaTime();
	_LerpRate = min(1.0f, _LerpRate);

	float angle = _LerpRate *  _Angle + (1.0f - _LerpRate) *_BefAngle;

	_HistoryPage->Rotation(angle);
	if (_LerpRate >= 1.0f)
	{
		_HistoryPage->ChangeState(HistoryPage::StateCodeE::TakeOff);
	}
}

/**
* 他の状態に変わるとき呼ばれる.
*/
void HistoryPageStatePutOut::Exit()
{
}
