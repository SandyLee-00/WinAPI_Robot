#pragma once
#include "Res.h"

/// <summary>
/// 역할 : 비트맵을 로드 및 생성
/// 담당자 : 공준열
/// </summary>
class Texture
    : public Res
{
private:

    /// 멤버 변수
    // Bitmap을 그리기 위한 변수들
    HDC m_memDC; // 도화지
    HBITMAP m_hBit; // 비트맵 핸들
    BITMAP  m_bitInfo; // 비트맵 정보

public:

    /// 멤버 함수
    void Load(const wstring& _filePath, HDC _mainDC);
    void Create(UINT _width, UINT _height, HDC _mainDC);
    UINT GetWidth() { return m_bitInfo.bmWidth; }
    UINT GetHeight() { return m_bitInfo.bmHeight; }
    HDC GetDC() { return m_memDC; }

private:

    Texture();
    virtual ~Texture();

    /// friend class 선언을 통해서 ResMgr는 Texture의 private에 접근이 가능하다
    /// 즉 Texture는 ResMgr를 통해서만 생성하겠다라는 의미이다. 
    friend class ResourceManager;
};