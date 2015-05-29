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
	texture = (diffuseTexture);
};
texture texShadowMap;
sampler DefSampler = sampler_state	// サンプラーステート
{
    texture = (texShadowMap);
    AddressU  = CLAMP;
    AddressV  = CLAMP;
    AddressW = CLAMP;
    MIPFILTER = LINEAR;
    MINFILTER = LINEAR;
    MAGFILTER = LINEAR;
};

struct VS_OUTPUT
{
   float4 Pos : POSITION;  			 // 射影変換座標
   float4 ZCalcTex : TEXCOORD0;   	// Z値算出用テクスチャ
   float4 Col : COLOR0;				// ディフューズ色
   float2 temp : TEXCOORD1;
};
// セルフシャドウアニメーションシェーダ
VS_OUTPUT BasicTrans_VS( float4 Pos : POSITION , float3 Norm : NORMAL)
{
  
VS_OUTPUT Out = (VS_OUTPUT)0;

   // 普通にカメラの目線によるワールドビュー射影変換をする
   float4x4 mat;
   mat  = mul( matPos, matView );
   mat  = mul( mat, matProj );
   Out.Pos = mul( Pos, mat );
   
   // ライトの目線によるワールドビュー射影変換をする
   mat  = mul( matPos, matLightView );
   mat  = mul( mat, matLightProj );
   Out.ZCalcTex = mul( Pos, mat );
   
   // 法線とライトの方向から頂点の色を決定
   //  濃くなりすぎないように調節しています
   float3 N = normalize( mul(Norm, matPos) );
   float3 LightDirect = normalize( float3(matLightView._13,matLightView._23,matLightView._33) );
   Out.Col = float4(1.0,0.0,1.0,1.0);// * (0.3 + dot(N, -LightDirect)*(1-0.3f));

   return Out;
}


float4 PS( float4 Col : COLOR, float4 ZCalcTex : TEXCOORD0 ) : COLOR
{
// ライト目線によるZ値の再算出
   float ZValue = ZCalcTex.z / ZCalcTex.w;
   
   // テクスチャ座標に変換
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
   TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
   
   // 同じ座標のZ値を抽出
   float SM_Z = tex2D( DefSampler, TransTexCoord ).z;
   
   // 算出点がシャドウマップのZ値よりも大きければ影と判断
   if( ZValue > SM_Z+0.005f ){
     Col.rgb = Col.rgb * 0.5f; 
    }
   
   return Col;
}

// セルフシャドウシェーダ not anim
VS_OUTPUT Shadow_VS( float4 Pos : POSITION , float3 Norm : NORMAL)
{
     VS_OUTPUT Out = (VS_OUTPUT)0;

   // 普通にカメラの目線によるワールドビュー射影変換をする
   float4x4 mat;
   mat  = mul( matPos, matView );
   mat  = mul( mat, matProj );
   Out.Pos = mul( Pos, mat );
   
   // ライトの目線によるワールドビュー射影変換をする
   mat  = mul( matPos, matLightView );
   mat  = mul( mat, matLightProj );
   Out.ZCalcTex = mul( Pos, mat );
   
   // 法線とライトの方向から頂点の色を決定
   //  濃くなりすぎないように調節しています
  // float3 N = normalize( mul(Norm, matPos) );
   Out.Col = float4(0.0,0.6,1.0,1.0);// * (0.3 + dot(N, -LightDirect)*(1-0.3f));
   Out.temp.x = Norm.x;
   Out.temp.y = Norm.y;
   return Out;
}

float4 ShadowPS( float4 Col : COLOR, float4 ZCalcTex : TEXCOORD0, float2 temp : TEXCOORD1 ) : COLOR
{
   // ライト目線によるZ値の再算出
   float ZValue = ZCalcTex.z / ZCalcTex.w;
   //ZValue = temp.x/temp.y;
   // テクスチャ座標に変換
   float2 TransTexCoord;
   TransTexCoord.x = (1.0f + ZCalcTex.x/ZCalcTex.w)*0.5f;
   TransTexCoord.y = (1.0f - ZCalcTex.y/ZCalcTex.w)*0.5f;
   
   // 同じ座標のZ値を抽出
   float SM_Z = tex2D( DefSampler, TransTexCoord ).z;
   
   // 算出点がシャドウマップのZ値よりも大きければ影と判断
   if( ZValue > SM_Z+0.005f ){
     Col.rgb = Col.rgb * 0.1f; 
    }
   
   return Col;
}

// 影用テクスチャ生成シェーダ用構造体
struct VS_OutTexCreate
{
  float4 Pos : POSITION;   // 射影変換座標
   float4 ShadowMapTex : TEXCOORD0;   // Zバッファテクスチャ
};

// 影生成シェーダ
VS_OutTexCreate ZTexCreate_VS( float4 Pos : POSITION )
{
   VS_OutTexCreate Out = (VS_OutTexCreate)0;

   // 普通にワールドビュー射影行列をする
   float4x4 mat;
   mat = mul( matPos, matView );
   mat = mul( mat, matProj );
   Out.Pos = mul( Pos, mat );
   
   // テクスチャ座標を頂点に合わせる
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