#include "PlayerStateRun.h"
#include "Player.h"

namespace
{
	const float SPEED = 30.0f;
	const float JUMP_POWER = 15.0f;
}

PlayerStateRun::PlayerStateRun(Player* player) :
	PlayerState(player)
{

}

PlayerStateRun::~PlayerStateRun()
{

}

void PlayerStateRun::Update()
{
	//�ړ����x
	Vector3 movespeed = Vector3::zero;
	movespeed.y = player->GetCharaCon().GetMoveSpeed().y;

	//�L�[�{�[�h��J�@or�@�p�b�h��A�{�^���ŃW�����v
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//�n�ʏ�ɂ���ꍇ
		if (player->GetCharaCon().IsOnGround())
		{
			//�W�����v�p���[��ݒ�
			movespeed.y = JUMP_POWER;
			//�L�����N�^�[�R���g���[���[���W�����v��
			player->GetCharaCon().Jump();
		}
	}

	//�Q�[���p�b�h����擾��������
	Vector3 dir = Vector3::zero;
	//�R���g���[���[�ړ�
	dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f) * SPEED;
	dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f) * SPEED;
#ifdef _DEBUG
	//�L�[�{�[�h(�f�o�b�O�p)
	if (KeyBoardInput->isPressed(DIK_W))
	{
		dir.z += SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		dir.z -= SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		dir.x -= SPEED;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		dir.x += SPEED;
	}
#endif
	//�ړ�������
	if (dir.Length() != 0)
	{
		//�J��������݂������ɕϊ�
		Camera* c = GameObjectManager::mainCamera;
		//dir = c->transform->Direction(Vector3::front);
		//dir = dir * SPEED;
		//dir = c->transform->Direction(dir)*Time::DeltaTime();
		//Y�̈ړ��ʂ�����
		movespeed = Vector3(dir.x, movespeed.y, dir.z);

		Vector3 vec = movespeed;
		//���K��
		vec.Normalize();
		//�x�N�g������p�x�����߂�
		float rot = D3DXToRadian(360) - atan2f(vec.z, vec.x);
		//��]
		player->transform->SetLocalAngle(Vector3(0.0f, D3DXToDegree(rot + D3DXToRadian(-90)), 0.0f));
	}
	//�ړ����Ă��Ȃ�
	if (player->GetCharaCon().GetMoveSpeed().Length() < 0.0001f)
	{
		player->ChangeState(Player::State::Idol);
	}

	//�L�����N�^�[�R���g���[���[�X�V
	player->GetCharaCon().SetMoveSpeed(movespeed);
	player->GetCharaCon().Execute();
}

void PlayerStateRun::Enter()
{

}

void PlayerStateRun::Leave()
{

}