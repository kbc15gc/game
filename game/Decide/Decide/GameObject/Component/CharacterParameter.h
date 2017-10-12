#pragma once
#include "fbEngine\_Object\_Component\Component.h"
#include "fbEngine\_Object\_GameObject\TextObject.h"
#include "fbEngine\fbstdafx.h"

class HoldWeapon;
class HoldArmor;

class CharacterParameter :public Component {
public:
	
#ifdef _DEBUG
	// デバッグ出力用の配列。
	// ※パラメーターの種類を追加したら順番通りに追加して。
	static const wchar_t* ENUM_NAME[];
#endif

	// 元素属性(火、水など)。
	enum class Element { None = -1 };
	// 物理属性(斬、打、魔など)。
	enum class Physical { None = -1 };

	// ダメージ情報。
	struct DamageInfo {
		int value = 0;	// ダメージ量。
		bool isCritical = false;	// クリティカルか。
		bool isMagic = false;	// 魔法攻撃か。
		bool isThrough = false;	// 防御貫通攻撃か。
		Element element;	// 元素属性(現在未使用)。
		Physical physical;	// 物理属性(現在未使用)。
	};

	// パラメーター列挙。
	//			HP。
	//			MP。
	//			攻撃力。
	//			魔法攻撃力。
	//			防御力。
	//			魔法防御。
	//			器用度(クリティカル発生率)。
	//			クリティカル威力。
	//			レベル。
	enum Param { MIN = -1,HP = 0, MP, ATK, MAT, DEF, MDE, DEX, CRT ,LV, MAX };

private:
	// 各種パラメータの状態管理用構造体。
	struct ParamInfo {
		// キャラクターの基礎パラメータ(この値をもとにバフ、デバフ、HP計算を行う)。
		// ※レベルアップなどによって基礎パラメータが更新される際はこちらを変更する。
		int originParam = 0;
		int param = 0;	// ゲームでの実際のパラメータ(バフ、デバフ後の値や、現在のHPやMPなど)。

		int buffPercentage = 0;		// パラメータに掛かっているバフの効果値(パーセント)。
		float buffInterval = 0.0f;		// バフの効果時間。
		float buffCounter = 0.0f;		// バフの経過時間。

		int debuffPercentage = 0;	// パラメータに掛かっているバフの効果値(パーセント)。
		float debuffInterval = 0.0f;	// デバフの効果時間。
		float debuffCounter = 0.0f;		// デバフの経過時間。
	};

public:
	CharacterParameter(GameObject* g, Transform* t) :Component(g, t, typeid(this).name()) {
#ifdef _DEBUG
		mbstowcs_s(nullptr, name, typeid(*this).name(), strlen(typeid(*this).name()));
#endif

	};
	~CharacterParameter() {
	};

	void Awake()override;

	void Update()override;



	// パラメータリセット。
	// 引数：	設定する各種基礎パラメータ。
	// ※デバフやバフがかかっている場合は新しいパラメータをもとに再計算されて設定される。
	void ParamReset(int param[Param::MAX]);
	// パラメータリセット。
	// 引数：	設定する各種基礎パラメータ。
	// ※デバフやバフがかかっている場合は新しいパラメータをもとに再計算されて設定される。
	void ParamReset(const vector<int>& param);

	// 被ダメージ処理(パラメーターにダメージを与える)。
	// 引数:		敵からのダメージ情報。
	//				防具(デフォルトはnull、武器未装備時はnullを設定)。
	//				キャラクターの行動で発生する防御率(防御行動などによって変動する値、デフォルトは1)。
	// 戻り値:		受けたダメージ。
	int ReciveDamage(const DamageInfo& info, HoldArmor* armor = nullptr, int defidx = 1);

	// 被ダメージ計算(計算のみでパラメーターに影響はない)。
	// 引数:		敵からのダメージ情報。
	//				防具(デフォルトはnull、武器未装備時はnullを設定)。
	//				キャラクターの行動で発生する防御率(防御行動などによって変動する値、デフォルトは1)。
	// 戻り値:		受けるダメージ。
	int ReceiveDamageMass(const DamageInfo& info, HoldArmor* armor = nullptr,int defidx = 1);

	// 防御力無視の被ダメージ処理。
	// 引数:		敵からのダメージ。
	int ReciveDamageThrough(int damage);

	//与ダメージ計算。
	// 引数：	魔法攻撃か。
	//			防御貫通攻撃か。
	//			武器(デフォルトはnull、武器未装備時はnullを設定)。	
	//			ダメージ率(攻撃の種類などによる攻撃力に対する割合、この値に0.01f掛けた値を攻撃力に乗算する、単位はパーセント、デフォルトは100)。
	// 戻り値:	与えるダメージの情報。
	unique_ptr<DamageInfo> GiveDamageMass(bool isMagic,bool isThrough, HoldWeapon* weapon = nullptr, int percentage = 100);

	// HP回復関数。
	// 引数：	回復量。
	// 戻り値：	回復できたか。
	bool HeelHP(int value);
	// MP回復関数。
	// 引数：	回復量。
	// 戻り値：	回復できたか。
	bool HeelMP(int value);

	// バフ関数。
	// 引数：	バフを掛けたいパラメータ。
	//			効果値(パーセント)。
	//			効果時間(秒)。
	// ※HPとMPとCRTとLVは無効。
	void Buff(Param idx, int percentage,float time);

	// バフ解除関数。
	// 引数：	解除したいパラメータ。
	// ※HPとMPとCRTとLVは無効。
	void BuffClear(Param idx);

	// 全バフ解除関数。
	// ※HPとMPとCRTとLVは無効。
	void BuffClearAll();

	// デバフ関数。
	// 引数：	デバフを掛けたいパラメータ。
	//			効果値(パーセント)。
	//			効果時間(秒)。
	// ※HPとMPとCRTとLVは無効。
	void Debuff(Param idx, int percentage, float time);

	// デバフ解除関数。
	// 引数：	解除したいパラメータ。
	// ※HPとMPとCRTとLVは無効。
	void DebuffClear(Param idx);

	// 全デバフ解除関数。
	// ※HPとMPとCRTとLVは無効。
	void DebuffClearAll();


	// 指定したパラメーター(バフ値を含む)取得。
	// 引数：	パラメータータイプ。
	inline int GetParam(Param idx)const {
		_OutCheck(idx);
		return _Info[idx].param;
	}
	// 指定したパラメーター(バフ値を除く)取得。
	// 引数：	パラメータータイプ。
	inline int GetPigmentParam(Param idx)const {
		if (idx == Param::HP || idx == Param::MP) {
			// HPとMPは非対応。
			abort();
		}
		_OutCheck(idx);
		return _Info[idx].originParam;
	}

	// 指定したパラメーターの現在のバフ値取得。
	// 引数：	パラメータータイプ。
	inline int GetBuffParam(Param idx)const {
		if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
			// バフとデバフに対応していないものは非対応。
			abort();
		}
		return static_cast<int>(static_cast<float>(_Info[idx].buffPercentage) * 0.01f * _Info[idx].originParam);
	}
	// 指定したパラメーターの現在のデバフ値取得。
	// 引数：	パラメータータイプ。
	inline int GetDebuffParam(Param idx)const {
		if (idx == Param::HP || idx == Param::MP || idx == Param::CRT || idx == Param::LV) {
			// バフとデバフに対応していないものは非対応。
			abort();
		}
		return static_cast<int>(static_cast<float>(_Info[idx].debuffPercentage) * 0.01f * _Info[idx].originParam);
	}

	inline int GetMaxHP()const {
		return _Info[Param::HP].originParam;
	}

	inline int GetMaxMP() const{
		return _Info[Param::MP].originParam;
	}

	//死んだかどうかのフラグを取得。
	//tureなら死んでいる。
	inline bool GetDeathFlg()
	{
		return _DeathFlag;
	}



private:

	// 配列外にアクセスしてないかチェック。
	inline void _OutCheck(short num)const {
		if (num <= Param::MIN || num >= Param::MAX) 
			abort();
	}

	// 現在のバフ値、デバフ値でパラメータを更新。
	void _UpdateParam(Param idx);

	// 各種パラメータの状態を更新。
	void _UpdateInfo();
private:
	// キャラクターの状態管理用構造体。
	// ※このクラス内でのみ使用。
	vector<ParamInfo> _Info;

	bool _DeathFlag = false;//死んだかどうかのフラグ。
};
