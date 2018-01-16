using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum CodeE
{
    ItemCodeE,
    ArmorCodeE,
    WeaponCodeE
};

[System.Serializable]
public class DropItemData
{
    //アイテムコード。
    public ItemCodeE code = ItemCodeE.None;
    //確率
    [Range(0.0f,100.0f)]
    public float probability;
}

[System.Serializable]
public class DropArmorData
{
    //アイテムコード。
    public ArmorCodeE code = ArmorCodeE.None;
    //確率
    [Range(0.0f, 100.0f)]
    public float probability;
}

[System.Serializable]
public class DropWeaponData
{
    //アイテムコード。
    public WeaponCodeE code = WeaponCodeE.None;
    //確率
    [Range(0.0f, 100.0f)]
    public float probability;
}

public class ExportEnemy : MonoBehaviour {

    //エネミーのタイプ
    public enum EnemyType
    {
        Bone,   	//骨
        BossDrarian,//第１のボスドラリアン
        Drarian,	//第１のボスドラリアンの取り巻き
        Golem,      //ゴーレム
        BossGolem,  //第2のボスゴーレム
        Soldier,    //兵士
        BossD,      //第3のボス
        Ghost,      //ゴースト。
        LasstBoss,  //ﾗｽﾎﾞｽ
    }
    //エネミーのタイプ指定。
    public EnemyType _EnemyType;
    //各ステータス。
    public int _HP;
    public int _MP;
    public int _ATK;
    public int _MAT;
    public int _DEF;
    public int _MDE;
    public int _DEX;
    public int _CRT;
    public int _LV;
    public int _DROPEXP;
    public int _MONEY;

    //落とすアイテムの種類。
    //落とすアイテムのID、落とさないアイテムは-1を入れる。
    public DropItemData[] _Item;
    public DropArmorData[] _Armor;
    public DropWeaponData[] _Weapon;

    //モンスターのカラー
    public int _ColorFlag = 0;
    public Color _Color = Color.white;

    //見える距離
    public float _ViewRange = 10.0f;
    // 徘徊範囲設定。
    public float _WanderingRange = 10.0f;
    // 追跡範囲設定。
    public float _DiscoveryRange = 10.0f;
    //リスポーン時間
    public float _RespawnTime = 60.0f;
}
