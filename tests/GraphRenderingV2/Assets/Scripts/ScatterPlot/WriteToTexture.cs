using System;
using System.Collections;
using System.Collections.Generic;
using Unity.Collections;
using UnityEngine;


public class WriteToTexture : MonoBehaviour
{

    Texture2D textureBuffer;

    [SerializeField]
    Material material;

    [SerializeField]
    Mesh mesh;
    [SerializeField, Range(1, 100)] float scale  = 10f;

    private float furthestXPoint = 1;
    private float furthestYPoint = 1;
    private float furthestZPoint = 1;

    [SerializeField, Range(1, 1024)] int texSize = 1024;

    // ComputeBuffer timeSeriesDataBuffer;


    int currNumPoints = 0;
    int currPointIndex = 0;

    struct TimeSeriesData{
        public float val1;
        public float val2;
        public float val3;
    }

    private void Awake()
    {
        textureBuffer = new(texSize, texSize, TextureFormat.RGBAFloat, false)
        {
            filterMode = FilterMode.Point
        };
        // timeSeriesDataBuffer = new(maxNumPoints, sizeof(float) * 3, ComputeBufferType.Structured);


        material.SetFloat("_Scale", scale);
        material.SetFloat("_FurthestXPoint", furthestXPoint);
        material.SetFloat("_FurthestYPoint", furthestYPoint);
        material.SetFloat("_FurthestZPoint", furthestZPoint);

        material.SetFloat("_ObjectScale", .1f);


        if(SystemInfo.supportsComputeShaders){
            Debug.Log("Compute shaders are supported");
        }
        else{
            Debug.Log("Compute shaders are NOT supported");
        }

    }

    private void Start()
    {
    }

    void Update()
    {

        // PopulateData();
        RenderData();

    }

    private void OnDestroy()
    {
        // if(timeSeriesDataBuffer != null)
        // {
        //     timeSeriesDataBuffer.Release();
        // }
    }

    public void PopulateData(float x, float y, float z)
    {

        // #if UNITY_INSTANCING_ENABLED

        int numPoints = 1;
        // NativeArray<TimeSeriesData> subData = timeSeriesDataBuffer.BeginWrite<TimeSeriesData>(currPointIndex, numPoints);

        // List<TimeSeriesData> subData = new(numPoints);


        for(int i = 0; i < numPoints; i++)
        {

            int xCoord = currPointIndex % texSize;
            int yCoord = currPointIndex / texSize;

            TimeSeriesData data = new()
            {
                val1 = x,
                val2 = y,
                val3 = z
            };


            data.val1 = Mathf.Abs(data.val1);
            data.val2 = Mathf.Abs(data.val2);
            data.val3 = Mathf.Abs(data.val3);

            Color color = new(data.val1, data.val2, data.val3);

            textureBuffer.SetPixel(xCoord, yCoord, color);
            // subData[i] = data;

            // subData.Insert(i, data);

            if(data.val1 > furthestXPoint)
            {
                furthestXPoint = data.val1;
            }
            if (data.val2 > furthestYPoint)
            {
                furthestYPoint = data.val2;
            }
            if (data.val3 > furthestZPoint)
            {
                furthestZPoint = data.val3;
            }


            currPointIndex++;
            currNumPoints++;

            if(currPointIndex >= texSize * texSize)
            {
                currPointIndex = 0;
            }

            if(currNumPoints >= texSize * texSize)
            {
                currNumPoints = texSize * texSize;
            }
        }

        textureBuffer.Apply();
        
        // timeSeriesDataBuffer.SetData(subData, 0, currPointIndex, numPoints);

        // timeSeriesDataBuffer.EndWrite<TimeSeriesData>(numPoints);

        // #endif
    }

    void RenderData()
    {
        RenderParams rp = new(material)
        {
            worldBounds = new Bounds(Vector3.zero, scale * Vector3.one), // use tighter bounds
            matProps = new MaterialPropertyBlock()
        };

        // rp.matProps.SetBuffer("_TimeSeriesData", timeSeriesDataBuffer);

        material.SetTexture("_TimeSeriesData", textureBuffer);
        material.SetFloat("_FurthestXPoint", furthestXPoint);
        material.SetFloat("_FurthestYPoint", furthestYPoint);
        material.SetFloat("_FurthestZPoint", furthestZPoint);

        Graphics.RenderMeshPrimitives(rp, mesh, 0, currNumPoints);
    }


    System.Random rand = new(); //reuse this if you are generating many
    float RandGaussian()
    {
        double mean = 0;
        double stdDev = 1;

        double u1 = 1.0 - rand.NextDouble(); //uniform(0,1] random doubles
        double u2 = 1.0 - rand.NextDouble();
        double randStdNormal = Math.Sqrt(-2.0 * Math.Log(u1)) *
                     Math.Sin(2.0 * Math.PI * u2); //random normal(0,1)
        double randNormal =
                     mean + stdDev * randStdNormal; //random normal(mean,stdDev^2)

        return (float)randNormal;
    }


}
