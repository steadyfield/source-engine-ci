//===== Copyright © 1996-2005, Valve Corporation, All rights reserved. ======//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "BaseVSShader.h"

#include "SDK_screenspaceeffect_vs20.inc"
#include "smod_radialblur_ps20b.inc"

BEGIN_VS_SHADER_FLAGS(smod_radialblur, "Custom radialblur shader", SHADER_NOT_EDITABLE)
BEGIN_SHADER_PARAMS
SHADER_PARAM(FBTEXTURE, SHADER_PARAM_TYPE_TEXTURE, "_rt_FullFrameFB", "")
SHADER_PARAM(BLURSCALE, SHADER_PARAM_TYPE_FLOAT, "1", " blurscale lol")
SHADER_PARAM(VIEWAMOUNT, SHADER_PARAM_TYPE_FLOAT, "1", " amount of view")
END_SHADER_PARAMS

SHADER_INIT
{
	if (params[FBTEXTURE]->IsDefined())
	{
		LoadTexture(FBTEXTURE);
	}
}

SHADER_FALLBACK
{
	// Requires DX9 + above
	if (g_pHardwareConfig->GetDXSupportLevel() < 90)
	{
		Assert(0);
		return "Wireframe";
	}
return 0;
}

SHADER_DRAW
{
	SHADOW_STATE
{
	pShaderShadow->EnableDepthWrites(false);

pShaderShadow->EnableTexture(SHADER_SAMPLER0, true);
//pShaderShadow->EnableTexture(SHADER_SAMPLER1, true);
int fmt = VERTEX_POSITION;
pShaderShadow->VertexShaderVertexFormat(fmt, 1, 0, 0);

// Pre-cache shaders
DECLARE_STATIC_VERTEX_SHADER(sdk_screenspaceeffect_vs20);
SET_STATIC_VERTEX_SHADER(sdk_screenspaceeffect_vs20);

if (g_pHardwareConfig->SupportsPixelShaders_2_b())
{
	DECLARE_STATIC_PIXEL_SHADER(smod_radialblur_ps20b);
	SET_STATIC_PIXEL_SHADER(smod_radialblur_ps20b);
}
}

DYNAMIC_STATE
{
	BindTexture(SHADER_SAMPLER0, FBTEXTURE, -1);
float scale[4];
scale[0] = params[BLURSCALE]->GetFloatValue();
scale[1] = scale[2] = scale[3] = scale[0];
pShaderAPI->SetPixelShaderConstant(0, scale);

float amount[4];
amount[0] = params[VIEWAMOUNT]->GetFloatValue();
amount[1] = amount[2] = amount[3] = amount[0];
pShaderAPI->SetPixelShaderConstant(1, amount);
DECLARE_DYNAMIC_VERTEX_SHADER(sdk_screenspaceeffect_vs20);
SET_DYNAMIC_VERTEX_SHADER(sdk_screenspaceeffect_vs20);

if (g_pHardwareConfig->SupportsPixelShaders_2_b())
{
	DECLARE_DYNAMIC_PIXEL_SHADER(smod_radialblur_ps20b);
	SET_DYNAMIC_PIXEL_SHADER(smod_radialblur_ps20b);
}
}
Draw();
}
END_SHADER
