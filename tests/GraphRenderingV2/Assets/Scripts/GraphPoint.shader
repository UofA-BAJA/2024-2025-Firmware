Shader "Custom/GraphPoint"
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

            struct v2f
            {
                float4 pos : SV_POSITION;
                float4 color : COLOR0;
            };

            sampler2D _TimeSeriesData;
            float4 _TimeSeriesData_ST;
            
            float _Scale;

            // These values are used to "normalize" the rest of the data.
            float _FurthestXPoint;
            float _FurthestYPoint;
            float _FurthestZPoint;

            float _ObjectScale;

            v2f vert (appdata_base v, uint instanceID : SV_InstanceID)
            {
                v2f o;

                float texWidth = 1024.0;

                // Compute the 2D indices from the instanceID
                float xIndex = fmod(instanceID, texWidth);
                float yIndex = floor(instanceID / texWidth);

                // Convert indices to normalized UV coordinates (add 0.5 to sample the center of the pixel)
                float uvX = (xIndex + 0.5) / texWidth;
                float uvY = (yIndex + 0.5) / texWidth;

                float3 pos = tex2Dlod(_TimeSeriesData, float4(uvX, uvY, 0, 0)).rgb;

                pos.r /= _FurthestXPoint;
                pos.g /= _FurthestYPoint;
                pos.b /= _FurthestZPoint;
                pos *= _Scale;


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

                o.color = float4(pos.r / 10, pos.g / 10, pos.b / 10, 1);
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
