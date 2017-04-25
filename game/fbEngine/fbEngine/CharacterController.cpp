/*!
* @brief	キャラクタのコリジョンコントロール。
*/

#include "CharacterController.h"

void CCharacterController::Init(GameObject* Object, Transform* tramsform, float radius, float height, Vector3 off, int type, Collider* capsule, float gravity)
{
	//コリジョン作成。
	m_radius = radius;
	m_height = height;
	//重力設定
	SetGravity(gravity);
	//m_collider->Create(radius, height);

	m_rigidBody.reset(new RigidBody(Object, tramsform));
	//リジッドボディ作成
	m_rigidBody->Create(0.0f, capsule, type, Vector3::zero, off);
	//スリープさせない(必要かどうかわからない。)
	static_cast<btRigidBody*>(m_rigidBody->GetCollisonObj())->setSleepingThresholds(0, 0);


	m_collider = capsule;
	//剛体を初期化。
	//RigidBodyInfo rbInfo;
	//rbInfo.collider = m_collider;
	//rbInfo.mass = 0.0f;
	//m_rigidBody->Create(0.0f, m_collider, 5);
	//btTransform& trans = m_rigidBody->GetCollisonObj()->getWorldTransform();
	////剛体の位置を更新。
	//trans.setOrigin(btVector3(transform->position.x, transform->position.y, transform->position.z));
	////@todo 未対応。trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
	//m_rigidBody->GetCollisonObj()->setUserIndex(enCollisionAttr_Character);
	m_rigidBody->GetCollisonObj()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	//PhysicsWorld::Instance()->AddRigidBody(m_rigidBody);
}
void CCharacterController::Execute()
{
	//速度に重力加速度を加える。
	m_moveSpeed.y += m_gravity * Time::DeltaTime();
	//次の移動先となる座標を計算する。
	Vector3 nextPosition = transform->GetLocalPosition();
	//速度からこのフレームでの移動量を求める。オイラー積分。
	Vector3 addPos = m_moveSpeed;
	addPos.Scale(Time::DeltaTime());
	nextPosition.Add(addPos);

	//XZ平面での衝突検出と衝突解決を行う。
	{
		int loopCount = 0;
		while (true) {
			//現在の座標から次の移動先へ向かうベクトルを求める。
			Vector3 addPos;
			addPos.Subtract(nextPosition, transform->GetLocalPosition());
			Vector3 addPosXZ = addPos;
			addPosXZ.y = 0.0f;
			if (addPosXZ.Length() < FLT_EPSILON) {
				//XZ平面で動きがないので調べる必要なし。
				//FLT_EPSILONは1より大きい、最小の値との差分を表す定数。
				//とても小さい値のことです。
				break;
			}
			//カプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
			Vector3 posTmp = transform->GetPosition();
			posTmp.y += m_height * 0.5f + m_radius + 0.2f;
			//レイを作成。
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
			start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
			//終点は次の移動先。XZ平面での衝突を調べるので、yはposTmp.yを設定する。
			end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

			fbPhysicsCallback::SweepResultWall callback;
			callback.me = m_rigidBody->GetCollisonObj();
			callback.startPos = posTmp;
			//衝突検出。
			PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

			if (callback.isHit) {
				//当たった。
				//壁。
				Vector3 vT0, vT1;
				//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
				vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
				vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
				//めり込みが発生している移動ベクトルを求める。
				Vector3 vMerikomi;
				vMerikomi.Subtract(vT0, vT1);
				//XZ平面での衝突した壁の法線を求める。。
				Vector3 hitNormalXZ = callback.hitNormal;
				hitNormalXZ.y = 0.0f;
				hitNormalXZ.Normalize();
				//めり込みベクトルを壁の法線に射影する。
				float fT0 = hitNormalXZ.Dot(vMerikomi);
				//押し戻し返すベクトルを求める。
				//押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
				Vector3 vOffset;
				vOffset = hitNormalXZ;
				vOffset.Scale(-fT0 + m_radius);
				nextPosition.Add(vOffset);
			}
			else {
				//どことも当たらないので終わり。
				break;
			}
			loopCount++;
			if (loopCount == 5) {
				break;
			}
		}
	}
	//XZの移動は確定。
	transform->SetLocalPosition(Vector3(nextPosition.x, transform->GetLocalPosition().y, nextPosition.x));
	//下方向を調べる。
	{
		Vector3 addPos;
		addPos.Subtract(nextPosition, transform->GetLocalPosition());

		transform->SetLocalPosition(nextPosition);	//移動の仮確定。
											//レイを作成する。
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//始点はカプセルコライダーの中心。
		start.setOrigin(btVector3(transform->GetLocalPosition().x, transform->GetLocalPosition().y + m_height * 0.5f + m_radius, transform->GetLocalPosition().z));
		//終点は地面上にいない場合は1m下を見る。
		//地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
		//地面上にいなくて降下中の場合はそのまま落下先を調べる。
		Vector3 endPos;
		endPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
		if (m_isOnGround == false) {
			if (addPos.y > 0.0f) {
				//ジャンプ中とかで上昇中。
				//上昇中でもXZに移動した結果めり込んでいる可能性があるので下を調べる。
				endPos.y -= addPos.y * 0.01f;
			}
			else {
				//落下している場合はそのまま下を調べる。
				endPos.y += addPos.y;
			}
		}
		else {
			//地面上にいない場合は1m下を見る。
			endPos.y -= 1.0f;
		}
		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		fbPhysicsCallback::SweepResultGround callback;
		callback.me = m_rigidBody->GetCollisonObj();
		callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
		//スタートとエンドの差
		btVector3 Sub = start.getOrigin() - end.getOrigin();
		//差が0.0001以上なら衝突検出します
		if (fabs(Sub.length()) > 0.0001f)
		{
			//衝突検出。
			PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//当たった。
				//当たった。
				m_moveSpeed.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosition.y = callback.hitPos.y;
			}
			else {
				//地面上にいない。
				m_isOnGround = false;
			}
		}
	}
	//移動確定。
	transform->SetLocalPosition(nextPosition);
	btRigidBody* btBody = (btRigidBody*)m_rigidBody->GetCollisonObj();
	//剛体を動かす。
	btBody->setActivationState(DISABLE_DEACTIVATION);
	btTransform& trans = btBody->getWorldTransform();
	//剛体の位置を更新。
	trans.setOrigin(btVector3(transform->GetLocalPosition().x, transform->GetLocalPosition().y, transform->GetLocalPosition().z));
	//@todo 未対応。 trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
}
/*!
* @brief	死亡したことを通知。
*/
void CCharacterController::RemoveRigidBoby()
{
	PhysicsWorld::Instance()->RemoveRigidBody(m_rigidBody.get());
}