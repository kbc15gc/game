using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.SceneManagement;
using System.IO;
using System;
using System.Text;

/// <summary>
/// CSVファイルを読み込み、データを保持するクラス
/// </summary>
public class CSVReader
{
    public const string _EXTENSION = ".csv";

    public const char _SPLIT_CHAR = ',';

    string m_commentString = "//";

    // 読み込んだデータ
    List<List<string>> m_data = new List<List<string>>();
    TextAsset m_textAsset = null;

    public CSVReader(string comment = "//")
    {
        m_textAsset = null;
        m_data = new List<List<string>>(10);
        m_commentString = comment;
    }

    private StringReader CreateTextReader(string fileName)
    {
        m_textAsset = Resources.Load<TextAsset>(fileName);

        return new StringReader(m_textAsset.text);
    }

    public bool Load(string fileName)
    {
        m_data.Clear();
        StringReader reader = CreateTextReader(fileName);

        int counter = 0;
        string line = "";
        while ((line = reader.ReadLine()) != null)
        {
            // コメントが入っている時はスキップする
            if (line.Contains(m_commentString))
            {
                continue;
            }

            // 今の列をマス毎に区切る
            string[] fields = line.Split(_SPLIT_CHAR);
            m_data.Add(new List<string>());

            foreach (var field in fields)
            {
                if (field.Contains(m_commentString) || field == "")
                {
                    continue;
                }
                m_data[counter].Add(field);
            }
            counter++;
        }

        // 読み込んだリソースを開放する
        Resources.UnloadAsset(m_textAsset);
        m_textAsset = null;
        Resources.UnloadUnusedAssets();
        return true;
    }

    public string GetString(int row, int col)
    {
        return m_data[row][col];
    }

    public bool GetBool(int row, int col)
    {
        string data = GetString(row, col);
        return bool.Parse(data);
    }

    public int GetInt(int row, int col)
    {
        string data = GetString(row, col);
        return int.Parse(data);
    }

    public float GetFloat(int row, int col)
    {
        string data = GetString(row, col);
        return float.Parse(data);
    }

    public List<string> GetLine(int row)
    {
        return m_data[row];
    }

    // 指定した行の要素数。
    public int GetMaxBlock_OneLine(int line)
    {
        return m_data[line].Count;
    }

    // 行数。
    public int GetMaxLine()
    {
        return m_data.Count;
    }
}

//CSVから読み込んだ情報でエディターに再配置(魔王城の未進化状態の建物データにのみ対応)。
public class LoadExportData : Editor {
    //グループを出力
    [MenuItem("Load/LoadGroup")]

    static void LoadObject()
    {
        //ファイルパス
        string path = /*Application.dataPath + */"Group3AObj";

        CSVReader reader = new CSVReader();
        reader.Load(path);

        GameObject lastCreate = null;

        for (int row = 1; row < reader.GetMaxLine(); row++)
        {

            List<string> datas = reader.GetLine(row);

            GameObject obj = new GameObject();

            if (datas[0] != "coll")
            {
                // オブジェクト生成。
                lastCreate = obj;

                // メッシュ生成。
                {
                    obj.AddComponent<MeshRenderer>();
                    string name = datas[0];
                    obj.name = name.Replace(".X", "");
                    string fileName = obj.name;
                    fileName = "Model/" + fileName;
                    Debug.Log(fileName);
                    Mesh mesh = Resources.Load<Mesh>(fileName);
                    Debug.Log(mesh);
                    mesh.RecalculateNormals();
                    mesh.RecalculateBounds();

                    obj.AddComponent<MeshFilter>().mesh = mesh;
                }
            }
            else if (lastCreate)
            {
                // 手動コリジョン。
                obj.name = "coll";
                obj.AddComponent<BoxCollider>();
                obj.transform.parent = lastCreate.transform;
            }

            // Transform設定。
            {
                string[] strPos = datas[1].Split('/');
                Vector3 pos = new Vector3(-float.Parse(strPos[0]), float.Parse(strPos[1]), -float.Parse(strPos[2]));
                obj.transform.position = pos;

                string[] strRot = datas[2].Split('/');
                Quaternion rot = new Quaternion(float.Parse(strRot[0]), float.Parse(strRot[1]), float.Parse(strRot[2]), float.Parse(strRot[3]));
                obj.transform.rotation = rot;

                string[] strScl = datas[3].Split('/');
                Vector3 scl = new Vector3(float.Parse(strScl[0]), float.Parse(strScl[1]), float.Parse(strScl[2]));
                obj.transform.localScale = scl;
            }

            ExportCollision export = obj.AddComponent<ExportCollision>();

            if (int.Parse(datas[4]) >= 1)
            {
                export.HitCamera = false;
            }
            else
            {
                export.HitCamera = true;
            }

            if (int.Parse(datas[5]) >= 1)
            {
                export.Coll = true;
            }
            else
            {
                export.Coll = false;
            }
        }
    }
}
