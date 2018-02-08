#include"stdafx.h"
#include "PlayerCamera.h"

#include "GameObject\Player\Player.h"
#include "fbEngine\_Object\_Component\_3D\Camera.h"
#include "GameObject\History\HistoryBook\HistoryBook.h"

namespace
{
	/** �v���C���[�̍���. */
	const Vector3 PLAYER_HEIGHT(0.0f, 1.5f, 0.0f);
	/** ��]���x. */
	//const float CAMERA_ROTSPEED = 2.0f;
}

PlayerCamera::PlayerCamera(const char * name) :
	GameCamera(name)
{
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
	GameCamera::Awake();

	//�J�����R���|�[�l���g
	_Camera = AddComponent<Camera>();
	_Camera->SetNear(0.01f);
	_Camera->SetFar(5000.0f);
	INSTANCE(GameObjectManager)->mainCamera = _Camera;

	//�J�����̃R���W�����̔��a�ݒ�
	_Radius = 0.2f;
	_Sphere = AddComponent<SphereCollider>();
	_Sphere->Create(_Radius);
	//�����̉����Ə��
	_Dist = 5.0f;

}

void PlayerCamera::Start()
{
	//�v���C���[�̃|�W�V�����ւ̎Q�Ƃ��擾
	_PlayerPos = &_Player->transform->GetPosition();
	// �����l�ݒ�̂��ߏ������ĂԁB
	// �������ȁB
	Init();

	//���j���������B
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

	//�J�������V���h�E�}�b�v�ɐݒ�.
	INSTANCE(SceneManager)->GetShadowMap()->SetCamera(_Camera);
}

void PlayerCamera::UpdateSubClass()
{
	float Pint = 4.0f;

	//���j�������Ă��邩�ǂ����B
	if (_IsMove)
	{
		_StandardBehavior();
	}
	else
	{
		Vector3 CameraToHistory;
		CameraToHistory.Subtract(_HistoryBook->transform->GetPosition(), transform->GetPosition());
		Pint = min(Pint, CameraToHistory.Length());
	}

	INSTANCE(SceneManager)->GetDepthofField().SetPint(Pint * 1000);
	INSTANCE(SceneManager)->GetDepthofField().SetFParam(5.6f);
	INSTANCE(SceneManager)->GetDepthofField().SetFocalLength(24.0f);
}

void PlayerCamera::LookAtTarget()
{
	auto next = _GetPlayerPos();

	_AutoSupport();
	transform->SetPosition(_FuturePos);

	_Camera->SetTarget(next);
	transform->LockAt(next);
}

Vector3 PlayerCamera::_GetPlayerPos()
{
	return (*_PlayerPos) + PLAYER_HEIGHT;
}

void PlayerCamera::_UpdateDist()
{
	if (VPadInput->IsPress(fbEngine::VPad::ButtonRB3))
	{
		auto vir = (VPadInput->GetAnalog(AnalogE::L_STICK) / 32767.0f) * 10.0f * Time::DeltaTime();
		_Dist += -vir.y;
		//3.0~10.0�̊ԂɎ��߂�B
		_Dist = min(15.0f, max(_Dist, 2.0f));
	}
}

/**
* �ʏ펞�̃J��������.
*/
void PlayerCamera::_StandardBehavior()
{
#ifdef _DEBUG
	//�J���������̐L�k
	_UpdateDist();
#endif	//_DEBUG
	bool before = false;
	auto beforeDir = _ToCameraDir;

	//�ǔ��J����.
	_AutoSupport();

	//�E��]
	if (KeyBoardInput->isPressed(DIK_RIGHT) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f) > 0.1f)
	{
		if (before == false)
		{
			_ToCameraDir = beforeDir;
			before = true;
		}
		_RotateHorizon(CAMERA_ROTSPEED * Time::DeltaTime());
	}
	//����]
	if (KeyBoardInput->isPressed(DIK_LEFT) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f) < -0.1f)
	{
		if (before == false)
		{
			_ToCameraDir = beforeDir;
			before = true;
		}
		_RotateHorizon(-CAMERA_ROTSPEED * Time::DeltaTime());
	}
	//��
	if (KeyBoardInput->isPressed(DIK_UP) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f) > 0.1f)
	{
		if (before == false)
		{
			_ToCameraDir = beforeDir;
			before = true;
		}
		_RotateVertical(CAMERA_ROTSPEED  * Time::DeltaTime());
	}
	//��
	if (KeyBoardInput->isPressed(DIK_DOWN) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f) < -0.1f)
	{
		if (before == false)
		{
			_ToCameraDir = beforeDir;
			before = true;
		}
		_RotateVertical(-CAMERA_ROTSPEED  * Time::DeltaTime());
	}

	//�J�������Z�b�g�B
	if (VPadInput->IsPush(fbEngine::VPad::ButtonLB1))
	{
		if (_Reset == false)
		{
			_Reset = true;
			_ResetDir = _Player->transform->GetForward() * _Dist * -1;;
		}
	}

	if (_Reset) {
		_ToCameraDir = _ResetDir;

		auto tmpdir = _ToCameraDir;
		auto nowdir = transform->GetPosition() - _GetPlayerPos();
		auto len = nowdir.Length();
		tmpdir.Normalize();
		nowdir.Normalize();
		float dot = tmpdir.Dot(nowdir);
		if (dot > 0.99f)
		{
			_Reset = false;
		}
	}

	//���C���΂��ăJ�����̂����蔻����Ƃ�A
	//�ŏI�I�ȍ��W���m��������B
	_FuturePos = _ClosetRay(_FutureTarget, _FutureTarget + _ToCameraDir);
	
	//�J�������ړ�������B
	transform->SetPosition(_SpringChaseMove(transform->GetPosition(), _FuturePos, _SpringM, _DampingM, _MoveV, Time::DeltaTime(), _SMoveSpeedM));
	//�^�[�Q�b�g��ǂ�������B
	_LookAtTarget();
}

void PlayerCamera::_AutoSupport()
{
	//�����_���王�_�܂ł̃x�N�g��
	Vector3 toCameraPosXZ = transform->GetPosition() - _Camera->GetTarget();
	//���_�ւ�Y�����̍���.
	//float height = toCameraPosXZ.y;
	//XZ���ʂɂ���̂ŁAY��0�ɂ���.
	toCameraPosXZ.y = 0.0f;
	//XZ���ʏ�ł̎��_�ƒ����_�̋��������߂�.
	float toCameraPosXZLen = toCameraPosXZ.Length();
	//���K��.
	toCameraPosXZ.Normalize();

	//�V���������_���A�N�^�[�̍��W���猈�߂�.
	Vector3 target = _GetPlayerPos();

	//�V���������_����J�����̎n�_�֌������x�N�g�������߂�.
	Vector3 toNewCameraPos = transform->GetPosition() - target;
	//XZ���ʂɂ���̂ŁAY��0�ɂ���.
	toNewCameraPos.y = 0.0f;
	//���K��.
	toNewCameraPos.Normalize();

	//���̃E�F�C�g�̒l��0.0�`1.0�̒l���Ƃ�B1.0�ɋ߂Â��قǒǔ��������Ȃ�.
	float weight = 0.16f;

	_ToCameraDir = toNewCameraPos * weight + toCameraPosXZ * (1.0f - weight);
	_ToCameraDir.Normalize();
	_ToCameraDir.Scale(_Dist);
	_ToCameraDir.y = _Height;

	_FutureTarget = target;
}

//����]�B
void PlayerCamera::_RotateHorizon(float roty)
{
	D3DXQUATERNION mAxisY;
	D3DXMATRIX rot;
	//Y�������roty�x��]����N�H�[�^�j�I�����쐬�B
	D3DXQuaternionRotationAxis(&mAxisY, &(const D3DXVECTOR3&)Vector3::up, roty);
	//�N�H�[�^�j�I�������]�s����쐬�B
	D3DXMatrixRotationQuaternion(&rot, &mAxisY);

	//�x�N�g������]�s����g����]������B
	D3DXVECTOR4 v;
	D3DXVec3Transform(&v, &(D3DXVECTOR3)_ToCameraDir, &rot);
	_ToCameraDir = v;
}

//�c��]�B
void PlayerCamera::_RotateVertical(float rotx)
{
	D3DXVECTOR3 Cross;
	D3DXQUATERNION zAxis;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;

	//�O�ςŒ�������x�N�g�����擾
	D3DXVec3Cross(&Cross, &(const D3DXVECTOR3&)Vector3::up, &(D3DXVECTOR3)_ToCameraDir);
	//�N�H�[�^�j�I���쐬�B
	D3DXQuaternionRotationAxis(&zAxis, &Cross, rotx);
	D3DXMatrixRotationQuaternion(&rot, &zAxis);
	//��]�s����g���ĉ�]������B
	D3DXVec3Transform(&v, &(D3DXVECTOR3)_ToCameraDir, &rot);
	
	//���K������B
	D3DXVECTOR3 toPosDir;
	D3DXVec3Normalize(&toPosDir, &D3DXVECTOR3(v.x, v.y, v.z));

	bool notlimit = true;
	//�J�����̏㉺�̏��
	if (rotx > 0.0f && toPosDir.y < -0.5f)
	{
		notlimit = false;
	}
	else if (rotx < 0.0f && toPosDir.y > 0.8f)
	{
		notlimit = false;
	}

	if (notlimit)
	{
		_ToCameraDir = v;
		_Height = _ToCameraDir.y;
	}
}

Vector3 PlayerCamera::_ClosetRay(Vector3 from, Vector3 to)
{
	// �Փ˂𖳎����鑮����ݒ�B
	int attri = (Collision_ID::ATTACK) | (Collision_ID::PLAYER) | (Collision_ID::ENEMY) | (Collision_ID::BOSS) | (Collision_ID::DRARIAN) | (Collision_ID::NOTHITCAMERA) | (Collision_ID::CHARACTER_GHOST);
	//���C���΂�
	auto ray = INSTANCE(PhysicsWorld)->ClosestRayShape(_Sphere, from, to, attri);

	//���C�������ɓ����������ǂ����H
	if (ray.hitObject)
	{
		//�Փ˓_�̖@�������ɔ��a���ړ�����x�N�g���B
		Vector3 normal = ray.hitNormal;
		normal.Scale(_Radius);
		//�Փ˓_����@���̕����ɉ����o���A�ړ�����Z�o�B
		return ray.hitPos + normal;
	}
	else
	{
		//�ǂ��ɂ�������Ȃ������̂ŁA���C�̏I�_��Ԃ��B
		return to;
	}
}

Vector3 PlayerCamera::_SpringChaseMove(const Vector3 & now, const Vector3 & target, float spring, float damping, Vector3 &velocity, float time, float speed)
{
	//���[���h�s��ł̃J�����̗��z�ʒu�B
	//auto vIdealPos = _FuturePos;

	//���̗��z�ʒu�ւ̃o�l�ɂ������x���v�Z�B
	auto vDisplace = now - target;
	auto vSpringAccel = (-spring * vDisplace) - (damping * velocity);
	//�I�C���[�ϕ����g���ăJ�����̑��x�ƈʒu���X�V�B
	velocity += vSpringAccel * time * speed;
	auto next = now + (velocity * time);
	auto diff = next - target;

	return next;
}

void PlayerCamera::_LookAtTarget()
{
	auto next = _SpringChaseMove(_Camera->GetTarget(), _FutureTarget, _SpringL, _DampingL, _LookV, Time::DeltaTime(), _SMoveSpeedL);
	_Camera->SetTarget(next);
	transform->LockAt(next);
}