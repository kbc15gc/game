/**
* �Q�[���J�����N���X�̎���.
*/
#include"stdafx.h"
#include "PlayerCamera.h"

#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "GameObject\Player\Player.h"
#include "GameObject\HistoryBook\HistoryBook.h"

namespace
{

	/** �v���C���[�̍���. */
	const Vector3 PLAYER_HEIGHT(0.0f, 1.5f, 0.0f);
	/** ��]���x. */
	const float CAMERA_SPEED = 1.0f;

}

/**
* �f�X�g���N�^.
*/
PlayerCamera::~PlayerCamera()
{
	//�V���h�E�}�b�v�ɐݒ肵���J�������O��
	INSTANCE(SceneManager)->GetShadowMap()->SetCamera(nullptr);
}

void PlayerCamera::Awake()
{
	//�J�����R���|�[�l���g
	_PlayerCamera = AddComponent<Camera>();
	_PlayerCamera->SetNear(0.01f);
	_PlayerCamera->SetFar(10000.0f);
	INSTANCE(GameObjectManager)->mainCamera = _PlayerCamera;
	//SetCamera(_PlayerCamera);

	//�J�����̃R���W�����̔��a�ݒ�
	_Radius = 0.5f;
	_Sphere = AddComponent<SphereCollider>();
	_Sphere->Create(_Radius);
	//�����̉����Ə��
	_Dist = 7.0f;

	//���`��Ԃ̔䗦���������B
	_LerpRate = 1.0f;
}

void PlayerCamera::Start()
{
	//�v���C���[������
	_Player = (Player*)INSTANCE(GameObjectManager)->FindObject("Player");
	//�v���C���[�̃|�W�V�����ւ̎Q�Ƃ��擾
	_PlayerPos = &_Player->transform->GetPosition();
	//���K����������������
	D3DXVec3Normalize(&_ToPlayerDir, &D3DXVECTOR3(0.0f, 3.0f, -4.0f));
	// �����l�ݒ�̂��ߏ������ĂԁB
	// �������ȁB
	{
		_Move();
		_PlayerCamera->Update();
	}

	//���j���������B
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

	//�J�������V���h�E�}�b�v�ɐݒ�.
	INSTANCE(SceneManager)->GetShadowMap()->SetCamera(_PlayerCamera);
}

void PlayerCamera::Update()
{
	float Pint = 3.0f;

	//���j�������Ă��邩�ǂ����B
	if (_HistoryBook->GetNowState() == (int)HistoryBook::StateCodeE::Unused)
	{
		_StandardBehavior();
		_CameraPos = transform->GetPosition();

		_toPosition.Subtract(transform->GetPosition(), _PlayerCamera->GetTarget());
		Pint = _toPosition.Length();
		Pint = 3.0f;

	}
	else
	{
		Vector3 CameraToHistory;
		CameraToHistory.Subtract(_HistoryBook->transform->GetPosition(), transform->GetPosition());
		Pint = min(3.0f, CameraToHistory.Length());
	}

	_toPosition.Subtract(transform->GetPosition(), _PlayerCamera->GetTarget());

	float Len = _toPosition.Length();
	Len = 3;

	INSTANCE(SceneManager)->GetDepthofField().SetPint(Len * 1000);
	INSTANCE(SceneManager)->GetDepthofField().SetFParam(5.6f);
	INSTANCE(SceneManager)->GetDepthofField().SetFocalLength(24.0f);

}

void PlayerCamera::_RotTransversal(float roty)
{
	D3DXQUATERNION mAxisY;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;
	D3DXQuaternionRotationAxis(&mAxisY, &(const D3DXVECTOR3&)Vector3::up, roty);
	D3DXMatrixRotationQuaternion(&rot, &mAxisY);
	D3DXVec3Transform(&v, &_ToPlayerDir, &rot);
	_ToPlayerDir.x = v.x;
	_ToPlayerDir.y = v.y;
	_ToPlayerDir.z = v.z;
}

void PlayerCamera::_RotLongitudinal(float rotx)
{
	D3DXVECTOR3 Cross;
	D3DXQUATERNION zAxis;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;
	//�O�ςŒ�������x�N�g�����擾
	D3DXVec3Cross(&Cross, &(const D3DXVECTOR3&)Vector3::up, &_ToPlayerDir);
	//
	D3DXQuaternionRotationAxis(&zAxis, &Cross, rotx);
	D3DXMatrixRotationQuaternion(&rot, &zAxis);
	D3DXVec3Transform(&v, &_ToPlayerDir, &rot);
	D3DXVECTOR3 oldDir = _ToPlayerDir;
	_ToPlayerDir.x = v.x;
	_ToPlayerDir.y = v.y;
	_ToPlayerDir.z = v.z;

	D3DXVECTOR3 toPosDir;
	D3DXVec3Normalize(&toPosDir, &_ToPlayerDir);
	//�J�����̏㉺�̏��
	if (toPosDir.y < -0.5f)
	{
		_ToPlayerDir = oldDir;
	}
	else if (toPosDir.y > 0.8f)
	{
		_ToPlayerDir = oldDir;
	}
}

void PlayerCamera::_Move()
{
	//�v���C���[�ƃJ�����̋���
	Vector3 dist = (Vector3)(_ToPlayerDir * _Dist);
	Vector3 from, to;
	//�����_
	from = (*_PlayerPos) + PLAYER_HEIGHT;
	//�J�����̈ړ���
	to = from + dist;
	//���C���΂�
	// �Փ˂𖳎����鑮�������Z�B
	int attri = static_cast<int>(fbCollisionAttributeE::ALL) & ~(Collision_ID::ATTACK) & ~(Collision_ID::PLAYER) & ~(Collision_ID::ENEMY) & ~(Collision_ID::BOSS);
	fbPhysicsCallback::ClosestConvexResultCallback ray = INSTANCE(PhysicsWorld)->ClosestRayShape(_Sphere, from, to, attri);
	//�ړ���|�W�V����
	Vector3 next;
	//���C�������ɓ����������ǂ����H
	if (ray.hitObject)
	{
		//�Փ˓_�̖@�������ɔ��a���ړ�����x�N�g���B
		Vector3 normal = ray.hitNormal;
		normal.Scale(_Radius);
		//�Փ˓_����@���̕����ɉ����o���A�ړ�����Z�o�B
		next = ray.hitPos + normal;
	}
	else
	{
		//�ǂ��ɂ�������Ȃ������̂ŁA���C�̏I�_�Ɉړ�����B
		next = to;
	}
	//�ړ�
	transform->SetPosition(next);
}

/**
* �ʏ펞�̃J��������.
*/
void PlayerCamera::_StandardBehavior()
{

	//��Ԃ̔䗦�����Z�B
	//���`��Ԓ��B
	if (_LerpRate < 1.0f)
	{
		_LerpRate += 0.01f;

		//�J�����̒����_�̐��`��Ԃ��s��(���j������v���C���[�Ɍ����ĕ��)�B
		_LerpCameraLookAtPos = (_HistoryBookPos* (1.0f - _LerpRate) + ((*_PlayerPos) + PLAYER_HEIGHT) * _LerpRate);

		//���`��Ԃ��s��(�v���C���[�̈ʒu������j�������n�߂����̃J�����̈ʒu�Ɍ����ĕ��)�B
		_LerpCameraPos = (_LerpCameraPos * (1.0f - _LerpRate) + _PrevGameCameraPos * _LerpRate);

		//�J�����̒����_����`��Ԃ��ꂽ�ʒu�ɐݒ�B
		transform->LockAt(_LerpCameraLookAtPos);
		_PlayerCamera->SetTarget(_LerpCameraLookAtPos);


		//�J�����̈ʒu����`�⊮���ꂽ�ʒu�ɐݒ�B
		transform->SetPosition(_LerpCameraPos);
	}
	//���`��Ԃ����I������̂Œʏ�̃J�����̓���������B
	else
	{
		//�E��]
		if (KeyBoardInput->isPressed(DIK_RIGHT) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).x / 32767.0f) > 0.1f)
		{
			_RotTransversal(CAMERA_SPEED * Time::DeltaTime());
		}
		//����]
		if (KeyBoardInput->isPressed(DIK_LEFT) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).x / 32767.0f) < -0.1f)
		{
			_RotTransversal(-CAMERA_SPEED * Time::DeltaTime());
		}
		//��
		if (KeyBoardInput->isPressed(DIK_UP) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).y / 32767.0f) > 0.1f)
		{
			_RotLongitudinal(CAMERA_SPEED * Time::DeltaTime());
		}
		//��
		if (KeyBoardInput->isPressed(DIK_DOWN) || (XboxInput(0)->GetAnalog(AnalogInputE::R_STICK).y / 32767.0f) < -0.1f)
		{
			_RotLongitudinal(-CAMERA_SPEED * Time::DeltaTime());
		}

		//�ړ�
		_Move();

		//���j�������n�߂����_�ł̃Q�[���J�����̈ʒu���m�ہB
		_PrevGameCameraPos = this->transform->GetPosition();

		_LerpRate = 1.0f;
	}

	//�v���C���[�̕�������
	transform->LockAt((*_PlayerPos) + PLAYER_HEIGHT);
	_PlayerCamera->SetTarget((*_PlayerPos) + PLAYER_HEIGHT);

}

/**
* �{�����Ă��鎞�̋���.
*/
void PlayerCamera::_HistoryBehavior()
{
	//���j���̈ʒu���擾�B
	_HistoryBookPos = _HistoryBook->transform->GetPosition();

	//��Ԃ̔䗦�����Z�B
	if (_LerpRate > 0.0f)
	{
		_LerpRate -= 0.01f;
	}
	else
	{
		_LerpRate = 0.0f;
	}

	//�J�����̒����_�̐��`��Ԃ��s��(�v���C���[������j���Ɍ����ĕ��)�B
	_LerpCameraLookAtPos = _HistoryBookPos * (1.0f - _LerpRate) + (*_PlayerPos) * _LerpRate;

	//�J�����̈ʒu�̐��`��Ԃ��s��(�Q�[���J�����̈ʒu����v���C���[�̈ʒu�Ɍ����ĕ��)�B
	Vector3 playerPos = (*_PlayerPos) + PLAYER_HEIGHT;

	_LerpCameraPos = playerPos * (1.0f - _LerpRate) + _CameraPos * _LerpRate;

	//�J�����̒����_����`��Ԃ��ꂽ�ʒu�ɐݒ�B
	transform->LockAt(_LerpCameraLookAtPos);
	_PlayerCamera->SetTarget(_LerpCameraLookAtPos);

	//�J�����̈ʒu����`�⊮���ꂽ�ʒu�ɐݒ�B�B
	transform->SetPosition(_LerpCameraPos);

}