#pragma once

//カラー
class Color {
public:
	union {
		struct { float r, g, b, a; };
		float color[4];
	};
	//赤
	static const Color red;
	//緑
	static const Color green;
	//青
	static const Color blue;
	//黄色
	static const Color yellow;
	//水
	static const Color water;
	//紫
	static const Color purple;
	//白
	static const Color white;
	//黒
	static const Color black;
	//無
	static const Color zero;

	//あおっぴ
	static const Color bluepo;

	//ドカベンフォント用カラー
	static const Color dokaben;
public:
	operator Vector4() { return Vector4(r, g, b, a); }
	operator D3DXCOLOR() { return static_cast<D3DXCOLOR>(*this); }
	operator LPVOID() { return static_cast<LPVOID>(this); }

	Color()
	{
		this->r = this->g = this->b = this->a = 0;
	}
	//float型コンストラクタ 0.0f~1.0fの間で指定。
	Color(const float& r,const float& g,const float& b,const float& a)
	{
		Set(r, g, b, a);
	}
	//float型コンストラクタ 0.0f~1.0fの間で指定。
	Color(const float& r, const float& g, const float& b)
	{
		Set(r, g, b, 1.0f);
	}
	//int型コンストラクタ 0~255で指定。
	Color(const unsigned int& r, const unsigned int& g, const unsigned int& b, const unsigned int& a)
	{
		Set(r, g, b, a);
	}
	//D3DXCOLORをColorに変換
	Color(D3DXCOLOR& c)
	{
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->a = c.a;
	}
	void Set(const float& r, const float& g, const float& b, const float& a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	void Set(const unsigned int& r, const unsigned int& g, const unsigned int& b, const unsigned int& a)
	{
		this->r = (float)r / 255.0f;
		this->g = (float)g / 255.0f;
		this->b = (float)b / 255.0f;
		this->a = (float)a / 255.0f;
	}
	//ColorをD3DXVECTOR3に変換コピーする
	void CopyFrom(D3DXCOLOR& c) const
	{
		c.r = r;
		c.g = g;
		c.b = b;
		c.a = a;
	}

	Color operator * (float f) const
	{
		Color out = *this;
		out.r = this->r * f;
		out.g = this->g * f;
		out.b = this->b * f;
		return out;
	}

	Color operator - (Color c) const
	{
		Color out = *this;
		out.r = this->r - c.r;
		out.g = this->g - c.g;
		out.b = this->b - c.b;
		return out;
	}
};