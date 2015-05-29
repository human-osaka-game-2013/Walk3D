float4x4 matWorld[4] : WORLD;   // ���[���h�ϊ��s��z��
float4x4 matView : VIEW;        // �r���[�ϊ��s��
float4x4 matProj : PROJECTION;  // �ˉe�ϊ��s��
float4x4 matPos;

// �g�̈ړ�
float Offset;
int iBlendNum;                  // �u�����h����z��̐�
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
