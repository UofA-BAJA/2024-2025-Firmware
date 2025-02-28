using RoyTheunissen.Graphing;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Test : MonoBehaviour
{
    [SerializeField] GraphComponent testGraph;

    private void Awake()
    {
    }
    void Update()
    {
        testGraph.Graph.SetDuration(10f);
        testGraph.Graph.AddValue(Mathf.Cos(Time.time * Mathf.PI), "Test value");
    }
}
