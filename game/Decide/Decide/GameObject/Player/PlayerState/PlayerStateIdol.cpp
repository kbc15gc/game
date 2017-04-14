#include "../Player.h"
#include "PlayerStateIdol.h"

PlayerStateIdol::PlayerStateIdol(Player* player) :
	PlayerState(player)
{

}

PlayerStateIdol::~PlayerStateIdol()
{

}


void PlayerStateIdol::Update()
{
	//�Q�[���p�b�h����擾��������
	Vector3 dir = Vector3::zero;
	//�R���g���[���[�ړ�
	dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
	//�L�[�{�[�h(�f�o�b�O�p)
	if (KeyBoardInput->isPressed(DIK_W))
	{
		dir.z += 1.0f;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		dir.z -= 1.0f;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		dir.x -= 1.0f;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		dir.x += 1.0f;
	}
#endif
	Vector3 movespeed = player->GetCharaCon().GetMoveSpeed();
	movespeed.x = 0.0f;
	movespeed.z = 0.0f;
	//�L�[�{�[�h��J�@or�@�p�b�h��A�{�^���ŃW�����v
	if (KeyBoardInput->isPush(DIK_J) || XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_A))
	{
		//�n�ʏ�ɂ���ꍇ
		if (player->GetCharaCon().IsOnGround() == true)
		{
			//�W�����v�p���[��ݒ�
			movespeed.y = _JumpSpeed;
			//�L�����N�^�[�R���g���[���[���W�����v��
			player->GetCharaCon().Jump();
		}
	}
	if (dir.Length() >= 0.0001f)
	{
		player->ChangeState(Player::State::Run);
	}

	movespeed.x = dir.x * _Speed;
	movespeed.z = dir.z * _Speed;
	
	player->GetCharaCon().SetMoveSpeed(movespeed);
	player->GetCharaCon().Execute();
	//�U���ֈړ�
	if (XboxInput(0)->IsPushButton(XINPUT_GAMEPAD_X) || KeyBoardInput->isPush(DIK_SPACE))
	{
		if (player->GetCharaCon().IsOnGround() == true)
		{
			player->ChangeState(Player::State::Attack);
		}
	}
}

void PlayerStateIdol::Enter()
{

}

void PlayerStateIdol::Leave()
{

}