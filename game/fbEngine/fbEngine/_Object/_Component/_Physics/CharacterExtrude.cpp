#include "fbstdafx.h"
#include "_Object\_Component\_Physics\CharacterExtrude.h"
#include "CharacterController.h"

void CharacterExtrude::Init(const vector<RigidBody*>& collisions, int attribute) {
	_collisions.clear();
	_halfSize.clear();

	// 押し出したい属性を設定。
	_attribute = attribute;

	_collisions = collisions;

	for (auto coll : _collisions) {
		btVector3 localScaling(1.0f, 1.0f, 1.0f); // Transformのスケール値。
		localScaling = coll->GetShape()->GetBody()->getLocalScaling();
		Vector3 halfSize = coll->GetShape()->GetHalfSize();
		halfSize.x *= localScaling.getX();
		halfSize.y *= localScaling.getY();
		halfSize.z *= localScaling.getZ();

		_halfSize.push_back(halfSize);
	}
}

void CharacterExtrude::Extrude(const Vector3& speed) {

	// スウィープテスト。
	{
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
	}


	// コンタクトテスト。
	{
		for (auto coll : _collisions) {
			fbPhysicsCallback::AllHitsContactResultCallback callback;
			vector<unique_ptr<fbPhysicsCallback::AllHitsContactResultCallback::hitInfo>> infoArray;
			//衝突検出。
			PhysicsWorld::Instance()->AllHitsContactTest(coll, &infoArray, &callback, _attribute);

			for (int idx = 0; idx < static_cast<int>(infoArray.size()); idx++) {
				Vector3 vT0, vT1;

				//めり込みが発生している移動ベクトルを求める。
				Vector3 vMerikomi;
				vMerikomi = infoArray[idx]->hitPosB - infoArray[idx]->hitPosA;
				vMerikomi.y = 0.0f;

				//押し出す方向を求める。
				Vector3 hitNormalXZ = infoArray[idx]->hitPosA - coll->GetOffsetPos();
				hitNormalXZ.y = 0.0f;
				hitNormalXZ.Normalize();

				//めり込みベクトルを法線に射影する。
				float fT0 = hitNormalXZ.Dot(vMerikomi);

				if (fabsf(fT0) <= 0.0f) {
					// Y成分のみのめり込みだった。

					// 例外処理。
					// とりあえず前方向に押し出す。
					hitNormalXZ = transform->GetForward();

					// 押し出す量を算出。
					Vector3 pos1 = coll->GetOffsetPos();
					Vector3 pos2 = infoArray[idx]->collision->GetOffsetPos();
					pos1.y = 0.0f;
					pos2.y = 0.0f;
					Vector3 work = pos2 - pos1;
					float fMerikomi = hitNormalXZ.Dot(work);
					fT0 = -(coll->GetShape()->GetHalfSize().z - fMerikomi);
				}

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
}
