#include"stdafx.h"
#include "LevelUpImage.h"
#include "GameObject\StatusWindow\StatusWindow.h"

namespace
{
	const Vector3 MAX_SCALE = { 1.5f,1.5f,1.5f };
	const float SCALE_END_TIME = 2.5f;
}

void LevelUpImage::Awake()
{
	//レベルアップImgae初期化
	_LevelUpImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("LevelUPImage", StatusWindow::WindowBackPriorty - 1);
	_LevelUpImage->SetTexture(LOADTEXTURE("lvup.png"));
	_Pos = { 640, 300, 0 };
	_LevelUpImage->transform->SetPosition(_Pos);
	_LevelUpImage->SetActive(false);
	//時間初
	_Time = 0.0f;
	//制限時間
	_LifeTime = 5.0f;
	//大きさ
	_Scale = Vector3::zero;
}

void LevelUpImage::Update()
{
	_Time += Time::DeltaTime();
	//制限時間を超えると
	if (_Time >= _LifeTime)
	{
		//表示しない
		_LevelUpImage->SetActive(false);
		_LevelUpImage->SetEffectFlg(fbSprite::SpriteEffectE::NONE);
	}
	//制限時間内なら
	else
	{
		//サイズ調整
		if (_Scale.y < MAX_SCALE.y && _Time <= SCALE_END_TIME)
		{
			_Scale.y += 0.05f;
		}
		if (_Time >= SCALE_END_TIME)
		{
			_Scale.y -= 0.05f;
		}
		_LevelUpImage->transform->SetScale(_Scale);
	}

}

void LevelUpImage::Init()
{
	//時間初期化
	_Time = 0.0f;
	//座標を初期化
	_Pos = { 640, 300, 0 };
	_LevelUpImage->transform->SetPosition(_Pos);
	//大きさ初期化
	_Scale = { MAX_SCALE.x,0.0f,MAX_SCALE.z };
	_LevelUpImage->transform->SetScale(_Scale);
	//イメージを表示
	_LevelUpImage->SetActive(true);

}
