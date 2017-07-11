/**
* 歴史書が開く状態クラスの実装.
*/
#include "stdafx.h"
#include"..\HistoryBook.h"
#include "HistoryBookStateOpen.h"

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryBookStateOpen::Entry()
{
	_HistoryBook->PlayAnimation(HistoryBook::AnimationCodeE::Open, 0.2f, 1);

}

/**
* 更新.
*/
void HistoryBookStateOpen::Update()
{
	//本を開ききったら本を開いている状態で固定。
	if (!_HistoryBook->GetIsPlay())
	{
		//開ききるアニメーションの再生が終わったら開いた状態に変更。
		_HistoryBook->ChangeState(HistoryBook::StateCodeE::Idol);
	}
}

/**
* 他の状態に変わるとき呼ばれる.
*/
void HistoryBookStateOpen::Exit()
{
	_HistoryBook->OpenPage();
}