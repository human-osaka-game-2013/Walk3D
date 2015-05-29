#ifndef BILLBOARD_H
#define BILLBOARD_H

struct BILLBOARD_VERTEX
{
	// 変換済み頂点
	FLOAT X, Y, Z;

	// ディフューズ色
	D3DCOLOR Color;

	// テクスチャ座標
	FLOAT TU, TV;
};

class CBillboard
{
	friend class CBillboardGroup;

	protected:
	// グループ
	CBillboardGroup* Group;
	// テクスチャ
	LPDIRECT3DTEXTURE9* Texture;
	// 画像の幅，高さ，最大幅，最大高さ，テクスチャ枚数（X方向，Y方向）
	//int Width, Height, MaxWidth, MaxHeight, XCount, YCount;
	// コンストラクタ，デストラクタ
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

	// Direct3Dデバイス，頂点バッファ
	LPDIRECT3DDEVICE9 Device;
	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	// 画像
	vector<CBillboard*> m_billboard;

	//const int m_width=1024,m_height=1024;
public:

	// コンストラクタ，デストラクタ
	CBillboardGroup(LPDIRECT3DDEVICE9 device);
	~CBillboardGroup();

	// 新しい画像
	CBillboard* NewBillboard(int anim_max);
};
#endif