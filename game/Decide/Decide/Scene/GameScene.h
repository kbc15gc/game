#pragma once
#include "fbEngine\_Scene\Scene.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject/Player/Player.h"
class VillageName;

namespace 
{
	//�I�u�W�F�N�g�̏ڍ�
	struct SCollisionInfo {
		const char* name;
		Vector3 pos;
		Quaternion angle;
		Vector3 scale;
	};
}


class GameScene : public Scene
{
public:
	enum class BGM
	{
		BOSS1 = 0,
		MATI1,
		MATI2,
		MATI3,
		WORLD,
		DEAD,
	};
	GameScene() {};
	void Start()override;
	void Update()override;
private:
	//�`�b�v���쐬����
	void _NewChip();
	//���[���h��BGM��ύX����B
	void _ChangeBGM(BGM);
	//���̂����蔻��
	//�ꏊ�ς��Ȃ���
	// ���E���iAABB�j�ɂ�铖���蔻��
	bool _IsCollideBoxAABB(Vector3 vMin1, Vector3 vMax1, Vector3 vMin2, Vector3 vMax2);
private:
	//�ʏ헬��Ă鉹�y�B
	SoundSource* _WorldBGM;
	SoundSource* _BossBGM;
	SoundSource* _MatiBGM;
	SoundSource* _Mati2BGM;
	SoundSource* _GameBGM;
	SoundSource* _DeadBGM;
	BGM _BGM;
	Player* _Player;

	VillageName* _VillageName;
};