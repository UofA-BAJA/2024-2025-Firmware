using System;
using System.Collections;
using System.Collections.Generic;
using Unity.Collections;
using UnityEngine;


public class WriteToTexture : MonoBehaviour
{

    [SerializeField]
    Material material;

    [SerializeField]
    Mesh mesh;

    [SerializeField, Range(1, 100)] float scale  = 10f;

    private float furthestXPoint = 1;
    private float furthestYPoint = 1;
    private float furthestZPoint = 1;

    [SerializeField] int maxNumPoints = 1000;

    ComputeBuffer timeSeriesDataBuffer;


    int currNumPoints = 0;
    int currPointIndex = 0;

    struct TimeSeriesData{
        public float val1;
        public float val2;
        public float val3;
    }

    private void Awake()
    {
        timeSeriesDataBuffer = new(maxNumPoints, sizeof(float) * 3, ComputeBufferType.Default, ComputeBufferMode.SubUpdates);


        material.SetFloat("_Scale", scale);
        material.SetFloat("_FurthestXPoint", furthestXPoint);
        material.SetFloat("_FurthestYPoint", furthestYPoint);
        material.SetFloat("_FurthestZPoint", furthestZPoint);

        material.SetFloat("_ObjectScale", .05f);

    }

    private void Start()
    {
    }

    void Update()
    {

        PopulateData();
        RenderData();

    }

    private void OnDestroy()
    {
        if(timeSeriesDataBuffer != null)
        {
            timeSeriesDataBuffer.Release();
        }
    }

    void PopulateData()
    {
        int numPoints = 50;
        NativeArray<TimeSeriesData> subData = timeSeriesDataBuffer.BeginWrite<TimeSeriesData>(currPointIndex, numPoints);

        for(int i = 0; i < numPoints; i++)
        {

            TimeSeriesData data = new TimeSeriesData();

            data.val1 = randGaussian() * 50;
            data.val2 = randGaussian() * 50;
            data.val3 = randGaussian() * 50;

            data.val1 = Mathf.Abs(data.val1);
            data.val2 = Mathf.Abs(data.val2);
            data.val3 = Mathf.Abs(data.val3);

            subData[i] = data;

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

            if(currPointIndex >= maxNumPoints)
            {
                currPointIndex = 0;
            }

            if(currNumPoints >= maxNumPoints)
            {
                currNumPoints = maxNumPoints;
            }
        }

        timeSeriesDataBuffer.EndWrite<TimeSeriesData>(numPoints);
    }

    void RenderData()
    {
        RenderParams rp = new(material)
        {
            worldBounds = new Bounds(Vector3.zero, scale * Vector3.one), // use tighter bounds
            matProps = new MaterialPropertyBlock()
        };

        rp.matProps.SetBuffer("_TimeSeriesData", timeSeriesDataBuffer);

        material.SetFloat("_FurthestXPoint", furthestXPoint);
        material.SetFloat("_FurthestYPoint", furthestYPoint);
        material.SetFloat("_FurthestZPoint", furthestZPoint);

        Graphics.RenderMeshPrimitives(rp, mesh, 0, currNumPoints);
    }


    System.Random rand = new(); //reuse this if you are generating many
    float randGaussian()
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
