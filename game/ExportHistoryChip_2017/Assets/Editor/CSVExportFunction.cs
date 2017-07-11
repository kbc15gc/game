using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.SceneManagement;
using System.IO;
using System;

public class CSVExportFunction : Editor
{
    //グループを出力
    [MenuItem("Export/ExportGroup")]
    static public void ExportGroup()
    {
        //オブジェクト検索
        string name = "Export";
        GameObject export = GameObject.Find(name);
        if (export == null)
        {
            Debug.Log(name + "が見つからなかったのでエクスポートできませんでした。");
            return;
        }

        bool all = true;
        //グループループ
        for (int idx = 0; idx < export.transform.childCount; idx++)
        {
            //グループ取得
            Transform group = export.transform.GetChild(idx);
            //オブジェクト書き出し
            bool obj = ExportObject(group);
            //NPC書き出し
            bool npc = ExportNPC(group);
            //enemy
            bool enemy = ExportEnemy(group);

            if (obj == true &&
                npc == true &&
                enemy == true)
            {
                Debug.Log(group.name + "の書き出しに成功");
            }
            else
            {
                all = false;
                Debug.LogError(group.name + "の書き出しに失敗");
            }
        }
        if (all)
            Debug.Log("全てのファイルを正常に書き出せました。");
    }

    //オブジェクトを出力
    static public bool ExportObject(Transform group)
    {
        string name = "ExportObjects";
        Transform objects;
        //オブジェクト検索
        if ((objects = FindObject(group, name)) == null)
        {
            return false;
        }

        //子供たちのTransformコンポーネントを取得
        Transform[] Children = objects.GetComponentsInChildren<Transform>();
        //ファイルパス
        string path = Application.dataPath + "/Export/Obj/" + group.name + "Obj" + ".csv";
        //ファイルを開く準備
        FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write);
        StreamWriter sw = new StreamWriter(fs);
        sw.WriteLine("name,pos,ang,sca");
        //1つのオブジェクトを取り出して書き出す。
        for (int idx = 0; idx < objects.childCount; idx++) 
        {
            //子を取得
            Transform child = objects.GetChild(idx);

            //名前書き出し
            string filename = child.name + ".X";
            //ポジション
            string pos = Vector3ToString(child.position, true);
            //回転
            string ang = QuaternionToString(child.rotation);
            //スケール
            string sca = Vector3ToString(child.lossyScale);

            string line;
            line = string.Format("{0},{1},{2},{3}", filename, pos, ang, sca);
            //1列書き出し。
            sw.WriteLine(line);

            //オブジェクトのあたり判定を書き出し。
            foreach (Transform coll in child.GetComponentsInChildren<Transform>())
            {
                if (coll.name == child.name)
                    continue;
                ExportCollision(coll, sw);
            }
        }
        sw.Close();
        fs.Close();

        return true;
    }

    //NPCを出力
    static public bool ExportNPC(Transform group)
    {        
        string name = "ExportNPCs";
        Transform npcs;
        //オブジェクト検索
        if ((npcs = FindObject(group, name)) == null)
        {
            return false;
        }    

        //子供たちのTransformコンポーネントを取得
        Transform[] Children = npcs.GetComponentsInChildren<Transform>();
        //ファイルパス
        string path = Application.dataPath + "/Export/NPC/" + group.name + "NPC" + ".csv";

        //ファイルを開く準備
        FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write);
        StreamWriter sw = new StreamWriter(fs);
        sw.WriteLine("name,pos,ang,sca,messeageid,showtitle");
        foreach (Transform child in Children)
        {
            if (child.name == npcs.name)
                continue;

            //メッシュ取得
            //Mesh mesh = child.GetComponent<MeshFilter>().mesh;

            //メッシュ名を書き出し
            //sw.Write(mesh.name.Replace(" Instance", "") + ".X");
            string filename = child.name + ".X";
            string pos = Vector3ToString(child.position, true);
            string ang = QuaternionToString(child.rotation);
            string sca = Vector3ToString(child.lossyScale);
            NPC npc = child.GetComponent<NPC>();
            int type = Convert.ToInt32(npc.NPCType);
            int shopid = npc.ShopID;
            int textid = npc.MesseageID;
            string flg = Convert.ToInt32(npc.ShowTitle).ToString();
            //
            string line = string.Format("{0},{1},{2},{3},{4},{5},{6},{7}", filename, pos, ang, sca, type, shopid, textid, flg);

            //列書き出し
            sw.WriteLine(line);
        }
        sw.Close();
        fs.Close();

        return true;
    }

    static public void ExportCollision(Transform coll,StreamWriter sw)
    {
        //名前書き出し
        string filename = "coll";
        //ポジション
        string pos = Vector3ToString(coll.localPosition,true);
        //回転
        string ang = QuaternionToString(coll.rotation);
        //スケール
        string sca = Vector3ToString(coll.lossyScale);

        string line;
        line = string.Format("{0},{1},{2},{3}", filename, pos, ang, sca);
        sw.WriteLine(line);
    }

    static public bool ExportEnemy(Transform group)
    {
        string name = "ExportEnemys";
        Transform enemys;
        //オブジェクト検索
        if ((enemys = FindObject(group, name)) == null)
        {
            return false;
        }

        //子供たちのTransformコンポーネントを取得
        Transform[] Children = enemys.GetComponentsInChildren<Transform>();
        //ファイルパス
        string path = Application.dataPath + "/Export/Enemy/" + group.name + "Enemy" + ".csv";

        //ファイルを開く準備
        FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write);
        StreamWriter sw = new StreamWriter(fs);
        sw.WriteLine("type,hp,mhp,mp,mmp,atk,def,dex,agi,lv,exp,dropexp,pos,Quaternion,sca");
        foreach (Transform child in Children)
        {
            if (child.name == enemys.name)
                continue;

            ExportEnemy e = child.GetComponent<ExportEnemy>();
            string type = Convert.ToString(Convert.ToInt32(e._EnemyType));

            string hp = Convert.ToString(e._HP);
            string mhp = Convert.ToString(e._MaxHP);
            string mp = Convert.ToString(e._MP);
            string mmp = Convert.ToString(e._MaxMP);
            string atk = Convert.ToString(e._ATK);
            string def = Convert.ToString(e._DEF);
            string dex = Convert.ToString(e._DEX);
            string agi = Convert.ToString(e._AGI);
            string lv = Convert.ToString(e._LV);
            string exp = Convert.ToString(e._EXP);
            string dropexp = Convert.ToString(e._DROPEXP);

            string pos = Vector3ToString(child.position,true);
            string quaternion = QuaternionToString(child.rotation);
            string sca = Vector3ToString(child.lossyScale);

            //
            string line = string.Format("{0},{1}/{2}/{3}/{4}/{5}/{6}/{7}/{8}/{9}/{10}/{11},{12},{13},{14}", type, hp, mhp, mp, mmp, atk, def, dex, agi, lv, exp, dropexp, pos, quaternion, sca);


            //列書き出し
            sw.WriteLine(line);
        }
        sw.Close();
        fs.Close();

        return true;
    }

    static public string Vector3ToString(Vector3 val,bool sign = false)
    {
        //"x/y/z"の形で返す。
        int Sign = sign ? -1 : 1;
        return String.Format("{0}/{1}/{2}", val.x * Sign, val.y, val.z * Sign);
    }

    static public string QuaternionToString(Quaternion val)
    {
        //"x/y/z/w"の形で返す。
        return String.Format("{0}/{1}/{2}/{3}", val.x, val.y, val.z, val.w);
    }

    static public void WriteVector3(StreamWriter sw,Vector3 val,bool comma = true)
    {
        sw.Write(-val.x);
        sw.Write('/');
        sw.Write(val.y);
        sw.Write('/');
        sw.Write(-val.z);
        //コンマを打つ
        if (comma)
        {
            sw.Write(',');
        }
    }

    static public Transform FindObject(Transform group,string name)
    {
        //オブジェクト検索
        Transform obj = group.FindChild(name);
        if (obj == null)
        {
            Debug.LogError(group.name + ":子に" + name + "が見つかりませんでした。");
        }
        return obj;
    }
}
