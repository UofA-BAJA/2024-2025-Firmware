using System.Collections;
using System.Collections.Generic;
using System.Linq;
using TMPro;
using UnityEngine;

public class GraphAxis : MonoBehaviour
{

    private List<TMP_Text> markers = new();
    [SerializeField] private Transform cam;

    private float maxAxisVal = 1f;
    private float minAxisVal = 0f;

    private void Awake()
    {
        markers = GetComponentsInChildren<TMP_Text>().ToList();
    }

    void LateUpdate()
    {
        foreach(TMP_Text marker in markers)
        {
            marker.transform.LookAt(cam);
            marker.transform.RotateAround(marker.transform.position, marker.transform.up, 180f);
        }
    }

    public void SetMaxAxisVal(float maxAxisVal)
    {
        this.maxAxisVal = maxAxisVal;
        for(int i = 0; i < markers.Count; i++)
        {
            markers[i].text = Mathf.Lerp(minAxisVal, maxAxisVal, (i + 1f) / markers.Count).ToString();
        }
    }

}
