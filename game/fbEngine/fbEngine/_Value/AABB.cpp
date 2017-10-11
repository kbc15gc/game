#include "fbstdafx.h"
#include "AABB.h"

AABB::AABB()
{
	memset(_VertexPosition, 0, sizeof(_VertexPosition));
}

void AABB::Init(const Vector3 & centerPosition, const Vector3 & halfSize)
{
	Update(centerPosition, halfSize);
}

void AABB::CalcVertexPosition()
{
	//8頂点の座標を計算。
	_VertexPosition[0] = _CenterPosition;
	_VertexPosition[0].x -= _HalfSize.x;
	_VertexPosition[0].y -= _HalfSize.y;
	_VertexPosition[0].z -= _HalfSize.z;

	_VertexPosition[1] = _CenterPosition;
	_VertexPosition[1].x += _HalfSize.x;
	_VertexPosition[1].y -= _HalfSize.y;
	_VertexPosition[1].z -= _HalfSize.z;

	_VertexPosition[2] = _CenterPosition;
	_VertexPosition[2].x -= _HalfSize.x;
	_VertexPosition[2].y += _HalfSize.y;
	_VertexPosition[2].z -= _HalfSize.z;

	_VertexPosition[3] = _CenterPosition;
	_VertexPosition[3].x += _HalfSize.x;
	_VertexPosition[3].y += _HalfSize.y;
	_VertexPosition[3].z -= _HalfSize.z;

	_VertexPosition[4] = _CenterPosition;
	_VertexPosition[4].x -= _HalfSize.x;
	_VertexPosition[4].y -= _HalfSize.y;
	_VertexPosition[4].z += _HalfSize.z;

	_VertexPosition[5] = _CenterPosition;
	_VertexPosition[5].x += _HalfSize.x;
	_VertexPosition[5].y -= _HalfSize.y;
	_VertexPosition[5].z += _HalfSize.z;

	_VertexPosition[6] = _CenterPosition;
	_VertexPosition[6].x -= _HalfSize.x;
	_VertexPosition[6].y += _HalfSize.y;
	_VertexPosition[6].z += _HalfSize.z;

	_VertexPosition[7] = _CenterPosition;
	_VertexPosition[7].x += _HalfSize.x;
	_VertexPosition[7].y += _HalfSize.y;
	_VertexPosition[7].z += _HalfSize.z;
}

void AABB::Update(const Vector3 & centerPosition, const Vector3 & halfSize)
{
	_CenterPosition = centerPosition;
	_HalfSize = halfSize;
	CalcVertexPosition();
}
