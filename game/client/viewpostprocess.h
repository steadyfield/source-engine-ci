//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================

#ifndef VIEWPOSTPROCESS_H
#define VIEWPOSTPROCESS_H

#if defined( _WIN32 )
#pragma once
#endif

void DoEnginePostProcessing( int x, int y, int w, int h, bool bFlashlightIsOn, bool bPostVGui = false );
void DoImageSpaceMotionBlur( const CViewSetup &view, int x, int y, int w, int h );
void DumpTGAofRenderTarget( const int width, const int height, const char *pFilename );

void RenderClassicRadialBlur(CViewSetup view, int x, int y, int width, int height);
void RenderClassicBlur(CViewSetup view, int x, int y, int width, int height);
void RenderRadialBlur(CViewSetup view, int x, int y, int width, int height);
void RenderDistanceBlur(CViewSetup view, int x, int y, int width, int height);
void RenderNewBloom(int x, int y, int width, int height);
void PerformNightVisionEffect( const CViewSetup &view );

#endif // VIEWPOSTPROCESS_H
