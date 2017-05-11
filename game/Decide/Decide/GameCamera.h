#pragma once
#include "fbEngine\_Object\_GameObject\GameObject.h"

class Player;
class HistoryBook;

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
	enum class State
	{
		LookAtHistoryBook= 0,		//���j�������Ă����ԁB
		LookAtHistoryBookStart,		//���j�������n�߂��̂ŕ�Ԃ��J�n�B
		LookAtPlayer,				//�v���C���[�����Ă����ԁB
		LookAtPlayerStart,			//���j�������I���v���C���[������悤�ɂ����Ԃ̊J�n�B
		DoingLerpMath,				//��Ԃ��v�Z���B
	};
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
	HistoryBook* _HistoryBook;
	//�v���C���[�̃|�W�V�����ւ̎Q�ƁB
	const Vector3* _HistoryBookPos;
	//���`��Ԃ��ꂽ�|�W�V�����B
	const Vector3* _LerpPos;
	//���`��ԂɎg����Ԕ䗦�B
	float _LerpRate;
	//���j��������O�̃J�����̈ʒu���i�[����p�B
	const Vector3* _PrevGameCameraPos;
	//��Ԃ̏�ԁB
	State _LerpState;
};