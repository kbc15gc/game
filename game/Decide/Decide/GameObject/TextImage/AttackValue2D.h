#pragma once
#include "fbEngine\_Object\_GameObject\TextObject.h"

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
	// ���W�B
	// �_���[�W�ʁB
	// �\�����鎞�ԁB
	// ���炷�ʁB
	//�J���[�B
	void Init(int damagevalue, bool critical, float lifetime = 1.0f, Vector3 offset = Vector3::zero, Color color = Color::red);
private:
	//�U���ʂ̃e�L�X�g
	TextObject* _AttackText;
	//�U���ʂ̍ő吶�����ԁB
	float _LifeTime;
	//�U���ʂ̌��݂̎��ԁB
	float _Time;
	//�|�W�V�����B
	Vector3 _Pos;
	//�ʒu����_����̋����ŕ\�����l�B
	Vector3 _Offset;
	//�㏸�l�B
	float _Up;
};