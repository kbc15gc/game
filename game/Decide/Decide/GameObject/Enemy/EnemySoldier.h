#pragma once
#include "GameObject\Enemy\EnemyCharacter.h"
class EnemySoldier :public EnemyCharacter
{
private:
	//ソルジャーのアニメーション番号
	enum class EnemySoldierAnim
	{

		Death = 0,
		Damage,
		Attack01,
		Run,
		Walk,
		Stand,
		Max
	};

public:
	EnemySoldier(const char* name);
	~EnemySoldier();
private:
	void _AwakeSubClass()override;
	void _StartSubClass()override;
	void _UpdateSubClass()override;

	EnemyAttack* _AttackSelectSubClass()override;

	void CreateAttackCollsion();

	//コリジョン定義関数。
	//コリジョンの形状やパラメータを設定する関数。
	void _ConfigCollision()override;

	//キャラクターコントローラーのパラメーターを設定する関数。
	//衝突するコリジョン属性や重力の値などをここで設定する。
	void _ConfigCharacterController()override;

	// キャラクターコントローラ押し出しコンポーネント用の剛体作成関数。
	// コリジョン属性や形状などを設定し、作成する。
	void _CreateExtrudeCollision()override;

	// キャラクターコントローラ押し出しコンポーネントのパラメーターを設定する関数。
	// 衝突するコリジョン属性や重力の値などをここで設定する。
	// ※処理自体は継承先に委譲。
	inline void _ConfigCharacterExtrude()override {
		_MyComponent.CharacterExtrude->Attribute_AllOff();
		_MyComponent.CharacterExtrude->AddAttribute(Collision_ID::PLAYER);
	}

	// アニメーション番号のテーブルを作成。
	// 引数：	アニメーション終了時間の格納用配列(この配列に終了時間を設定する、添え字はモデルに設定されているアニメーション番号)。
	// 受け取る配列内の値はデフォルトで-1となっているので、アニメーションの終了時間が1秒以上のものは設定しなくてよい。
	void _BuildAnimationSubClass(vector<double>& datas)override;

	// アニメーションイベントを設定する関数。
	void _ConfigAnimationEvent()override;

	inline void _DropSubClass()override {
	}
protected:
	void _EndNowStateCallback(State EndStateType)override;
private:
	unique_ptr<EnemySingleAttack> _SingleAttack;	// 単攻撃処理。
};

