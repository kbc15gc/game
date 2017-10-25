#pragma once
#include "GameObject\Enemy\EnemyCharacter.h"
class EnemySoldier :public EnemyCharacter
{
private:
	//ゴーレムのアニメーション番号
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
	// ※添え字にはこのクラス定義したAnimationType列挙体を使用。
	void _BuildAnimationSubClass(vector<double>& datas)override;

	// アニメーションイベントを設定する関数。
	void _ConfigAnimationEvent()override;

	// 効果音のテーブル作成関数。
	void _BuildSoundTable()override;

	inline void _DropSubClass()override {
		DropItem* item = INSTANCE(GameObjectManager)->AddNew<DropItem>("DropItem", 9);
		item->Create(INSTANCE(ItemManager)->GetItemInfo(2, Item::ItemCodeE::Armor), transform->GetPosition(), 2);
	}
protected:
	void _EndNowStateCallback(State EndStateType)override;
private:
	unique_ptr<EnemySingleAttack> _SingleAttack;	// 単攻撃処理。
};

