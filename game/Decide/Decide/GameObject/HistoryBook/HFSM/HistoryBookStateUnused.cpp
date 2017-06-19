/**
* 歴史書の状態の基底クラスの実装.
*/
#include"stdafx.h"
#include "GameObject\HistoryBook\HistoryBook.h"
#include "HistoryBookStateUnused.h"

/**
* 状態に切り替えたとき呼ばれる.
*/
void HistoryBookStateUnused::Entry()
{
	_HistoryBook->SetEnable(false);
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

}