using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;
using System;

public class CreateEnumFromCSV : Editor
{


    //Enumを定義したファイルを作成。
    [MenuItem("Create/CreateEnum")]
    static public void CreateEnum()
    {
        //
        var fullpath = EditorUtility.OpenFilePanel("ファイルを指定してください。", "", "csv");

        //
        var idx = fullpath.IndexOf("Assets");
        fullpath.Substring(idx);

        //読み込むファイル。
        FileStream resd = new FileStream(fullpath, FileMode.Open, FileAccess.Read);
        StreamReader rw = new StreamReader(resd);

        //書き込むファイル。
        FileStream write = new FileStream("enum", FileMode.Create, FileAccess.Write);
        StreamWriter sw = new StreamWriter(write);

        sw.WriteLine("public enum" + "item" + "{");

        while (true)
        {
            string output = string.Format("", "");
            break;
        }

        sw.WriteLine("}");

        resd.Close();
        rw.Close();

        write.Close();
        sw.Close();
    }
}
