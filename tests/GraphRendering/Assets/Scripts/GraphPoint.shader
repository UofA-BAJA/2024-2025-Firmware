Shader "Custom/GraphPoint"
{
    SubShader
    {
        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct v2f
            {
                float4 pos : SV_POSITION;
                float4 color : COLOR0;
            };

            //uniform float4x4 _ObjectToWorld;
            //uniform float _NumInstances;

            struct TimeSeriesData{
                float val1;
                float val2;
                float val3;
            };

            StructuredBuffer<TimeSeriesData> _TimeSeriesData;
            float _Scale;

            // These values are used to "normalize" the rest of the data.
            float _FurthestXPoint;
            float _FurthestYPoint;
            float _FurthestZPoint;

            float _ObjectScale;

            v2f vert(appdata_base v, uint instanceID : SV_InstanceID)
            {

                v2f o;

                float3 pos = float3(
                    _TimeSeriesData[instanceID].val1 / _FurthestXPoint,
                    _TimeSeriesData[instanceID].val2 / _FurthestYPoint,
                    _TimeSeriesData[instanceID].val3 / _FurthestZPoint
                );
                pos *= _Scale;


                //unity_ObjectToWorld._m00_m11_m22 *= _ObjectScale;
                float4 wpos = mul(unity_ObjectToWorld, v.vertex * _ObjectScale + float4(pos, 0));                  

                float3 instanceCenter = mul(unity_ObjectToWorld, float4(pos, 1.0)).xyz;

                // CHAT CODE

                // Compute the camera's right and up vectors from the inverse view matrix.
                // In camera space, (1,0,0) is right and (0,1,0) is up.
                float3 camRight = normalize(float3(UNITY_MATRIX_I_V[0][0], UNITY_MATRIX_I_V[1][0], UNITY_MATRIX_I_V[2][0]));
                float3 camUp    = normalize(float3(UNITY_MATRIX_I_V[0][1], UNITY_MATRIX_I_V[1][1], UNITY_MATRIX_I_V[2][1]));

                // Assume v.vertex holds the per-vertex offset for the quad (e.g. (-0.5,-0.5,0) to (0.5,0.5,0)).
                // Compute the billboard offset using the camera's right and up vectors.
                float3 billboardOffset = (v.vertex.x * camRight + v.vertex.y * camUp) * _ObjectScale;

                // Final world position is the instance center plus the billboard offset.
                float3 finalPos = instanceCenter + billboardOffset;
                // END OF CHAT CODE

                o.pos = mul(UNITY_MATRIX_VP, float4(finalPos, 1.0));

                o.color = float4(_TimeSeriesData[instanceID].val1 / 100, _TimeSeriesData[instanceID].val2 / 100, _TimeSeriesData[instanceID].val3 / 100, 1);
                return o;
            }

            float4 frag(v2f i) : SV_Target
            {
                return i.color;
            }
            ENDCG
        }
    }
}