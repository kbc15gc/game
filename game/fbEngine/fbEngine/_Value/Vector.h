/*!
 * @brief	ベクトル
 */

#pragma once

//二次元ベクトル
class Vector2 {
public:
	//共用体
	union {
		struct { float x, y; };
		float in[2];
	};
	static const Vector2 zero;
	static const Vector2 right;
	static const Vector2 left;
	static const Vector2 up;
	static const Vector2 down;
	static const Vector2 one;

public:

	Vector2() 
	{
		this->x = this->y = 0;
	}

	Vector2(float xy)
	{
		Set(xy, xy);
	}

	Vector2(const float x,const float y)
	{
		Set(x, y);
	}
	
	void Set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	/*!
	* @brief	長さを取得
	*/
	float Length()
	{
		return sqrt(LengthSq());
	}
	/*!
	* @brief	長さの二乗を取得
	*/
	float LengthSq()
	{
		return x * x + y * y;
	}
	//内積
	float Dot(const Vector2& in)
	{
		return x * in.x + y * in.y;
	}
	//外積
	float Cross(const Vector2& in)
	{
		return x * in.y - y * in.x;
	}
	//角度＋回転方向を求める（単位はラジアン）
	float Rot()
	{
		//上向きのベクトルとの角度をとる。
		return atan2f(this->Cross(Vector2(0, 1)), this->Dot(Vector2(0, 1)));
	}
	//正規化
	void Normalize()
	{
		float len = Length();
		x /= len;
		y /= len;
	}

	Vector2 operator + (const Vector2& in) const
	{
		Vector2 vec;
		vec.x = this->x + in.x;
		vec.y = this->y + in.y;
		return vec;
	}

	Vector2 operator - (const Vector2& in) const
	{
		Vector2 vec;
		vec.x = this->x - in.x;
		vec.y = this->y - in.y;
		return vec;
	}

	Vector2 operator * (const Vector2& in) const
	{
		Vector2 vec;
		vec.x = this->x * in.x;
		vec.y = this->y * in.y;
		return vec;
	}

	Vector2 operator * (const float& in) const
	{
		Vector2 vec;
		vec.x = this->x * in;
		vec.y = this->y * in;
		return vec;
	}

	Vector2 operator / (const Vector2& in) const
	{
		Vector2 vec;
		vec.x = this->x / in.x;
		vec.y = this->y / in.y;
		return vec;
	}

	Vector2 operator / (const float& in) const
	{
		Vector2 vec;
		vec.x = this->x / in;
		vec.y = this->y / in;
		return vec;
	}

	Vector2 operator *= (const float& in)
	{
		this->x *= in;
		this->y *= in;
		return *this;
	}

	Vector2 operator /= (const float& in)
	{
		this->x /= in;
		this->y /= in;
		return *this;
	}
};

//三次元ベクトル
class Vector3{
public:
	union{
		struct { float x, y, z; };
		float in[3];
	};
	static const Vector3 zero;
	static const Vector3 right;
	static const Vector3 left;
	static const Vector3 up;
	static const Vector3 down;
	static const Vector3 front;
	static const Vector3 back;
	static const Vector3 axisX;
	static const Vector3 axisY;
	static const Vector3 axisZ;
	static const Vector3 one;
public:
	operator D3DXVECTOR3() 
	{
		return D3DXVECTOR3(x, y, z);
	}
	operator LPVOID() { return static_cast<LPVOID>(this); }
	
	Vector3() 
	{
		this->x = this->y = this->z = 0;
	}
	//コンストラクタ
	Vector3(float xyz)
	{
		Set(xyz, xyz, xyz);
	}
	Vector3(float x, float y, float z)
	{
		Set(x, y, z);
	}
	Vector3(const Vector2& v2,float z = 0.0f)
	{
		this->x = v2.x;
		this->y = v2.y;
		this->z = z;
	}
	Vector3(D3DXVECTOR3& in)
	{
		this->x = in.x;
		this->y = in.y;
		this->z = in.z;
	}

	//Vector3をD3DXVECTOR3に変換コピーする
	void CopyFrom(D3DXVECTOR3& in) const
	{
		in.x = x;
		in.y = y;
		in.z = z;
	}
	template<class TVector>
	void CopyTo(TVector& dst) const
	{
		dst.x = x;
		dst.y = y;
		dst.z = z;
	}
	/*!
	* @brief	ベクトルの各要素を設定。
	*/
	void Set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	template<class TVector>
	void Set(TVector& dst)
	{
		this->x = dst.x;
		this->y = dst.y;
		this->z = dst.z;
	}
	/*!
	 * @brief	ベクトルを加算。
	 */
	void Add(const Vector3& in)
	{
		x += in.x;
		y += in.y;
		z += in.z;
	}
	void Add(const Vector3& v0, const Vector3& v1)
	{
		x = v0.x + v1.x;
		y = v0.y + v1.y;
		z = v0.z + v1.z;
	}
	/*!
	 * @brief	ベクトルを減算。
	 */
	void Subtract(const Vector3& in)
	{
		x -= in.x;
		y -= in.y;
		z -= in.z;
	}
	void Subtract(const Vector3& v0, const Vector3& v1)
	{
		x = v0.x - v1.x;
		y = v0.y - v1.y;
		z = v0.z - v1.z;
	}
	/*!
	 * @brief	内積。
	 */
	float Dot(const Vector3& in)const
	{
		return min(1.0f,max(-1.0f,x * in.x + y * in.y + z * in.z));	// -1.0 〜 1.0が正しい結果。わずかでも範囲外の値(1.0000002fなど)をacosf関数などに渡すとnanとなるため、対応しておく。
	}
	/*!
	 * @brief	外積。
	 */
	Vector3 Cross(const Vector3& in)const
	{
		float _x = (y * in.z) - (z * in.y);
		float _y = (z * in.x) - (x * in.z);
		float _z = (x * in.y) - (y * in.x);
		return Vector3(_x, _y, _z);
	}
	void Cross(const Vector3& v0, const Vector3& v1)
	{
		float _x = (v0.y * v1.z) - (v1.y * v0.z);
		float _y = (v0.z * v1.x) - (v1.z * v0.x);
		float _z = (v0.x * v1.y) - (v1.x * v0.y);
		x = _x;
		y = _y;
		z = _z;
	}

	/*!
	 * @brief	長さを取得
	 */
	float Length() const
	{
		return sqrt(LengthSq());
	}
	/*!
	 * @brief	長さの二乗を取得
	 */
	float LengthSq() const
	{
		return x * x + y * y + z * z;
	}
	/*!
	* @brief	拡大。
	*/
	void Scale(float s)
	{
		x *= s;
		y *= s;
		z *= s;
	}
	/*!
	* @brief	法線を正規化。
	*/
	void Normalize()
	{
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
	}
	/*!
	* @brief	除算。
	*/
	void Div(float d)
	{
		x /= d;
		y /= d;
		z /= d;
	}
	//線形補完
	//補完先、割合(0.0f~1.0f)
	void Lerp(const Vector3& in,float wariai)
	{
		wariai = min(1.0f, max(0.0f, wariai));
		//ベクトルを求める
		Vector3 ToI = in - *this;
		*this += (ToI * wariai);
	}

	static Vector3 Lerp(const Vector3& a, const Vector3& b,float wariai)
	{
		wariai = min(1.0f, max(0.0f, wariai));
		//ベクトルを求める
		Vector3 To = b - a;
		return a + (To * wariai);
	}

	//
	void Transform(const D3DXMATRIX& matrix)
	{
		D3DXVECTOR4 out;
		D3DXVECTOR3 vec3(x, y, z);
		D3DXVec3Transform(&out, &vec3, &matrix);
		//自身に代入。
		this->x = out.x;
		this->y = out.y;
		this->z = out.z;
	}

	void operator = (D3DXVECTOR4 in)
	{
		this->x = in.x;
		this->y = in.y;
		this->z = in.z;
	}

	void operator += (Vector3 in)
	{
		this->x += in.x;
		this->y += in.y;
		this->z += in.z;
	}

	void operator -= (Vector3 in) {
		this->x -= in.x;
		this->y -= in.y;
		this->z -= in.z;
	}

	Vector3 operator + (Vector3 in) const
	{
		Vector3 out;
		out.x = this->x + in.x;
		out.y = this->y + in.y;
		out.z = this->z + in.z;
		return out;
	}

	Vector3 operator + (D3DXVECTOR4 in) const
	{
		Vector3 out;
		out.x = this->x + in.x;
		out.y = this->y + in.y;
		out.z = this->z + in.z;
		return out;
	}

	Vector3 operator - (Vector3 in) const
	{
		Vector3 out;
		out.x = this->x - in.x;
		out.y = this->y - in.y;
		out.z = this->z - in.z;
		return out;
	}

	Vector3 operator * (Vector3 in) const
	{
		Vector3 out;
		out.x = this->x * in.x;
		out.y = this->y * in.y;
		out.z = this->z * in.z;
		return out;
	}

	Vector3 operator * (float in) const
	{
		Vector3 out;
		out.x = this->x * in;
		out.y = this->y * in;
		out.z = this->z * in;
		return out;
	}

	Vector3 operator / (Vector3 in) const
	{
		Vector3 out;
		out.x = this->x / in.x;
		out.y = this->y / in.y;
		out.z = this->z / in.z;
		return out;
	}

	Vector3 operator / (float in) const
	{
		Vector3 out;
		out.x = this->x / in;
		out.y = this->y / in;
		out.z = this->z / in;
		return out;
	}

	//// 受け取ったベクトルとの角度を求める(ラジアン)。
	//float ToRadian(const Vector3& vec)const {
	//	Vector3 dir = Vector3(x, y, z);
	//	dir.Normalize();
	//	Vector3 work = vec;
	//	work.Normalize();
	//	float rad = dir.Dot(work);
	//	float radDir = 1.0f;
	//	// 0.0から2.0の範囲にして角度の判定。
	//	if (rad + 1.0f < 0.0f) {
	//		// 符号が-。
	//		radDir *= -1.0f;
	//	}

	//	rad = acosf(rad);
	//	if (rad > D3DXToRadian(180.0f)) {
	//		rad = (D3DXToRadian(360.0f) - rad) * -1.0f;
	//	}
	//	rad *= radDir;
	//	return rad;
	//}

	// 受け取ったベクトルとの直交ベクトルを求める。
	Vector3 ToCrossVec(const Vector3& vec) {
		Vector3 work1 = Vector3(x,y,z);
		work1.Normalize();
		Vector3 work2 = vec;
		work2.Normalize();
		float rad = work1.Dot(work2);

		if (fabsf(rad) == 1.0f) {
			// 二つのベクトルが同じ向きか真逆を向いている。

			if (fabsf(work1.Dot(Vector3::up)) == 1.0f) {
				return work1.Cross(Vector3::right);
			}
			else {
				return work1.Cross(Vector3::up);
			}
		}
		else {
			return work1.Cross(work2);
		}
	}
};
	
//四次元ベクトル
class Vector4{
public:
	union{
		struct { float x, y, z, w; };
		float in[4];
	};
public:
	operator D3DXVECTOR4(void) { return static_cast<D3DXVECTOR4>(*this); }
	Vector4(){}
	/*!
	 *@brief	コンストラクタ
	 */
	Vector4(float x, float y, float z, float w)
	{
		Set(x, y, z, w);
	}
	Vector4(const Vector3& vec3, float w) {
		Set(vec3.x, vec3.y, vec3.z, w);
	}
	/*!
	 *@brief	ベクトルの各要素を設定。
	 */
	void Set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	/*!
	 *@brief	ベクトルを加算。
	 */
	void Add(const Vector4& in)
	{
		x += in.x;
		y += in.y;
		z += in.z;
		w += in.w;
	}
	void Add(const Vector4& v0, const Vector4& v1)
	{
		x = v0.x + v1.x;
		y = v0.y + v1.y;
		z = v0.z + v1.z;
		w = v0.w + v1.w;
	}
	/*!
	 *@brief	ベクトルを減算。
	 */
	void Subtract(const Vector4& in)
	{
		x -= in.x;
		y -= in.y;
		z -= in.z;
	}
	void Subtract(const Vector4& v0, const Vector4& v1)
	{
		x = v0.x - v1.x;
		y = v0.y - v1.y;
		z = v0.z - v1.z;
		w = v0.w - v1.w;
	}
	/*!
	 *@brief	内積
	 */
	float Dot(const Vector4& in)
	{
		return x * in.x + y * in.y + z * in.z + w * in.w;
	}
	/*!
	 * @brief	長さを取得
	 */
	float Length()
	{
		return sqrt(LengthSq());
	}
	/*!
	 * @brief	長さの二乗を取得
	 */
	float LengthSq()
	{
		return x * x + y * y + z * z + w * w;
	}
	//正規化
	void Normalize()
	{
		//長さを取得
		float len = Length();
		//各要素を長さで割る。
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}
	/*!
	* @brief	拡大。
	*/
	void Scale(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
	}

	Vector4 operator + (Vector4 in)
	{
		Vector4 out;
		out.x = this->x + in.x;
		out.y = this->y + in.y;
		out.z = this->z + in.z;
		out.w = this->w + in.w;
		return out;
	}

	Vector4 operator - (Vector4 in)
	{
		Vector4 out;
		out.x = this->x - in.x;
		out.y = this->y - in.y;
		out.z = this->z - in.z;
		out.w = this->w - in.w;
		return out;
	}

	Vector4 operator * (Vector4 in)
	{
		Vector4 out;
		out.x = this->x * in.x;
		out.y = this->y * in.y;
		out.z = this->z * in.z;
		out.w = this->w * in.w;
		return out;
	}

	Vector4 operator / (Vector4 in)
	{
		Vector4 out;
		out.x = this->x / in.x;
		out.y = this->y / in.y;
		out.z = this->z / in.z;
		out.w = this->w / in.w;
		return out;
	}
};
	
class Quaternion : public Vector4{
public:
	static const Quaternion Identity;		//!<単位クォータニオン。
	Quaternion() 
	{
		this->x = this->y = this->z = 0.0f;
		this->w = 1.0f;
	}
	Quaternion(float x, float y, float z, float w) :
		Vector4(x, y, z, w)
	{
	}

	Quaternion(Vector4 v) :
		Vector4(v)
	{
	}
	//D3DXVECTOR3をVector3に変換
	Quaternion(D3DXQUATERNION& q)
	{
		this->x = q.x;
		this->y = q.y;
		this->z = q.z;
		this->w = q.w;
	}

	/*!
	 *@brief	任意の軸周りの回転クォータニオンを作成。
	 */
	void SetRotation(const Vector3& axis, float angle)
	{
		float s;
		float halfAngle = angle * 0.5f;
		s = sin(halfAngle);
		w = cos(halfAngle);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}
	//オイラー角度からクォータニオン作成
	void SetEuler(Vector3 ang)
	{
		D3DXQUATERNION q;
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(ang.y), D3DXToRadian(ang.x), D3DXToRadian(ang.z));
		(*this) = q;
	}

	// ベクトル1からベクトル2までの回転クォータニオンを作成する。
	void CreateVector3ToVector3(const Vector3& vec1, const Vector3& vec2) {
		Vector3 work1 = vec1;
		work1.Normalize();
		Vector3 work2 = vec2;
		work2.Normalize();
		float rad = work1.Dot(work2);

		Vector3 axis;
		if (fabsf(rad) == 1.0f) {
			// 二つのベクトルが同じ向きか真逆を向いている。

			if (fabsf(work1.Dot(Vector3::up)) == 1.0f) {
				axis = work1.Cross(Vector3::right);
			}
			else {
				axis = work1.Cross(Vector3::up);
			}
		}
		else {
			axis = work1.Cross(work2);
		}

		SetRotation(axis, acosf(rad));
	}

	// 引数で受け取ったベクトルをこのクォータニオンで回す。
	Vector3 RotationVector3(const Vector3& vec)const {

		// 回転させるために行列を作成。
		D3DXMATRIX rotMat;
		D3DXMatrixIdentity(&rotMat);
		// Z方向を回転させたいベクトルにする。
		Vector3 work = vec;
		work.Normalize();
		rotMat.m[2][0] = work.x;
		rotMat.m[2][1] = work.y;
		rotMat.m[2][2] = work.z;

		Vector3 vecX, vecY;
		// 回転させたいベクトルの横方向を求める。
		if (fabsf(work.Dot(Vector3::up)) == 1.0f) {
			// 受け取ったベクトルが上を向いている。

			// 上方向との外積を行っても直交するベクトルは求まらないので奥行方向と外積を取る。
			vecX = work.Cross(Vector3::front);
		}
		else {
			// ワールド座標の上方向と外積を行う。
			vecX = work.Cross(Vector3::up);
		}

		rotMat.m[0][0] = vecX.x;
		rotMat.m[0][1] = vecX.y;
		rotMat.m[0][2] = vecX.z;

		// 回転させたいベクトルの上方向を求める。
		vecY = work.Cross(vecX);

		rotMat.m[1][0] = vecY.x;
		rotMat.m[1][1] = vecY.y;
		rotMat.m[1][2] = vecY.z;

		// このクォータニオンから行列を作成して回転させる。
		D3DXMATRIX mat = Quaternion::GetRotationMatrix();
		D3DXMatrixMultiply(&rotMat, &rotMat,&mat);

		// 回転後の奥行ベクトルを取得。
		Vector3 dir;
		dir.x = rotMat.m[2][0];
		dir.y = rotMat.m[2][1];
		dir.z = rotMat.m[2][2];

		dir.Normalize();
		// 回転する際に向きベクトルにしたので、元のベクトルの大きさを掛ける。
		dir = dir * vec.Length();

		return dir;
	}

	//回転行列取得
	const D3DXMATRIX GetRotationMatrix()const
	{
		D3DXMATRIX rot;
		D3DXMatrixIdentity(&rot);
		//クォータニオンをコピー
		D3DXQUATERNION q;
		this->CopyFrom(q);
		//クォータニオンから回転行列作成
		D3DXMatrixRotationQuaternion(&rot, &q);
		return rot;
	}
	/*!
	*@brief	クォータニオン同士の積。
	*/
	void Multiply(const Quaternion& rot)
	{
		float pw, px, py, pz;
		float qw, qx, qy, qz;

		pw = w;
		px = x;
		py = y;
		pz = z;

		qw = rot.w;
		qx = rot.x;
		qy = rot.y;
		qz = rot.z;

		w = pw * qw - px * qx - py * qy - pz * qz;
		x = pw * qx + px * qw + py * qz - pz * qy;
		y = pw * qy - px * qz + py * qw + pz * qx;
		z = pw * qz + px * qy - py * qx + pz * qw;

	}
	void CopyFrom(D3DXQUATERNION& q)const
	{
		q.x = x;
		q.y = y;
		q.z = z;
		q.w = w;
	}
	//逆クォータニオンにする。
	void Inverse()
	{
		//正規化されている必要がある
		Normalize();
		//共役クォータニオン作成(マイナスの奴を作る)
		Quaternion qminus = Quaternion(-(this->x), -(this->y), -(this->z), this->w);
		//クォータニオンの長さの二乗
		Quaternion qlength = (qminus*(*this));
		if (qlength.x == 0.0f ||
			qlength.y == 0.0f ||
			qlength.z == 0.0f)
			return;
		//逆クォータニオン作成
		*this = qminus / qlength;
	}
	//各軸の回転量を取得
	Vector3 GetAngle()
	{
		Vector3 ang;
		//θをラジアンに変換する(ToDegree)
		//この式は間違い。
		ang.x = D3DXToDegree(asin(this->x) * 2.0f);
		ang.y = D3DXToDegree(asin(this->y) * 2.0f);
		ang.z = D3DXToDegree(asin(this->z) * 2.0f);
		return ang;
	}
	Quaternion operator * (const Quaternion& in)
	{
		float pw, px, py, pz;
		float qw, qx, qy, qz;
		Quaternion out;

		pw = w; px = x; py = y; pz = z;
		qw = in.w; qx = in.x; qy = in.y; qz = in.z;

		out.w = pw * qw - px * qx - py * qy - pz * qz;
		out.x = pw * qx + px * qw + py * qz - pz * qy;
		out.y = pw * qy - px * qz + py * qw + pz * qx;
		out.z = pw * qz + px * qy - py * qx + pz * qw;
		return out;
	}

	Quaternion operator * (float in) {
		return Quaternion(x * in, y * in, z * in, w * in);
	}

	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float per)
	{
		per = min(1.0f, max(0.0f, per));
		D3DXQUATERNION out;
		D3DXQuaternionSlerp(&out, (D3DXQUATERNION*)&a, (D3DXQUATERNION*)&b, per);
		return out;
	}
};