using System;
using System.Collections;
using System.Collections.Generic;
using Unity.Collections;
using UnityEngine;

public class ScatterPlot3D : MonoBehaviour
{

    [SerializeField] private int test = 0;

    [SerializeField] private LayerMask graphLayer;

    [SerializeField] private GraphAxis xAxis;
    [SerializeField] private GraphAxis yAxis;
    [SerializeField] private GraphAxis zAxis;

    [SerializeField] private Material graphPointMaterial;
    [SerializeField] private Material graphPointMaterialLegacy;

    private Material currGraphPointmaterial;

    [SerializeField] private Mesh quad;

    [SerializeField, Range(1, 100)] private float graphScale = 10f;
    [SerializeField] private int maxNumPoints = 1000;


    private int texWidth;
    private int maxTextureSize = 2048;


    private float furthestXPoint = 1;
    private float furthestYPoint = 1;
    private float furthestZPoint = 1;

    [SerializeField] Camera cam;

    private ComputeBuffer timeSeriesDataBuffer;
    // timeSeriesDataTexture is for support for computers that don't have compute shader capabilities
    Texture2D timeSeriesDataTexture;

    private bool useComputeShader;


    int currNumPoints = 0;
    int currPointIndex = 0;

    struct TimeSeriesData
    {
        public float val1;
        public float val2;
        public float val3;
    }

    private void Awake()
    {
        cam.enabled = false;
        // Make all the children of this object on the graph layer, that way the main camera doesn't render them.
        gameObject.layer = (int)Mathf.Log(graphLayer.value, 2);

        foreach (Transform go in GetComponentsInChildren<Transform>())
        {
            go.gameObject.layer = (int)Mathf.Log(graphLayer.value, 2);
        }

        useComputeShader = SystemInfo.supportsComputeShaders;
        //useComputeShader = false;

        if (useComputeShader)
        {
            timeSeriesDataBuffer = new(maxNumPoints, sizeof(float) * 3, ComputeBufferType.Default, ComputeBufferMode.SubUpdates);
            currGraphPointmaterial = graphPointMaterial;
        }
        else
        {
            texWidth = Mathf.FloorToInt(Mathf.Sqrt(maxNumPoints));

            if (texWidth > maxTextureSize)
            {
                texWidth = maxTextureSize;
            }

            currGraphPointmaterial = graphPointMaterialLegacy;
            currGraphPointmaterial.SetInteger("_TexWidth", texWidth);

            timeSeriesDataTexture = new(texWidth, texWidth, TextureFormat.RGBAFloat, false)
            {
                filterMode = FilterMode.Point
            };

            Debug.LogWarning("Compute shaders are not supported. Fallback to legacy graph point shader.");
        }


        currGraphPointmaterial.SetFloat("_Scale", graphScale);
        currGraphPointmaterial.SetFloat("_FurthestXPoint", furthestXPoint);
        currGraphPointmaterial.SetFloat("_FurthestYPoint", furthestYPoint);
        currGraphPointmaterial.SetFloat("_FurthestZPoint", furthestZPoint);

        currGraphPointmaterial.SetFloat("_ObjectScale", .1f);

    }

    private void Start()
    {

    }


    int numFrames = 0;
    void Update()
    {

        if(numFrames % 2 == test)
        {
            PopulateData();
            RenderData();
            cam.Render();
        }

        numFrames++;
    }

    private void OnDestroy()
    {
        if (timeSeriesDataBuffer != null)
        {
            timeSeriesDataBuffer.Release();
        }
    }

    void PopulateData()
    {
        int numPoints = 50;
        NativeArray<TimeSeriesData> subData = new NativeArray<TimeSeriesData>();

        if (useComputeShader)
        {
            subData = timeSeriesDataBuffer.BeginWrite<TimeSeriesData>(currPointIndex, numPoints);
        }

        for (int i = 0; i < numPoints; i++)
        {

            float x = 10 * currPointIndex / (float)numPoints;

            TimeSeriesData data = new()
            {
                val1 = randGaussian(),
                val2 = randGaussian() * 10,
                val3 = randGaussian() * 10,
            };

            data.val1 = Mathf.Abs(data.val1);
            data.val2 = Mathf.Abs(data.val2);
            data.val3 = Mathf.Abs(data.val3);

            if (useComputeShader)
            {
                subData[i] = data;
            }
            else
            {
                int xCoord = currPointIndex % texWidth;
                int yCoord = currPointIndex / texWidth;
                Color color = new(data.val1, data.val2, data.val3);
                timeSeriesDataTexture.SetPixel(xCoord, yCoord, color);
            }




            if (data.val1 > furthestXPoint)
            {
                furthestXPoint = (int)data.val1 + 1;
                xAxis.SetMaxAxisVal(furthestXPoint);
            }
            if (data.val2 > furthestYPoint)
            {
                furthestYPoint = (int)data.val2 + 1;
                yAxis.SetMaxAxisVal(furthestYPoint);

            }
            if (data.val3 > furthestZPoint)
            {
                furthestZPoint = (int)data.val3 + 1;
                zAxis.SetMaxAxisVal(furthestZPoint);
            }


            currPointIndex++;
            currNumPoints++;

            if (currPointIndex >= maxNumPoints)
            {
                currPointIndex = 0;
            }

            if (currNumPoints >= maxNumPoints)
            {
                currNumPoints = maxNumPoints;
            }
        }

        if (useComputeShader)
        {
            timeSeriesDataBuffer.EndWrite<TimeSeriesData>(numPoints);
        }
        else
        {
            timeSeriesDataTexture.Apply();
        }

    }

    private float NormalDistributionPDF(float x, float mean, float stdDev)
    {
        float y = 1f / Mathf.Sqrt(2f * Mathf.PI * stdDev * stdDev);
        y *= Mathf.Exp(-((x - mean) * (x - mean)) / (2f * stdDev * stdDev));
        return y;
    }

    void RenderData()
    {
        RenderParams rp = new(currGraphPointmaterial)
        {
            worldBounds = new Bounds(Vector3.zero, graphScale * Vector3.one), // use tighter bounds
            matProps = new MaterialPropertyBlock()
        };


        if (useComputeShader)
        {
            rp.matProps.SetBuffer("_TimeSeriesData", timeSeriesDataBuffer);
        }
        else
        {
            rp.matProps.SetTexture("_TimeSeriesData", timeSeriesDataTexture);
        }


        currGraphPointmaterial.SetFloat("_FurthestXPoint", furthestXPoint);
        currGraphPointmaterial.SetFloat("_FurthestYPoint", furthestYPoint);
        currGraphPointmaterial.SetFloat("_FurthestZPoint", furthestZPoint);

        Graphics.RenderMeshPrimitives(rp, quad, 0, currNumPoints);
    }


    System.Random rand = new(); //reuse this if you are generating many
    float randGaussian(double mean = 0, double stdDev = 1)
    {

        double u1 = 1.0 - rand.NextDouble(); //uniform(0,1] random doubles
        double u2 = 1.0 - rand.NextDouble();
        double randStdNormal = Math.Sqrt(-2.0 * Math.Log(u1)) *
                     Math.Sin(2.0 * Math.PI * u2); //random normal(0,1)
        double randNormal =
                     mean + stdDev * randStdNormal; //random normal(mean,stdDev^2)

        return (float)randNormal;
    }
}
