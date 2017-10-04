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
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif
	}
	~CCharacterController()
	{

	}

	/*!
	* @brief	初期化。
	* param		モデルの中心とコリジョンの中心の差分。
	*			コリジョンの属性。
	*			コリジョン形状。
	*			重力。
	*			衝突した際押し戻される属性(左右方向、設定した属性のコリジョンにキャラクターが押し戻される、レイヤーマスクなのでビット演算)。
	*			衝突した際押し戻される属性(上下方向、設定した属性のコリジョンにキャラクターが押し戻される、レイヤーマスクなのでビット演算)。
	*			即時に物理ワールドにコリジョンを登録する(falseにした場合は後で登録関数を呼ばないと登録されない)。
	*/
	void Init(Vector3 off , int type, Collider* coll , float gravity,int attributeXZ = -1, int attributeY = -1/*static_cast<int>(fbCollisionAttributeE::ALL)*/,bool isAddWorld = true);
	
	/*!
	* @brief	キャラクターコントローラに設定されている移動量分移動。
	* 戻り値：　衝突解決した後の実際の移動量(デルタタイムがかかってない移動量)。
	*/
	const Vector3& Execute();

	/*!
	* @brief	移動量を設定(デルタタイムがかかってない移動量)。
	*/
	inline void SetMoveSpeed(const Vector3& speed) {
		m_moveSpeed = speed;
	}
	/*!
	* @brief	移動量を取得(デルタタイムがかかってない移動量)。
	*/
	inline const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}
	// 外的要因で発生した移動量を加算。
	// 引数：	1秒単位での移動量(デルタタイムがかかってない移動量)。
	inline void AddOutsideSpeed(const Vector3& add){
		_outsideSpeed = _outsideSpeed + add;
	}
	// 衝突解決を含めた実際の移動量を取得。
	// 戻り値：	衝突解決した後の実際の移動量(デルタタイムがかかってない移動量)。
	// ※Excute関数が呼ばれる前に呼ばれた場合(0,0,0)か前回のExcuteの結果が返却される。
	inline const Vector3& GetmoveSpeedExcute()const {
		return _moveSpeedExcute;
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
	// 要素別に移動量を選択する処理。
	float _MoveSpeedSelection(float mySpeed, float outSpeed);

private:
	Vector3 				m_moveSpeed = Vector3::zero;	//キャラクターが自発的に移動する量。
	Vector3					_outsideSpeed = Vector3::zero;	//キャラクターが外的要因で移動する量。 
	Vector3					_moveSpeedExcute = Vector3::zero;				// 衝突解決終了後の実際に移動した量。
	bool 					m_isJump = false;				//ジャンプ中？
	bool					m_isOnGround = true;			//地面の上にいる？
	Collider*				m_collider = nullptr;			//コライダー。
	Vector3					_halfSize;						// コライダーのサイズ(実際のサイズの半分)。
	RigidBody*				m_rigidBody = nullptr;			//剛体。
	float					m_gravity = -9.8f;				//重力。
	int						m_attributeXZ;					// 衝突した際押し戻される属性(左右方向、設定した属性のコリジョンにキャラクターが押し戻される)。
	int						m_attributeY;					// 衝突した際押し戻される属性(上下方向、設定した属性のコリジョンにキャラクターが押し戻される)。
};