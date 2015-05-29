float4x4 matWorldViewProj;   // ワールドビュー射影変換行列
float4x4 matWorld[4] : WORLD;   // ワールド変換行列配列
float4x4 matView : VIEW;        // ビュー変換行列
float4x4 matProj : PROJECTION;  // 射影変換行列
float4x4 matPos;
int iBlendNum;                  // ブレンドする配列の数
float4x4 matLightView;				// ライトビュー変換行列
float4x4 matLightProj;				// 射影変換行列
// 周期
float Period;

// 振幅
float Amplitude;
// 波の移動
float Offset;

texture diffuseTexture;
sampler diffuseSampler = sampler_state {
	texture = <diffuseTexture>;
};
texture texShadowMap;				// シャドウマップテクスチャ
sampler DefSampler = sampler_state	// サンプラーステート
{
    texture = <texShadowMap>;
    AddressU  = CLAMP;        
    AddressV  = CLAMP;
    AddressW = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};


struct VS_OUTPUT_S
{
  float4 Pos : POSITION;   // 射影変換座標
  //float4 ShadowMapTex : TEXCOORD1;   // Zバッファテクスチャ
  float2 ZCalcTex : TEXCOORD0;      // 貼り付けるテクスチャ
float temp : TEXCOORD2;
  float4 Col : COLOR0;				// ディフューズ色
};

// 頂点シェーダ
VS_OUTPUT_S ZBufferCalc_VS(float4 Pos : POSITION, float4 W : BLENDWEIGHT, float3 Norm : NORMAL, float2 inTexCoord : TEXCOORD0 )
{
   VS_OUTPUT_S Out = (VS_OUTPUT_S)0;

//----------------------------------------------------------------
//  	あにめーしょん
//
//-----------------------------------------
   float   Weight[4] = (float[4])W;      // 重みをfloatに分割します
   float    LastBlendWeight = 0.0f;        // 最後の行列に掛けられる重み
   float4x4 matCombWorld = 0.0f;           // 合成ワールド変換行列
   //int num = iBlendNum - 1;
   // ワールド変換行列をブレンド
//if( num > 4 ) num = 4;
   for(int i=0; i < iBlendNum-1;i++)
   {
      LastBlendWeight += Weight[i];   // 最後の重みをここで計算しておく
      matCombWorld += matWorld[i] * Weight[i];
   }

   // 最後の重みを足し算
   matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);

 //-----------------------------------------

   // 普通にカメラの目線によるワールドビュー射影変換をする
   float4x4 mat;
   mat  = mul( matPos, matView );
   mat  = mul( mat, matCombWorld );
   mat  = mul( mat, matProj );
   Out.Pos = mul( Pos, mat );
   // ライトの目線によるワールドビュー射影変換をする
   mat  = mul( matPos, matLightView );
   mat  = mul( mat, matCombWorld );
   mat  = mul( mat, matLightProj );
   Out.ZCalcTex = mul( Pos, mat );
   
   // 法線とライトの方向から頂点の色を決定
   //  濃くなりすぎないように調節しています
   float3 N = normalize( mul(Norm, matPos) );
   float3 LightDirect = normalize( float3(matLightView._13,matLightView._23,matLightView._33) );
//   ライトのあたり具合で明るさ変更
   Out.Col = float4(0.0,0.6,1.0,1.0) * (0.3 + dot(N, -LightDirect)*(1-0.3f));
   Out.Col.r = inTexCoord.x;
   Out.Col.g = inTexCoord.y;
   Out.Col.b = N.x;
   Out.Col.a = N.y;
   Out.temp = N.z;
   Out.Col = float4(LastBlendWeight,LastBlendWeight,LastBlendWeight,LastBlendWeight);
   return Out;
}


// ピクセルシェーダ
float4 ZBufferPlot_PS( float4 Col : COLOR, float4 ZCalcTex : TEXCOORD0, float2 texCoord : TEXCOORD1, float temp : TEXCOORD2 ) : COLOR
{
  float4 Color;
  float3 N = float3(Col.b,Col.a,temp);
  float3 LightDirect = normalize( float3(matLightView._13,matLightView._23,matLightView._33) );
  Color=tex2D(diffuseSampler,float2(Col.r,Col.g));
//Color = Col;
  // ライト目線によるZ値の再算出
   float ZValue = Col.z / Col.w;
 // テクスチャ座標に変換
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + Col.x/Col.w)*0.5f;
   TransTexCoord.y = (1.0f - Col.y/Col.w)*0.5f;
   
   // 同じ座標のZ値を抽出
   float SM_Z = tex2D( DefSampler, TransTexCoord ).x;
   
// ライトの向きからモデルの明るさを調整
Color = Color;// * (0.3 + dot(N, -LightDirect)*(1-0.3f));
Color.a = 1.f;
   // 算出点がシャドウマップのZ値よりも大きければ影と判断
   if( ZValue > SM_Z+0.005f ){
     Color.rgb = Color.rgb * 0.5f; 
    }

   return Color;
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
   
  // outVS.P_after = Pos;
//return outVS;

   float   Weight[4] = (float[4])W;      // 重みをfloatに分割します
   float    LastBlendWeight = 0.0f;        // 最後の行列に掛けられる重み
   float4x4 matCombWorld = 0.0f;           // 合成ワールド変換行列

   // ワールド変換行列をブレンド
//   for(int i=0; i<iBlendNum-1; i++)
 //  {
 //     LastBlendWeight += Weight[i];   // 最後の重みをここで計算しておく
 //     matCombWorld += matWorld[i] * Weight[i];
 //  }

   // 最後の重みを足し算
 //  matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);


 // outVS.P_after = mul( Pos, matCombWorld );   // ワールド変換
   outVS.P_after = mul( Pos, matWorldViewProj);   // ワールド変換
   outVS.P_after = mul( outVS.P_after, matPos );
 //
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
 //return float4( 1.0f,1.0f,0.0f,1.0f);
}

struct VS_OutTexCreate
{
  float4 Pos : POSITION;   // 射影変換座標
   float4 ShadowMapTex : TEXCOORD0;   // Zバッファテクスチャ
};

// 頂点シェーダ
VS_OutTexCreate ZTexCreate_VS( float4 Pos : POSITION, float4 W : BLENDWEIGHT )
{
   VS_OutTexCreate Out = (VS_OutTexCreate)0;

  // float   Weight[4] = (float[4])W;      // 重みをfloatに分割します
  // float    LastBlendWeight = 0.0f;        // 最後の行列に掛けられる重み
  // float4x4 matCombWorld = 0.0f;           // 合成ワールド変換行列

   // ワールド変換行列をブレンド
 //  for(int i=0; i<iBlendNum-1; i++)
//   {
   //   LastBlendWeight += Weight[i];   // 最後の重みをここで計算しておく
     // matCombWorld += matWorld[i] * Weight[i];
 //  }

   // 最後の重みを足し算
  // matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);
   Out.Pos = mul( Pos, matPos);   // ワールド変換
   //Out.Pos = mul( Out.Pos, matCombWorld );   // ワールド変換
   Out.Pos = mul( Out.Pos, matView );    // ビュー変換
   Out.Pos = mul( Out.Pos, matProj );    // 射影変換

   Out.ShadowMapTex = Out.Pos;

   return Out;
}


// ピクセルシェーダ
float4 ZTexCreate_PS( float4 ShadowMapTex : TEXCOORD0 ) : COLOR
{
   // Z値算出
//return float4(1.f,0.f,1.f,1.f);
   return ShadowMapTex.z / ShadowMapTex.w;
//return float4(1.f,0.f,1.f,1.f);
}



technique BasicTec
{
   pass P0
   {
      VertexShader = compile vs_2_0 BasicTrans_VS();
      PixelShader = compile ps_2_0 NoWorkingPixelShader();
   }
   pass P1
   {
      VertexShader = compile vs_2_0 ZBufferCalc_VS();
      PixelShader = compile ps_2_0 ZBufferPlot_PS();
   }
   pass P2
   {
      VertexShader = compile vs_2_0 ZTexCreate_VS();
      PixelShader = compile ps_2_0 ZTexCreate_PS();
   }
}
