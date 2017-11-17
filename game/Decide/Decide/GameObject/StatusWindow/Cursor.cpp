#include "stdafx.h"
#include "GameObject\StatusWindow\Cursor.h"

void Cursor::CreateCursor(const char* filepath, Transform* parent, const Vector2& localPos, const Vector2& size, float move, int max, int range, bool isLoop, float angle, CursorMoveType type) {

	// �J�[�\���̈ʒu�ݒ�B
	if (parent) {
		transform->SetParent(parent);
	}
	transform->SetLocalPosition(Vector3(localPos, 0.0f));

	// �J�[�\���̉摜�쐬�B
	_image = INSTANCE(GameObjectManager)->AddNew<ImageObject>("CursorImage", 9);
	_image->SetTexture(LOADTEXTURE(const_cast<char*>(filepath)));
	_image->SetSize(Vector2(size.y, size.x));
	_image->transform->SetParent(transform);
	_image->transform->SetLocalPosition(Vector3::zero);
	Quaternion rot = Quaternion::Identity;
	rot.SetEuler(Vector3(0.0f, 0.0f, angle));
	_image->transform->SetLocalRotation(rot);

	// �p�����[�^�ݒ�B
	_maxNum = max;
	_range = range;
	_move = move;
	_isLoop = isLoop;
	_moveType = type;
	_indexInfo.allRangeIndex = 0;
	_indexInfo.rangeIndex = 0;
	_indexInfo.offset = 0;
	_initPos = localPos;


}

void Cursor::_Move(short num, float offset, int max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction) {
	// �v�f�ԍ��̃C���f�b�N�X���X�V�B
	if (num > 0) {
		_NextMove(num, offset,max, maxMove,isLoop,type,direction);
	}
	else if(num < 0){
		_PrevMove(-num, offset, max, maxMove, isLoop, type, direction);
	}
	else {
		return;
	}
}

void Cursor::_UpdateCursor(float offset, CursorMoveType type, const Vector2& direction) {
	Vector2 dir;
	switch (type) {
	case CursorMoveType::UnMove:
		// �����Ȃ��B
		dir = Vector2(0.0f,0.0f);
		break;
	case CursorMoveType::UpDown:
		// �㉺�ړ��B
		dir = Vector2(0.0f, 1.0f);
		break;
	case CursorMoveType::LeftRight:
		// ���E�ړ��B
		dir = Vector2(1.0f, 0.0f);
		break;
	case CursorMoveType::User:
		// ���[�U�[�B
		dir = direction;
		dir.Normalize();
		break;
	}

	// �J�[�\�����ړ��B
	transform->SetLocalPosition(Vector3(_initPos + (dir * offset),0.0f));
}

const Cursor::CursorIndex& Cursor::_NextMove(short num, float offset, int Max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction) {
	
	//�ύX�O�̒l.
	int befIndex = _indexInfo.rangeIndex;
	
	maxMove = min(Max, maxMove);
	int maxMoveStart = max(0,Max - maxMove);	// �X�^�[�g�ʒu�̍ő�l�͍ő�v�f����ő�ړ��v�f�B

	// �ő�ړ��ʂ͗v�f��-1�B
	Max -= 1;
	maxMove -= 1;
	if (_indexInfo.allRangeIndex < Max) {
		// ���݈ʒu���Ō�̗v�f���w���Ă��Ȃ��B

		int work = min(Max, _indexInfo.allRangeIndex + num);
		num = work - _indexInfo.allRangeIndex;	// ���ۂɉ��Z�����l���Z�o�B
		_indexInfo.allRangeIndex = work;	// �C���f�b�N�X�X�V�B

		if (_indexInfo.allRangeIndex > (_indexInfo.offset + maxMove)) {
			// �J�[�\�����ړ��ł���g���͈ړ������B

			// �J�[�\���̈ړ��J�n�ʒu�X�V�B
			_indexInfo.offset = min(maxMoveStart, _indexInfo.offset + num);
			// �J�[�\���ʒu�X�V�B
			_indexInfo.rangeIndex = min(maxMove, _indexInfo.rangeIndex + num);
		}
		else {
			_indexInfo.rangeIndex += num;
		}
	}
	else {
		// ���ݎw���Ă���v�f���Ō�̗v�f�B

		if (isLoop) {
			// �擪�Ƀ��[�v�B
			_indexInfo.allRangeIndex = 0;
			_indexInfo.rangeIndex = 0;
			_indexInfo.offset = 0;
		}
	}

	if (befIndex != _indexInfo.rangeIndex)
	{
		//�J�[�\���̈ʒu���ς���Ă���.
		SoundSource* changeSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
		changeSE->Init("Asset/Sound/UI/Select.wav");
		changeSE->SetDelete(true);
		changeSE->Play(false);
	}

	// �J�[�\���̈ʒu���X�V�B
	_UpdateCursor(offset, type, direction);

	return _indexInfo;
}

const Cursor::CursorIndex& Cursor::_PrevMove(short num, float offset, int Max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction) {
	
	//�ύX�O�̒l.
	int befIndex = _indexInfo.rangeIndex;
	
	maxMove = min(Max, maxMove);
	int maxMoveStart = max(0, Max - maxMove);	// �X�^�[�g�ʒu�̍ő�l�͍ő�v�f����ő�ړ��v�f�B

	// �ő�ړ��ʂ͗v�f��-1�B
	Max -= 1;
	maxMove -= 1;
	if (_indexInfo.allRangeIndex > 0) {
		// ���݈ʒu���ŏ��̗v�f���w���Ă��Ȃ��B

		int work = max(0, _indexInfo.allRangeIndex - num);
		num = _indexInfo.allRangeIndex - work;	// ���ۂ̌��Z�ʂ��Z�o�B
		_indexInfo.allRangeIndex = work;

		if (_indexInfo.allRangeIndex < _indexInfo.offset) {
			// �J�[�\�����ړ��J�n�ʒu�����O�̂��̂��������B
			_indexInfo.offset = _indexInfo.allRangeIndex;	// �ړ��J�n�ʒu���X�V�B

			_indexInfo.rangeIndex = max(0, _indexInfo.rangeIndex - num);	// ���݂̃J�[�\���ʒu���X�V�B
		}
		else {
			_indexInfo.rangeIndex -= num;
		}
	}
	else {
		// ���ݎw���Ă���v�f���ŏ��̗v�f�B

		if (isLoop) {
			// �I�[�Ƀ��[�v�B
			_indexInfo.allRangeIndex = Max;
			_indexInfo.rangeIndex = maxMove;
			_indexInfo.offset = max(0,Max - maxMove);
		}
	}

	if (befIndex != _indexInfo.rangeIndex)
	{
		//�J�[�\���̈ʒu���ς���Ă���.
		SoundSource* changeSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
		changeSE->Init("Asset/Sound/UI/Select.wav");
		changeSE->SetDelete(true);
		changeSE->Play(false);
	}

	// �J�[�\���̈ʒu���X�V�B
	_UpdateCursor(offset, type, direction);

	return _indexInfo;
}
