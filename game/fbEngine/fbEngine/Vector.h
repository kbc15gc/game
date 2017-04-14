/*!
 * @brief	�x�N�g��
 */

#pragma once


//�񎟌��x�N�g��
class Vector2 {
public:
	//���p��
	union {
		struct { float x, y; };
		float in[2];
	};

	Vector2() 
	{
		this->x = this->y = 0;
	}

	Vector2(float x, float y)
	{
		Set(x, y);
	}
	
	void Set(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	/*!
	* @brief	�������擾
	*/
	float Length()
	{
		return sqrt(LengthSq());
	}
	/*!
	* @brief	�����̓����擾
	*/
	float LengthSq()
	{
		return x * x + y * y;
	}
	//����
	float Dot(const Vector2& in)
	{
		return x * in.x + y * in.y;
	}
	//�O��
	float Cross(const Vector2& in)
	{
		return x * in.y - y * in.x;
	}
	//�p�x�{��]���������߂�i�P�ʂ̓��W�A���j
	float Rot()
	{
		//������̃x�N�g���Ƃ̊p�x���Ƃ�B
		return atan2f(this->Cross(Vector2(0, 1)), this->Dot(Vector2(0, 1)));
	}
	//���K��
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
};

//�O�����x�N�g��
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
	operator D3DXVECTOR3() { return static_cast<D3DXVECTOR3>(*this); }
	operator LPVOID() { return static_cast<LPVOID>(this); }
	
	Vector3() 
	{
		this->x = this->y = this->z = 0;
	}
	//D3DXVECTOR3��Vector3�ɕϊ�
	Vector3(D3DXVECTOR3& in)
	{
		this->x = in.x;
		this->y = in.y;
		this->z = in.z;
	}
	/*!
	* @brief	�R���X�g���N�^�B
	*/
	Vector3(float x, float y, float z)
	{
		Set(x, y, z);
	}
	//Vector3��D3DXVECTOR3�ɕϊ��R�s�[����
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
	* @brief	�x�N�g���̊e�v�f��ݒ�B
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
	 * @brief	�x�N�g�������Z�B
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
	 * @brief	�x�N�g�������Z�B
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
	 * @brief	���ρB
	 */
	float Dot(const Vector3& in)
	{
		return x * in.x + y * in.y + z * in.z;
	}
	/*!
	 * @brief	�O�ρB
	 */
	Vector3 Cross(const Vector3& in)
	{
		float _x = (y * in.z) - (in.y * z);
		float _y = (z * in.x) - (in.z * x);
		float _z = (x * in.y) - (in.x * y);
		return Vector3(_x, _y, _z);
	}
	/*!
	 * @brief	�������擾
	 */
	float Length() const
	{
		return sqrt(LengthSq());
	}
	/*!
	 * @brief	�����̓����擾
	 */
	float LengthSq() const
	{
		return x * x + y * y + z * z;
	}
	/*!
	* @brief	�g��B
	*/
	void Scale(float s)
	{
		x *= s;
		y *= s;
		z *= s;
	}
	/*!
	* @brief	�@���𐳋K���B
	*/
	void Normalize()
	{
		float len = Length();
		x /= len;
		y /= len;
		z /= len;
	}
	/*!
	* @brief	���Z�B
	*/
	void Div(float d)
	{
		x /= d;
		y /= d;
		z /= d;
	}
	//���`�⊮
	//�⊮��A�����鎞�ԁA����
	void Lerp(const Vector3& in,float time,float wariai)
	{
		//�������߂�
		Vector3 diff = in - (*this);
		//1�������
		diff.Div(time);
		//������
		diff.Scale(wariai);
		this->Add(diff);
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
};
	
//�l�����x�N�g��
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
	 *@brief	�R���X�g���N�^
	 */
	Vector4(float x, float y, float z, float w)
	{
		Set(x, y, z, w);
	}
	/*!
	 *@brief	�x�N�g���̊e�v�f��ݒ�B
	 */
	void Set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	/*!
	 *@brief	�x�N�g�������Z�B
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
	 *@brief	�x�N�g�������Z�B
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
	 *@brief	����
	 */
	float Dot(const Vector4& in)
	{
		return x * in.x + y * in.y + z * in.z + w * in.w;
	}
	/*!
	 * @brief	�������擾
	 */
	float Length()
	{
		return sqrt(LengthSq());
	}
	/*!
	 * @brief	�����̓����擾
	 */
	float LengthSq()
	{
		return x * x + y * y + z * z + w * w;
	}
	//���K��
	void Normalize()
	{
		//�������擾
		float len = Length();
		//�e�v�f�𒷂��Ŋ���B
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}
	/*!
	* @brief	�g��B
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
	static const Quaternion Identity;		//!<�P�ʃN�H�[�^�j�I���B
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
	//D3DXVECTOR3��Vector3�ɕϊ�
	Quaternion(D3DXQUATERNION& q)
	{
		this->x = q.x;
		this->y = q.y;
		this->z = q.z;
		this->w = q.w;
	}

	/*!
	 *@brief	�C�ӂ̎�����̉�]�N�H�[�^�j�I�����쐬�B
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
	//�I�C���[�p�x����N�H�[�^�j�I���쐬
	void SetEuler(Vector3 ang)
	{
		D3DXQUATERNION q;
		D3DXQuaternionRotationYawPitchRoll(&q, D3DXToRadian(ang.y), D3DXToRadian(ang.x), D3DXToRadian(ang.z));
		(*this) = q;
	}
	//��]�s��擾
	D3DXMATRIX GetRotationMatrix()
	{
		D3DXMATRIX rot;
		D3DXMatrixIdentity(&rot);
		//�N�H�[�^�j�I�����R�s�[
		D3DXQUATERNION q;
		this->CopyFrom(q);
		//�N�H�[�^�j�I�������]�s��쐬
		D3DXMatrixRotationQuaternion(&rot, &q);
		return rot;
	}
	/*!
	*@brief	�N�H�[�^�j�I�����m�̐ρB
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
	//�t�N�H�[�^�j�I���ɂ���B
	void Inverse()
	{
		//���K������Ă���K�v������
		Normalize();
		//�����N�H�[�^�j�I���쐬(�}�C�i�X�̓z�����)
		Quaternion qminus = Quaternion(-(this->x), -(this->y), -(this->z), this->w);
		//�N�H�[�^�j�I���̒����̓��
		Quaternion qlength = (qminus*(*this));
		//�t�N�H�[�^�j�I���쐬
		*this = qminus / qlength;
	}
	//�e���̉�]�ʂ��擾
	Vector3 GetAngle()
	{
		Vector3 ang;
		//�Ƃ����W�A���ɕϊ�����(ToDegree)
		ang.x = D3DXToDegree(asin(this->x) * 2.0f);
		ang.y = D3DXToDegree(asin(this->y) * 2.0f);
		ang.z = D3DXToDegree(asin(this->z) * 2.0f);
		return ang;
	}
};