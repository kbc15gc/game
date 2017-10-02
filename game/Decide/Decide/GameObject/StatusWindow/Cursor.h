#pragma once
#include "fbEngine\_Object\_GameObject\ImageObject.h"
#include "fbEngine\fbstdafx.h"

class Cursor : public GameObject {
public:
	enum class CursorMoveType {
		None = -1,
		UpDown = 0,
		LeftRight,
		UnMove,
		User,	// ���[�U�[�̎w�肵�������Ɉړ�����(�ړ������̍ۂɃx�N�g����n���Ȃ���΂Ȃ�Ȃ�)�B
	};

	// �J�[�\�����w���v�f�ԍ��̍\���́B
	struct CursorIndex {
		int allRangeIndex = 0;		// �ő�v�f���̒��ŁA���ݎw���Ă���v�f�ԍ�(�X�N���[�����l�����A�J�[�\������O�̗v�f���܂߉��Ԗڂ�)�B
		int rangeIndex = 0;	// �J�[�\�����w���Ă���C���f�b�N�X(����̒��ŉ��Ԗڂ�)�B
		int offset = 0;	// �J�[�\���̉���J�n�ʒu�ƍŏ��̗v�f�̍���(�ő�v�f���̉��Ԗڂ�����悩)�B
	};
public:
	Cursor(char* name) :GameObject(name) {};
	~Cursor() {};

	// �J�[�\���쐬�֐��B
	// �����F	�J�[�\���摜�̃t�@�C���p�X�B
	//			�e�ɐݒ肷��Transform���B
	//			�����ʒu(���[�J�����W)�B
	//			�摜�T�C�Y�B
	//			1���ڂňړ�����ʁB
	//			���v�f���ڂ����邩�B
	//			���v�f�̊Ԃ��J�[�\�����ړ��ł��邩�B
	//			�Ō�̗v�f�̎����ŏ��̗v�f�ɁA�ŏ��̗v�f�̑O���Ō�̗v�f�ɂ��邩�B
	//			�p�x(�摜�̕\���p�x�A�f�t�H���g�͉�]�Ȃ�)�B
	//			�J�[�\���̈ړ��^�C�v(�f�t�H���g�͏㉺)�B
	void CreateCursor(const char* filepath,Transform* parent,const Vector2& localPos,const Vector2& size,float move,int max,int range, bool isLoop,float angle = 0.0f, CursorMoveType type = CursorMoveType::UpDown);

	//// �J�[�\���ړ��֐��B
	//// �����F	�ړ�����v�f��(�}�C�i�X���w�肷��Ɣ��Ε����ɐi�ށA�f�t�H���g��1)�B
	////			1���ڂňړ������(�ꎞ�I�Ɉړ��ʂ�ς��������ȂǂɎg�p�A�f�t�H���g�͊��ɐݒ肳��Ă���ړ���)�B
	////			���v�f���ڂ����邩�B
	////			���v�f�̊Ԃ��ړ����邩(�f�t�H���g�͂��łɐݒ肳��Ă���v�f��)�B
	////			�Ō�̗v�f�̎����ŏ��̗v�f�ɁA�ŏ��̗v�f�̑O���Ō�̗v�f�ɂ��邩(�ꎞ�I�ɏ�����ς��������ȂǂɎg�p�A�f�t�H���g��false)�B
	////			�ړ��^�C�v(�ꎞ�I�Ɉړ��^�C�v��ς��������ȂǂɎg�p�A�f�t�H���g�͊��ɐݒ肳��Ă���ړ��^�C�v)�B
	////			�ړ�����(�ړ��^�C�v�����[�U�[�̎��̂ݎg�p�A�f�t�H���g��(0,0))�B
	//inline void Move(short num, float offset = 0.0f, int max = 0,int maxMove = 0, bool isLoop = false, CursorMoveType type = CursorMoveType::None, const Vector2& direction = Vector2::zero) {
	//	_Move(num, offset, max,maxMove, isLoop, type, direction);
	//}

	inline const CursorIndex& Move(short num) {
		_Move(num, _move, _maxNum, _range, _isLoop, _moveType, Vector2::zero);
		return _indexInfo;
	}

	// �J�[�\�������̗v�f�Ɉړ��B
	// �����F	�ړ�����v�f���B
	// �߂�l�F	�ړ��������ʃJ�[�\�����w���Ă���v�f�̍\���́B
	inline const CursorIndex& NextMove(short num = 1) {
		return _NextMove(num, _move, _maxNum, _range, _isLoop, _moveType, Vector2::zero);
	}

	// �J�[�\����O�̗v�f�Ɉړ��B
	// �����F	�ړ�����v�f���B
	// �߂�l�F	�ړ��������ʃJ�[�\�����w���Ă���v�f�̍\���́B
	inline const CursorIndex& PrevMove(short num = 1) {
		return _PrevMove(num, _move, _maxNum, _range, _isLoop, _moveType, Vector2::zero);
	}

	// �ő�v�f���ݒ�B
	inline void SetMax(int max) {
		_maxNum = max;
	}

	// ���݃J�[�\�����w���Ă���v�f(������ł̔ԍ�)��ύX�B
	inline void SetRamgeIndex(int idx) {
		_Move(_indexInfo.rangeIndex - idx, _move, _maxNum, _range, _isLoop, _moveType, Vector2::zero);
	}

	// �S�̗v�f�̐擪�Ɖ���̐擪�̍�����ݒ�
	inline void SetOffset(int idx) {
		_indexInfo.offset = idx;
	}
private:
	// �J�[�\���ړ��֐��B
	// �����F	�ړ�����v�f��(�}�C�i�X���w�肷��Ɣ��Ε����ɐi�ށA�f�t�H���g��1)�B
	//			1���ڂňړ������(�ꎞ�I�Ɉړ��ʂ�ς��������ȂǂɎg�p�A�f�t�H���g�͊��ɐݒ肳��Ă���ړ���)�B
	//			���v�f���ڂ����邩�B
	//			���v�f�̊Ԃ��ړ����邩(�f�t�H���g�͂��łɐݒ肳��Ă���v�f��)�B
	//			�Ō�̗v�f�̎����ŏ��̗v�f�ɁA�ŏ��̗v�f�̑O���Ō�̗v�f�ɂ��邩(�ꎞ�I�ɏ�����ς��������ȂǂɎg�p�A�f�t�H���g��false)�B
	//			�ړ��^�C�v(�ꎞ�I�Ɉړ��^�C�v��ς��������ȂǂɎg�p�A�f�t�H���g�͊��ɐݒ肳��Ă���ړ��^�C�v)�B
	//			�ړ�����(�ړ��^�C�v�����[�U�[�̎��̂ݎg�p�A�f�t�H���g��(0,0))�B
	void _Move(short num, float offset,int Max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction);

	void _UpdateCursor(float offset,CursorMoveType type,const Vector2& direction);

	// �J�[�\�������̗v�f�Ɉړ��B
	// �����F	�ړ�����v�f���B
	//			1���ڂňړ������(�ꎞ�I�Ɉړ��ʂ�ς��������ȂǂɎg�p�A�f�t�H���g�͊��ɐݒ肳��Ă���ړ���)�B
	//			���v�f���ڂ����邩�B
	//			���v�f�̊Ԃ��ړ����邩(�f�t�H���g�͂��łɐݒ肳��Ă���v�f��)�B
	//			�Ō�̗v�f�̎����ŏ��̗v�f�ɁA�ŏ��̗v�f�̑O���Ō�̗v�f�ɂ��邩(�ꎞ�I�ɏ�����ς��������ȂǂɎg�p�A�f�t�H���g��false)�B
	//			�ړ��^�C�v(�ꎞ�I�Ɉړ��^�C�v��ς��������ȂǂɎg�p�A�f�t�H���g�͊��ɐݒ肳��Ă���ړ��^�C�v)�B
	//			�ړ�����(�ړ��^�C�v�����[�U�[�̎��̂ݎg�p�A�f�t�H���g��(0,0))�B
	// �߂�l�F	�ړ���̃J�[�\�����w�������v�f�ԍ��̍\���́B
	const CursorIndex& _NextMove(short num, float offset, int Max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction);

	// �J�[�\����O�̗v�f�Ɉړ��B
	// �����F	�ړ�����v�f���B
	//			1���ڂňړ������(�ꎞ�I�Ɉړ��ʂ�ς��������ȂǂɎg�p�A�f�t�H���g�͊��ɐݒ肳��Ă���ړ���)�B
	//			���v�f���ڂ����邩�B
	//			���v�f�̊Ԃ��ړ����邩(�f�t�H���g�͂��łɐݒ肳��Ă���v�f��)�B
	//			�Ō�̗v�f�̎����ŏ��̗v�f�ɁA�ŏ��̗v�f�̑O���Ō�̗v�f�ɂ��邩(�ꎞ�I�ɏ�����ς��������ȂǂɎg�p�A�f�t�H���g��false)�B
	//			�ړ��^�C�v(�ꎞ�I�Ɉړ��^�C�v��ς��������ȂǂɎg�p�A�f�t�H���g�͊��ɐݒ肳��Ă���ړ��^�C�v)�B
	//			�ړ�����(�ړ��^�C�v�����[�U�[�̎��̂ݎg�p�A�f�t�H���g��(0,0))�B
	// �߂�l�F	�ړ���̃J�[�\�����w�������v�f�ԍ��̍\���́B
	const CursorIndex& _PrevMove(short num, float offset, int Max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction);

private:
	ImageObject* _image = nullptr;

	int _maxNum = 0;	// �ő�v�f���B
	int _range = 0;	// �J�[�\���̉���B
	bool _isLoop = false;	// �v�f�̐擪�ƏI�[�Ń��[�v���邩�B

	CursorIndex _indexInfo;	// �J�[�\�����w���v�f�ԍ��̍\���́B

	Vector2 _initPos;	// �����ʒu�B
	float _move = 0.0f;	// ��v�f�̈ړ��ʁB
	CursorMoveType _moveType;	// �J�[�\���̈ړ������B
};