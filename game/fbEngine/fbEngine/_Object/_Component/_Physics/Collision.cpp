#include"fbstdafx.h"
#include "Collision.h"
#include "Collider.h"

#ifdef _DEBUG
const wchar_t* Collision::TypeName[] = {
	L"Rigidbody",
	L"GhostObject"
};
#endif

Collision::~Collision()
{
	_Shape = nullptr;

	// シェアードポインタなので、このコンポーネントが削除された後もコリジョンオブジェクトが参照されることを考慮する。
	_CollisionObject->setUserPointer(nullptr);
}

void Collision::Awake()
{
	_Offset = Vector3::zero;
	_Shape = nullptr;
	_CollisionObject = nullptr;
	_FilterGroup = (short)fbCollisionFilterE::A;
	_FilterMask = (short)fbCollisionFilterE::ALLFILTER;
}

void Collision::Update()
{
	//トランスフォームの更新
	_UpdateCollisionTrans();
}

void Collision::OnDestroy() {
	// ※RemoveWorld関数は内部で仮想関数を呼び出すため、デストラクタでは正しい挙動とならない。
	if (_CollisionObject)
	{
		RemoveWorld();
	}
}

void Collision::Create(btCollisionObject * collision, Collider * shape, const int & id, Vector3 offset,bool isAddWorld)
{
	_Offset = offset;
	_Shape = shape;
	//コリジョンオブジェクト設定
	_CollisionObject.reset(collision);
	_CollisionObject->setCollisionShape(_Shape->GetBody());
	_CollisionObject->setUserPointer(this);	//コンポーネントのアドレス設定。
	_CollisionObject->setUserIndex(id);		//コリジョンID設定
	//トランスフォーム更新
	_UpdateCollisionTrans();

	if (isAddWorld) {
		AddWorld(); 
	}

	// とりあえずここでコライダーの描画をオンにする。
	_Shape->CreateViewModel(_CollisionObject->getWorldTransform());
}

void Collision::SetFilter(short group, short mask)
{
	_FilterGroup = group;
	_FilterMask = mask;
}

void Collision::SetFilterGroup(short group)
{
	_FilterGroup = group;
}

void Collision::SetFilterMask(short mask)
{
	_FilterMask = mask;
}

void Collision::_UpdateCollisionTrans()
{
	//ゲームオブジェクトのトランスフォーム情報を物理エンジンに送る。
	if (_CollisionObject.get() && _physicsType == PhysicsType::Kinematick) {
		//コリジョンのトランスフォームの参照を取得
		btTransform& trans = _CollisionObject->getWorldTransform();
		//移動を設定
		trans.setOrigin(btVector3(GetOffsetPos().x, GetOffsetPos().y, GetOffsetPos().z));
		//回転を設定
		trans.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));

		// コリジョン描画用モデルのTransform情報更新。
		_Shape->UpdateTransform(trans);
	}
}

// ワールドに登録。
void Collision::AddWorld() {
	if (!_isAddWorld) {
		_UpdateCollisionTrans();	// コリジョン登録時にAABBボックスが生成されるため、ここでTransform情報を更新する。
		_AddWorldSubClass();
		_isAddWorld = true;
	}
}
// ワールドから削除。
void Collision::RemoveWorld() {
	if (_isAddWorld) {
		_RemoveWorldSubClass();
		_isAddWorld = false;
	}
}
