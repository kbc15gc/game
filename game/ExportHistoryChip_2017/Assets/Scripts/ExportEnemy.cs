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
    }
    //エネミーのタイプ指定。
    public EnemyType _EnemyType;
    //各ステータス。
    public int _HP;
    public int _MaxHP;
    public int _MP;
    public int _MaxMP;
    public int _ATK;
    public int _MAT;
    public int _DEF;
    public int _MDE;
    public int _DEX;
    public int _CRT;
    public int _LV;
    public int _DROPEXP;
    public int _MONEY;
    
}
