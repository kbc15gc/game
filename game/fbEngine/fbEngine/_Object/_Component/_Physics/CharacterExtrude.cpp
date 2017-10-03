#include "fbstdafx.h"
#include "_Object\_Component\_Physics\CharacterExtrude.h"
#include "CharacterController.h"

void CharacterExtrude::Init(RigidBody* collision, int attributeXZ, int attributeY) {
	// 衝突を検出したい属性を設定。
	_attributeXZ = attributeXZ;
	_attributeY = attributeY;

	_collision = collision;

	btVector3 localScaling(1.0f, 1.0f, 1.0f); // Transformのスケール値。
	localScaling = _collision->GetShape()->GetBody()->getLocalScaling();
	_halfSize = _collision->GetShape()->GetHalfSize();
	_halfSize.x *= localScaling.getX();
	_halfSize.y *= localScaling.getY();
	_halfSize.z *= localScaling.getZ();
}

void CharacterExtrude::Extrude(const Vector3& speed) {
	Vector3 startPos;
	Vector3 endPos;
	startPos = endPos = _collision->GetOffsetPos();	// 移動前のコリジョンの座標(原点)を設定。
	startPos -= speed;	// スタート位置を移動前の位置に設定。

	//XZ平面での衝突検出と衝突解決を行う。
	{
		Vector3 NowToNext(endPos - startPos);
		NowToNext.y = 0.0f;
		if (NowToNext.Length() <= FLT_EPSILON) {
			//XZ平面で動きがないので調べる必要なし。
			//FLT_EPSILONは1より大きい、最小の値との差分を表す定数。
			//とても小さい値のことです。

		}
		else {
			//レイを作成。
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はカプセルコライダーの中心。
			// ※平らな地面に引っかからないよう少し上げる。
			start.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
			// ※平らな地面に引っかからないよう少し上げる。
			end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));

			fbPhysicsCallback::SweepResultExtrude callback(Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z()), _collision->GetCollisionObj(), _attributeXZ);
			//衝突検出。
			PhysicsWorld::Instance()->ConvexSweepTest(static_cast<const btConvexShape*>(_collision->GetShape()->GetBody()), start, end, callback);

			if (callback.isHit) {
				//当たった。

				vector<fbPhysicsCallback::SweepResultExtrude::hitInfo*> infoArray = callback.GetInfoArray();
				for (int idx = 0; idx < infoArray.size(); idx++) {
					Vector3 vT0, vT1;
					//めり込みが発生している移動ベクトルを求める。
					Vector3 vMerikomi;
					vMerikomi = (endPos - startPos) * (1.0f - infoArray[idx]->hitFraction);
					vMerikomi.y = 0.0f;

					// テスト。
					CCharacterController* CC = infoArray[idx]->collision->gameObject->GetComponent<CCharacterController>();
					if (CC) {
						CC->AddOutsideSpeed(vMerikomi);
					}

					////XZ平面での衝突した壁の法線を求める。。
					//Vector3 hitNormalXZ = callback.hitNormal;
					//hitNormalXZ.y = 0.0f;
					//hitNormalXZ.Normalize();
					////めり込みベクトルを壁の法線に射影する。
					//float fT0 = hitNormalXZ.Dot(vMerikomi);
					////押し戻し返すベクトルを求める。
					////押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
					//Vector3 vOffset;
					//vOffset = hitNormalXZ * (-fT0 + _halfSize.x);
					//nextPosTmp += vOffset;
				}
			}
		}
	}

	////下方向を調べる。
	//{

	//	Vector3 NowToNext(endPos - startPos);
	//	NowToNext.x = 0.0f;
	//	NowToNext.z = 0.0f;
	//	if (NowToNext.Length() <= FLT_EPSILON) {
	//		//Y平面で動きがないので調べる必要なし。
	//		//FLT_EPSILONは1より大きい、最小の値との差分を表す定数。
	//		//とても小さい値のことです。

	//	}
	//	else {
	//		//レイを作成。
	//		btTransform start, end;
	//		start.setIdentity();
	//		end.setIdentity();
	//		//始点はカプセルコライダーの中心。
	//		// ※平らな地面に引っかからないよう少し上げる。
	//		start.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
	//		// ※平らな地面に引っかからないよう少し上げる。
	//		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));

	//		fbPhysicsCallback::SweepResultExtrude callback(Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z()), _collision->GetCollisionObj(), _attributeXZ);
	//		//衝突検出。
	//		PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)_collision->GetCollisionObj(), start, end, callback);

	//		if (callback.isHit) {
	//			//当たった。

	//			vector<unique_ptr<fbPhysicsCallback::SweepResultExtrude::hitInfo>> infoArray = callback.GetInfoArray();
	//			for (int idx = 0; idx < infoArray.size(); idx++) {
	//				Vector3 vT0, vT1;
	//				//めり込みが発生している移動ベクトルを求める。
	//				Vector3 vMerikomi;
	//				vMerikomi = (endPos - startPos) * (1.0f - infoArray[idx]->hitFraction);

	//				// テスト。
	//				CCharacterController* CC = infoArray[idx]->collision->gameObject->GetComponent<CCharacterController>();
	//				if (CC) {
	//					CC->AddOutsideSpeed(vMerikomi);
	//				}

	//				////XZ平面での衝突した壁の法線を求める。。
	//				//Vector3 hitNormalXZ = callback.hitNormal;
	//				//hitNormalXZ.y = 0.0f;
	//				//hitNormalXZ.Normalize();
	//				////めり込みベクトルを壁の法線に射影する。
	//				//float fT0 = hitNormalXZ.Dot(vMerikomi);
	//				////押し戻し返すベクトルを求める。
	//				////押し返すベクトルは壁の法線に射影されためり込みベクトル+半径。
	//				//Vector3 vOffset;
	//				//vOffset = hitNormalXZ * (-fT0 + _halfSize.x);
	//				//nextPosTmp += vOffset;
	//			}
	//		}
	//	}


		//Vector3 addPos = nextPosTmp - nowPosTmp;

		////レイを作成する。
		//btTransform start, end;
		//start.setIdentity();
		//end.setIdentity();
		////始点は仮確定したカプセルコライダーの中心位置(Y成分は移動前)。
		//start.setOrigin(btVector3(nextPosTmp.x, nextPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y, nextPosTmp.z));
		////終点は地面上にいない場合は1m下を見る。
		////地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
		////地面上にいなくて降下中の場合はそのまま落下先を調べる。
		//// ※この処理は坂道の処理で必要。
		//Vector3 endPos;
		//endPos = Vector3(start.getOrigin().getX(), start.getOrigin().getY(), start.getOrigin().getZ());
		//if (m_isOnGround == false) {
		//	if (addPos.y > 0.0f) {
		//		//ジャンプ中とかで上昇中。
		//		//上昇中でもXZに移動した結果めり込んでいる可能性がある(上り坂など)ので、移動後の座標から下を調べる。
		//		endPos.y -= addPos.y * 0.01f;
		//	}
		//	else {
		//		//落下している場合はそのまま下を調べる。
		//		endPos.y += addPos.y;
		//	}
		//}
		//else {
		//	//地面上にいる場合は1m下を見る。
		//	// 下り坂を降りるときにガタガタしないよう。
		//	endPos.y -= 1.0f;
		//}
		//end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		////スタートとエンドの差
		//btVector3 Sub = start.getOrigin() - end.getOrigin();
		////差が0.0001以上なら衝突検出する。
		//if (fabs(Sub.length()) > 0.0001f)
		//{
		//	//衝突検出。
		//	fbPhysicsCallback::SweepResultGround callback;
		//	callback.me = m_rigidBody->GetCollisionObj();
		//	callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
		//	callback._attribute = m_attributeY;

		//	PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
		//	if (callback.isHit) {
		//		//地面に接触している。

		//		if (gameObject == INSTANCE(GameObjectManager)->FindObject("Player")) {
		//			OutputDebugString("aa");
		//		}

		//		m_isJump = false;
		//		m_isOnGround = true;
		//		nextPosTmp.y = callback.hitPos.y;
		//	}
		//	else {
		//		//地面上にいない。

		//		m_isOnGround = false;
		//	}
		//}
	//}

	//nextPosTmp.y += m_rigidBody->GetShape()->GetHalfSize().y;	// 足元の位置に設定していたのでコリジョンの中心に戻す。
	//															//移動確定。
	//nextPosTmp -= m_rigidBody->GetOffset();
	//m_moveSpeed = nextPosTmp - transform->GetPosition();	// 衝突解決後の最終的な移動量を保存。
	//transform->SetPosition(nextPosTmp);	// nextPosTmpはモデルの原点とイコールでない可能性があるのでOffset分減算して設定する。
	//m_rigidBody->Update();	// transformを更新したのでコリジョンのTransfiormも更新する。
	//						//剛体を動かす。
	//m_rigidBody->GetCollisionObj()->setActivationState(DISABLE_DEACTIVATION);

	//return m_moveSpeed;
}
