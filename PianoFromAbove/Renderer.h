/*************************************************************************************************
*
* File: Renderer.h
*
* Description: Defines rendering objects. Only one for now.
*
* Copyright (c) 2010 Brian Pantano. All rights reserved.
*
*************************************************************************************************/
#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

class Renderer
{
public:
    enum FontSize { Small, SmallBold, SmallComic, Medium, Large };

    Renderer(void) : m_iBufferWidth( 0 ), m_iBufferHeight( 0 ), m_bLimitFPS( true ) {};
    virtual ~Renderer(void) {};

    virtual HRESULT Init( HWND hWnd, bool bLimitFPS ) = 0;
    virtual HRESULT ResetDeviceIfNeeded() = 0;
    virtual HRESULT ResetDevice() = 0;
    virtual HRESULT Clear( DWORD color ) = 0;
    virtual HRESULT BeginScene() = 0;
    virtual HRESULT EndScene() = 0;
    virtual HRESULT Present() = 0;
    virtual HRESULT BeginText() = 0;
    virtual HRESULT DrawTextW( const WCHAR *sText, FontSize fsFont, LPRECT rcPos, DWORD dwFormat, DWORD dwColor, INT iChars = -1 ) = 0;
    virtual HRESULT DrawTextA( const CHAR *sText, FontSize fsFont, LPRECT rcPos, DWORD dwFormat, DWORD dwColor, INT iChars = -1 ) = 0;
    virtual HRESULT EndText() = 0;
    virtual HRESULT DrawRect( float x, float y, float cx, float cy, DWORD color ) = 0;
    virtual HRESULT DrawRect( float x, float y, float cx, float cy,
                              DWORD c1, DWORD c2, DWORD c3, DWORD c4 ) = 0;
    virtual HRESULT DrawSkew( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, DWORD color ) = 0;
    virtual HRESULT DrawSkew( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
                              DWORD c1, DWORD c2, DWORD c3, DWORD c4 ) = 0;
    // 添加钢琴背景绘制函数
    virtual HRESULT DrawPianoBackground(float x, float y, float cx, float cy) = 0;

    bool GetLimitFPS() const { return m_bLimitFPS; }
    HRESULT SetLimitFPS( bool bLimitFPS );

    int GetBufferWidth() const { return m_iBufferWidth; }
    int GetBufferHeight() const { return m_iBufferHeight; }

protected:
    int m_iBufferWidth, m_iBufferHeight;
    bool m_bLimitFPS;
};

class D3D9Renderer : public Renderer
{
public:
    struct SCREEN_VERTEX
    {
        float x, y, z, h;
        D3DCOLOR color;

        const static DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
    };

    static const int MaxTriangles = 200;
    static const int VertexBufferSize = sizeof( SCREEN_VERTEX ) * 3 * MaxTriangles;

    D3D9Renderer() : m_pD3D( NULL ), m_pd3dDevice( NULL ), m_pTextSprite( NULL ),
                     m_pVertexBuffer( NULL ), m_pStaticVertexBuffer( NULL ),
                     m_pSmallFont( NULL ), m_pSmallBoldFont( NULL ), m_pSmallComicFont( NULL ),
                     m_pMediumFont( NULL ), m_pLargeFont( NULL ),
                     m_iTriangle( 0 ), m_bIsDeviceValid( false ),
                     m_iStaticTriangle( 0 ), m_iStaticMaxTriangles( 0 ), m_bStatic( false ) {}
    ~D3D9Renderer();

    HRESULT Init( HWND hWnd, bool bLimitFPS );
    HRESULT ResetDeviceIfNeeded();
    HRESULT ResetDevice();
    HRESULT Clear( DWORD color );
    HRESULT BeginScene();
    HRESULT EndScene();
    HRESULT Present();
    HRESULT BeginText();
    HRESULT DrawTextW( const WCHAR *sText, FontSize fsFont, LPRECT rcPos, DWORD dwFormat, DWORD dwColor, INT iChars = -1 );
    HRESULT DrawTextA( const CHAR *sText, FontSize fsFont, LPRECT rcPos, DWORD dwFormat, DWORD dwColor, INT iChars = -1 );
    HRESULT EndText();
    HRESULT DrawRect( float x, float y, float cx, float cy, DWORD color );
    HRESULT DrawRect( float x, float y, float cx, float cy,
                      DWORD c1, DWORD c2, DWORD c3, DWORD c4 );
    HRESULT DrawSkew( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, DWORD color );
    HRESULT DrawSkew( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
                      DWORD c1, DWORD c2, DWORD c3, DWORD c4 );
    // 实现钢琴背景绘制函数
    HRESULT DrawPianoBackground(float x, float y, float cx, float cy);
    HRESULT BeginStaticBuffer( int iTriangles );
    HRESULT EndStaticBuffer();
    HRESULT DrawStaticBuffer();

private:
    HRESULT RestoreDeviceObjects();
    void DestroyDeviceObjects();

    LPDIRECT3D9 m_pD3D; // Used to create the D3DDevice
    LPDIRECT3DDEVICE9 m_pd3dDevice; // The rendering device
    LPD3DXSPRITE m_pTextSprite;
    LPD3DXFONT m_pSmallFont;
    LPD3DXFONT m_pSmallBoldFont;
    LPD3DXFONT m_pSmallComicFont;
    LPD3DXFONT m_pMediumFont;
    LPD3DXFONT m_pLargeFont;
    D3DPRESENT_PARAMETERS m_d3dPP; // Presentation parameters
    bool m_bIsDeviceValid;
    
    HRESULT PrepBuffer( int iTriangles );
    HRESULT FlushBuffer();
    LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
    int m_iTriangle;
    unsigned char *m_pVertexData;

    HRESULT ReleaseStaticBuffer();
    LPDIRECT3DVERTEXBUFFER9 m_pStaticVertexBuffer;
    int m_iStaticTriangle, m_iStaticMaxTriangles;
    unsigned char *m_pStaticVertexData;
    bool m_bStatic;

    HRESULT Blit( SCREEN_VERTEX *data, int iTriangles );
};
