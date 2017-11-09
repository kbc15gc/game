/**
* 歴史書の待機状態クラスの実装.
*/
#include"stdafx.h"
#include"..\HistoryBook.h"
#include "HistoryBookStateClose.h"

#include"GameObject\Player\Player.h"

namespace
{

	/** 歴史書の出現位置. */
	const Vector3 HISTORY_POINT(0.0f, 0.5f, 0.0f);

}

/**
* コンストラクタ.
*/
HistoryBookStateClose::HistoryBookStateClose(HistoryBook * historybook) :
	IHistoryBookState(historybook)
{
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
}

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryBookStateClose::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::Close, 0.0f, 1);
}

/**
* 更新.
*/
void HistoryBookStateClose::Update()
{
	if (_HistoryBook->GetIsPlayAnim(HistoryBook::AnimationCodeE::Close) && !_HistoryBook->GetIsPlay())
	{
		_HistoryBook->ChangeState(HistoryBook::StateCodeE::Move);
	}
}

/**
* 他の状態に変わるとき呼ばれる.
*/
void HistoryBookStateClose::Exit()
{
	_PlayerFoward = _Player->transform->GetForward();
	Vector3 foward = _PlayerFoward + HISTORY_POINT;
	_HistoryBook->SetDestPos(foward + _Player->transform->GetPosition());
}