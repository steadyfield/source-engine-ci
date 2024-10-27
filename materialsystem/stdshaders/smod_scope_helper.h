//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================

#ifndef smod_scope_HELPER_H
#define smod_scope_HELPER_H
#ifdef _WIN32
#pragma once
#endif

#include <string.h>

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CBaseVSShader;
class IMaterialVar;
class IShaderDynamicAPI;
class IShaderShadow;

//-----------------------------------------------------------------------------
// Init params/ init/ draw methods
//-----------------------------------------------------------------------------
struct smod_scope_Vars_t
{
	smod_scope_Vars_t() { memset(this, 0xFF, sizeof(*this)); }

	int m_nBaseTexture;
	int m_nFrame;
	int m_nRefractAmount;
	int m_nRefractTint;
	int m_nNormalMap;
	int m_nNormalMap2;
	int m_nBumpFrame;
	int m_nBumpFrame2;
	int m_nBumpTransform;
	int m_nBumpTransform2;
	int m_nBlurAmount;
	int m_nFadeOutOnSilhouette;
	int m_nEnvmap;
	int m_nEnvmapFrame;
	int m_nEnvmapTint;
	int m_nEnvmapContrast;
	int m_nEnvmapSaturation;
	int m_nRefractTintTexture;
	int m_nRefractTintTextureFrame;
	int m_nFresnelReflection;
	int m_nNoWriteZ;
	int m_nMasked;
	int m_nVertexColorModulate;
	int m_nForceAlphaWrite;
};

void InitParamssmod_scope(CBaseVSShader *pShader, IMaterialVar** params, const char *pMaterialName,
	smod_scope_Vars_t &info);
void Initsmod_scope(CBaseVSShader *pShader, IMaterialVar** params, smod_scope_Vars_t &info);
void Drawsmod_scope(CBaseVSShader *pShader, IMaterialVar** params, IShaderDynamicAPI *pShaderAPI,
	IShaderShadow* pShaderShadow, smod_scope_Vars_t &info, VertexCompressionType_t vertexCompression);

#endif // smod_scope_HELPER_H
