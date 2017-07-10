#pragma once
#include "_Include\CollisionInclude.h"

namespace fbPhysicsCallback
{
	//ゴーストと重なっているペアを探すコールバック
	struct MyGhostPairCallback : public btGhostPairCallback
	{
	public:
		//重なったときに呼ばれる関数
		btBroadphasePair*	addOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1)
		{
			btCollisionObject* colObj0 = (btCollisionObject*)proxy0->m_clientObject;
			btCollisionObject* colObj1 = (btCollisionObject*)proxy1->m_clientObject;
			//ゴーストオブジェクトにアップキャスト
			//(元がゴーストオブジェクト以外ならnullになる)
			btGhostObject* ghost0 = btGhostObject::upcast(colObj0);
			btGhostObject* ghost1 = btGhostObject::upcast(colObj1);
			Collision* coll0 = (Collision*)colObj0->getUserPointer();
			Collision* coll1 = (Collision*)colObj1->getUserPointer();

			if (ghost0)
			{
				//ペア追加
				ghost0->addOverlappingObjectInternal(proxy1, proxy0);
			}
			if (ghost1)
			{
				//ペア追加
				ghost1->addOverlappingObjectInternal(proxy0, proxy1);				
			}
			return 0;
		}
		//重なりから抜けた時に呼ばれる関数
		void*	removeOverlappingPair(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1, btDispatcher* dispatcher)
		{
			btCollisionObject* colObj0 = (btCollisionObject*)proxy0->m_clientObject;
			btCollisionObject* colObj1 = (btCollisionObject*)proxy1->m_clientObject;
			//ゴーストオブジェクトにアップキャスト
			//(元がゴーストオブジェクト以外ならnullになる)
			btGhostObject* ghost0 = btGhostObject::upcast(colObj0);
			btGhostObject* ghost1 = btGhostObject::upcast(colObj1);
			if (ghost0)
			{
				ghost0->removeOverlappingObjectInternal(proxy1, dispatcher, proxy0);
			}
			if (ghost1)
			{
				ghost1->removeOverlappingObjectInternal(proxy0, dispatcher, proxy1);
			}
			return 0;
		}
	};

	//ヒットした中で最も近いものを返すコールバック
	struct ClosestContactResultCallback : public btCollisionWorld::ContactResultCallback
	{
	public:
		ClosestContactResultCallback()
		{

		}
		//衝突された時の関数
		virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
		{

			//ポジション取得
			const Vector3* vColl0Pos = (Vector3*)(&colObj0Wrap->getWorldTransform().getOrigin());
			const Vector3* vColl1Pos = (Vector3*)(&colObj1Wrap->getWorldTransform().getOrigin());
			Vector3 vDist;
			//自身と当たったオブジェクトとの距離を計算
			vDist.Subtract(*vColl0Pos, *vColl1Pos);
			//距離(2乗のままなのは計算を省くため)
			float distTmpSq = vDist.LengthSq();
			//一時的(temporary)にコリジョンを格納する
			Collision* hitObjectTmp;
			//近いなら
			if (distTmpSq < distSq) {
				// col0 と col1 のどちらかは自分なのでアドレスを比較して確かめる
				if (me->GetCollisionObj() == colObj0Wrap->getCollisionObject()) {
					hitObjectTmp = (Collision*)colObj1Wrap->getCollisionObject()->getUserPointer();
				}
				else {
					hitObjectTmp = (Collision*)colObj0Wrap->getCollisionObject()->getUserPointer();
				}
				//hitオブジェクトがある
				if (hitObjectTmp)
				{
					//属性が一致するか？マスクをとる
					if ((attribute & hitObjectTmp->GetCollisionObj()->getUserIndex()) != 0)
					{
						//距離を更新
						distSq = distTmpSq;
						//コリジョンを更新
						hitObject = hitObjectTmp;
					}
				}
			}

			return 0.0f;
		}
	public:
		int attribute;					//指定したコリジョン属性とのみ当たり判定をとる
		float distSq = FLT_MAX;			//距離を保持
		Collision* me = nullptr;		//自身のアドレス
		Collision* hitObject = nullptr;	//ヒットしたオブジェクト
	};

	//ヒットしたもの全てを返すコールバック
	struct AllHitsContactResultCallback : public btCollisionWorld::ContactResultCallback
	{
	public:
		AllHitsContactResultCallback()
		{

		}
		//衝突された時の関数
		virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
		{
			//一時的(temporary)にコリジョンを格納する
			Collision* hitObjectTmp = nullptr;

			// col0 と col1 のどちらかは自分なのでアドレスを比較して確かめる
			if (me->GetCollisionObj() == colObj0Wrap->getCollisionObject()) {
				hitObjectTmp = (Collision*)colObj1Wrap->getCollisionObject()->getUserPointer();
			}
			else {
				hitObjectTmp = (Collision*)colObj0Wrap->getCollisionObject()->getUserPointer();
			}
			
			//hitオブジェクトがある
			if (hitObjectTmp)
			{

				//属性が一致するか？マスクをとる
				if ((attribute & hitObjectTmp->GetCollisionObj()->getUserIndex()) != 0)
				{
					//重複チェック
					for each (Collision* coll in hitObjects)
					{
						//アドレス比較
						if (coll == hitObjectTmp)
						{
							return 0.0f;
						}
					}
					//コリジョンを追加
					hitObjects.push_back(hitObjectTmp);
				}
			}
			return 0.0f;
		}
	public:
		int attribute;					//指定したコリジョン属性とのみ当たり判定をとる
		Collision* me = nullptr;		//自身のアドレス
		vector<Collision*> hitObjects;	//ヒットしたオブジェクト
	};

	//レイを飛ばしてヒットした中で最も近いものを返す。
	struct	ClosestRayResultCallback : public btCollisionWorld::RayResultCallback
	{
	public:
		ClosestRayResultCallback(const btVector3& from, const btVector3& to, const int& attr)
		{
			_fromPos = from;
			_toPos = to;
			_attribute = attr;

			m_hitPointWorld = btVector3(0, 0, 0);
		}

		virtual	btScalar	addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			if ((_attribute & rayResult.m_collisionObject->getUserIndex()) == 0)
			{
				return 0.0f;
			}
			//一時的に
			float fracTmp = rayResult.m_hitFraction;
			//近いなら
			if (fracTmp < _Fraction)
			{
				//始点から終点を0～1にした時のヒットした位置の割合。
				_Fraction = m_closestHitFraction = fracTmp;
				//ヒットしたコリジョン
				m_collisionObject = rayResult.m_collisionObject;
				if (normalInWorldSpace)
				{
					m_hitNormalWorld = rayResult.m_hitNormalLocal;
				}
				else
				{
					///need to Transform normal into worldspace
					m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*rayResult.m_hitNormalLocal;
				}
				//ヒットした位置を取得
				m_hitPointWorld.setInterpolate3(_fromPos, _toPos, rayResult.m_hitFraction);
				//ヒットしたコリジョン取得
				hitObject = (Collision*)m_collisionObject->getUserPointer();
			}
			return rayResult.m_hitFraction;
		}
	private:
		btVector3	_fromPos;				//レイの始点
		btVector3	_toPos;					//レイの終点
		int _attribute;						//指定したコリジョン属性とのみ当たり判定をとる
		float _Fraction = FLT_MAX;			//比較用に保持
	public:
		btVector3	m_hitNormalWorld;		//衝突点の法線
		btVector3	m_hitPointWorld;		//レイがヒットした位置
		Collision* hitObject = nullptr;		//ヒットしたオブジェクト
	};

	//コリジョンを飛ばして最初にヒットしたコリジョンを返すコールバック
	struct ClosestConvexResultCallback : public btCollisionWorld::ConvexResultCallback
	{
	public:
		ClosestConvexResultCallback(const Vector3& start, const int& attr, const btCollisionObject* me)
		{
			_startPos = start;
			_attribute = attr;
			_me = me;
		}
		//衝突したときに呼ばれる関数。
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			//自身と衝突したか？ &&
			//属性が合わなかった
			if (convexResult.m_hitCollisionObject == _me ||
				(_attribute & convexResult.m_hitCollisionObject->getUserIndex()) == 0)
			{
				//ヒットしなかった。
				return 0.0f;
			}

			//ヒットした位置
			Vector3 hitPosTmp = *(Vector3*)&convexResult.m_hitPointLocal;
			//衝突点の距離を求める
			Vector3 vDist;
			vDist.Subtract(hitPosTmp, _startPos);
			float distTmp = vDist.Length();
			//距離比較
			if (_dist > distTmp) {
				m_closestHitFraction = convexResult.m_hitFraction;
				//更新
				hitPos = hitPosTmp;
				//法線更新
				hitNormal = *(Vector3*)&convexResult.m_hitNormalLocal;
				_dist = distTmp;
				hitObject = (Collision*)convexResult.m_hitCollisionObject->getUserPointer();
			}
			return 0.0f;
		}
	private:
		int _attribute;										//指定したコリジョン属性とのみ当たり判定をとる。
		float _dist = FLT_MAX;								//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
		Vector3 _startPos = Vector3::zero;					//レイの始点。
		const btCollisionObject* _me = nullptr;					//自分自身。自分自身との衝突を除外するためのメンバ。
	public:
		Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);		//衝突点。
		Vector3 hitNormal = Vector3::zero;					//衝突点の法線。
		Collision* hitObject = nullptr;						//ヒットしたオブジェクト
	};

	//衝突したときに呼ばれる関数オブジェクト(地面用)
	struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit = false;									//衝突フラグ。
		Vector3 hitPos = Vector3(0.0f, -FLT_MAX, 0.0f);	//衝突点。
		Vector3 startPos = Vector3::zero;					//レイの始点。
		Vector3 hitNormal = Vector3::zero;				//衝突点の法線。
		btCollisionObject* me = nullptr;					//自分自身。自分自身との衝突を除外するためのメンバ。
		float dist = FLT_MAX;								//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
		Vector3 vDist = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		vector<unsigned int> Through_ID;		// 無視するコリジョン属性。
		int	_attribute = 0;					//指定したコリジョン属性とのみ当たり判定をとる
		bool isSlip = false;	// 滑るか。
		float SlipAngle = 90.0f;			// 上方向との角度がこの角度以上なら滑る(度)。

		//衝突したときに呼ばれるコールバック関数。
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me) {
				//自分に衝突した。or キャラクタ属性のコリジョンと衝突した。
				return 0.0f;
			}

			if ((_attribute & convexResult.m_hitCollisionObject->getUserIndex()) == 0) {
				// 衝突を取りたいコリジョンではなかった。
				return 0.0f;
			}

			//衝突点の法線を引っ張ってくる。
			D3DXVECTOR3 hitNormalTmp(convexResult.m_hitNormalLocal.x(), convexResult.m_hitNormalLocal.y(), convexResult.m_hitNormalLocal.z());
			//// ワールド行列取得。
			D3DXMATRIX worldMat;
			worldMat = static_cast<Collision*>(convexResult.m_hitCollisionObject->getUserPointer())->gameObject->transform->GetWorldMatrix();
			//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitNormalTmp), &hitNormalTmp, &worldMat);	// ワールド座標に変換。
			//D3DXVec3Normalize(&hitNormalTmp, &hitNormalTmp);

			//上方向と衝突点の法線のなす角度を求める。
			float angle = fabsf(acosf(D3DXVec3Dot(&hitNormalTmp, &D3DXVECTOR3(0.0f, 1.0f, 0.0f))));

			if (((angle < D3DXToRadian(54.0f))		//地面の傾斜が54度より小さいので地面とみなす。
				|| convexResult.m_hitCollisionObject->getUserIndex() == (int)fbCollisionAttributeE::GROUND) //もしくはコリジョン属性が地面と指定されている。
				|| convexResult.m_hitCollisionObject->getUserIndex() == BIT(7)
				) {

				//衝突している。
				isHit = true;

				D3DXVECTOR3 hitPosTmp(convexResult.m_hitPointLocal.x(), convexResult.m_hitPointLocal.y(), convexResult.m_hitPointLocal.z());
				//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitPosTmp), &hitPosTmp, &worldMat);	// ワールド座標に変換。			

				//交点との距離を調べる。
				Vector3 vDistTmp;
				vDistTmp.Subtract(Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z), startPos);
				//vDistTmp.y = 0.0f;
				float distTmp = vDistTmp.Length();
				if (dist > distTmp) {
					//この衝突点の方が近いので、最近傍の衝突点を更新する。
					hitPos = Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z);
					dist = distTmp;
					vDist = vDistTmp;
					hitNormal = Vector3(hitNormalTmp.x, hitNormalTmp.y, hitNormalTmp.z);
					if (angle >= D3DXToRadian(SlipAngle)) {
						// 指定角度以上なので滑らせる。
						isSlip = true;
					}
				}
			}

			return 0.0f;
		}
	};

	struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
	{
		bool isHit = false;						//衝突フラグ。
		Vector3 hitPos = Vector3::zero;		//衝突点。
		Vector3 startPos = Vector3::zero;		//レイの始点。
		float dist = FLT_MAX;					//衝突点までの距離。一番近い衝突点を求めるため。FLT_MAXは単精度の浮動小数点が取りうる最大の値。
		Vector3 vDist = Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		Vector3 hitNormal = Vector3::zero;	//衝突点の法線。
		btCollisionObject* me = nullptr;		//自分自身。自分自身との衝突を除外するためのメンバ。
		int	_attribute = 0;					//指定したコリジョン属性とのみ当たり判定をとる

		//衝突したときに呼ばれるコールバック関数。
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me) {
				//自分に衝突した。
				return 0.0f;
			}

			if ((_attribute & convexResult.m_hitCollisionObject->getUserIndex()) == 0) {
				// 衝突を取りたいコリジョンではなかった。
				return 0.0f;
			}

			//衝突点の法線を引っ張ってくる。
			D3DXVECTOR3 hitNormalTmp(convexResult.m_hitNormalLocal.x(), convexResult.m_hitNormalLocal.y(), convexResult.m_hitNormalLocal.z());		
			//// ワールド行列取得。
			D3DXMATRIX worldMat;
			worldMat = static_cast<Collision*>(convexResult.m_hitCollisionObject->getUserPointer())->gameObject->transform->GetWorldMatrix();
			//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitNormalTmp), &hitNormalTmp, &worldMat);	// ワールド座標に変換。
			//D3DXVec3Normalize(&hitNormalTmp, &hitNormalTmp);
			//上方向と衝突点の法線のなす角度を求める。
			float angle = fabsf(acosf(D3DXVec3Dot(&hitNormalTmp,&D3DXVECTOR3(0.0f,1.0f,0.0f))));
			if (((angle >= D3DXToRadian(54.0f))		//地面の傾斜が54度以上なので壁とみなす。
				|| convexResult.m_hitCollisionObject->getUserIndex() == (int)fbCollisionAttributeE::GROUND)
					|| convexResult.m_hitCollisionObject->getUserIndex() == (int)fbCollisionAttributeE::CHARACTER
					|| convexResult.m_hitCollisionObject->getUserIndex() == BIT(7)
				) {
				isHit = true;

				D3DXVECTOR3 hitPosTmp(convexResult.m_hitPointLocal.x(), convexResult.m_hitPointLocal.y(), convexResult.m_hitPointLocal.z());
				//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitPosTmp), &hitPosTmp, &worldMat);	// ワールド座標に変換。			

				//交点との距離を調べる。
				Vector3 vDistTmp;
				vDistTmp.Subtract(Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z), startPos);
				float distTmp = Vector3(vDistTmp.x,0.0f, vDistTmp.z).Length();
				if (distTmp < dist) {
					//この衝突点の方が近いので、最近傍の衝突点を更新する。
					hitPos = Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z) ;
					dist = distTmp;
					vDist = vDistTmp;
					hitNormal = Vector3(hitNormalTmp.x, hitNormalTmp.y, hitNormalTmp.z);
				}
			}

			return 0.0f;
		}
	};
}