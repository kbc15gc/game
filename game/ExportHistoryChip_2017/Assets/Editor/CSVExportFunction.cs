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
        sw.WriteLine("name,pos,ang,sca,coll");
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

            ExportCollision c = child.gameObject.GetComponent<ExportCollision>();
            string collisiton = (Convert.ToInt32(c.Coll)).ToString();

            string line;
            line = string.Format("{0},{1},{2},{3},{4},{5}", filename, pos, ang, sca, "0", collisiton);
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
        sw.WriteLine("name,pos,ang,sca,type,shopid,messeageid,showtitle");
        foreach (Transform child in Children)
        {
            if (child.name == npcs.name)
                continue;

            //メッシュ取得
            //Mesh mesh = child.GetComponent<MeshFilter>().mesh;

            //メッシュ名を書き出し
            //sw.Write(mesh.name.Replace(" Instance", "") + ".X");
            string filename = child.name + ".X";
            string pos = Vector3ToString(child.localPosition, true);
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
        string ang = QuaternionToString(coll.localRotation);
        //スケール
        string sca = Vector3ToString(coll.lossyScale);
        ExportCollision c = coll.gameObject.GetComponent<ExportCollision>();
        string hitcamera = (Convert.ToInt32(c.HitCamera)).ToString();
        string collisiton = (Convert.ToInt32(c.Coll)).ToString();
        string line;
        line = string.Format("{0},{1},{2},{3},{4},{5}", filename, pos, ang, sca, hitcamera, collisiton);
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
        sw.WriteLine("type,param,dropexp,money,pos,Quaternion,sca,item,armor,weapon,colorflag,color");
        foreach (Transform child in Children)
        {
            if (child.name == enemys.name)
                continue;

            ExportEnemy e = child.GetComponent<ExportEnemy>();
            string type = Convert.ToString(Convert.ToInt32(e._EnemyType));

            //パラメータ。
            string param = "";
            {
                param = string.Format("[{0},{1},{2},{3},{4},{5},{6},{7},{8}]",
                    e._HP, e._MP, e._ATK, e._MAT, e._DEF, e._MDE, e._DEX, e._CRT, e._LV);
            }
            string dropexp = Convert.ToString(e._DROPEXP);
            string money = Convert.ToString(e._MONEY);

            string pos = Vector3ToString(child.position, true);
            string quaternion = QuaternionToString(child.rotation);
            //string quaternion = string.Format("{0}/{1}/{2}/{3}", child.rotation.x, child.rotation.y, child.rotation.z, 1.0f);
            string sca = Vector3ToString(child.lossyScale);

            //アイテム、防具、武器。
            string[] drop = { "", "", "" };
            {
                drop[0] += '['; drop[1] += '['; drop[2] += '[';
                bool next = false;
                for (int i = 0; i < 5; i++)
                {
                    if (next)
                    {
                        drop[0] += ',';
                        drop[1] += ',';
                        drop[2] += ',';
                    }
                    drop[0] += Convert.ToString((int)e._Item[i].code);
                    drop[1] += Convert.ToString((int)e._Armor[i].code);
                    drop[2] += Convert.ToString((int)e._Weapon[i].code);
                    next = true;
                }
                drop[0] += ']'; drop[1] += ']'; drop[2] += ']';
            }
            //確率の出力
            string probability = "[";
            {
                bool next = false;
                for (int i = 0; i < 5; i++)
                {
                    if (next)
                        probability += ',';
                    probability += Convert.ToString(e._Item[i].probability);
                }
                for (int i = 0; i < 5; i++)
                {
                    probability += ',';
                    probability += Convert.ToString(e._Armor[i].probability);
                }
                for (int i = 0; i < 5; i++)
                {
                    probability += ',';
                    probability += Convert.ToString(e._Weapon[i].probability);
                }
                probability += "]";
            }

            //カラー
            string colorflag = Convert.ToString(e._ColorFlag);
            string color = Vector4ToString(new Vector4(e._Color.r, e._Color.g, e._Color.b, e._Color.a));

            string line = string.Format("{0},{1},{2},{3},{4},{5},{6},{7},{8},{9},{10},{11},{12}", type, param, dropexp, money, pos, quaternion, sca, drop[0], drop[1], drop[2],probability, colorflag, color);

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

    static public string Vector3ToString2(Vector3 val)
    {
        //"x/y/z"の形で返す。
        
        return String.Format("{0}/{1}/{2}", val.x , val.y, val.z);
    }

    static public string Vector4ToString(Vector4 val)
    {
        //"x/y/z/w"の形で返す。
        float x = val.x;
        float y = val.y;
        float z = val.z;
        float w = val.w;
        return String.Format("{0}/{1}/{2}/{3}", x.ToString("f6"), y.ToString("f6"), z.ToString("f6"), w.ToString("f6"));
        //return String.Format("{0}/{1}/{2}/{3}", val.x, val.y, val.z, val.w);
    }

    static public string QuaternionToString(Quaternion val)
    {
        //"x/y/z/w"の形で返す。
        float x = val.x;
        float y = val.y;
        float z = val.z;
        float w = val.w;
        return String.Format("{0}/{1}/{2}/{3}", x.ToString("f6"), y.ToString("f6"), z.ToString("f6"), w.ToString("f6"));
        //return String.Format("{0}/{1}/{2}/{3}", val.x, val.y, val.z, val.w);
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
        Transform obj = group.Find(name);
        if (obj == null)
        {
            Debug.LogError(group.name + ":子に" + name + "が見つかりませんでした。");
        }
        return obj;
    }

    [MenuItem("Export/EventCameraInfo")]
    static public void ExportEventCameraInfo()
    {
        //検索。
        var name = "EventCamera";
        var export = GameObject.Find(name);
        if (export == null)
        {
            Debug.Log(name + "が見つからなかったのでエクスポートできませんでした。");
            return;
        }

        string path = Application.dataPath + "/Export/EventCameraInfo.csv";

        FileStream fs = new FileStream(path, FileMode.Create, FileAccess.Write);
        StreamWriter sw = new StreamWriter(fs);
        sw.WriteLine("pos[],rot[],time[],fade[],size");

        for (int idx = 0, num = export.transform.childCount; idx < num; idx++)
        {
            Transform No = export.transform.GetChild(idx);
            //配列の要素数。
            int size = No.transform.childCount;
            string pos = "", rot = "", time = "", fade = "";
            bool conma = false;
            for (int i = 0; i < size; i++)
            {
                if (conma)
                {
                    pos += ',';
                    rot += ',';
                    time += ',';
                    fade += ',';
                }

                //カメラの情報とか持ってるやつ。
                Transform child = No.transform.GetChild(i);
                pos += Vector3ToString(child.position,true);
                rot += QuaternionToString(child.rotation);
                var info = child.gameObject.GetComponent<EventCameraInfo>();
                time += info.time.ToString();
                fade += Convert.ToInt16(info.fade).ToString();

                conma = true;
            }
            string line = string.Format("[{0}],[{1}],[{2}],[{3}],{4}", pos, rot, time, fade, size);
            //列書き出し
            sw.WriteLine(line);
        }
        sw.Close();
        fs.Close();

        Debug.Log("ExportEventCameraInfo");
    }
}
