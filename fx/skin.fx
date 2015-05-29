float4x4 matWorld[4] : WORLD;   // ワールド変換行列配列
float4x4 matView : VIEW;        // ビュー変換行列
float4x4 matProj : PROJECTION;  // 射影変換行列
int iBlendNum;                  // ブレンドする配列の数


float4 BasicTrans_VS( float4 Pos : POSITION, float4 W : BLENDWEIGHT ) : POSITION
{
   float4 P_after;   // 出力頂点座標

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


  P_after = mul( Pos, matCombWorld );   // ワールド変換
   P_after = mul( P_after, matView );    // ビュー変換
   P_after = mul( P_after, matProj );    // 射影変換

   return P_after;
}


float4 PS() : COLOR0 {
	return float4( 1.0f, 1.0f, 1.0f, 1.0f );
}


technique Tech {
	pass p0 {
		VertexShader = compile vs_2_0 BasicTrans_VS();
		PixelShader = compile ps_2_0 PS();
	}
}