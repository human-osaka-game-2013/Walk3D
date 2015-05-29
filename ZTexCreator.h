// ZTexCreator.h (2007. 2. 28)

// Z�l�e�N�X�`�������N���X�錾��
//  ������ �F IKD



#pragma once

#include <d3dx9.h>
#include "ComPtr.h"
#include "EffectResource.h"

namespace IKD
{

///////////////////////////////////////////////
// CZTexCreator�N���X
//
//   Z�l�e�N�X�`�����쐬����w���p�[�N���X
//////////////////////////////
class CZTexCreator
{
protected:
	Com_ptr<IDirect3DDevice9> m_cpDev;			// �`��f�o�C�X
	Com_ptr<IDirect3DTexture9> m_cpZTex;		// Z�l�e�N�X�`��
	Com_ptr<IDirect3DSurface9> m_cpZTexSurf;	// Z�l�e�N�X�`���T�[�t�F�C�X
	Com_ptr<IDirect3DSurface9> m_cpDepthBuff;	// �[�x�o�b�t�@
	Com_ptr<IDirect3DSurface9> m_cpDevBuffer;	// �f�o�C�X�o�b�N�o�b�t�@
	Com_ptr<IDirect3DSurface9> m_cpDevDepth;	// �f�o�C�X�[�x�o�b�t�@
	Com_ptr<ID3DXEffect> m_cpEffect;			// Z�l�v���b�g�G�t�F�N�g
	D3DXMATRIX m_matWorld;						// ���[���h�ϊ��s��
	D3DXMATRIX m_matView;						// �r���[�ϊ��s��
	D3DXMATRIX m_matProj;						// �ˉe�ϊ��s��
	D3DXHANDLE m_hWorldMat;						// ���[���h�ϊ��s��n���h��
	D3DXHANDLE m_hViewMat;						// �r���[�ϊ��s��n���h��
	D3DXHANDLE m_hProjMat;						// �ˉe�ϊ��s��n���h��
	D3DXHANDLE m_hTechnique;					// �e�N�j�b�N�ւ̃n���h��

public:
	CZTexCreator();
	virtual ~CZTexCreator();

public:
	// ���������\�b�h
	bool Init( Com_ptr<IDirect3DDevice9> &cpDev, UINT ZTexWidth, UINT ZTexHeight, D3DFORMAT ZTexFormat=D3DFMT_A8R8G8B8 );

	// �`��ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s���ݒ�
	void SetWorldMatrix( D3DXMATRIX *pMat );

	// �r���[�s���ݒ�
	void SetViewMatrix( D3DXMATRIX *pMat );

	// �ˉe�ϊ��s���ݒ�
	void SetProjMatrix( D3DXMATRIX *pMat );

	// �`��̊J�n��錾����
	HRESULT Begin();

	// �`��̏I����錾����
	HRESULT End();

	// �p�X�̊J�n��錾����
	HRESULT BeginPass();

	// �p�X�̏I����錾����
	HRESULT EndPass();

	// �o�^����Ă���p�����[�^�����G�t�F�N�g�ɃZ�b�g
	bool SetParamToEffect();

	// Z�l�e�N�X�`�����擾����
	bool GetZTex( Com_ptr<IDirect3DTexture9> &cpTex );
};


}