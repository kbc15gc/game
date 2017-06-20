/**
* 歴史書の待機状態クラスの実装.
*/
#include"stdafx.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "HistoryBookStateIdol.h"

/**
* コンストラクタ.
*/
HistoryBookStateIdol::HistoryBookStateIdol(HistoryBook * historybook) :
	IHistoryBookState(historybook)
{
	_GameCamera = (GameCamera*)INSTANCE(GameObjectManager)->FindObject("GameCamera");
}

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryBookStateIdol::Entry()
{
	if (_HistoryBook->GetIsOpenOrClose())
	{
		_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::OpenIdol, 0.1f);
	}
	else
	{
		_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::CloseIdol, 0.1f);
	}
}

/**
* 更新.
*/
void HistoryBookStateIdol::Update()
{

}

/**
* 他の状態に変わるとき呼ばれる.
*/
void HistoryBookStateIdol::Exit()
{

}