#pragma once

// 視野角判定クラス。
class SearchViewAngle {
public:
	// 視野角に入っているか判定。
	// 引数：	視線の位置。
	//			視野角に入っているか判定したいオブジェクトの位置。
	//			視線。
	//			視野角(度)。
	//			見える距離。
	bool IsDiscovery(const Vector3& pos1, const Vector3& pos2, const Vector3& Direction, float angle, float range);
};