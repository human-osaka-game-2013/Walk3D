float4x4 matWorld[4] : WORLD;   // ワールド変換行列配列
float4x4 matView : VIEW;        // ビュー変換行列
float4x4 matProj : PROJECTION;  // 射影変換行列
float4x4 matPos;
// 周期
float Period;

// 振幅
float Amplitude;
// 波の移動
float Offset;
int iBlendNum;                  // ブレンドする配列の数
texture diffuseTexture;
sampler diffuseSampler = sampler_state {
	texture = <diffuseTexture>;
};

struct VS_OUTPUT_S
{
  float4 Pos : POSITION;   // 射影変換座標
   float4 ShadowMapTex : TEXCOORD0;   // Zバッファテクスチャ
};

// 頂点シェーダ
VS_OUTPUT_S ZBufferCalc_VS( float4 Pos : POSITION )
{
   VS_OUTPUT_S Out = (VS_OUTPUT_S)0;

   // 普通にワールドビュー射影行列をする
   Out.Pos = mul( Pos, matCombWorld );   // ワールド変換
   Out.Pos = mul( Out.Pos, matView );    // ビュー変換
   Out.Pos = mul( Out.Pos, matProj );    // 射影変換

   // テクスチャ座標を頂点に合わせる
   Out.ShadowMapTex = Out.Pos;

   return Out;
}


// ピクセルシェーダ
float4 ZBufferPlot_PS( float4 ShadowMapTex : TEXCOORD0 ) : COLOR
{
   // Z値算出
   return ShadowMapTex.z / ShadowMapTex.w;
}

struct OUTPUT_VS
{
	float4 P_after : POSITION;
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
    //float4 output;
  //  float2 uv;
   // uv = texCoord;

	//output = float4(1.f,1.f,0.f,1.f);
   // output = tex2D( diffuseSampler, uv );
   // output *= float4( 1.0f,1.0f,1.0f,1.0f);
   // output /= 1.0;
   // return output;


  ax=texCoord.x+sin((texCoord.y+ Offset )  *  Period  ) * Amplitude;
  ay=texCoord.y;
  Color=tex2D(diffuseSampler,texCoord/*float2(ax,ay)*/);
  return Color;
 //return float4( 1.0f,1.0f,1.0f,1.0f);
}


technique BasicTec
{
   pass P0
   {
      VertexShader = compile vs_2_0 BasicTrans_VS();
      PixelShader = compile ps_2_0 NoWorkingPixelShader();
   }
//   pass P1
//   {
//      VertexShader = compile vs_2_0 ZBufferCalc_VS();
//      PixelShader = compile ps_2_0 ZBufferPlot_PS();
//   }
}

