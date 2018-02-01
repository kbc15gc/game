#pragma once
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "GameObject\Village\EventManager.h"

class AttackValue2D : public GameObject
{
public:
	AttackValue2D(const char * name) :GameObject(name)
	{
	}
	~AttackValue2D() {
	}

	void Awake()override;
	void Update()override;
	void LateUpdate()override;
	//�_���[�W�ʂ��o���QD�̏�����
	//�_���[�W���o���I�u�W�F�N�g�̃g�����X�t�H�[��
	// �_���[�W��(0��菬�����l�Ő�����\��)�B
	// �N���e�B�J�����B
	// �\�����鎞�ԁB
	// ���炷�ʁB
	//�J���[�B
	// ���̑��\�������������B
	// ���̑��\�������������̈ʒu(��������̍����A�f�t�H���g�͏d�Ȃ��ĕ\���A������\�����Ȃ��ꍇ�������̂ݏo����)�B
	// ���̑��\�������������̃e�L�X�g�T�C�Y�B
	// ���̑��\�������������̃e�L�X�g�J���[�B
	void Init(Transform* t,int damagevalue, bool critical, float lifetime = 1.0f, const Vector3& offset = Vector3::zero, const Color& color = Color::red,const wchar_t* text = L"",const Vector2& pos = Vector2::zero,float size = 30.0f,const Color& textColor = Color::white);
private:
	//�U���ʂ̃e�L�X�g
	TextObject* _AttackText = nullptr;
	TextObject* _sonotaText = nullptr;	// ���̑��\�����������́B
	//�U���ʂ̍ő吶�����ԁB
	float _LifeTime;
	//�U���ʂ̌��݂̎��ԁB
	float _Time;
	//�ʒu����_����̋����ŕ\�����l�B
	Vector3 _Offset;
	Vector2 _sonotaOffset;
	//�㏸�l
	float _Up;
	//�I�u�W�F�N�g�̃g�����X�t�H�[��
	Transform* trans;
};