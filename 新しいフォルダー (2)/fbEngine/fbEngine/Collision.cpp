#include "Collision.h"
#include "Collider.h"


Collision::~Collision()
{
	_Shape = nullptr;
	_CollisionObject = nullptr;
}
#ifdef _DEBUG
#include "GameObject.h"
#include "SkinModel.h"
#include "SkinModelData.h"
#endif // _DEBUG
void Collision::Awake()
{
#ifdef _DEBUG
	//四角いモデル描画
	SkinModel* model = gameObject->AddComponent<SkinModel>();
	SkinModelData* data = new SkinModelData();
	//そのうち形変えたい
	data->CloneModelData(SkinModelManager::LoadModel("BoxCollision.X"));
	model->SetModelData(data);
	//赤いコリジョン
	model->SetAllBlend(Color(1.0f, 0.0f, 0.0f, 0.5f));
#endif // _DEBUG

	_Offset = Vector3::zero;
	_Shape = nullptr;
	_CollisionObject = nullptr;
}

void Collision::Create(btCollisionObject * collision, Collider * shape, const int & id, Vector3 offset)
{
	_IsHit = false;
	_Offset = offset;
	_Shape = shape;
	//コリジョンオブジェクト設定
	_CollisionObject.reset(collision);
	_CollisionObject->setCollisionShape(_Shape->GetBody());
	_CollisionObject->setUserPointer(this);
	_CollisionObject->setUserIndex(id);

	//トランスフォーム更新
	_UpdateCollisionTrans();
}

void Collision::_UpdateCollisionTrans()
{
	//コリジョンのトランスフォームの参照を取得
	btTransform& trans = _CollisionObject->getWorldTransform();
	//移動を設定
	trans.setOrigin(btVector3(transform->position.x + _Offset.x, transform->position.y + _Offset.y, transform->position.z + _Offset.z));
	//回転を設定
	trans.setRotation(btQuaternion(transform->rotation.x, transform->rotation.y, transform->rotation.z, transform->rotation.w));
}