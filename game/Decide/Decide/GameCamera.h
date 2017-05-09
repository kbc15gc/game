#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;

//�Q�[���J����
class GameCamera :public GameObject
{
public:
	GameCamera(const char* name) :GameObject(name) {}
	void Awake()override;
	void Start()override;
	void Update()override;
	//�J��������]
	void RotTransversal(float roty);
	//�J�������c�ɉ�]�B
	void RotLongitudinal(float rotx);
private:
	//�ړ��֐�
	void _Move();
private:
	//���C�̌`��
	SphereCollider* _Sphere;
	//�J�����R���|�[�l���g
	Camera* _Camera;
	//�v���C���[�I�u�W�F�N�g
	Player*		_Player;
	//�v���C���[�̃|�W�V�����ւ̎Q��
	const Vector3* _PlayerPos;
	//�v���C���[�֌������x�N�g��
	D3DXVECTOR3 _ToPlayerDir;
	//����
	float _Dist;
	//�����蔻��̔��a
	float _Radius;
};