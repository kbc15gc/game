/*!
* @brief	キャラクタのコリジョンコントロール。
*/
#include"fbstdafx.h"
#include "CharacterController.h"
#include "_Object\_Component\_Physics\Collider.h"

#define TEST_CHARACON_XZ
#define TEST_CHARCON_Y


void CCharacterController::Init(GameObject* Object, Transform* tramsform, Vector3 off, int type, Collider* capsule, float gravity, int attributeXZ, int attributeY, bool isAddWorld)
{
	//コリジョン作成。
	//重力設定。
	SetGravity(gravity);

	// 衝突を検出したい属性を設定。
	m_attributeXZ = attributeXZ;
	m_attributeY = attributeY;

	m_rigidBody = gameObject->AddComponent<RigidBody>();
	//リジッドボディ作成
	m_rigidBody->Create(0.0f, capsule, type, Vector3::zero, off,Collision::PhysicsType::Kinematick,isAddWorld);
	//スリープさせない(必要かどうかわからない。)
	static_cast<btRigidBody*>(m_rigidBody->GetCollisionObj())->setSleepingThresholds(0, 0);


	m_collider = capsule;

	btVector3 size(0.0f,0.0f,0.0f);	// コライダーサイズ。
	btVector3 localScaling(1.0f,1.0f,1.0f); // Transformのスケール値。
	localScaling = m_collider->GetBody()->getLocalScaling();
	_halfSize = m_rigidBody->GetShape()->GetHalfSize();
	_halfSize.x *= localScaling.getX();
	_halfSize.y *= localScaling.getY();
	_halfSize.z *= localScaling.getZ();

	//もともとのフラグを残したまま新しいフラグを追加。
	m_rigidBody->GetCollisionObj()->setCollisionFlags(m_rigidBody->GetCollisionObj()->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);
}
void CCharacterController::Execute()
{
	//速度に重力加速度を加える。
	m_moveSpeed.y += m_gravity * Time::DeltaTime();
	//次の移動先となる座標を計算する。
	Vector3 nextPosition = transform->GetPosition();
	//速度からこのフレームでの移動量を求める。オイラー積分。
	Vector3 addPos = m_moveSpeed;
	addPos.Scale(Time::DeltaTime());
	nextPosition.Add(addPos);

	//現在の座標から次の移動先へのベクトルを求める。
	Vector3 add = Vector3(nextPosition - transform->GetPosition());
	//カプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
	// コリジョンの中心からコライダーサイズ分の差分を算出。
	//float Offset;
	// 次の座標への向きベクトル。
	add.Normalize();
	//Offset = acosf(add.Dot(_halfSize));	// 向きベクトルにサイズを射影。
	Vector3 posTmp = transform->GetPosition();
	//posTmp += add * Offset;
	Vector3 nextTmp = nextPosition;
	//nextTmp += add * Offset;

#ifdef TEST_CHARACON_XZ

	//XZ平面での衝突検出と衝突解決を行う。
	{
		int loopCount = 0;
		while (true) {
			Vector3 NowToNext(nextPosition - transform->GetPosition());
			NowToNext.y = 0.0f;
			if (NowToNext.Length() /*<*/ > FLT_EPSILON) {
				//XZ平面で動きがないので調べる必要なし。
				//FLT_EPSILONは1より大きい、最小の値との差分を表す定数。
				//とても小さい値のことです。
				//レイを作成。
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//始点はカプセルコライダーの中心座標にサイズ分を加算した地点。
				// ※平らな地面に引っかからないよう少し上げる。
				start.setOrigin(btVector3(m_rigidBody->GetOffsetPos().x, m_rigidBody->GetOffsetPos().y + 0.2f, m_rigidBody->GetOffsetPos().z));
				// ※平らな地面に引っかからないよう少し上げる。
				end.setOrigin(btVector3(nextPosition.x + m_rigidBody->GetOffset().x, m_rigidBody->GetOffsetPos().y + 0.2f, nextPosition.z + m_rigidBody->GetOffset().z));

				fbPhysicsCallback::SweepResultWall callback;
				callback.me = m_rigidBody->GetCollisionObj();
				callback.startPos = Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
				callback._attribute = m_attributeXZ;
				//衝突検出。
				PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

				if (callback.isHit) {
					//当たった。
					//壁。
					{
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
						vOffset.Scale(-fT0 + _halfSize.x);
						nextPosition.Add(vOffset);
					}

					{
						//Vector3 vT0, vT1;
						////XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
						//vT0.Set(nextPosition.x, 0.0f, nextPosition.z);
						//vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
						////めり込みが発生している移動ベクトルを求める。
						//Vector3 vMerikomi;
						//vMerikomi = vT0 - vT1;
						////XZ平面での衝突した壁の法線を求める。。
						//Vector3 hitNormalXZ = callback.hitNormal;
						//hitNormalXZ.y = 0.0f;
						//hitNormalXZ.Normalize();
						////めり込みベクトルを壁の法線に射影する。
						//float fT0 = fabsf(hitNormalXZ.Dot(vMerikomi));
						////押し戻し返すベクトルを求める。
						////押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
						//Vector3 vOffset;
						//vOffset = hitNormalXZ * (-fT0 + _halfSize.x);
						//nextPosition.Add(vOffset);
					}
				}
				else {
					//どことも当たらないので終わり。
					break;
				}
			}
			loopCount++;
			if (loopCount >= 5) {
				break;
			}
		}
	}
#endif

#ifdef TEST_CHARCON_Y

	////XZの移動は確定。
	//transform->SetPosition(Vector3(nextPosition.x, transform->GetPosition().y, nextPosition.z));
	//下方向を調べる。
	{
		float addPosY = nextPosition.y - transform->GetPosition().y;

		transform->SetPosition(nextPosition.x, transform->GetPosition().y, nextPosition.z);	//移動の仮確定。

		//レイを作成する。
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//始点は仮確定した位置。
		start.setOrigin(btVector3(m_rigidBody->GetOffsetPos().x, /*posTmp*/m_rigidBody->GetOffsetPos().y/*transform->GetPosition().y - _halfSize.y + 0.2f*//* + m_height * 0.5f + m_radius*/, m_rigidBody->GetOffsetPos().z));
		//終点は落下後の位置。
		end.setOrigin(btVector3(m_rigidBody->GetOffsetPos().x, nextPosition.y + m_rigidBody->GetOffset().y/* + m_height * 0.5f + m_radius*/, m_rigidBody->GetOffsetPos().z));
		
		Vector3 endPos;
		endPos.Set(end.getOrigin().x(), end.getOrigin().y(), end.getOrigin().z());
		if (m_isOnGround == false) {
			if (addPosY > 0.0f) {
				//ジャンプ中とかで上昇中。
				//上昇中でもXZに移動した結果めり込んでいる可能性があるので下を調べる。
				endPos.y -= addPosY * 0.01f;
			}
			else {
				//落下している場合はそのまま下を調べる。
			}
		}
		else {
			//地面上にいる場合は1m下を見る。
			// 下り坂を降りるときにガタガタしないよう。
			endPos.y -= 1.0f;
		}
		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		//スタートとエンドの差
		btVector3 Sub = start.getOrigin() - end.getOrigin();
		//差が0.0001以上なら衝突検出します
		if (fabs(Sub.length()) > 0.0001f)
		{
			//衝突検出。
			fbPhysicsCallback::SweepResultGround callback;
			callback.me = m_rigidBody->GetCollisionObj();
			callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
			callback._attribute = m_attributeY;
			callback.SlipAngle = 54.0f;

			//int LoopCount = 0;
			//while (true) {
			PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//当たった。

				m_moveSpeed.y = 0.0f;
				//if (callback.isSlip) {
				//	// 衝突面に沿ったベクトルを算出。
				//	Vector3 normal = callback.hitNormal;
				//	Vector3 tangent = normal;
				//	normal.Cross(Vector3::up);
				//	tangent.Cross(normal);
				//	if (tangent.y > 0.0f) {
				//		tangent = tangent * -1.0f;
				//	}
				//	// めり込み量取得。
				//	float merikomi;
				//	merikomi = Vector3(callback.hitPos - nextPosition).Length();
				//	nextPosition.y = callback.hitPos.y;
				//	nextPosition += tangent * merikomi;

				//	m_isJump = true;
				//	m_isOnGround = false;
				//}
				//else {
					m_isJump = false;
					m_isOnGround = true;
					nextPosition.y = callback.hitPos.y + _halfSize.y - m_rigidBody->GetOffset().y;
				//}
				//break;
			//LoopCount++;
			//if (LoopCount >= 5) {
			//	break;
			//}
			}
			else {
				//地面上にいない。
				m_isOnGround = false;
				//break;
			}
		}
	}
#endif

	//移動確定。
	transform->SetPosition(nextPosition);
	m_rigidBody->Update();
	//btRigidBody* btBody = (btRigidBody*)m_rigidBody->GetCollisonObj();
	//剛体を動かす。
	m_rigidBody->GetCollisionObj()->setActivationState(DISABLE_DEACTIVATION);
	//btTransform& trans = btBody->getWorldTransform();
	////剛体の位置を更新。
	//trans.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z));
	//@todo 未対応。 trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
}
