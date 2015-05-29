float4x4 matWorld[4] : WORLD;   // ワールド変換行列配列
float4x4 matView : VIEW;        // ビュー変換行列
float4x4 matProj : PROJECTION;  // 射影変換行列
float4x4 matPos;

// 波の移動
float Offset;
int iBlendNum;                  // ブレンドする配列の数
texture diffuseTexture;
sampler diffuseSampler = sampler_state {
	texture = <diffuseTexture>;
};

struct OUTPUT_VS
{
	float4 P_after : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct OUTPUTz_VS
{
	float4 P_after : POSITION;
	float4 specular : SPECULAR;
	float2 texCoord : TEXCOORD0;
};

OUTPUT_VS BasicTrans_VS( float4 Pos : POSITION, float4 W : BLENDWEIGHT, float2 inTexCoord : TEXCOORD0 )
{
   OUTPUT_VS outVS = (OUTPUT_VS)0;
   outVS.texCoord = inTexCoord;
   float   Weight[4] = (float[4])W;      // 重みをfloatに分割します
   float    LastBlendWeight = 0.0f;        // 最後の行列に掛けられる重み
   float4x4 matCombWorld = 0.0f;           // 合成ワールド変換行列

   // ワールド変換行列をブレンド
   for(int i=0; i<iBlendNum-1; i++)
   {
      LastBlendWeight += Weight[i];   // 最後の重みをここで計算しておく
      matCombWorld += matWorld[i] * Weight[i];
   }

   // 最後の重みを足し算
   matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);


   outVS.P_after = mul( Pos, matCombWorld );   // ワールド変換
   //outVS.P_after = mul( Pos, matPos );
   outVS.P_after = mul( outVS.P_after, matView );    // ビュー変換
   outVS.P_after = mul( outVS.P_after, matProj );    // 射影変換

   return outVS;
}

// ピクセルシェーダ
float4 NoWorkingPixelShader( float2 texCoord : TEXCOORD0 ) : COLOR
{
 float4 Color;
 float2 xy;
float ax;
float ay;
    float4 output;
    float2 uv;
    uv = texCoord;

	//output = float4(1.f,1.f,0.f,1.f);
   // output = tex2D( diffuseSampler, uv );
   // output *= float4( 1.0f,1.0f,1.0f,1.0f);
   // output /= 1.0;
   // return output;


  ax=texCoord.x+sin((texCoord.y+ Offset )  *  Period  ) * Amplitude;
  ay=texCoord.y;
  Color=tex2D(diffuseSampler,float2(ax,ay));
  return Color;
 //return float4( 1.0f,1.0f,1.0f,1.0f);
}


technique BasicTec
{
   pass P0
   {
      VertexShader = compile vs_2_0 BasicTransform();
      PixelShader = compile ps_2_0 NoWorkingPixelShader();
   }
}
