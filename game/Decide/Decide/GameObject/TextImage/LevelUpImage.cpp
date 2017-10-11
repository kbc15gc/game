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
	//���x���A�b�vImgae������
	_LevelUpImage = INSTANCE(GameObjectManager)->AddNew<ImageObject>("LevelUPImage", StatusWindow::WindowBackPriorty - 1);
	_LevelUpImage->SetTexture(LOADTEXTURE("lvup.png"));
	_Pos = { 640, 300, 0 };
	_LevelUpImage->transform->SetPosition(_Pos);
	_LevelUpImage->SetActive(false);
	//���ԏ�
	_Time = 0.0f;
	//��������
	_LifeTime = 5.0f;
	//�傫��
	_Scale = Vector3::zero;
}

void LevelUpImage::Update()
{
	_Time += Time::DeltaTime();
	//�������Ԃ𒴂����
	if (_Time >= _LifeTime)
	{
		//�\�����Ȃ�
		_LevelUpImage->SetActive(false);
		_LevelUpImage->SetEffectFlg(fbSprite::SpriteEffectE::NONE);
	}
	//�������ԓ��Ȃ�
	else
	{
		//�T�C�Y����
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
	//���ԏ�����
	_Time = 0.0f;
	//���W��������
	_Pos = { 640, 300, 0 };
	_LevelUpImage->transform->SetPosition(_Pos);
	//�傫��������
	_Scale = { MAX_SCALE.x,0.0f,MAX_SCALE.z };
	_LevelUpImage->transform->SetScale(_Scale);
	//�C���[�W��\��
	_LevelUpImage->SetActive(true);

}
