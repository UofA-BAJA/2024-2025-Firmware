Shader "Custom/GraphPointLegacy"
{
    Properties
    {

        _TimeSeriesData ("Data Texture", 2D) = "white" {}
        _Scale ("Scale", Float) = 1
        _FurthestXPoint ("Furthest X", Float) = 1
        _FurthestYPoint ("Furthest Y", Float) = 1
        _FurthestZPoint ("Furthest Z", Float) = 1
        _ObjectScale ("Object Scale", Float) = 1
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma target 3.1
            #pragma multi_compile_instancing
            #include "UnityCG.cginc"

            struct appdata_custom {
                float4 vertex : POSITION;
                float2 uv     : TEXCOORD0;
            };

            struct v2f
            {
                float4 pos : SV_POSITION;
                float4 color : COLOR0;
            };

            sampler2D _TimeSeriesData;
            float4 _TimeSeriesData_ST;
            uniform int _TexWidth;

            half _Scale;

            // These values are used to "normalize" the rest of the data.
            half _FurthestXPoint;
            half _FurthestYPoint;
            half _FurthestZPoint;

            half _ObjectScale;

            v2f vert (appdata_custom v, uint instanceID : SV_InstanceID)
            {
                v2f o;

                half invTexWidth = 1.0 / _TexWidth;
                float3 invFurthest = half3(1.0 / _FurthestXPoint, 1.0 / _FurthestYPoint, 1.0 / _FurthestZPoint);

                // Compute the 2D indices from the instanceID
                int xIndex = instanceID % _TexWidth;
                int yIndex = instanceID * invTexWidth;

                // Convert indices to normalized UV coordinates (add 0.5 to sample the center of the pixel)
                half uvX = (xIndex + 0.5) * invTexWidth;
                half uvY = (yIndex + 0.5) * invTexWidth;

                half3 pos = tex2Dlod(_TimeSeriesData, half4(uvX, uvY, 0, 0)).rgb;

                pos *= invFurthest * _Scale;


                half3 instanceCenter = mul(unity_ObjectToWorld, float4(pos, 1.0)).xyz;

                // CHAT CODE
                // Compute the camera's right and up vectors from the inverse view matrix.
                // In camera space, (1,0,0) is right and (0,1,0) is up.
                half3 camRight = half3(UNITY_MATRIX_I_V[0][0], UNITY_MATRIX_I_V[1][0], UNITY_MATRIX_I_V[2][0]);
                half3 camUp    = half3(UNITY_MATRIX_I_V[0][1], UNITY_MATRIX_I_V[1][1], UNITY_MATRIX_I_V[2][1]);

                // Assume v.vertex holds the per-vertex offset for the quad (e.g. (-0.5,-0.5,0) to (0.5,0.5,0)).
                // Compute the billboard offset using the camera's right and up vectors.
                half3 billboardOffset = (v.vertex.x * camRight + v.vertex.y * camUp) * _ObjectScale;

                // Final world position is the instance center plus the billboard offset.
                half3 finalPos = instanceCenter + billboardOffset;
                // END OF CHAT CODE

                o.pos = mul(UNITY_MATRIX_VP, half4(finalPos, 1.0));

                o.color = half4(pos.r * .1, pos.g * .1, pos.b * .1, 1);
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // fixed4 col = tex2D(_TimeSeriesData, i.uv);
                return i.color;
            }
            ENDCG
        }
    }
}
