#include "PlayerStateRun.h"
#include "../Player.h"
#include "fbEngine\Camera.h"

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
		if (player->GetCharaCon().IsOnGround() == true)
		{
			//�W�����v�p���[��ݒ�
			movespeed.y = _JumpSpeed;
			//�L�����N�^�[�R���g���[���[���W�����v��
			player->GetCharaCon().Jump();
		}
	}

	//�Q�[���p�b�h����擾��������
	Vector3 dir = Vector3::zero;
	//�R���g���[���[�ړ�
	dir.x += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).x / 32767.0f);
	dir.z += (XboxInput(0)->GetAnalog(AnalogInputE::L_STICK).y / 32767.0f);
#ifdef _DEBUG
	//�L�[�{�[�h(�f�o�b�O�p)
	if (KeyBoardInput->isPressed(DIK_W))
	{
		dir.z++;
	}
	if (KeyBoardInput->isPressed(DIK_S))
	{
		dir.z--;
	}
	if (KeyBoardInput->isPressed(DIK_A))
	{
		dir.x--;
	}
	if (KeyBoardInput->isPressed(DIK_D))
	{
		dir.x++;
	}
#endif
	//�ړ�������
	if (dir.Length() != 0)
	{
		//�J��������݂������ɕϊ�
		Camera* camera = GameObjectManager::mainCamera;
		//�J�����̃r���[�s����Q�b�g
		D3DXMATRIX view = camera->GetViewMat();
		//�r���[�s��̋t�s��
		D3DXMATRIX viewinv;
		D3DXMatrixInverse(&viewinv, NULL, &view);
		//�J������Ԃ��猩���������̃x�N�g�����擾�B
		Vector3 cameraZ;
		cameraZ.x = viewinv.m[2][0];
		cameraZ.y = 0.0f;		//Y������Ȃ��B
		cameraZ.z = viewinv.m[2][2];
		cameraZ.Normalize();	//Y����ł������Ă���̂Ő��K������B
								//�J�������猩���������̃x�N�g�����擾�B
		Vector3 cameraX;
		cameraX.x = viewinv.m[0][0];
		cameraX.y = 0.0f;		//Y���͂���Ȃ��B
		cameraX.z = viewinv.m[0][2];
		cameraX.Normalize();	//Y����ł������Ă���̂Ő��K������B

		//�J��������݂�����
		movespeed.x = cameraX.x * dir.x + cameraZ.x * dir.z;
		movespeed.y = movespeed.y;	//Y���͂���Ȃ��B
		movespeed.z = cameraX.z * dir.x + cameraZ.z * dir.z;
		//�ړ����x�ɃX�s�[�h��������
		movespeed.x *= _Speed;
		movespeed.z *= _Speed;
		//�ړ������������̃x�N�g��
		Vector3 vec = movespeed;
		//���K��
		vec.Normalize();
		//�x�N�g������p�x�����߂�
		float rot = D3DXToRadian(360) - atan2f(vec.z, vec.x);
		//��]
		player->transform->SetLocalAngle(Vector3(0.0f, D3DXToDegree(rot + D3DXToRadian(-90)), 0.0f));
	}
	//�ړ����Ă��Ȃ�
	if (dir.Length() < 0.0001f)
	{
		player->ChangeState(Player::State::Idol);
	}

	//�L�����N�^�[�R���g���[���[�X�V
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

void PlayerStateRun::Enter()
{

}

void PlayerStateRun::Leave()
{

}