/**
* 血液エフェクトクラスの実装.
*/
#include"stdafx.h"
#include"BloodEffect.h"

/**
* 初期化.
*/
void BloodImage::Awake()
{
	_Image = AddComponent<Sprite>();
}

/**
* 更新.
*/
void BloodImage::LateUpdate()
{
	_LocalTime += Time::DeltaTime();

	if (_LocalTime >= _LimitTime)
	{
		_Alpha -= Time::DeltaTime() / 1.0f;
		if (_Alpha <= 0.0f)
		{
			SetActive(false);
		}
	}
	_Image->SetBlendColor(Color(1, 1, 1, _Alpha));
}

/**
* 有効になった.
*/
void BloodImage::OnEnable()
{
	_LocalTime = 0.0f; 
	_Alpha = 1.0f;

	//座標をランダムで決める.
	Vector2 pos = Vector2::zero;
	pos.x = WindowW * Random::RandDouble();
	pos.y = WindowH * Random::RandDouble();
	transform->SetLocalPosition(pos);
	//角度をランダムで決める.
	float angle = 360.0f * Random::RandDouble();
	transform->SetAngle(0, 0, angle);

	//テクスチャをランダムで決める.
	int num = Random::Range(0, 7);
	char path[30] = { "" };
	sprintf(path, "Effect/blood_%d.png", num);

	_Image->SetTexture(LOADTEXTURE(path));
	_Image->SetSize(_Image->GetSize() * 0.1f);
}

/**
* 初期化.
*/
void BloodEffect::Awake()
{
	for (int i = 0; i < BIMaxSize; i++)
	{
		BloodImage* bi = INSTANCE(GameObjectManager)->AddNew<BloodImage>("BloodImage", 9);
		bi->SetActive(false);
		_BloodImageList[i] = bi;
	}
}

/**
* ダメージ受けた.
*/
void BloodEffect::Damage(int maxHp, int damage)
{
	int count = (float)damage / (float)maxHp * 10;
	count += 2;

	for (int i = 0; i < count; i++)
	{
		BloodImage* bloodImage = nullptr;
		for (auto bi : _BloodImageList)
		{
			if (!bi->GetActive())
			{
				bloodImage = bi;
			}
		}
		if (bloodImage)
		{
			bloodImage->SetActive(true);
		}
	}
}
