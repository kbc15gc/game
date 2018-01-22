#pragma once
// ‚È‚ñ‚Æ‚È‚­‚ ‚ê‚Î•Ö—˜‚»‚¤‚È\‘¢‘Ì‚Å‚à’u‚¢‚Æ‚­B

struct Int3 {
	Int3(int a, int b, int c) {
		x = a;
		y = b;
		z = c;
	};
	Int3() {
		x = 0;
		y = 0;
		z = 0;
	}
	int x = 0;
	int y = 0;
	int z = 0;

	Int3 operator + (const Int3 num3)const {
		Int3 ret = *this;
		ret.x += num3.x;
		ret.y += num3.y;
		ret.z += num3.z;
		return ret;
	}
	Int3 operator - (const Int3 num3) const{
		Int3 ret = *this;
		ret.x -= num3.x;
		ret.y -= num3.y;
		ret.z -= num3.z;
		return ret;
	}

};
