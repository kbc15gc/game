using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.SceneManagement;
using System.IO;

public class CSVExportFunction : Editor
{
    [MenuItem("Export/ExportCSV")]
    static public void Export()
    {
        //オブジェクト検索
        GameObject parent = GameObject.Find("ExportObjects");
        //子供たちのTransformコンポーネントを取得
        Transform[] Children = parent.GetComponentsInChildren<Transform>();
        //ファイルパス
        string path = Application.dataPath + "/Export/" + SceneManager.GetActiveScene().name + ".csv";
        //ファイルを開く準備
        const int size = 9;
        FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write);
        StreamWriter sw = new StreamWriter(fs);
        sw.WriteLine("pos,ang,sca");
        foreach (Transform child in Children)
        {
            if (child.name == parent.name)
                continue;
            float[] output = new float[size];
            //ポジション
            output[0] = child.position.x;
            output[1] = child.position.y;
            output[2] = child.position.z;
            //回転
            output[3] = child.eulerAngles.x;
            output[4] = child.eulerAngles.y;
            output[5] = child.eulerAngles.z;
            //スケール
            output[6] = child.lossyScale.x;
            output[7] = child.lossyScale.y;
            output[8] = child.lossyScale.z;

            //名前書き出し
            sw.Write(child.name + ".X");
            sw.Write(',');
            int num = 0;
            //書き出す
            for (short i = 0; i < size; i++)
            {
                //値書き出し
                sw.Write(output[i]);
                //仕切り文字書き出し
                if (num < 2)
                {
                    num++;
                    sw.Write('/');
                }
                else
                {
                    num = 0;
                    //最後以外
                    if (i != size - 1)
                    {
                        //こんま区切り
                        sw.Write(',');
                    }
                }
            }
            //改行
            sw.Write("\r\n");
        }
        sw.Close();
        fs.Close();
    }
}
