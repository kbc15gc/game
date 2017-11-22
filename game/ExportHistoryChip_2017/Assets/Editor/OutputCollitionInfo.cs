using UnityEngine;
using System.Collections;
using UnityEditor;
using System.IO;
using System.Text;

public class OutputCollitionInfo2D : EditorWindow
{

    [MenuItem("Window/コリジョン 出力")]
    public static void ShowWindow()
    {
        GameObject collision = Selection.activeGameObject;
        Transform[] collisions = collision.GetComponentsInChildren<Transform>();
        string headerTxt = "";
        foreach (Transform tr in collisions)
        {
            if (tr.gameObject == collision)
            {
                continue;
            }
            
            headerTxt += "{\n";
            headerTxt += string.Format("\t\"{0}.x\",\n", tr.name);
            headerTxt += string.Format("\tVector3({0:f}f, {1:f}f, {2:f}f),             //平行移動\n", -tr.position.x, tr.position.y, -tr.position.z);
            headerTxt += string.Format("\tQuaternion({0:f}, {1:f}f, {2:f}f, {3:f}f ),    //回転\n", tr.rotation.x, tr.rotation.y, tr.rotation.z, tr.rotation.w);
            headerTxt += string.Format("\tVector3({0:f}f, {1:f}f, {2:f}f ),            //拡大\n", tr.lossyScale.x, tr.lossyScale.y, tr.lossyScale.z);
            headerTxt += "},\n";
        }
      //  collision.name
        StreamWriter sw = new StreamWriter("C:/GitHub/game/game/ExportHistoryChip_2017/Assets/Export" + collision.name + ".h", false, Encoding.UTF8);
        sw.Write(headerTxt);
        sw.Close();

    }

    void OnGUI()
    {
        
    }

}
