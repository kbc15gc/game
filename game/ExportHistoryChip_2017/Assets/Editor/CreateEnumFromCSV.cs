using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;
using System;

public class CreateEnumFromCSV : EditorWindow
{
    string name = "";
    //! MenuItem("メニュー名/項目名") のフォーマットで記載してね
    [MenuItem("Create/CreateEnum")]
    static void ShowWindow()
    {
        // ウィンドウを表示！
        EditorWindow.GetWindow<CreateEnumFromCSV>();
    }

    /**
     * ウィンドウの中身
     */
    void OnGUI()
    {
        name = EditorGUILayout.TextField(name);
        if (GUILayout.Button("作成"))
        {
            CreateEnum(name);
        }
    }

    //Enumを定義したファイルを作成。
    [MenuItem("Create/CreateItemCode")]
    static public void CreateItemCode()
    {
        CreateEnum("ItemCodeE");
    }
    static public void CreateEnum(string name)
    {
        //
        var fullpath = EditorUtility.OpenFilePanel("ファイルを指定してください。", "", "csv");

        //
        var idx = fullpath.IndexOf("Assets");
        //fullpath.Substring(idx);

        //読み込むファイル。
        FileStream resd = new FileStream(fullpath, FileMode.Open, FileAccess.Read);
        StreamReader rw = new StreamReader(resd);

        //書き込むファイル。
        FileStream write = new FileStream("Assets/" + name + ".cs", FileMode.Create, FileAccess.ReadWrite);
        StreamWriter sw = new StreamWriter(write);

        rw.ReadLine();
        sw.WriteLine("public enum " + name + "{");
        sw.WriteLine("    None = -1,");

        while (!rw.EndOfStream)
        {
            string[] line = rw.ReadLine().Split(',');
            string output = string.Format("    {0} = {1}", line[2], line[1]);
            output = output.Replace(".", "");
            output = output.Replace("・", "");
            output = output.Replace("-", "");
            output = output.Replace("！", "");
            if (!rw.EndOfStream)
                output += ',';
            sw.WriteLine(output);
        }

        sw.WriteLine("}");

        resd.Close();
        rw.Close();

        sw.Flush();

        sw.Close();
        write.Close();
    }
}
