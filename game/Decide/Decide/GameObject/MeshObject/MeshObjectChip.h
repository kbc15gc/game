#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class MeshObjectChip : public GameObject
{
public:
	MeshObjectChip(const char* name);
	void Awake()override;
	void Start()override;
	void Update()override;

	//メッシュオブジェクトマネージャーで呼んで初期化。
	//filename
	//座標
	//拡大
	//回転
	void Init(const char* name, const Vector3& pos, const Vector3& sca, const Quaternion& q);
};