float4x4 matWorld[4] : WORLD;   // ���[���h�ϊ��s��z��
float4x4 matView : VIEW;        // �r���[�ϊ��s��
float4x4 matProj : PROJECTION;  // �ˉe�ϊ��s��
float4x4 matPos;
// ����
float Period;

// �U��
float Amplitude;
// �g�̈ړ�
float Offset;
int iBlendNum;                  // �u�����h����z��̐�
texture diffuseTexture;
sampler diffuseSampler = sampler_state {
	texture = <diffuseTexture>;
};

struct VS_OUTPUT_S
{
  float4 Pos : POSITION;   // �ˉe�ϊ����W
   float4 ShadowMapTex : TEXCOORD0;   // Z�o�b�t�@�e�N�X�`��
};

// ���_�V�F�[�_
VS_OUTPUT_S ZBufferCalc_VS( float4 Pos : POSITION )
{
   VS_OUTPUT_S Out = (VS_OUTPUT_S)0;

   // ���ʂɃ��[���h�r���[�ˉe�s�������
   Out.Pos = mul( Pos, matCombWorld );   // ���[���h�ϊ�
   Out.Pos = mul( Out.Pos, matView );    // �r���[�ϊ�
   Out.Pos = mul( Out.Pos, matProj );    // �ˉe�ϊ�

   // �e�N�X�`�����W�𒸓_�ɍ��킹��
   Out.ShadowMapTex = Out.Pos;

   return Out;
}


// �s�N�Z���V�F�[�_
float4 ZBufferPlot_PS( float4 ShadowMapTex : TEXCOORD0 ) : COLOR
{
   // Z�l�Z�o
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


   outVS.P_after = mul( Pos, matCombWorld );   // ���[���h�ϊ�
   //outVS.P_after = mul( Pos, matPos );
   outVS.P_after = mul( outVS.P_after, matView );    // �r���[�ϊ�
   outVS.P_after = mul( outVS.P_after, matProj );    // �ˉe�ϊ�

   return outVS;
}

// �s�N�Z���V�F�[�_
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

