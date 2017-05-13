using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.SceneManagement;
using System.IO;
using System;

public class CSVExportFunction : Editor
{
    //オブジェクトを出力
    [MenuItem("Export/ExportObject")]
    static public void ExportObjects()
    {
        //オブジェクト検索
        GameObject parent = GameObject.Find("ExportObjects");
        if (parent == null)
        {
            Debug.Log("ExportObjectsが見つからなかったのでエクスポートできませんでした。");
            return;
        }
        //子供たちのTransformコンポーネントを取得
        Transform[] Children = parent.GetComponentsInChildren<Transform>();
        //ファイルパス
        string path = Application.dataPath + "/Export/Obj/" + SceneManager.GetActiveScene().name + "Obj" + ".csv";
        //ファイルを開く準備
        FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write);
        StreamWriter sw = new StreamWriter(fs);
        sw.WriteLine("name,pos,ang,sca");
        foreach (Transform child in Children)
        {
            if (child.name == parent.name)
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

    //オブジェクトを出力
    [MenuItem("Export/ExportNPC")]
    static public void ExportNPCs()
    {
        //オブジェクト検索
        GameObject parent = GameObject.Find("ExportNPCs");
        if (parent == null)
        {
            Debug.Log("ExportObjectsが見つからなかったのでエクスポートできませんでした。");
            return;
        }
        //子供たちのTransformコンポーネントを取得
        Transform[] Children = parent.GetComponentsInChildren<Transform>();
        //ファイルパス
        string path = Application.dataPath + "/Export/NPC/" + SceneManager.GetActiveScene().name + "NPC" + ".csv";
        //ファイルを開く準備
        FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write);
        StreamWriter sw = new StreamWriter(fs);
        sw.WriteLine("name,pos,ang,sca,messeageid,showtitle");
        foreach (Transform child in Children)
        {
            if (child.name == parent.name)
                continue;

            //名前書き出し
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
