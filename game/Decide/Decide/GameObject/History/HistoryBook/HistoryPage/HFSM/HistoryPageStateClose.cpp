/**
* 歴史書のページの閉じる状態クラスの実装.
*/
#include"stdafx.h"
#include"HistoryPageStateClose.h"

#include"..\HistoryPage.h"

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryPageStateClose::Entry()
{
	_BefAngle = _HistoryPage->GetAngle();

	_MoveSpeed = 1.0f;
	_Angle = 0.0f;

	_LerpRate = 0.0f;
}

/**
* 更新.
*/
void HistoryPageStateClose::Update()
{
	_LerpRate += _MoveSpeed * Time::DeltaTime();
	_LerpRate = min(1.0f, _LerpRate);

	float angle = _LerpRate * _Angle + (1.0f - _LerpRate) * _BefAngle;

	_HistoryPage->Rotation(angle);
}

/**
* 他の状態に変わるとき呼ばれる.
*/
void HistoryPageStateClose::Exit()
{
}
