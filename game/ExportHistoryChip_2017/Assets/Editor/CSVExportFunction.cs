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
            //コリジョン書き出し。
            bool coll = ExportCollision(group);

            if (obj == true &&
                npc == true &&
                coll == true)
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
        foreach (Transform child in Children)
        {
            if (child.name == objects.name)
                continue;

            //名前書き出し
            sw.Write(child.name + ".X");
            sw.Write(',');
            //ポジション
            WriteVector3(sw, child.position);
            //回転
            WriteVector3(sw, child.eulerAngles);
            //スケール
            WriteVector3(sw, child.lossyScale,false);

            //改行
            sw.Write("\r\n");
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
            string pos = Vector3ToString(child.position);
            string ang = Vector3ToString(child.eulerAngles);
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

    static public bool ExportCollision(Transform group)
    {
        string name = "ExportCollisions";
        Transform colls;
        //オブジェクト検索
        if ((colls = FindObject(group, name)) == null)
        {
            return false;
        }

        //子供たちのTransformコンポーネントを取得
        Transform[] Children = colls.GetComponentsInChildren<Transform>();
        //ファイルパス
        string path = Application.dataPath + "/Export/Coll/" + group.name + "Coll" + ".csv";

        //ファイルを開く準備
        FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write);
        StreamWriter sw = new StreamWriter(fs);
        sw.WriteLine("pos,ang,sca,hitcamera");
        foreach (Transform child in Children)
        {
            if (child.name == colls.name)
                continue;
                        
            string pos = Vector3ToString(child.position);
            string ang = Vector3ToString(child.eulerAngles);
            string sca = Vector3ToString(child.lossyScale);
            ExportCollision npc = child.GetComponent<ExportCollision>();
            int camera = Convert.ToInt32(npc.HitCamera);
            //
            string line = string.Format("{0},{1},{2},{3}", pos, ang, sca, camera);

            //列書き出し
            sw.WriteLine(line);
        }
        sw.Close();
        fs.Close();

        return true;
    }

    static public string Vector3ToString(Vector3 val)
    {
        //"x/y/z"の形で返す。
        return String.Format("{0}/{1}/{2}", -val.x, val.y, -val.z);
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
