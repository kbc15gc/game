#pragma once
#include "fbEngine\_Scene\Scene.h"
#include "fbEngine\_Object\_GameObject\SoundSource.h"
#include "GameObject/Player/Player.h"

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

class HistoryMenu;
class HistoryBook;
class VillageName;

class GameScene : public Scene
{
public:
	enum class BGM
	{
		BOSS1 = 0,
		MATI1,
		MATI2,
		MATI3,
		MAOU,
		WORLD,
		DEAD,
		NUM,
	};
	GameScene() {};
	void Start()override;
	void Update()override;
private:
	//�`�b�v���쐬����
	void _NewChip();
	//���[���h��BGM��ύX����B
	void _ChangeBGM(BGM);
	//BGM������
	//BGMState
	//���O
	//�{�����[��
	void InitBGM(BGM, char*, float);
	//���̂����蔻��
	//�ꏊ�ς��Ȃ���
	// ���E���iAABB�j�ɂ�铖���蔻��
	bool _IsCollideBoxAABB(Vector3 vMin1, Vector3 vMax1, Vector3 vMin2, Vector3 vMax2);
private:
	vector<SoundSource*> _SoundBGM;
	SoundSource* _GameBGM;
	BGM _BGM;
	Player* _Player;
	HistoryMenu* _HistoryMenu = nullptr;
	HistoryBook* _HistoryBook = nullptr;
	VillageName* _VillageName = nullptr;
};