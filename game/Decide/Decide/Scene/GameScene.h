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

class SplitSpace;
class HistoryMenu;
class HistoryBook;
class VillageName;
class GameLight;
class Ground;

class GameScene : public Scene
{
public:
	//BGM�̎��
	enum class BGM
	{
		NONE = -1,
		BOSS1 = 0,		//�h�����A��
		BOSS2,			//�{�X�S�[����
		BOSS3,			//�R�[�h�l�[��D
		MATI1,			//�n�܂�̑�
		MATI2,			//��̑�
		MATI3,			//���W�̊X	
		MAOU1,			//������1�w
		MAOU2,			//������2�w
		MAOU3,			//������3�w
		WORLD,			//���E	
		DEAD,			//���S
		BOSS,			//�H
		LASTBOSS,		//׽�޽
		NUM,
	};
	GameScene() {};
	void Start()override;
	void Update()override;

	// �{�X��ȂǂŃt�B�[���hBGM���~�߂���A���������ǃt�B�[���hBGM���Đ����邽�߂Ɏg�p�B
	void ResetBGMIndex() {
		_BGM = BGM::NONE;
	}

	// �{�X��ŗ�������BGM���w�肵�čĐ�����B
	void PlayBossBGM(BGM bgm) {
		if (bgm >= BGM::BOSS) {
			_ChangeBGM(bgm);
		}
	}

	// �{�X��ȂǂŃt�B�[���hBGM���~�߂����Ƃ��Ɏg�p�B
	void StopFieldBGM() {
		if (_GameBGM) {
			_GameBGM->Stop();
		}
	}

	SplitSpace* GetNowSplitSpace() {
		return _nowSplitSpace;
	}


private:
	// ���̐F���Ԃ��Ȃ���ύX�B
	void _MoveMoonColor();

	// �ύX�J�n�B
	void _StartMoveMoonColor(const Color& target,float time);

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
	SoundSource* _SoundBGM[static_cast<int>(BGM::NUM)];
	SoundSource* _GameBGM;
	BGM _BGM = BGM::NONE;
	BGM _BefBGM = BGM::NONE;

	GameLight* _GameLight = nullptr;
	Ground* _Ground = nullptr;

	Player* _Player;
	HistoryMenu* _HistoryMenu = nullptr;
	HistoryBook* _HistoryBook = nullptr;
	VillageName* _VillageName = nullptr;
	bool _IsEnding;

	SplitSpace* _splitWorld = nullptr;
	SplitSpace* _splitMaouzyou = nullptr;

	SplitSpace* _nowSplitSpace = nullptr;	// ���ݗL���ɂȂ��Ă����ԕ����B

	bool _isMaouzyou = false;	// ������ɂ��邩�B
	bool _isFirstFrame = true;

	Color _defaultColor;

	//Color _saveMoonColor;	// ���̐F��ۑ��B
	Color _targetMoonColor;	// ���̐F�ڕW�l�B
	Color _colorOffset = Color::black;	// �ڕW�̐F�܂ł̍����B
	float _colorMoveSpeed;				// �F���ω����鑬���B
};