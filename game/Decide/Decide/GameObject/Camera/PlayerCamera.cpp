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
	const float CAMERA_SPEED = 3.5f;

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
	_Camera->SetFar(10000.0f);
	INSTANCE(GameObjectManager)->mainCamera = _Camera;

	//�J�����̃R���W�����̔��a�ݒ�
	_Radius = 0.001f;
	_Sphere = AddComponent<SphereCollider>();
	_Sphere->Create(_Radius);
	//�����̉����Ə��
	_Dist = 5.0f;

}

void PlayerCamera::Start()
{
	//�v���C���[�̃|�W�V�����ւ̎Q�Ƃ��擾
	_PlayerPos = &_Player->transform->GetPosition();
	//���K������������
	D3DXVECTOR3 baseDir(0.5f, 0.3f, -0.7f);
	D3DXVec3Normalize(&_ToPlayerDir, &baseDir);
	// �����l�ݒ�̂��ߏ������ĂԁB
	// �������ȁB
	{
		_Camera->SetTarget(_GetPlayerPos());
		transform->SetPosition(_GetPlayerPos() + (_ToPlayerDir * _Dist));
		_Camera->Update();
	}

	//���j���������B
	_HistoryBook = (HistoryBook*)INSTANCE(GameObjectManager)->FindObject("HistoryBook");

	//�J�������V���h�E�}�b�v�ɐݒ�.
	INSTANCE(SceneManager)->GetShadowMap()->SetCamera(_Camera);
}

void PlayerCamera::UpdateSubClass()
{
	float Pint = 3.0f;

	//���j�������Ă��邩�ǂ����B
	if (_IsMove)
	{
		_StandardBehavior();
		Pint = 3.0f;
	}
	else
	{
		Vector3 CameraToHistory;
		CameraToHistory.Subtract(_HistoryBook->transform->GetPosition(), transform->GetPosition());
		Pint = min(3.0f, CameraToHistory.Length());
	}

	INSTANCE(SceneManager)->GetDepthofField().SetPint(Pint * 1000);
	INSTANCE(SceneManager)->GetDepthofField().SetFParam(5.6f);
	INSTANCE(SceneManager)->GetDepthofField().SetFocalLength(24.0f);
}

Vector3 PlayerCamera::_GetPlayerPos()
{
	return (*_PlayerPos) + PLAYER_HEIGHT;
}

/**
* �ʏ펞�̃J��������.
*/
void PlayerCamera::_StandardBehavior()
{
	auto toC = transform->GetPosition() - _GetPlayerPos();
	_ToCamera = D3DXVECTOR3(toC.x, toC.y, toC.z);

	//�E��]
	if (KeyBoardInput->isPressed(DIK_RIGHT) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f) > 0.1f)
	{
		_RotateHorizon(CAMERA_SPEED * Time::DeltaTime());
	}
	//����]
	if (KeyBoardInput->isPressed(DIK_LEFT) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).x / 32767.0f) < -0.1f)
	{
		_RotateHorizon(-CAMERA_SPEED * Time::DeltaTime());
	}
	//��
	if (KeyBoardInput->isPressed(DIK_UP) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f) > 0.1f)
	{
		_RotateVertical(CAMERA_SPEED * Time::DeltaTime());
	}
	//��
	if (KeyBoardInput->isPressed(DIK_DOWN) || (XboxInput(0)->GetAnalog(AnalogE::R_STICK).y / 32767.0f) < -0.1f)
	{
		_RotateVertical(-CAMERA_SPEED * Time::DeltaTime());
	}

	//�ړ���|�W�V�������擾�B
	_TargetPos = _ClosetRay();

	//�v���C���[������B
	_LookAtPlayer();
	//�J�������ړ�������B
	_Move();
}

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
	D3DXVec3Transform(&v, &_ToPlayerDir, &rot);

	_ToPlayerDir.x = v.x;
	_ToPlayerDir.y = v.y;
	_ToPlayerDir.z = v.z;

	D3DXVec3Transform(&v, &_ToCamera, &rot);
	transform->SetPosition(_GetPlayerPos() + Vector3(v.x, v.y, v.z));
}

void PlayerCamera::_RotateVertical(float rotx)
{
	//�O�̊p�x��ێ��B
	D3DXVECTOR3 before = _ToPlayerDir;

	D3DXVECTOR3 Cross;
	D3DXQUATERNION zAxis;
	D3DXVECTOR4 v;
	D3DXMATRIX rot;

	//�O�ςŒ�������x�N�g�����擾
	D3DXVec3Cross(&Cross, &(const D3DXVECTOR3&)Vector3::up, &_ToPlayerDir);
	//�N�H�[�^�j�I���쐬�B
	D3DXQuaternionRotationAxis(&zAxis, &Cross, rotx);
	D3DXMatrixRotationQuaternion(&rot, &zAxis);
	//��]�s����g���ĉ�]������B
	D3DXVec3Transform(&v, &_ToPlayerDir, &rot);
	_ToPlayerDir.x = v.x;
	_ToPlayerDir.y = v.y;
	_ToPlayerDir.z = v.z;

	//���K������B
	D3DXVECTOR3 toPosDir;
	D3DXVec3Normalize(&toPosDir, &_ToPlayerDir);
	bool notlimit = true;
	//�J�����̏㉺�̏��
	if (toPosDir.y < -0.5f)
	{
		_ToPlayerDir = before;
		notlimit = false;
	}
	else if (toPosDir.y > 0.8f)
	{
		_ToPlayerDir = before;
		notlimit = false;
	}

	if (notlimit)
	{
		D3DXVec3Transform(&v, &_ToCamera, &rot);
		transform->SetPosition(_GetPlayerPos() + Vector3(v.x, v.y, v.z));
	}
}

Vector3 PlayerCamera::_ClosetRay()
{
	//�v���C���[�ƃJ�����̋���
	Vector3 dist = (Vector3)(_ToPlayerDir * _Dist);
	Vector3 from, to;
	//�����_
	from = _Camera->GetTarget();
	//�J�����̈ړ���
	to = from + dist;

	// �Փ˂𖳎����鑮����ݒ�B
	int attri = (Collision_ID::ATTACK) | (Collision_ID::PLAYER) | (Collision_ID::ENEMY) | (Collision_ID::BOSS) | (Collision_ID::NOTHITCAMERA) | (Collision_ID::CHARACTER_GHOST);
	//���C���΂�
	auto ray = INSTANCE(PhysicsWorld)->ClosestRayShape(_Sphere, from, to, attri);

	//���C�������ɓ����������ǂ����H
	if (_IsHitObject = ray.hitObject)
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

void PlayerCamera::_LookAtPlayer()
{

	//Vector3 now = _Camera->GetTarget();
	//Vector3 trg = _GetPlayerPos();
	//auto pos = _CalcSpringDamp(now, trg, _PrevPosition, Time::DeltaTime(), 9.0f, 0.8f, 0.01f);
	//_PrevPosition = trg;
	////�v���C���[�̕�������
	//_Camera->SetTarget(pos);
	//transform->LockAt(pos);

	auto pos = _Camera->GetTarget();
	auto trg = _GetPlayerPos();
	//pos.Lerp(trg, 0.7f);
	_Camera->SetTarget(trg);
	transform->LockAt(trg);
}

void PlayerCamera::_Move()
{
	//�⊮�B
	auto now = transform->GetPosition();
	//�ړ���v�Z�B
	auto pos = _CalcSpringDamp(now, _TargetPos, _PrevPosition, Time::DeltaTime(), 9.0f, 0.8f, 0.01f);
	_PrevPosition = _TargetPos;
	transform->SetPosition(pos);

	//�ړ�
	/*auto now = transform->GetPosition();
	now.Lerp(_TargetPos, 0.8f);
	transform->SetPosition(now);*/
}

Vector3 PlayerCamera::_CalcSpringDamp(Vector3 curr, Vector3 trgpos, Vector3 prevtrg, float delta, float spring, float damp, float springlen)
{
	Vector3 disp;		//�ψʁB
	Vector3 velocity;	//���x�B
	float forceMag;		//�͂̑傫���B
				   
	//�o�l�̗͂��v�Z�B
	disp = curr - trgpos;
	if (disp.Length() == 0.0f)
		return curr;
	velocity = (prevtrg - trgpos) * delta;

	forceMag = spring * (springlen - disp.Length()) + damp * (disp.Dot(velocity)) / disp.Length();

	//�o�l�̗͂�K�p�B
	disp.Normalize();
	disp = disp * forceMag * delta;
	return curr + disp;
}