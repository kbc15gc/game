#pragma once
//オブジェクトのカリング処理のインターフェースクラス。

class AABB;

/*!
*@brief	CPUでの早期カリング処理のインターフェースクラス。
*@details
* Strategyパターンを使用して設計されています。
*/
class IObjectCulling {
public:
	IObjectCulling() {}
	virtual ~IObjectCulling() {}

	//カリングアルゴリズムを実行。
	virtual void Execute(const AABB& aabb,const D3DXMATRIX& rotation, const D3DXMATRIX& view, const D3DXMATRIX& proj) = 0;
	//カリングされているか判定。
	bool IsCulling() const
	{
		return _isCulling;
	}
protected:
	//カリングフラグを設定。
	void SetCullingFlag(bool flag)
	{
		_isCulling = flag;
	}
private:
	//カリングされているかのフラグ。
	bool	_isCulling = false;
};