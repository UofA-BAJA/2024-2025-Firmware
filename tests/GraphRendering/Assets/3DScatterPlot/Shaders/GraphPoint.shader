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
                half val1;
                half val2;
                half val3;
            };

            StructuredBuffer<TimeSeriesData> _TimeSeriesData;
            half _Scale;

            // These values are used to "normalize" the rest of the data.
            half _FurthestXPoint;
            half _FurthestYPoint;
            half _FurthestZPoint;

            half _ObjectScale;

            v2f vert(appdata_base v, uint instanceID : SV_InstanceID)
            {

                v2f o;

                half3 pos = half3(
                    _TimeSeriesData[instanceID].val1 / _FurthestXPoint,
                    _TimeSeriesData[instanceID].val2 / _FurthestYPoint,
                    _TimeSeriesData[instanceID].val3 / _FurthestZPoint
                );
                pos *= _Scale;


                //unity_ObjectToWorld._m00_m11_m22 *= _ObjectScale;
                //float4 wpos = mul(unity_ObjectToWorld, v.vertex * _ObjectScale + float4(pos, 0));                  

                // CHAT CODE
                half3 instanceCenter = mul(unity_ObjectToWorld, half4(pos, 1.0)).xyz;
                // Compute the camera's right and up vectors from the inverse view matrix.
                // In camera space, (1,0,0) is right and (0,1,0) is up.
                half3 camRight = normalize(half3(UNITY_MATRIX_I_V[0][0], UNITY_MATRIX_I_V[1][0], UNITY_MATRIX_I_V[2][0]));
                half3 camUp    = normalize(half3(UNITY_MATRIX_I_V[0][1], UNITY_MATRIX_I_V[1][1], UNITY_MATRIX_I_V[2][1]));
                // Assume v.vertex holds the per-vertex offset for the quad (e.g. (-0.5,-0.5,0) to (0.5,0.5,0)).
                // Compute the billboard offset using the camera's right and up vectors.
                half3 billboardOffset = (v.vertex.x * camRight + v.vertex.y * camUp) * _ObjectScale;
                // Final world position is the instance center plus the billboard offset.
                half3 finalPos = instanceCenter + billboardOffset;
                // END OF CHAT CODE
                o.pos = mul(UNITY_MATRIX_VP, half4(finalPos, 1.0));

                o.color = half4(pos.x * .1, pos.y * .1, pos.z * .1, 1);
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