/*!
* @brief	キャラクタコントローラー。
*/
#pragma once

#include "_Object\_Component\Component.h"
#include "_Include\CollisionEnum.h"

/*!
* @brief	動いた結果衝突したオブジェクトを押し出すクラス。
*/
class CharacterExtrude : public Component {
public:
	CharacterExtrude(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	}
	~CharacterExtrude()
	{

	}

	/*!
	* @brief	初期化。
	* param		コリジョン。
	*			押し出したい属性(左右方向、押し戻されたい場合に設定、レイヤーマスクなのでビット演算)。
	*			押し出したい属性(上下方向、押し戻されたい場合に設定、レイヤーマスクなのでビット演算)。
	*/
	void Init(RigidBody* collision,int attributeXZ = -1, int attributeY = -1);

	/*!
	* @brief	押し出し実行。
	* param		移動量。
	* ※キャラクターコントローラがアタッチされていないオブジェクトは押し出せない。
	*/
	void Extrude(const Vector3& speed);

	// 全レイヤーマスクオフ(左右)。
	// すべての衝突を無視(衝突解決の省略のみでワールドで判定は取れる)。
	inline void AttributeXZ_AllOff() {
		SetAttributeXZ(0x00000000);
	}
	// 全レイヤーマスクオフ(上下)。
	// すべての衝突を無視(衝突解決の省略のみでワールドで判定は取れる)。
	inline void AttributeY_AllOff() {
		SetAttributeY(0x00000000);
	}

	// 全レイヤーマスクオン(左右)。
	// すべてのコリジョンと当たり判定を行う(衝突解決の省略のみでワールドで判定は取れる)。
	inline void AttributeXZ_AllOn() {
		SetAttributeXZ(static_cast<int>(fbCollisionAttributeE::ALL));
	}
	// 全レイヤーマスクオン(上下)。
	// すべてのコリジョンと当たり判定を行う(衝突解決の省略のみでワールドで判定は取れる)。
	inline void AttributeY_AllOn() {
		SetAttributeY(static_cast<int>(fbCollisionAttributeE::ALL));
	}

	// フィルターマスクに加算(左右、衝突解決の省略のみでワールドで判定は取れる)。
	inline void AddAttributeXZ(short bit) {
		SetAttributeXZ(_attributeXZ | bit);
	}
	// フィルターマスクに加算(上下、衝突解決の省略のみでワールドで判定は取れる)。
	inline void AddAttributeY(short bit) {
		SetAttributeY(_attributeY | bit);
	}

	// フィルターマスクから減算(左右、衝突解決の省略のみでワールドで判定は取れる)。
	inline void SubAttributeXZ(short bit) {
		// すべてのbitを反転し、目的のビットのみ0、他は1にする。
		bit = ~bit;
		SetAttributeXZ(_attributeXZ & bit);
	}
	// フィルターマスクから減算(上下、衝突解決の省略のみでワールドで判定は取れる)。
	inline void SubAttributeY(short bit) {
		// すべてのbitを反転し、目的のビットのみ0、他は1にする。
		bit = ~bit;
		SetAttributeY(_attributeY & bit);
	}

	// 衝突を取りたい属性を設定(左右、衝突解決の省略のみでワールドで判定は取れる)。
	inline void SetAttributeXZ(short mask) {
		_attributeXZ = mask;
	}
	// 衝突を取りたい属性を設定(上下、衝突解決の省略のみでワールドで判定は取れる)。
	inline void SetAttributeY(short mask) {
		_attributeY = mask;
	}

private:
	Vector3	_halfSize;					// コライダーのサイズ(実際のサイズの半分)。
	RigidBody* _collision = nullptr;	//剛体。
	int	_attributeXZ;					// 衝突を取りたい属性(左右方向、衝突解決の省略のみでワールドで判定は取れる)。
	int	_attributeY;					// 衝突を取りたい属性(上下方向、衝突解決の省略のみでワールドで判定は取れる)。
};