﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExportEnemy : MonoBehaviour {

    //エネミーのタイプ
    public enum EnemyType
    {
        Bone,   //骨
    }
    //エネミーのタイプ指定。
    public EnemyType _EnemyType;
    //各ステータス。
    public int _HP;
    public int _MaxHP;
    public int _MP;
    public int _MaxMP;
    public int _ATK;
    public int _DEF;
    public int _DEX;
    public int _AGI;
    
}
