// ZTexCreator.cpp (2007. 2. 28)

// Z�l�e�N�X�`�������N���X������


#include "ZTexCreator.h"

using namespace IKD;

// �R���X�g���N�^
CZTexCreator::CZTexCreator()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	m_hWorldMat = NULL;
	m_hViewMat = NULL;
	m_hProjMat = NULL;
	m_hTechnique = NULL;
}

// �f�X�g���N�^
CZTexCreator::~CZTexCreator()
{
	// COM�|�C���^�𖾎��I�ɏ���
	m_cpDepthBuff = NULL;	// �[�x�o�b�t�@
	m_cpZTexSurf = NULL;	// Z�l�e�N�X�`���T�[�t�F�C�X
	m_cpZTex = NULL;		// Z�l�e�N�X�`��
	m_cpDevBuffer = NULL;	// �f�o�C�X�o�b�N�o�b�t�@
	m_cpDevDepth = NULL;	// �f�o�C�X�[�x�o�b�t�@
	m_cpEffect = NULL;		// Z�l�v���b�g�G�t�F�N�g
	m_cpDev = NULL;			// �`��f�o�C�X
}


// ���������\�b�h
bool CZTexCreator::Init( Com_ptr<IDirect3DDevice9> &cpDev, UINT ZTexWidth, UINT ZTexHeight, D3DFORMAT ZTexFormat )
{
	if( cpDev.GetPtr()==NULL) return false;
	if( ZTexWidth == 0 || ZTexHeight == 0 ) return false;

	HRESULT hr;

	// ���\�[�X�ɂ���Z�l�v���b�g�V�F�[�_�v���O������ǂݍ���
	if(FAILED( D3DXCreateEffectFromResource(
		cpDev.GetPtr(),
		NULL,
		MAKEINTRESOURCE(FXID_ZVALUEPLOT),
		NULL,
		NULL,
		0,
		NULL,
		m_cpEffect.ToCreator(),
		NULL )))
		return false;

	// �G�t�F�N�g���̊e��p�����[�^�n���h�����擾
	m_hWorldMat = m_cpEffect->GetParameterByName( NULL, "matWorld" );
	m_hViewMat  = m_cpEffect->GetParameterByName( NULL, "matView" );
	m_hProjMat  = m_cpEffect->GetParameterByName( NULL, "matProj" );
	m_hTechnique  = m_cpEffect->GetTechniqueByName( "ZValuePlotTec" );
	if( !m_hWorldMat || !m_hViewMat || !m_hProjMat || !m_hTechnique )
		return false;

	// �w���Z�l�e�N�X�`���𐶐�
	hr = D3DXCreateTexture(
		cpDev.GetPtr(),
		ZTexWidth, ZTexHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		ZTexFormat,
		D3DPOOL_DEFAULT,
		m_cpZTex.ToCreator() );
	if( FAILED( hr ) ) 
		return false;

	m_cpZTex->GetSurfaceLevel( 0, m_cpZTexSurf.ToCreator() );	// �T�[�t�F�C�X�擾

	// �`��f�o�C�X�ɒ�`����Ă���o�b�t�@�̔\�͂��擾
	IDirect3DSurface9 *pSurf;
	cpDev->GetDepthStencilSurface( &pSurf );
	D3DSURFACE_DESC Desc;
	pSurf->GetDesc( &Desc );
	pSurf->Release();

	// �Ǝ��[�x�o�b�t�@���쐬
	hr = cpDev->CreateDepthStencilSurface(
		ZTexWidth, ZTexHeight,
		Desc.Format,
		Desc.MultiSampleType,
		Desc.MultiSampleQuality,
		FALSE,
		m_cpDepthBuff.ToCreator(),
		NULL
		);
	if( FAILED( hr ) )
		return false;

	m_cpDev = cpDev;

	return true;
}


// �`��ΏۃI�u�W�F�N�g�̃��[���h�ϊ��s���ݒ�
void CZTexCreator::SetWorldMatrix( D3DXMATRIX *pMat )
{
	m_matWorld = *pMat;
}


// �r���[�s���ݒ�
void CZTexCreator::SetViewMatrix( D3DXMATRIX *pMat )
{
	m_matView = *pMat;
}


// �ˉe�ϊ��s���ݒ�
void CZTexCreator::SetProjMatrix( D3DXMATRIX *pMat )
{
	m_matProj = *pMat;
}


// �`��̊J�n��錾����
HRESULT CZTexCreator::Begin()
{
	// ���������������s���Ă��邩�`�F�b�N
	if( m_cpDepthBuff.GetPtr()==NULL || m_cpZTex.GetPtr()==NULL )
		return E_FAIL;

	// �f�o�C�X�������Ă���o�b�t�@���ꎞ�ۑ�
	m_cpDev->GetRenderTarget( 0, m_cpDevBuffer.ToCreator() );
	m_cpDev->GetDepthStencilSurface( m_cpDevDepth.ToCreator() );

	// �f�o�C�X��Z�l�e�N�X�`���T�[�t�F�C�X�Ɛ[�x�o�b�t�@��ݒ�
	m_cpDev->SetRenderTarget( 0, m_cpZTexSurf.GetPtr() );
	m_cpDev->SetDepthStencilSurface( m_cpDepthBuff.GetPtr() );

	// �e�T�[�t�F�C�X��������
	m_cpDev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0 );

	// �v���O���}�u���V�F�[�_�̃e�N�j�b�N��ݒ�
	m_cpEffect->SetTechnique( m_hTechnique );

	// �V�F�[�_�̊J�n��錾
	UINT Tmp;
	m_cpEffect->Begin( &Tmp, 0 );

	return S_OK;
}


// �p�X�̊J�n��錾����
HRESULT CZTexCreator::BeginPass()
{
	m_cpEffect->BeginPass(0);	// Z�l�v�Z��1�p�X
	return S_OK;
}


// �o�^����Ă���p�����[�^�����G�t�F�N�g�ɃZ�b�g
bool CZTexCreator::SetParamToEffect()
{
	if(m_cpEffect.GetPtr()==NULL) return false;

	m_cpEffect->SetMatrix( m_hWorldMat, &m_matWorld );
	m_cpEffect->SetMatrix( m_hViewMat, &m_matView );
	m_cpEffect->SetMatrix( m_hProjMat, &m_matProj );

	return true;
}


// �p�X�̏I����錾����
HRESULT CZTexCreator::EndPass()
{
	m_cpEffect->EndPass();
	return S_OK;
}


// �`��̏I����錾����
HRESULT CZTexCreator::End()
{
	m_cpEffect->End();

	// �f�o�C�X�Ɍ��̃T�[�t�F�C�X��߂�
	m_cpDev->SetRenderTarget( 0, m_cpDevBuffer.GetPtr() );
	m_cpDev->SetDepthStencilSurface( m_cpDevDepth.GetPtr() );

	m_cpDevBuffer = NULL;
	m_cpDevDepth = NULL;

	// �Œ�@�\�ɖ߂�
	m_cpDev->SetVertexShader( NULL );
	m_cpDev->SetPixelShader( NULL );

	return S_OK;
}


// Z�l�e�N�X�`�����擾����
bool CZTexCreator::GetZTex( Com_ptr<IDirect3DTexture9> &cpTex )
{
	cpTex = m_cpZTex;
	return true;
}