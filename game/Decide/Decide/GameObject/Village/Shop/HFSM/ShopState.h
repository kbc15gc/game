#pragma once
#include "stdafx.h"
#include "GameObject\Village\Shop\Shop.h"

//ショップの状態を示すクラスのインターフェース。
class IShopState
{
public:	
	//コンストラクタ。
	//[in] ショップのポインタ。
	//[in] 親のステート。
	IShopState(Shop* shop) :
		_Shop(shop),
		_Caller(Shop::ShopStateE::None){}
	
	//デストラクタ。
	virtual ~IShopState(){}
	
	//毎フレーム呼ばれる更新処理。
	virtual void Update() = 0;

	//状態に切り替えたとき呼ばれる処理。
	void Enter(Shop::ShopStateE call)
	{
		if (_Caller == Shop::ShopStateE::None)
			_Caller = call;
		EnterState();
	}

	virtual void EnterState() = 0;
	
	//状態から抜ける時に呼ばれる処理。
	virtual void Exit(Shop::ShopStateE next)
	{
		if (next == _Caller)
		{
			ExitState();
			_Caller = Shop::ShopStateE::None;
		}
		else
		{
			DiveState();
		}
	}

	//
	virtual void ExitState() = 0;

	//
	virtual void DiveState() = 0;
protected:
	//ショップへのポインタ。
	Shop* _Shop;
	//呼び出し元。
	Shop::ShopStateE _Caller;
};