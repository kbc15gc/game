using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExportEnemy : MonoBehaviour {

    //エネミーのタイプ
    public enum EnemyType
    {
        Bone,   	//骨
        BossDrarian,//第１のボスドラリアン
        Drarian,	//第１のボスドラリアンの取り巻き
        Golem,      //ゴーレム
        Soldier,    //兵士
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
    public int[] _Item = new int[5];
    public int[] _Armor = new int[5];
    public int[] _Weapon = new int[5];
}
