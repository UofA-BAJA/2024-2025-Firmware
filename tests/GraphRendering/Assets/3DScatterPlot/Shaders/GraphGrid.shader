Shader "Custom/GraphGrid"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
    }
    SubShader
    {
        Tags { "RenderType"="Transparent" "Queue"="Transparent"}
        LOD 100
        Blend SrcAlpha OneMinusSrcAlpha
        Cull Off
        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            //sampler2D _MainTex;
            //float4 _MainTex_ST;

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                //o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                o.uv = v.uv;
                return o;
            }


            /**
             * Returns ~1 if the pixel lies on the grid, zero if it doesn't
             */
            float GridTest(float2 uv)
            {

                float x = uv.x * (10.0); // 10 is the world size of the graph.
                x = abs(sin(3.1415926 * .8 * x)); // .8 is the num of subdivisions (8/10)
                x = 1.0 - x;
                x = smoothstep(.8, 1.0, x);

                float y = uv.y * (10.0);
                y = abs(sin(3.1415926 * .8 * y));
                y = 1.0 - y;
                y = smoothstep(.88, 1.0, y);


                //return uv.x;
                return max(x, y);

            }

            fixed4 frag (v2f i) : SV_Target
            {
                //fixed4 col = tex2D(_MainTex, i.uv);
                return float4(0, 0, 0, GridTest(i.uv) * .8);
            }
            ENDCG
        }
    }
}
