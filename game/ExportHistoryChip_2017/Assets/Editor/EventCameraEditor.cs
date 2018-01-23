using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CustomEditor(typeof(EventCameraInfo))]
public class EventCameraEditor : Editor
{
    
    public override void OnInspectorGUI()
    {
        //距離。
        float length = -1.0f;

        var info = target as EventCameraInfo;
        var trans = info.gameObject.transform;
        //子供たち。
        var childs = trans.parent.GetComponentsInChildren<EventCameraInfo>();
        var index = trans.GetSiblingIndex();

        //一つ前の要素。
        var before = (0 == index) ? trans.parent.position : childs[index - 1].transform.position;

        //距離計算。
        if (index < childs.Length)
        {
            var dir = before - trans.position;
            length = dir.magnitude;
        }
        else
        {
            length = -1;
        }
       

        EditorGUILayout.LabelField("前の要素への距離", length.ToString());
        //時間。
        if (info.isCalcTime = EditorGUILayout.Toggle("時間を計算",info.isCalcTime))
        {
            EditorGUI.BeginChangeCheck();
            info.speed = EditorGUILayout.FloatField("移動速度 m/s", info.speed);
            var change = EditorGUI.EndChangeCheck();
            //if (change)
                info.time = length / info.speed;
            EditorGUILayout.LabelField("遷移時間", info.time.ToString());
        }
        else
        {
            info.time = EditorGUILayout.FloatField("遷移時間", info.time);
        }
        info.fade = EditorGUILayout.Slider("フェード開始時間",info.fade, -1.0f, info.time);
        info.Target = EditorGUILayout.ObjectField("注視点", info.Target,typeof(GameObject))as GameObject;

        if(info.Target != null)
        {
            info.gameObject.transform.LookAt(info.Target.transform);
        }
    }

}
