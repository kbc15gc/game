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
	operator D3DXVECTOR3() { return (*this); }
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
	Vector3(const Vector2& v2,float z)
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
		return x * in.x + y * in.y + z * in.z;
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

	static Vector3& Lerp(const Vector3& a, const Vector3& b,float wariai)
	{
		wariai = min(1.0f, max(0.0f, wariai));
		//ベクトルを求める
		Vector3 To = b - a;
		return a + (To * wariai);
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
	//回転行列取得
	const D3DXMATRIX GetRotationMatrix()
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

		pw = w; px = x; py = y; pz = z;
		qw = rot.w; qx = rot.x; qy = rot.y; qz = rot.z;

		w = pw * qw - px * qx - py * qy - pz * qz;
		x = pw * qx + px * qw + py * qz - pz * qy;
		y = pw * qy - px * qz + py * qw + pz * qx;
		z = pw * qz + px * qy - py * qx + pz * qw;

	}
	void CopyFrom(D3DXQUATERNION& q)
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
};