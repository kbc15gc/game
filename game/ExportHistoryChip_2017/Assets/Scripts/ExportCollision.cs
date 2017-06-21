using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ExportCollision : MonoBehaviour {
    [SerializeField, TooltipAttribute("カメラと当たるかどうか？")]
    public bool HitCamera = true;
}
