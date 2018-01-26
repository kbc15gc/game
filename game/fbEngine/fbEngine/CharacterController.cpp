/*!
* @brief	キャラクタのコリジョンコントロール。
*/
#include"fbstdafx.h"
#include "CharacterController.h"
#include "_Object\_Component\_Physics\Collider.h"

#define TEST_CHARACON_XZ
#define TEST_CHARCON_Y


void CCharacterController::Init(Vector3 off, int type, Collider* coll, float gravity, int attributeXZ, int attributeY, bool isAddWorld)
{
	//コリジョン作成。
	//重力設定。
	SetGravity(gravity);

	// 衝突を検出したい属性を設定。
	m_attributeXZ = attributeXZ;
	m_attributeY = attributeY;

	//// ゴーストは当たらないように設定。
	//SubAttributeXZ(static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST));
	//SubAttributeY(static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST));

	m_rigidBody = gameObject->AddComponent<RigidBody>();
	//リジッドボディ作成
	m_rigidBody->Create(0.0f, coll, type, Vector3::zero, off,Collision::PhysicsType::Kinematick,isAddWorld);
	//スリープさせない(必要かどうかわからない。)
	static_cast<btRigidBody*>(m_rigidBody->GetCollisionObj())->setSleepingThresholds(0, 0);


	m_collider = coll;

	btVector3 localScaling(1.0f,1.0f,1.0f); // Transformのスケール値。
	localScaling = m_collider->GetBody()->getLocalScaling();
	_halfSize = m_rigidBody->GetShape()->GetHalfSize();
	//_halfSize.x *= localScaling.getX();
	//_halfSize.y *= localScaling.getY();
	//_halfSize.z *= localScaling.getZ();

	//もともとのフラグを残したまま新しいフラグを追加。
	m_rigidBody->GetCollisionObj()->setCollisionFlags(m_rigidBody->GetCollisionObj()->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);
}

const Vector3& CCharacterController::Execute()
{
	if (GetEnable() == false)
		return Vector3::zero;
	// ゴーストは当たらないように設定。
	SubAttributeXZ(static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST));
	SubAttributeY(static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST));

	_isHillThrough = false;

	float deltaTime = Time::DeltaTime();

	if (!_isHillThrough) {
		// 坂道を登るときに抵抗を受ける。
		_isOnWall = false;

		// 重力による現在の速度を更新。
		_nowGravitySpeed += (m_gravity * deltaTime);

		Vector3 nowPosTmp;
		Vector3 nextPosTmp;
		Vector3 originalXZDir;
		//次の移動先となる座標を計算する。
		{
			nextPosTmp = nowPosTmp = m_rigidBody->GetOffsetPos();	// 移動前のコリジョンの座標(原点)を設定。

			nextPosTmp.y = nowPosTmp.y = nowPosTmp.y - m_rigidBody->GetShape()->GetHalfSize().y;	// 位置情報をコリジョンの足元に合わせる。
			//キャラクターの移動量と外的要因による移動量を加算。
			Vector3 addPos;
			addPos = (m_moveSpeed + _outsideSpeed) * deltaTime;
			// y成分は重力を加算。
			addPos.y += _nowGravitySpeed;

			nextPosTmp += addPos;

			originalXZDir = addPos;
			originalXZDir.y = 0.0f;
			originalXZDir.Normalize();
		}


#ifdef TEST_CHARACON_XZ

		//XZ平面での衝突検出と衝突解決を行う。
		{
			int loopCount = 0;
			while (true) {
				Vector3 NowToNext(nextPosTmp - nowPosTmp);
				NowToNext.y = 0.0f;
				if (NowToNext.Length() <= FLT_EPSILON) {
					//XZ平面で動きがないので調べる必要なし。
					//FLT_EPSILONは1より大きい、最小の値との差分を表す定数。
					//とても小さい値のことです。
					break;
				}
				//レイを作成。
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//始点はカプセルコライダーの中心。
				// ※平らな地面に引っかからないよう少し上げる。
				start.setOrigin(btVector3(nowPosTmp.x, nowPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y + 0.2f, nowPosTmp.z));
				start.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
				// ※平らな地面に引っかからないよう少し上げる。
				end.setOrigin(btVector3(nextPosTmp.x, nowPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y + 0.2f, nextPosTmp.z));
				end.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));

				fbPhysicsCallback::SweepResultWall callback;
				callback.me = gameObject;
				callback.startPos = Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
				callback._attribute = m_attributeXZ;
				//衝突検出。
				INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

				if (callback.isHit/* && callback.hitID != static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST)*/) {
					//当たった。
					// かつゴーストではない。

					Vector3 vT0, vT1;
					//XZ平面上での移動後の座標をvT0に、交点の座標をvT1に設定する。
					vT0.Set(nextPosTmp.x, 0.0f, nextPosTmp.z);
					vT1.Set(callback.hitPos.x, 0.0f, callback.hitPos.z);
					//めり込みが発生している移動ベクトルを求める。
					Vector3 vMerikomi;
					vMerikomi = vT0 - vT1;
					//XZ平面での衝突した壁の法線を求める。。
					Vector3 hitNormalXZ = callback.hitNormal;
					hitNormalXZ.y = 0.0f;
					hitNormalXZ.Normalize();
					//めり込みベクトルを壁の法線に射影する。
					float fT0 = hitNormalXZ.Dot(vMerikomi);
					//押し戻し返すベクトルを求める。
					//押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
					Vector3 vOffset;
					vOffset = hitNormalXZ * (-fT0 + _halfSize.x);
					nextPosTmp += vOffset;
					Vector3 currentDir;
					currentDir = nextPosTmp - nowPosTmp;
					currentDir.y = 0.0f;
					currentDir.Normalize();
					if (currentDir.Dot(originalXZDir) < 0.0f) {
						//角に入った時のキャラクタの振動を防止するために、
						//移動先が逆向きになったら移動をキャンセルする。
						nextPosTmp.x = nowPosTmp.x;
						nextPosTmp.z = nowPosTmp.z;
						break;
					}

					_isOnWall = true;
				}
				else {
					//どことも当たらないので終わり。
					break;
				}
				loopCount++;
				if (loopCount >= 5) {
					break;
				}
			}
		}
#endif

#ifdef TEST_CHARCON_Y

		//XZの移動は確定。
		nowPosTmp.x = nextPosTmp.x;
		nowPosTmp.z = nextPosTmp.z;
		//下方向を調べる。
		{
			// Y方向のみの移動量。
			Vector3 addPos = nextPosTmp - nowPosTmp;

			//レイを作成する。
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();

			// 開始位置と足元の差分。
			float startOffset;

			if (_isOnWall) {
				// 壁に押し戻された。

				//始点は仮確定したカプセルコライダーの中心位置(Y成分は移動前)。
				startOffset = m_rigidBody->GetShape()->GetHalfSize().y;
				start.setOrigin(btVector3(nextPosTmp.x, nextPosTmp.y + startOffset, nextPosTmp.z));
			}
			else {
				// 壁には当たっていない。

				//始点は上り坂を上る場合を考慮して仮確定したカプセルコライダーの中心位置より上から。
				startOffset = m_rigidBody->GetShape()->GetHalfSize().y * 2.0f;
				start.setOrigin(btVector3(nextPosTmp.x, nextPosTmp.y + startOffset, nextPosTmp.z));
			}

			start.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));

			//終点は地面上にいない場合は1m下を見る。
			//地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
			//地面上にいなくて降下中の場合はそのまま落下先を調べる。
			// ※この処理は坂道の処理で必要。
			Vector3 endPos;
			endPos = Vector3(start.getOrigin().getX(), start.getOrigin().getY(), start.getOrigin().getZ());
			if (m_isOnGround == false) {
				if (addPos.y > 0.0f) {
					//ジャンプ中などで上昇中。
					//上昇中でもXZに移動した結果めり込んでいる可能性がある(上り坂など)ので、移動後の座標から下を調べる。
					//endPos.y -= addPos.y * 0.01f;
					start.getOrigin().setY(start.getOrigin().getY() + addPos.y);
					endPos.y -= addPos.y * 0.01f;
				}
				else {
					////落下している場合はそのまま下を調べる。
					//endPos.y += addPos.y;

					//上り坂を考慮して開始位置を高めにしているので、終了位置はその差分も加味する。
					endPos.y += (addPos.y - startOffset);
				}
			}
			else {
				//地面上にいる。
				
				//// 1m下を見る。
				//// 下り坂を降りるときにガタガタしないよう。
				//endPos.y -= 1.0f;

				//落下している。

				//上り坂を考慮して開始位置を高めにしているので、終了位置はその差分も加味する。
				// 1m下を見るのは下り坂を降りるときにガタガタしないよう。
				endPos.y -= (1.0f + startOffset);
			}
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
			end.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
			////スタートとエンドの差
			//btVector3 Sub = start.getOrigin() - end.getOrigin();
			////差が0.0001以上なら衝突検出する。
			//if (fabs(Sub.length()) > 0.0001f)
			//{
				//衝突検出。
			fbPhysicsCallback::SweepResultGround callback;
			callback.me = gameObject;
			callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
			callback._attribute = m_attributeY;

			INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//地面に接触している。

				//if (callback.hitID == static_cast<int>(fbCollisionAttributeE::CHARACTER_GHOST)) {
				//	// ゴーストは無視。
				//}
				//else {
					//m_moveSpeed.y = 0.0f;	// 地面に当たったので移動量を0にする。
					m_isJump = false;
					m_isOnGround = true;
					_nowGravitySpeed = 0.0f;	// 地面に当たっているので重力によるスピードを無効にする。
					nextPosTmp.y = callback.hitPos.y;	// 衝突点は足元。
				//}
			}
			else {
				//地面上にいない。

				m_isOnGround = false;
			}
			//}
		}
#endif

		nextPosTmp.y += m_rigidBody->GetShape()->GetHalfSize().y;	// 足元の位置に設定していたのでコリジョンの中心に戻す。
		//移動確定。
		m_rigidBody->SubOffset(nextPosTmp);	// コリジョンの中心座標なのでモデルの原点まで差分の分減算。

		_moveSpeedExcute = nextPosTmp - transform->GetPosition();	// 実際の移動量を保存。

		transform->SetPosition(nextPosTmp);
		m_rigidBody->Update();	// transformを更新したのでコリジョンのTransformも更新する。
		//剛体を動かす。
		m_rigidBody->GetCollisionObj()->setActivationState(DISABLE_DEACTIVATION);

		_outsideSpeed = Vector3::zero;

		_moveSpeedExcute = _moveSpeedExcute / deltaTime;
	}
	else {
		// 坂道をするする登る。

		float radius = 0.2f;
		float height = 1.3f;

		// 重力による現在の速度を更新。
		_nowGravitySpeed += (m_gravity * deltaTime);

		//速度に重力加速度を加える。
		m_moveSpeed.y += _nowGravitySpeed;
		//次の移動先となる座標を計算する。
		Vector3 nextPosition = transform->GetPosition();
		//速度からこのフレームでの移動量を求める。オイラー積分。
		Vector3 addPos = m_moveSpeed;
		addPos.Scale(deltaTime);
		nextPosition.Add(addPos);
		Vector3 originalXZDir = addPos;
		originalXZDir.y = 0.0f;
		originalXZDir.Normalize();
		//XZ平面での衝突検出と衝突解決を行う。
		{
			int loopCount = 0;
			while (true) {
				//現在の座標から次の移動先へ向かうベクトルを求める。
				Vector3 addPos;
				addPos.Subtract(nextPosition, transform->GetPosition());
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
				posTmp.y += radius + height * 0.5f + 0.2f;
				//レイを作成。
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//始点はカプセルコライダーの中心座標 + 0.2の座標をposTmpに求める。
				start.setOrigin(btVector3(posTmp.x, posTmp.y, posTmp.z));
				//終点は次の移動先。XZ平面での衝突を調べるので、yはposTmp.yを設定する。
				end.setOrigin(btVector3(nextPosition.x, posTmp.y, nextPosition.z));

				fbPhysicsCallback::SweepResultWall callback;
				callback.me = gameObject;
				callback.startPos = posTmp;
				//衝突検出。
				INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

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
					vOffset.Scale(-fT0 + radius);
					nextPosition.Add(vOffset);
					Vector3 currentDir;
					currentDir.Subtract(nextPosition, transform->GetPosition());
					currentDir.y = 0.0f;
					currentDir.Normalize();
					if (currentDir.Dot(originalXZDir) < 0.0f) {
						//角に入った時のキャラクタの振動を防止するために、
						//移動先が逆向きになったら移動をキャンセルする。
						nextPosition.x = transform->GetPosition().x;
						nextPosition.z = transform->GetPosition().z;
						break;
					}
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
		transform->SetPosition(Vector3(nextPosition.x, transform->GetPosition().y, nextPosition.z));
		//下方向を調べる。
		{
			Vector3 addPos;
			addPos.Subtract(nextPosition, transform->GetPosition());

			transform->SetPosition(nextPosition);	//移動の仮確定。
										//レイを作成する。
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はカプセルコライダーの中心。
			start.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y + radius + height * 0.5f, transform->GetPosition().z));
			//終点は地面上にいない場合は1m下を見る。
			//地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
			//地面上にいなくて降下中の場合はそのまま落下先を調べる。
			Vector3 endPos;
			endPos = Vector3(start.getOrigin().getX(), start.getOrigin().getY(), start.getOrigin().getZ());
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
			callback.me = gameObject;
			callback.startPos.Set(Vector3(start.getOrigin().getX(), start.getOrigin().getY(), start.getOrigin().getZ()));
			//衝突検出。
			INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//当たった。
				_nowGravitySpeed = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosition.y = callback.hitPos.y;
			}
			else {
				//地面上にいない。
				m_isOnGround = false;
			}
		}
		//移動確定。
		transform->SetPosition(nextPosition);
		//btRigidBody* btBody = static_cast<btRigidBody*>(m_rigidBody->GetCollisionObj());
		////剛体を動かす。
		//btBody->setActivationState(DISABLE_DEACTIVATION);
		//btTransform& trans = btBody->getWorldTransform();
		////剛体の位置を更新。
		//trans.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z));
	}

	return _moveSpeedExcute;
}

/**
* 足元を地面に合わせる.
*/
void CCharacterController::FitGround()
{
	//レイを作成する.
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	//開始位置と足元の差分.
	float startOffset = 2;

	Vector3 pos = transform->GetPosition();
	//開始地点を設定.
	start.setOrigin(btVector3(pos.x, pos.y + startOffset, pos.z));

	//終了地点を設定.
	//2メートル下を見る.
	end.setOrigin(start.getOrigin() - btVector3(0, startOffset + 2, 0));

	fbPhysicsCallback::SweepResultGround callback;
	callback.me = gameObject;
	callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
	callback._attribute = m_attributeY;

	INSTANCE(PhysicsWorld)->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

	if (callback.isHit)
	{
		pos = callback.hitPos;
		pos.y += (m_rigidBody->GetShape()->GetHalfSize().y) * transform->GetScale().y;	// 足元の位置に設定していたのでコリジョンの中心に戻す。
																	//移動確定。
		m_rigidBody->SubOffset(pos);	// コリジョンの中心座標なのでモデルの原点まで差分の分減算。

		transform->SetPosition(pos);
		m_rigidBody->Update();	// transformを更新したのでコリジョンのTransformも更新する。
	}
}
