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
	m_moveSpeed.y += m_gravity;
	Vector3 nowPosTmp; 
	Vector3 nextPosTmp;
	Vector3 originalXZDir;
	//次の移動先となる座標を計算する。
	{
		nextPosTmp = nowPosTmp = m_rigidBody->GetOffsetPos();	// 移動前のコリジョンの座標(原点)を設定。
		nextPosTmp.y = nowPosTmp.y = nowPosTmp.y - m_rigidBody->GetShape()->GetHalfSize().y;	// 位置情報をコリジョンの足元に合わせる。
		//速度からこのフレームでの移動量を求める。オイラー積分。
		Vector3 addPos = m_moveSpeed;
		addPos.Scale(Time::DeltaTime());
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
			//始点はカプセルコライダーの足元の地点。
			// ※平らな地面に引っかからないよう少し上げる。
			start.setOrigin(btVector3(nowPosTmp.x, nowPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y + 0.2f, nowPosTmp.z));
			// ※平らな地面に引っかからないよう少し上げる。
			end.setOrigin(btVector3(nextPosTmp.x, nowPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y + 0.2f, nextPosTmp.z));

			fbPhysicsCallback::SweepResultWall callback;
			callback.me = m_rigidBody->GetCollisionObj();
			callback.startPos = Vector3(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
			callback._attribute = m_attributeXZ;
			//衝突検出。
			PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);

			if (callback.isHit) {
				//当たった。
				//壁。
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

		Vector3 addPos = nextPosTmp - nowPosTmp;

		//レイを作成する。
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//始点は仮確定したカプセルコライダーの中心位置(Y成分は移動前)。
		start.setOrigin(btVector3(nextPosTmp.x, nextPosTmp.y + m_rigidBody->GetShape()->GetHalfSize().y, nextPosTmp.z));
		//終点は地面上にいない場合は1m下を見る。
		//地面上にいなくてジャンプで上昇中の場合は上昇量の0.01倍下を見る。
		//地面上にいなくて降下中の場合はそのまま落下先を調べる。
		// ※この処理は坂道の処理で必要。
		Vector3 endPos;
		endPos = Vector3(start.getOrigin().getX(), start.getOrigin().getY(), start.getOrigin().getZ());
		if (m_isOnGround == false) {
			if (addPos.y > 0.0f) {
				//ジャンプ中とかで上昇中。
				//上昇中でもXZに移動した結果めり込んでいる可能性がある(上り坂など)ので、移動後の座標から下を調べる。
				endPos.y -= addPos.y * 0.01f;
			}
			else {
				//落下している場合はそのまま下を調べる。
				endPos.y += addPos.y;
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
		//差が0.0001以上なら衝突検出する。
		if (fabs(Sub.length()) > 0.0001f)
		{
			//衝突検出。
			fbPhysicsCallback::SweepResultGround callback;
			callback.me = m_rigidBody->GetCollisionObj();
			callback.startPos.Set(start.getOrigin().x(), start.getOrigin().y(), start.getOrigin().z());
			callback._attribute = m_attributeY;

			PhysicsWorld::Instance()->ConvexSweepTest((const btConvexShape*)m_collider->GetBody(), start, end, callback);
			if (callback.isHit) {
				//地面に接触している。

				m_moveSpeed.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosTmp.y = callback.hitPos.y;
			}
			else {
				//地面上にいない。

				m_isOnGround = false;
			}
		}
	}
#endif

	nextPosTmp.y += m_rigidBody->GetShape()->GetHalfSize().y;	// 足元の位置に設定していたのでコリジョンの中心に戻す。
	//移動確定。
	transform->SetPosition(nextPosTmp - m_rigidBody->GetOffset());	// nextPosTmpはモデルの原点とイコールでない可能性があるのでOffset分減算して設定する。
	m_rigidBody->Update();	// transformを更新したのでコリジョンのTransfiormも更新する。
	//剛体を動かす。
	m_rigidBody->GetCollisionObj()->setActivationState(DISABLE_DEACTIVATION);
}
