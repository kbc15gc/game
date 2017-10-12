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
	*			押し出したい属性(レイヤーマスクなのでビット演算)。
	*/
	void Init(const vector<RigidBody*>& collisions,int attribute = -1);

	/*!
	* @brief	押し出し実行。
	* param		1秒単位での移動量(デルタタイムがかかってない移動量)。
	* ※キャラクターコントローラがアタッチされていないオブジェクトは押し出せない。
	*/
	void Extrude(const Vector3& speed);

	// 押し出したい属性オールオフ。
	// すべてのコリジョンを押し出さない。
	inline void Attribute_AllOff() {
		SetAttribute(0x00000000);
	}

	// 押し出したい属性オールオン。
	// すべてのコリジョンを押し出す。
	inline void Attribute_AllOn() {
		SetAttribute(static_cast<int>(fbCollisionAttributeE::ALL));
	}

	// 押し出したい属性に加算。
	inline void AddAttribute(int bit) {
		SetAttribute(_attribute | bit);
	}

	// 押し出したい属性から減算。
	inline void SubAttribute(int bit) {
		// すべてのbitを反転し、目的のビットのみ0、他は1にする。
		bit = ~bit;
		SetAttribute(_attribute & bit);
	}

	// 押し出したい属性を設定。
	inline void SetAttribute(int mask) {
		_attribute = mask;
	}

private:
	vector<Vector3>	_halfSize;					// コライダーのサイズ(実際のサイズの半分)。
	vector<RigidBody*> _collisions;	//剛体。
	int	_attribute;					// 押し出したい属性。
};