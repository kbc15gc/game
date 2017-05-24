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

        //グループループ
        for (int idx = 0; idx < export.transform.childCount; idx++)
        {
            //グループ取得
            Transform group = export.transform.GetChild(idx);
            //オブジェクト書き出し
            ExportObject(group);
            //NPC書き出し
            ExportNPC(group);
            Debug.Log(group.name + "の書き出しに成功");
        }
        Debug.Log("全てのファイルを正常に書き出せました。");
    }

    //オブジェクトを出力
    static public void ExportObject(Transform group)
    {
        string name = "ExportObjects";
        //オブジェクト検索
        Transform objects = group.FindChild(name);
        if (objects == null)
        {
            Debug.Log(name + "が見つからなかったのでエクスポートできませんでした。");
            return;
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
    }

    //NPCを出力
    static public void ExportNPC(Transform group)
    {
        string name = "ExportNPCs";
        //オブジェクト検索
        Transform npcs = group.FindChild(name);
        if (npcs == null)
        {
            Debug.Log(name + "が見つからなかったのでエクスポートできませんでした。");
            return;
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
            Mesh mesh = child.GetComponent<MeshFilter>().mesh;

            //メッシュ名を書き出し
            //sw.Write(mesh.name.Replace(" Instance", "") + ".X");

            //名前を書き出し
            sw.Write(child.name + ".X");
            sw.Write(',');
            //ポジション
            WriteVector3(sw, child.position);
            //回転
            WriteVector3(sw, child.eulerAngles);
            //スケール
            WriteVector3(sw, child.lossyScale);

            NPC npc = child.GetComponent<NPC>();
            sw.Write(npc.MesseageID);
            sw.Write(',');

            sw.Write(Convert.ToInt32(npc.ShowTitle));

            //改行
            sw.Write("\r\n");
        }
        sw.Close();
        fs.Close();
    }

    static public void WriteVector3(StreamWriter sw,Vector3 val,bool comma = true)
    {
        sw.Write(val.x);
        sw.Write('/');
        sw.Write(val.y);
        sw.Write('/');
        sw.Write(val.z);
        //コンマを打つ
        if (comma)
        {
            sw.Write(',');
        }
    }
}
