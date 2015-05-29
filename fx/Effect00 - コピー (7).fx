float4x4 matWorld[4] : WORLD;   // ワールド変換行列配列
float4x4 matView : VIEW;        // ビュー変換行列
float4x4 matProj : PROJECTION;  // 射影変換行列
float4x4 matPos : POSITION;
int iBlendNum;                  // ブレンドする配列の数
float4x4 matLightView;				// ライトビュー変換行列
float4x4 matLightProj;				// 射影変換行列


texture diffuseTexture;
sampler diffuseSampler = sampler_state
{
	texture = <diffuseTexture>;
};
texture texShadowMap;
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

struct VS_OUTPUT
{
	float4 Pos : POSITION;
	float2 Texcord: TEXCOORD0;
	float4 ZCalcTex: TEXCOORD1;
	float temp : TEXCOORD2;
	float4 Col : COLOR0;
};
// セルフシャドウアニメーションシェーダ
VS_OUTPUT BasicTrans_VS( float4 Pos : POSITION, float4 W : BLENDWEIGHT, float2 inTexCoord : TEXCOORD0, float3 Norm : NORMAL )
{
   VS_OUTPUT Out = (VS_OUTPUT)0;
   Out.Texcord = inTexCoord;

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

   float4x4 mat_light;

   Out.Pos = mul( Pos, matCombWorld );   // ワールド変換
   //mat_light = Out.Pos;
   Out.Pos = mul( Out.Pos, matView );    // ビュー変換
   Out.Pos = mul( Out.Pos, matProj );    // 射影変換
   
   //mat_light = mul( mat_light, matLightView );    // ビュー変換
   //mat_light = mul( mat_light, matLightProj );    // 射影変換
   mat_light  = mul( matPos, matLightView );
   mat_light  = mul( mat_light, matLightProj );
   Out.ZCalcTex = mul( Pos, mat_light );
   Out.ZCalcTex = mat_light;
   
   float3 N = normalize( mul(Norm, matPos) );
   Out.Col.r = inTexCoord.x;
   Out.Col.g = inTexCoord.y;
   Out.Col.b = N.x;
   Out.Col.a = N.y;
   Out.temp = N.z;
   return Out;
}


float4 PS( float4 Col : COLOR, float2 inTex : TEXCOORD0, float4 ZCalcTex : TEXCOORD1,float temp : TEXCOORD2) : COLOR0
{
	float4 Color;
	Color=tex2D(diffuseSampler,float2(Col.r,Col.g));
	float3 N = float3(Col.b,Col.a,temp);
	// ライト目線によるZ値の再算出
	float ZValue = Col.z / Col.w;
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
	TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
	
	Color=tex2D(diffuseSampler,inTex);
	float SM_Z = tex2D( DefSampler, TransTexCoord ).x;
   
	// 算出点がシャドウマップのZ値よりも大きければ影と判断
	if( ZValue > SM_Z+0.025f ){
	Color.rgb = Color.rgb * 0.5f; 
	}
	return Color;
}

// セルフシャドウシェーダ not anim
VS_OUTPUT Shadow_VS( float4 Pos : POSITION, float2 inTexCoord : TEXCOORD0, float3 Norm : NORMAL )
{
   VS_OUTPUT Out = (VS_OUTPUT)0;
   Out.Texcord = inTexCoord;
   float4 mat_light;

   Out.Pos = mul( Pos,     matPos  );
   Out.Pos = mul( Out.Pos, matView );    // ビュー変換
   Out.Pos = mul( Out.Pos, matProj );    // 射影変換
   
   mat_light = mul( Pos, matLightView );    // ビュー変換
   mat_light = mul( mat_light, matLightProj );    // 射影変換
   
   Out.ZCalcTex = mat_light;
   
   float3 N = normalize( mul(Norm, matPos) );
   Out.Col.r = inTexCoord.x;
   Out.Col.g = inTexCoord.y;
   Out.Col.b = N.x;
   Out.Col.a = N.y;
   Out.temp = N.z;
   return Out;
}

float4 ShadowPS( float4 Col : COLOR, float2 inTex : TEXCOORD0, float4 ZCalcTex : TEXCOORD1,float temp : TEXCOORD2) : COLOR0
{
	float4 Color;
	Color=tex2D(diffuseSampler,float2(Col.r,Col.g));
	float3 N = float3(Col.b,Col.a,temp);
	// ライト目線によるZ値の再算出
	float ZValue = Col.z / Col.w;
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
	TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
	
	Color=tex2D(diffuseSampler,inTex);
	float SM_Z = tex2D( DefSampler, TransTexCoord ).x;
   
	// 算出点がシャドウマップのZ値よりも大きければ影と判断
	if( ZValue > SM_Z ){
	Color.rgb = Color.rgb * 0.1f; 
	}
	return Color;
}

// 影用テクスチャ生成シェーダ用構造体
struct VS_OutTexCreate
{
  float4 Pos : POSITION;   // 射影変換座標
   float4 ShadowMapTex : TEXCOORD0;   // Zバッファテクスチャ
};

// 影生成シェーダ
VS_OutTexCreate ZTexCreate_VS( float4 Pos : POSITION, float4 W : BLENDWEIGHT )
{
   VS_OutTexCreate Out = (VS_OutTexCreate)0;

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

   Out.Pos = mul( Pos, matCombWorld );   // ワールド変換
   Out.Pos = mul( Out.Pos, matView );    // ビュー変換
   Out.Pos = mul( Out.Pos, matProj );    // 射影変換

   Out.ShadowMapTex = Out.Pos;
   return Out;
}
// 影生成用ピクセルシェーダ
float4 ZTexCreate_PS( float4 ShadowMapTex : TEXCOORD0 ) : COLOR
{
   // Z値算出
   return ShadowMapTex.z / ShadowMapTex.w;
}

technique BasicTec {
	pass p0 {
		VertexShader = compile vs_2_0 BasicTrans_VS();
		PixelShader = compile ps_2_0 PS();
	}
pass p01 {
		VertexShader = compile vs_2_0 Shadow_VS();
		PixelShader = compile ps_2_0 ShadowPS();
	}
pass p2 {
		VertexShader = compile vs_2_0 ZTexCreate_VS();
		PixelShader = compile ps_2_0 ZTexCreate_PS();
	}
}