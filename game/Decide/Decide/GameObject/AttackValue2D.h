#pragma once
#include "fbEngine\_Object\_GameObject\TextObject.h"

class Camera;

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
	void Init(Vector3 pos,int damagevalue, float lifetime, Vector3 offset = Vector3::zero);
private:
	//�U���ʂ̃e�L�X�g
	TextObject* _AttackText;
	//�U���ʂ̍ő吶�����ԁB
	float _LifeTime;
	//�U���ʂ̌��݂̎��ԁB
	float _Time;
	//�U���ʃe�L�X�g���쐬����Ă��邩�B
	bool _CreatAttackValue;
	//�|�W�V�����B
	Vector3 _Pos;
	//�J����
	Camera* camera;
};