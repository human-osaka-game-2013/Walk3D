#ifndef BILLBOARD_H
#define BILLBOARD_H

struct BILLBOARD_VERTEX
{
	// �ϊ��ςݒ��_
	FLOAT X, Y, Z;

	// �f�B�t���[�Y�F
	D3DCOLOR Color;

	// �e�N�X�`�����W
	FLOAT TU, TV;
};

class CBillboard
{
	friend class CBillboardGroup;

	protected:
	// �O���[�v
	CBillboardGroup* Group;
	// �e�N�X�`��
	LPDIRECT3DTEXTURE9* Texture;
	// �摜�̕��C�����C�ő啝�C�ő卂���C�e�N�X�`�������iX�����CY�����j
	//int Width, Height, MaxWidth, MaxHeight, XCount, YCount;
	// �R���X�g���N�^�C�f�X�g���N�^
	CBillboard(CBillboardGroup *group,LPDIRECT3DDEVICE9 Device,int anim_max);
	~CBillboard();
	private:
		//D3DXMATRIX m_rot;
		/*float m_xpos;
		float m_ypos;
		float m_zpos;
		float m_size;*/
		//LPDIRECT3DTEXTURE9* Texture;
		int m_anim_count;
		int m_anim_max;
		int m_anim_frame;
		int m_run_flag;
		int m_frame_count;

	public:
		void Draw(float xpos,float ypos,float zpos,float size,D3DXVECTOR3 _Cz,bool additivesynthesis=true);
		void GroundDraw(float xpos,float zpos,float size,bool additivesynthesis=true);
		void Control();
		void ResetAnimCount();
		bool IsAnimCountEnd();
		bool loadTexture(string file_name);
		void setAnimFrame(int frame);
		void setRunFlag()
		{
			m_run_flag = true;
		}
};

class CBillboardGroup
{
	friend class CBillboard;
protected:

	// Direct3D�f�o�C�X�C���_�o�b�t�@
	LPDIRECT3DDEVICE9 Device;
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	// �摜
	vector<CBillboard*> m_billboard;

	//const int m_width=1024,m_height=1024;
public:

	// �R���X�g���N�^�C�f�X�g���N�^
	CBillboardGroup(LPDIRECT3DDEVICE9 device);
	~CBillboardGroup();

	// �V�����摜
	CBillboard* NewBillboard(int anim_max);
};
#endif