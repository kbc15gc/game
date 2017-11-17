#include "stdafx.h"
#include "GameObject\StatusWindow\Cursor.h"

void Cursor::CreateCursor(const char* filepath, Transform* parent, const Vector2& localPos, const Vector2& size, float move, int max, int range, bool isLoop, float angle, CursorMoveType type) {

	// カーソルの位置設定。
	if (parent) {
		transform->SetParent(parent);
	}
	transform->SetLocalPosition(Vector3(localPos, 0.0f));

	// カーソルの画像作成。
	_image = INSTANCE(GameObjectManager)->AddNew<ImageObject>("CursorImage", 9);
	_image->SetTexture(LOADTEXTURE(const_cast<char*>(filepath)));
	_image->SetSize(Vector2(size.y, size.x));
	_image->transform->SetParent(transform);
	_image->transform->SetLocalPosition(Vector3::zero);
	Quaternion rot = Quaternion::Identity;
	rot.SetEuler(Vector3(0.0f, 0.0f, angle));
	_image->transform->SetLocalRotation(rot);

	// パラメータ設定。
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
	// 要素番号のインデックスを更新。
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
		// 動かない。
		dir = Vector2(0.0f,0.0f);
		break;
	case CursorMoveType::UpDown:
		// 上下移動。
		dir = Vector2(0.0f, 1.0f);
		break;
	case CursorMoveType::LeftRight:
		// 左右移動。
		dir = Vector2(1.0f, 0.0f);
		break;
	case CursorMoveType::User:
		// ユーザー。
		dir = direction;
		dir.Normalize();
		break;
	}

	// カーソルを移動。
	transform->SetLocalPosition(Vector3(_initPos + (dir * offset),0.0f));
}

const Cursor::CursorIndex& Cursor::_NextMove(short num, float offset, int Max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction) {
	
	//変更前の値.
	int befIndex = _indexInfo.rangeIndex;
	
	maxMove = min(Max, maxMove);
	int maxMoveStart = max(0,Max - maxMove);	// スタート位置の最大値は最大要素から最大移動要素。

	// 最大移動量は要素数-1。
	Max -= 1;
	maxMove -= 1;
	if (_indexInfo.allRangeIndex < Max) {
		// 現在位置が最後の要素を指していない。

		int work = min(Max, _indexInfo.allRangeIndex + num);
		num = work - _indexInfo.allRangeIndex;	// 実際に加算した値を算出。
		_indexInfo.allRangeIndex = work;	// インデックス更新。

		if (_indexInfo.allRangeIndex > (_indexInfo.offset + maxMove)) {
			// カーソルが移動できる枠分は移動した。

			// カーソルの移動開始位置更新。
			_indexInfo.offset = min(maxMoveStart, _indexInfo.offset + num);
			// カーソル位置更新。
			_indexInfo.rangeIndex = min(maxMove, _indexInfo.rangeIndex + num);
		}
		else {
			_indexInfo.rangeIndex += num;
		}
	}
	else {
		// 現在指している要素が最後の要素。

		if (isLoop) {
			// 先頭にループ。
			_indexInfo.allRangeIndex = 0;
			_indexInfo.rangeIndex = 0;
			_indexInfo.offset = 0;
		}
	}

	if (befIndex != _indexInfo.rangeIndex)
	{
		//カーソルの位置が変わっている.
		SoundSource* changeSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
		changeSE->Init("Asset/Sound/UI/Select.wav");
		changeSE->SetDelete(true);
		changeSE->Play(false);
	}

	// カーソルの位置を更新。
	_UpdateCursor(offset, type, direction);

	return _indexInfo;
}

const Cursor::CursorIndex& Cursor::_PrevMove(short num, float offset, int Max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction) {
	
	//変更前の値.
	int befIndex = _indexInfo.rangeIndex;
	
	maxMove = min(Max, maxMove);
	int maxMoveStart = max(0, Max - maxMove);	// スタート位置の最大値は最大要素から最大移動要素。

	// 最大移動量は要素数-1。
	Max -= 1;
	maxMove -= 1;
	if (_indexInfo.allRangeIndex > 0) {
		// 現在位置が最初の要素を指していない。

		int work = max(0, _indexInfo.allRangeIndex - num);
		num = _indexInfo.allRangeIndex - work;	// 実際の減算量を算出。
		_indexInfo.allRangeIndex = work;

		if (_indexInfo.allRangeIndex < _indexInfo.offset) {
			// カーソルが移動開始位置よりも前のものをさした。
			_indexInfo.offset = _indexInfo.allRangeIndex;	// 移動開始位置を更新。

			_indexInfo.rangeIndex = max(0, _indexInfo.rangeIndex - num);	// 現在のカーソル位置を更新。
		}
		else {
			_indexInfo.rangeIndex -= num;
		}
	}
	else {
		// 現在指している要素が最初の要素。

		if (isLoop) {
			// 終端にループ。
			_indexInfo.allRangeIndex = Max;
			_indexInfo.rangeIndex = maxMove;
			_indexInfo.offset = max(0,Max - maxMove);
		}
	}

	if (befIndex != _indexInfo.rangeIndex)
	{
		//カーソルの位置が変わっている.
		SoundSource* changeSE = INSTANCE(GameObjectManager)->AddNew<SoundSource>("StartSE", 0);
		changeSE->Init("Asset/Sound/UI/Select.wav");
		changeSE->SetDelete(true);
		changeSE->Play(false);
	}

	// カーソルの位置を更新。
	_UpdateCursor(offset, type, direction);

	return _indexInfo;
}
