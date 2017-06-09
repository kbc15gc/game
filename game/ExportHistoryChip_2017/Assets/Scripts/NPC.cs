using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NPC : MonoBehaviour {
    //NPCのタイプ
    public enum NPCTypeE : int
    {
        VILLAGER,   //普通の村人
        SHOP,       //店員
    }
    public NPCTypeE NPCType = NPCTypeE.VILLAGER; 
    //店のID
    public int ShopID = -1;
    //メッセージのID
    public int MesseageID = 0;
    //近づいたときにタイトルを表示するかどうか？
    public bool ShowTitle = true;
}