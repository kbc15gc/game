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
	//Vector3 startPos;
	//Vector3 endPos;
	//startPos = endPos = _collision->GetOffsetPos();	// 移動前のコリジョンの座標(原点)を設定。
	//startPos -= speed;	// スタート位置を移動前の位置に設定。

	////XZ平面での衝突検出と衝突解決を行う。
	//{
	//	Vector3 NowToNext(endPos - startPos);
	//	NowToNext.y = 0.0f;
	//	if (NowToNext.Length() <= FLT_EPSILON) {
	//		//XZ平面で動きがないので調べる必要なし。
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
	//		start.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));
	//		// ※平らな地面に引っかからないよう少し上げる。
	//		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
	//		end.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().y, transform->GetRotation().z, transform->GetRotation().w));

	//		fbPhysicsCallback::SweepResultExtrude callback(Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z()), _collision->GetCollisionObj(), _attributeXZ);
	//		//衝突検出。
	//		PhysicsWorld::Instance()->ConvexSweepTest(static_cast<const btConvexShape*>(_collision->GetShape()->GetBody()), start, end, callback);

	//		if (callback.isHit) {
	//			//当たった。

	//			vector<fbPhysicsCallback::SweepResultExtrude::hitInfo*> infoArray = callback.GetInfoArray();
	//			for (int idx = 0; idx < infoArray.size(); idx++) {
	//				Vector3 vT0, vT1;

	//				// 衝突したポイントを求める。
	//				Vector3 hitPos = startPos + ((endPos - startPos) * infoArray[idx]->hitFraction);

	//				//めり込みが発生している移動ベクトルを求める。
	//				Vector3 vMerikomi;
	//				vMerikomi = endPos - hitPos;
	//				vMerikomi.y = 0.0f;

	//				//押し出す方向を求める。
	//				Vector3 hitNormalXZ = infoArray[idx]->hitPos - hitPos;
	//				hitNormalXZ.y = 0.0f;
	//				hitNormalXZ.Normalize();
	//				Vector3 dir = endPos - startPos;
	//				dir.y = 0.0f;
	//				dir.Normalize();
	//				float dot = hitNormalXZ.Dot(dir);
	//				if (hitNormalXZ.Dot(dir) < 0.0f) {
	//					// 当たったコリジョンが進行方向の反対側にいる。

	//					// 進む方向に押し出すので無視。
	//					return;
	//				}

	//				//めり込みベクトルを法線に射影する。
	//				float fT0 = hitNormalXZ.Dot(vMerikomi);
	//				//押し返すベクトルを求める。
	//				//押し返すベクトルは法線に射影されためり込みベクトル。
	//				Vector3 vOffset;
	//				vOffset = hitNormalXZ * fT0;
	//				vOffset = vOffset / Time::DeltaTime();

	//				// 衝突したコリジョンに力を加える。
	//				CCharacterController* CC = infoArray[idx]->collision->gameObject->GetComponent<CCharacterController>();
	//				if (CC) {
	//					CC->AddOutsideSpeed(vOffset);
	//				}
	//			}
	//		}
	//	}
	//}


	// コンタクトテスト。
	{
		fbPhysicsCallback::AllHitsContactResultCallback callback;
		callback.me = _collision;
		callback.attribute = _attributeXZ;
		vector<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo*> infoArray;
		//衝突検出。
		PhysicsWorld::Instance()->AllHitsContactTest(_collision, infoArray, _attributeXZ);

		for (int idx = 0; idx < infoArray.size(); idx++) {
			Vector3 vT0, vT1;

			//めり込みが発生している移動ベクトルを求める。
			Vector3 vMerikomi;
			vMerikomi = infoArray[idx]->hitPosB - infoArray[idx]->hitPosA;
			vMerikomi.y = 0.0f;

			//押し出す方向を求める。
			Vector3 hitNormalXZ = infoArray[idx]->hitPosA - _collision->GetOffsetPos();
			hitNormalXZ.y = 0.0f;
			hitNormalXZ.Normalize();
			//Vector3 dir = endPos - startPos;
			//dir.y = 0.0f;
			//dir.Normalize();
			//float dot = hitNormalXZ.Dot(dir);
			//if (hitNormalXZ.Dot(dir) < 0.0f) {
			//	// 当たったコリジョンが進行方向の反対側にいる。

			//	// 進む方向に押し出すので無視。
			//	return;
			//}

			//めり込みベクトルを法線に射影する。
			float fT0 = hitNormalXZ.Dot(vMerikomi);
			//押し返すベクトルを求める。
			//押し返すベクトルは法線に射影されためり込みベクトル。
			Vector3 vOffset;
			vOffset = hitNormalXZ * -fT0;
			vOffset = vOffset / Time::DeltaTime();

			// 衝突したコリジョンに力を加える。
			CCharacterController* CC = infoArray[idx]->collision->gameObject->GetComponent<CCharacterController>();
			if (CC) {
				CC->AddOutsideSpeed(vOffset);
			}
		}
	}
}
