#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;
class HistoryBook;

//�Q�[���J����
class GameCamera :public GameObject
{
public:
	GameCamera(const char* name) :GameObject(name) {}
	~GameCamera();
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
	//�ʏ펞�̃J��������
	void _StandardBehavior();
	//�{�����Ă��鎞�̋���
	void _HistoryBehavior();
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

	//���j���I�u�W�F�N�g�B
	HistoryBook* _HistoryBook = nullptr;
	//���j���̃|�W�V�����ւ̎Q�ƁB
	Vector3 _HistoryBookPos;
	//���`��Ԃ��ꂽ�J�����̈ʒu�B
	Vector3 _LerpCameraPos;
	//���`��Ԃ��ꂽ�J�����̒����_�̈ʒu�B
	Vector3 _LerpCameraLookAtPos;
	//���`��ԂɎg����Ԕ䗦�B
	float _LerpRate;
	//���j��������O�̃J�����̈ʒu���i�[����p�B
	Vector3 _PrevGameCameraPos;

	Vector3 _toPosition = Vector3::zero;

};