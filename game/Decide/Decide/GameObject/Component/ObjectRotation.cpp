#include"stdafx.h"
#include "ObjectRotation.h"

void ObjectRotation::RotationToObject_XZ(const GameObject& Object) {
	Vector3 MyToObject = Object.transform->GetPosition() - gameObject->transform->GetPosition();
	MyToObject.Normalize();

	// 向かせたい向きのXとZから回転角度を求める。
	// ※求まるのは絶対角度。
	float angle  = atan2f(MyToObject.x, MyToObject.z);
	// クォータニオン作成。
	Quaternion Rota = Quaternion::Identity;
	Rota.SetRotation(Vector3::axisY, angle);

	// クォータニオン設定。
	gameObject->transform->SetRotation(Rota);


	// 外積と内積を使った回転。
	// ※一応残しとく。
	{
		//D3DXMATRIX work = gameObject->transform->GetWorldMatrix();
		//Vector3 MyDirection = Vector3(work.m[2][0], work.m[2][1], work.m[2][2]);
		//MyDirection.y = 0.0f;	// Y軸成分は除く。
		//MyDirection.Normalize();

		//Vector3 MyToObject = Object.transform->GetPosition() - gameObject->transform->GetPosition();
		//MyToObject.y = 0.0f;	// Y軸成分は除く。
		//MyToObject.Normalize();

		//// 今の向きとプレイヤーまでの向きの角度を算出。

		//float rad = acosf(MyToObject.Dot(MyDirection));

		////// 1フレーム前の回転方向。
		////static float PrevAxisY = 0.0f;

		//if (rad > 0.001f) {
		//	// プレイヤーのほうを向いていない。

		//	// 回転方向算出。

		//	D3DXVECTOR3 Axis;

		//	D3DXVec3Cross(&Axis, &D3DXVECTOR3(MyDirection.x, MyDirection.y, MyDirection.z), &D3DXVECTOR3(MyToObject.x, MyToObject.y, MyToObject.z));
		//	//Vector3 Axis = EnemyToPlayer;
		//	//Axis.Cross(EnemyDir);
		//	//Axis.Normalize();


		//	//if (PrevAxisY >= 0.0001f) {
		//	//	// 初期値じゃない。

		//	//	if (Axis.y * PrevAxisY < 0.0f) {
		//	//		// 回転方向が反転したので強制的に戻す。
		//	//		OutputDebugString(_T("あああ"));
		//	//		Axis *= -1.0f;	// 再反転。
		//	//	}
		//	//}

		//	//// 更新。
		//	//PrevAxisY = Axis.y;

		//	// クォータニオンで回転。
		//	Quaternion quat = Quaternion::Identity;
		//	quat.SetRotation(Axis, rad);
		//	Quaternion Rotation = gameObject->transform->GetRotation();
		//	quat.Multiply(Rotation);
		//	gameObject->transform->SetLocalAngle(quat.GetAngle());
		//}
		////else {
		////	PrevAxisY = 0.0f;
		////}
	}
}
