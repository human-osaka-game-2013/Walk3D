float4x4 matWorld[4] : WORLD;   // ���[���h�ϊ��s��z��
float4x4 matView : VIEW;        // �r���[�ϊ��s��
float4x4 matProj : PROJECTION;  // �ˉe�ϊ��s��
int iBlendNum;                  // �u�����h����z��̐�


float4 BasicTrans_VS( float4 Pos : POSITION, float4 W : BLENDWEIGHT ) : POSITION
{
   float4 P_after;   // �o�͒��_���W

   float   Weight[4] = (float[4])W;      // �d�݂�float�ɕ������܂�
   float    LastBlendWeight = 0.0f;        // �Ō�̍s��Ɋ|������d��
   float4x4 matCombWorld = 0.0f;           // �������[���h�ϊ��s��

   // ���[���h�ϊ��s����u�����h
   for(int i=0; i<iBlendNum-1; i++)
   {
      LastBlendWeight += Weight[i];   // �Ō�̏d�݂������Ōv�Z���Ă���
      matCombWorld += matWorld[i] * Weight[i];
   }

   // �Ō�̏d�݂𑫂��Z
   matCombWorld += matWorld[iBlendNum-1] * (1.0f-LastBlendWeight);


  P_after = mul( Pos, matCombWorld );   // ���[���h�ϊ�
   P_after = mul( P_after, matView );    // �r���[�ϊ�
   P_after = mul( P_after, matProj );    // �ˉe�ϊ�

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