/*!
* @brief	キャラクタコントローラー。
*/
#pragma once

#include "_Object\_Component\Component.h"
#include "_Include\CollisionEnum.h"

/*!
* @brief	キャラクタコントローラー。
*/
class CCharacterController : public Component {
public:
	CCharacterController(GameObject* g, Transform* t) :
		Component(g, t, typeid(this).name())
	{
		//m_collider = g->AddComponent<CCapsuleCollider>();
	}
	~CCharacterController()
	{

	}

	/*!
	* @brief	初期化。
	* param		ゲームオブジェクト。
	*			トランスフォーム。
	*			半径。
	*			高さ。
	*			モデルの中心とコリジョンの中心の差分。
	*			コリジョンの属性。
	*			コリジョン形状。
	*			重力。
	*			衝突を取りたい属性(左右方向、レイヤーマスクなのでビット演算)。
	*			衝突を取りたい属性(上下方向、レイヤーマスクなのでビット演算)。
	*			即時に物理ワールドにコリジョンを登録する(falseにした場合は後で登録関数を呼ばないと登録されない)。
	*/
	void Init(GameObject* Object, Transform* tramsform, float radius, float height, Vector3 off , int type, Collider* capsule , float gravity,int attributeXZ = -1, int attributeY = -1/*static_cast<int>(fbCollisionAttributeE::ALL)*/,bool isAddWorld = true);
	
	/*!
	* @brief	実行。
	*/
	void Execute();
	/*!
	* @brief	移動速度を設定。
	*/
	void SetMoveSpeed(const Vector3& speed)
	{
		m_moveSpeed = speed;
	}
	/*!
	* @brief	移動速度を取得。
	*/
	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}
	/*!
	* @brief	ジャンプさせる。
	*/
	void Jump()
	{
		m_isJump = true;
		m_isOnGround = false;
	}
	/*!
	* @brief	ジャンプ中か判定
	*/
	bool IsJump() const
	{
		return m_isJump;
	}
	/*!
	* @brief	地面上にいるか判定。
	*/
	bool IsOnGround() const
	{
		return m_isOnGround;
	}
	/*!
	* @brief	コライダーを取得。
	*/
	Collider* GetCollider() const
	{
		return m_collider;
	}
	/*!
	* @brief	重力を取得。
	*/
	void SetGravity(float gravity)
	{
		m_gravity = gravity;
	}
	/*!
	* @brief	剛体を取得。
	*/
	RigidBody* GetRigidBody()
	{
		return m_rigidBody;
	}

	/*!
	* @brief	剛体を物理エンジンに登録。。
	*/
	inline void AddRigidBody() {
		m_rigidBody->AddWorld();
	}
	/*!
	* @brief	剛体を物理エンジンから削除。。
	*/
	inline void RemoveRigidBoby() {
		m_rigidBody->RemoveWorld();
	}

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
		SetAttributeXZ(m_attributeXZ | bit);
	}
	// フィルターマスクに加算(上下、衝突解決の省略のみでワールドで判定は取れる)。
	inline void AddAttributeY(short bit) {
		SetAttributeY(m_attributeY | bit);
	}

	// フィルターマスクから減算(左右、衝突解決の省略のみでワールドで判定は取れる)。
	inline void SubAttributeXZ(short bit) {
		// すべてのbitを反転し、目的のビットのみ0、他は1にする。
		bit = ~bit;
		SetAttributeXZ(m_attributeXZ & bit);
	}
	// フィルターマスクから減算(上下、衝突解決の省略のみでワールドで判定は取れる)。
	inline void SubAttributeY(short bit) {
		// すべてのbitを反転し、目的のビットのみ0、他は1にする。
		bit = ~bit;
		SetAttributeY(m_attributeY & bit);
	}

	// 衝突を取りたい属性を設定(左右、衝突解決の省略のみでワールドで判定は取れる)。
	inline void SetAttributeXZ(short mask) {
		m_attributeXZ = mask;
	}
	// 衝突を取りたい属性を設定(上下、衝突解決の省略のみでワールドで判定は取れる)。
	inline void SetAttributeY(short mask) {
		m_attributeY = mask;
	}

private:
	Vector3 				m_moveSpeed = Vector3::zero;	//移動速度。 
	bool 					m_isJump = false;				//ジャンプ中？
	bool					m_isOnGround = true;			//地面の上にいる？
	Collider*				m_collider = nullptr;			//コライダー。
	float					m_radius = 0.0f;				//半径。
	float					m_height = 0.0f;				//高さ。
	RigidBody*				m_rigidBody = nullptr;			//剛体。
	float					m_gravity = -9.8f;				//重力。
	int						m_attributeXZ;					// 衝突を取りたい属性(左右方向、衝突解決の省略のみでワールドで判定は取れる)。
	int						m_attributeY;					// 衝突を取りたい属性(上下方向、衝突解決の省略のみでワールドで判定は取れる)。
};