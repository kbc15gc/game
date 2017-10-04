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

		struct hitInfo {
			Vector3 hitPosA = Vector3::zero;	// コリジョンAの衝突点。
			Vector3 hitPosB = Vector3::zero;	// コリジョンBの衝突点。
			Vector3 hitNormalB = Vector3::zero;	// コリジョンBの衝突法線。
			Collision* collision = nullptr;		// 衝突したコリジョン(コリジョンB)。
		};

		inline void SetHitInfoArray(vector<unique_ptr<hitInfo>>* hitInfo) {
			hitInfoArray = hitInfo;
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

				//属性が一致するかマスクをとる
				if (attribute & hitObjectTmp->GetCollisionObj()->getUserIndex())
				{

					//重複チェック
					for each (auto& info in *hitInfoArray)
					{
						//アドレス比較
						if (info->collision == hitObjectTmp)
						{

							if (!_IsThroughCollision(cp)) {
								// このコリジョンは無視しない。

								if (_IsNearLength(info.get(),cp)) {
									// 新しく衝突した点のほうがめり込み量が少ない。

									// 新しい情報で以前の情報を上書き。
									_ConfigInfo(info.get(), cp);
								}
							}
							return 0.0f;
						}
					}
					unique_ptr<hitInfo> newInfo;
					newInfo.reset(new hitInfo);

					if (!_IsThroughCollision(cp)) {
						newInfo->collision = hitObjectTmp;
						_ConfigInfo(newInfo.get(), cp);
						//コリジョンを追加
						hitInfoArray->push_back(move(newInfo));
					}
				}
			}
			return 0.0f;
		}

	private:
		void _ConfigInfo(hitInfo* info, btManifoldPoint& cp) {
			info->hitPosA = Vector3(cp.m_positionWorldOnA.x(), cp.m_positionWorldOnA.y(), cp.m_positionWorldOnA.z());
			info->hitPosB = Vector3(cp.m_positionWorldOnB.x(), cp.m_positionWorldOnB.y(), cp.m_positionWorldOnB.z());
			info->hitNormalB = Vector3(cp.m_normalWorldOnB.x(), cp.m_normalWorldOnB.y(), cp.m_normalWorldOnB.z());
		}

		// 無視するコリジョンか調べる。
		virtual bool _IsThroughCollision(btManifoldPoint& cp) {
			return false;
		}

		virtual bool _IsNearLength(hitInfo* info,btManifoldPoint& cp) {
			if (btVector3(cp.m_positionWorldOnA - cp.m_positionWorldOnB).length() < Vector3(info->hitPosA - info->hitPosB).Length()) {
				return true;
			}
			else {
				return false;
			}
		}

	public:
		int attribute;					//指定したコリジョン属性とのみ当たり判定をとる
		Collision* me = nullptr;		//自身のアドレス
	private:
		vector<unique_ptr<hitInfo>>* hitInfoArray = nullptr;	//ヒットしたオブジェクト
	};

	// 押し出しコンポーネント用。
	struct AllHitsContactResultExtrude : AllHitsContactResultCallback {
		// 無視するコリジョンか調べる。
		bool _IsThroughCollision(btManifoldPoint& cp)override {

			btVector3 newVec = cp.m_positionWorldOnA - cp.m_positionWorldOnB;

			// Y成分は除く。
			newVec.setY(0.0f);
			if (newVec.length() <= 0.001f) {
				// Y成分のめり込みだった。

				return true;
			}
			else {
				return false;
			}
		}

		bool _IsNearLength(hitInfo* info, btManifoldPoint& cp)override {
			btVector3 newVec = cp.m_positionWorldOnA - cp.m_positionWorldOnB;
			Vector3 nowVec = info->hitPosA - info->hitPosB;
			newVec.setY(0.0f);
			nowVec.y = 0.0f;

			if (newVec.length() < nowVec.Length()) {
				return true;
			}
			else {
				return false;
			}
		}
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
				(_attribute & convexResult.m_hitCollisionObject->getUserIndex()) > 0)
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
		int _attribute;										//指定したコリジョン属性との当たり判定を無視する。
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
		int	_attribute = 0;					//指定したコリジョン属性とのみ当たり判定をとる
		//bool isSlip = false;	// 滑るか。
		//float SlipAngle = 90.0f;			// 上方向との角度がこの角度以上なら滑る(度)。

		//衝突したときに呼ばれるコールバック関数。
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == me) {
				//自分に衝突した。
				return 0.0f;
			}

			//衝突点の法線を引っ張ってくる。
			D3DXVECTOR3 hitNormalTmp(convexResult.m_hitNormalLocal.x(), convexResult.m_hitNormalLocal.y(), convexResult.m_hitNormalLocal.z());
			//// ワールド行列取得。
			//D3DXMATRIX worldMat;
			//worldMat = static_cast<Collision*>(convexResult.m_hitCollisionObject->getUserPointer())->gameObject->transform->GetWorldMatrix();
			//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitNormalTmp), &hitNormalTmp, &worldMat);	// ワールド座標に変換。
			D3DXVec3Normalize(&hitNormalTmp, &hitNormalTmp);

			//上方向と衝突点の法線のなす角度を求める。
			float angle = fabsf(acosf(D3DXVec3Dot(&hitNormalTmp, &D3DXVECTOR3(0.0f, 1.0f, 0.0f))));

			if (((angle < D3DXToRadian(54.0f))		//地面の傾斜が54度より小さいので地面とみなす。
				&& (_attribute & convexResult.m_hitCollisionObject->getUserIndex()))
				|| convexResult.m_hitCollisionObject->getUserIndex() == (int)fbCollisionAttributeE::GROUND
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
					hitNormal = Vector3(hitNormalTmp.x, hitNormalTmp.y, hitNormalTmp.z);
					//if (angle >= D3DXToRadian(SlipAngle)) {
					//	// 指定角度以上なので滑らせる。
					//	isSlip = true;
					//}
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

			//衝突点の法線を引っ張ってくる。
			D3DXVECTOR3 hitNormalTmp(convexResult.m_hitNormalLocal.x(), convexResult.m_hitNormalLocal.y(), convexResult.m_hitNormalLocal.z());
			//// ワールド行列取得。
			//D3DXMATRIX worldMat;
			//worldMat = static_cast<Collision*>(convexResult.m_hitCollisionObject->getUserPointer())->gameObject->transform->GetWorldMatrix();
			//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitNormalTmp), &hitNormalTmp, &worldMat);	// ワールド座標に変換。
			//D3DXVec3Normalize(&hitNormalTmp, &hitNormalTmp);
			//上方向と衝突点の法線のなす角度を求める。
			float angle = fabsf(acosf(D3DXVec3Dot(&hitNormalTmp, &D3DXVECTOR3(0.0f, 1.0f, 0.0f))));
			if (((angle >= D3DXToRadian(54.0f))
				&& (_attribute & convexResult.m_hitCollisionObject->getUserIndex()))		//傾斜が54度以上かつ衝突を取りたい属性なので壁とみなす。
				|| convexResult.m_hitCollisionObject->getUserIndex() == static_cast<int>(fbCollisionAttributeE::GROUND)	// もしくは地面は抜けないようにする。
				) {
				isHit = true;

				D3DXVECTOR3 hitPosTmp(convexResult.m_hitPointLocal.x(), convexResult.m_hitPointLocal.y(), convexResult.m_hitPointLocal.z());
				//D3DXVec3Transform(&static_cast<D3DXVECTOR4>(hitPosTmp), &hitPosTmp, &worldMat);	// ワールド座標に変換。			


				//交点との距離を調べる。
				Vector3 vDistTmp;
				vDistTmp.Subtract(Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z), startPos);
				float distTmp = Vector3(vDistTmp.x, 0.0f, vDistTmp.z).Length();
				if (distTmp < dist) {
					if (convexResult.m_hitCollisionObject->getUserIndex() == BIT(3)) {
						OutputDebugString("aaa");
					}

					//この衝突点の方が近いので、最近傍の衝突点を更新する。
					hitPos = Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z);
					dist = distTmp;
					hitNormal = Vector3(hitNormalTmp.x, hitNormalTmp.y, hitNormalTmp.z);
				}
			}

			return 0.0f;
		}
	};

	// 移動した際に他のコリジョンを押し出す処理で使用。
	struct SweepResultExtrude : public btCollisionWorld::ConvexResultCallback
	{
		// 引数：	レイの始点。
		//			自分自身。自分自身との衝突を除外するためのメンバ。
		//			指定したコリジョン属性とのみ当たり判定をとる。
		SweepResultExtrude(const Vector3& start, btCollisionObject* me, int attribute) {
			_startPos = start;
			_me = me;
			_attribute = attribute;
		}

		~SweepResultExtrude() {
			for (auto info : _hitInfo) {
				SAFE_DELETE(info);
			}
			_hitInfo.clear();
		}

		struct hitInfo {
			Vector3 hitPos = Vector3::zero;		//衝突点。
			Vector3 hitNormal = Vector3::zero;	//衝突点の法線。
			Collision* collision = nullptr;	// 衝突したコリジョン。
			float hitFraction = 0.0f;	// 開始位置から終端位置までの長さと、開始位置から衝突した位置までの比。
		};

		const vector<hitInfo*>& GetInfoArray() const {
			return _hitInfo;
		}

		bool isHit = false;						//衝突フラグ。
	private:

		Vector3 _startPos = Vector3::zero;		//レイの始点。
		btCollisionObject* _me = nullptr;		//自分自身。自分自身との衝突を除外するためのメンバ。
		int	_attribute = 0;					//指定したコリジョン属性とのみ当たり判定をとる。

		vector<hitInfo*> _hitInfo;

	public:
		//衝突したときに呼ばれるコールバック関数。
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			if (convexResult.m_hitCollisionObject == _me) {
				//自分に衝突した。
				return 0.0f;
			}

			//衝突点の法線を引っ張ってくる。
			D3DXVECTOR3 hitNormalTmp(convexResult.m_hitNormalLocal.x(), convexResult.m_hitNormalLocal.y(), convexResult.m_hitNormalLocal.z());
			if (_attribute & convexResult.m_hitCollisionObject->getUserIndex()) {
				// 押し出す属性だった。

				isHit = true;

				D3DXVECTOR3 hitPosTmp(convexResult.m_hitPointLocal.x(), convexResult.m_hitPointLocal.y(), convexResult.m_hitPointLocal.z());
				hitInfo* info = new hitInfo;
				info->hitPos = Vector3(hitPosTmp.x, hitPosTmp.y, hitPosTmp.z);
				info->hitNormal = Vector3(hitNormalTmp.x, hitNormalTmp.y, hitNormalTmp.z);
				info->hitNormal = info->hitNormal * -1.0f;	// 欲しいのは自身の法線なので反転。
				info->collision = static_cast<Collision*>(convexResult.m_hitCollisionObject->getUserPointer());
				info->hitFraction = convexResult.m_hitFraction;	// 開始位置から終端位置までの長さと、開始位置から衝突した位置までの比を取得。

				_hitInfo.push_back(info);
			}

			return 0.0f;
		}
	};

}