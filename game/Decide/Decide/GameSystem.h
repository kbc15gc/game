#pragma once

static const float WindowW = 1280.0f;
static const float WindowH = 720.0f;
//�嗤�̐�
static const UINT CONTINENT_NUM = 1;
//���j�`�b�v�̐�
static const UINT HISTORY_CHIP_NUM = 3;

/** �R���e�B�j���[�t���O. */
extern bool IS_CONTINUE;

enum Collision_ID
{
	NOT_ID = fbCollisionAttributeE::NOTHING,
	GROUND = fbCollisionAttributeE::GROUND,
	PLAYER = fbCollisionAttributeE::CHARACTER,
	CHARACTER_GHOST = fbCollisionAttributeE::CHARACTER_GHOST,	//�L�����N�^�[�R���g���[���������Ɏg�p���邪�A���̃I�u�W�F�N�g�ɉe����^�������Ȃ��Ƃ��ȂǂɎg�p(�L�����N�^�[�R���g���[�������̑����̃R���W�����ƏՓ˂����ꍇ�A�A�g���r���[�g�Ɋ֌W�Ȃ��Փˉ������s��Ȃ��̂Œ��ӁB���̑����̃L�����N�^�[�R���g���[�������̃I�u�W�F�N�g�ɓ��������Ƃ��͏Փˉ��������A�܂����ʍs)�B
	ENEMY = BIT(4),	// �v���C���[�������߂��R���W�����ƍU������p�̃R���W�����Ƃ��Ďg�p(�R���W����������������Ȃ��P���`��̃G�l�~�[�Ȃ�΃L�����N�^�[�R���g���[���ɂ�������g�p����)�B
	NOTHITCAMERA = BIT(5),
	ATTACK = BIT(6),
	SPACE = BIT(7),
	BUILDING = BIT(8),
	ITEMRANGE = BIT(9),
	DROPITEM=BIT(10),	//�h���b�v�A�C�e���p�̃R���W����ID(�n�ʈӊO�Ɠ����蔻����s��Ȃ�)�B
	BOSS = BIT(11),
};


//#define DEBUG_SPLITSPACE	// ��`�ŋ�ԕ����f�o�b�O�I���B
//#define OFF_SPLITSPACE	// ��`�ŋ�ԕ����������B
//#define RELEASE_LOW

