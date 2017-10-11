#pragma once
#include "Vector.h"

//AABB
//軸平行バウンディングボックスクラス。</br>
//メンバにバウンディングボックスの中心位置、</br>
//サイズ、バウンディングボックスを構成する8頂点の座標を保持しています。
class AABB {
public:
	AABB();
	//初期化。
	void Init(const Vector3& centerPosition, const Vector3& halfSize);
	//更新。
	//[in] 中心座標。
	//[in] バウンディングボックスのハーフサイズ。
	void Update(const Vector3& centerPosition, const Vector3& halfSize);
	//バウンディングボックスを構成する8頂点の座標を取得。
	//[in] 頂点番号。0～7の番号を渡してください。
	const Vector3& GetVertexPosition(unsigned int vertNo) const
	{
		//TK_ASSERT(vertNo < 8, "vertNo is invalid");
		return _VertexPosition[vertNo];
	}
private:
	void CalcVertexPosition();
private:
	Vector3	_CenterPosition = Vector3::zero;		//中心座標。
	Vector3	_HalfSize = Vector3::zero;			//ハーフサイズ。
	Vector3	_VertexPosition[8];	//バウンディングボックスを構成する8頂点の座標。
};