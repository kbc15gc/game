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
		User,	// ユーザーの指定した方向に移動する(移動処理の際にベクトルを渡さなければならない)。
	};

	// カーソルが指す要素番号の構造体。
	struct CursorIndex {
		int allRangeIndex = 0;		// 最大要素数の中で、現在指している要素番号(スクロールを考慮し、カーソル可動域外の要素も含め何番目か)。
		int rangeIndex = 0;	// カーソルが指しているインデックス(可動域の中で何番目か)。
		int offset = 0;	// カーソルの可動域開始位置と最初の要素の差分(最大要素数の何番目から可動域か)。
	};
public:
	Cursor(char* name) :GameObject(name) {};
	~Cursor() {};

	// カーソル作成関数。
	// 引数：	カーソル画像のファイルパス。
	//			親に設定するTransform情報。
	//			初期位置(ローカル座標)。
	//			画像サイズ。
	//			1項目で移動する量。
	//			何要素項目があるか。
	//			何要素の間をカーソルが移動できるか。
	//			最後の要素の次を最初の要素に、最初の要素の前を最後の要素にするか。
	//			角度(画像の表示角度、デフォルトは回転なし)。
	//			カーソルの移動タイプ(デフォルトは上下)。
	void CreateCursor(const char* filepath,Transform* parent,const Vector2& localPos,const Vector2& size,float move,int max,int range, bool isLoop,float angle = 0.0f, CursorMoveType type = CursorMoveType::UpDown);

	//// カーソル移動関数。
	//// 引数：	移動する要素数(マイナスを指定すると反対方向に進む、デフォルトは1)。
	////			1項目で移動する量(一時的に移動量を変えたい時などに使用、デフォルトは既に設定されている移動量)。
	////			何要素項目があるか。
	////			何要素の間を移動するか(デフォルトはすでに設定されている要素数)。
	////			最後の要素の次を最初の要素に、最初の要素の前を最後の要素にするか(一時的に処理を変えたい時などに使用、デフォルトはfalse)。
	////			移動タイプ(一時的に移動タイプを変えたい時などに使用、デフォルトは既に設定されている移動タイプ)。
	////			移動方向(移動タイプがユーザーの時のみ使用、デフォルトは(0,0))。
	//inline void Move(short num, float offset = 0.0f, int max = 0,int maxMove = 0, bool isLoop = false, CursorMoveType type = CursorMoveType::None, const Vector2& direction = Vector2::zero) {
	//	_Move(num, offset, max,maxMove, isLoop, type, direction);
	//}

	inline const CursorIndex& Move(short num) {
		_Move(num, _move, _maxNum, _range, _isLoop, _moveType, Vector2::zero);
		return _indexInfo;
	}

	// カーソルを次の要素に移動。
	// 引数：	移動する要素数。
	// 戻り値：	移動した結果カーソルが指している要素の構造体。
	inline const CursorIndex& NextMove(short num = 1) {
		return _NextMove(num, _move, _maxNum, _range, _isLoop, _moveType, Vector2::zero);
	}

	// カーソルを前の要素に移動。
	// 引数：	移動する要素数。
	// 戻り値：	移動した結果カーソルが指している要素の構造体。
	inline const CursorIndex& PrevMove(short num = 1) {
		return _PrevMove(num, _move, _maxNum, _range, _isLoop, _moveType, Vector2::zero);
	}

	// 最大要素数設定。
	inline void SetMax(int max) {
		_maxNum = max;
	}

	// 現在カーソルが指している要素(可動域内での番号)を変更。
	inline void SetRamgeIndex(int idx) {
		_Move(_indexInfo.rangeIndex - idx, _move, _maxNum, _range, _isLoop, _moveType, Vector2::zero);
	}

	// 全体要素の先頭と可動域の先頭の差分を設定
	inline void SetOffset(int idx) {
		_indexInfo.offset = idx;
	}
private:
	// カーソル移動関数。
	// 引数：	移動する要素数(マイナスを指定すると反対方向に進む、デフォルトは1)。
	//			1項目で移動する量(一時的に移動量を変えたい時などに使用、デフォルトは既に設定されている移動量)。
	//			何要素項目があるか。
	//			何要素の間を移動するか(デフォルトはすでに設定されている要素数)。
	//			最後の要素の次を最初の要素に、最初の要素の前を最後の要素にするか(一時的に処理を変えたい時などに使用、デフォルトはfalse)。
	//			移動タイプ(一時的に移動タイプを変えたい時などに使用、デフォルトは既に設定されている移動タイプ)。
	//			移動方向(移動タイプがユーザーの時のみ使用、デフォルトは(0,0))。
	void _Move(short num, float offset,int Max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction);

	void _UpdateCursor(float offset,CursorMoveType type,const Vector2& direction);

	// カーソルを次の要素に移動。
	// 引数：	移動する要素数。
	//			1項目で移動する量(一時的に移動量を変えたい時などに使用、デフォルトは既に設定されている移動量)。
	//			何要素項目があるか。
	//			何要素の間を移動するか(デフォルトはすでに設定されている要素数)。
	//			最後の要素の次を最初の要素に、最初の要素の前を最後の要素にするか(一時的に処理を変えたい時などに使用、デフォルトはfalse)。
	//			移動タイプ(一時的に移動タイプを変えたい時などに使用、デフォルトは既に設定されている移動タイプ)。
	//			移動方向(移動タイプがユーザーの時のみ使用、デフォルトは(0,0))。
	// 戻り値：	移動後のカーソルが指し示す要素番号の構造体。
	const CursorIndex& _NextMove(short num, float offset, int Max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction);

	// カーソルを前の要素に移動。
	// 引数：	移動する要素数。
	//			1項目で移動する量(一時的に移動量を変えたい時などに使用、デフォルトは既に設定されている移動量)。
	//			何要素項目があるか。
	//			何要素の間を移動するか(デフォルトはすでに設定されている要素数)。
	//			最後の要素の次を最初の要素に、最初の要素の前を最後の要素にするか(一時的に処理を変えたい時などに使用、デフォルトはfalse)。
	//			移動タイプ(一時的に移動タイプを変えたい時などに使用、デフォルトは既に設定されている移動タイプ)。
	//			移動方向(移動タイプがユーザーの時のみ使用、デフォルトは(0,0))。
	// 戻り値：	移動後のカーソルが指し示す要素番号の構造体。
	const CursorIndex& _PrevMove(short num, float offset, int Max, int maxMove, bool isLoop, CursorMoveType type, const Vector2& direction);

private:
	ImageObject* _image = nullptr;

	int _maxNum = 0;	// 最大要素数。
	int _range = 0;	// カーソルの可動域。
	bool _isLoop = false;	// 要素の先頭と終端でループするか。

	CursorIndex _indexInfo;	// カーソルが指す要素番号の構造体。

	Vector2 _initPos;	// 初期位置。
	float _move = 0.0f;	// 一要素の移動量。
	CursorMoveType _moveType;	// カーソルの移動方向。
};