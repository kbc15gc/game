#include"stdafx.h"
#include "GameObject\LevelUpImage.h"



void LevelUpImage::Awake()
{
	//���x���A�b�vImgae������
	_LevelUpImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("LevelUPImage", 9);
	_LevelUpImage->SetTexture(LOADTEXTURE("levelup.png"));
	_Pos = { 640, 300, 0 };
	_LevelUpImage->transform->SetPosition(_Pos);
	_LevelUpImage->SetActive(false);
	_Time = 0.0f;
	_LifeTime = 2.0f;
	_Scale = { 1.0f,1.0f,1.0f };
}

void LevelUpImage::Update()
{
	_Time += Time::DeltaTime();
	if (_Time >= _LifeTime)
	{
		_LevelUpImage->SetActive(false);
		_LevelUpImage->SetEffectFlg(fbSprite::SpriteEffectE::NONE);
	}
	else
	{
		_Scale.x += 0.01f;
		_Scale.y += 0.01f;
		_LevelUpImage->transform->SetScale(_Scale);
		_Pos.y -= 0.5f;
		_LevelUpImage->transform->SetPosition(_Pos);
	}

}

void LevelUpImage::Init()
{
	//���ԏ�����
	_Time = 0.0f;
	//���W��������
	_Pos = { 640, 300, 0 };
	_LevelUpImage->transform->SetPosition(_Pos);
	//�傫��������
	_Scale = { 1.0f,1.0f,1.0f };
	_LevelUpImage->transform->SetScale(_Scale);
	//_LevelUpImage->SetEffectFlg(fbSprite::SpriteEffectE::FADEOUT);
	//�C���[�W��\��
	_LevelUpImage->SetActive(true);

}
