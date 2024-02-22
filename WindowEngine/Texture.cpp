#include "pch.h"
#include "Texture.h"
#include "GameProcess.h" // Core의 hdc를 가져오기 위함

// 생성자
Texture::Texture() : m_memDC(0), m_hBit(0), m_bitInfo()
{}

Texture::~Texture()
{
	DeleteDC(m_memDC); // Compatible Device Context를 삭제
	DeleteObject(m_hBit); // 비트맵 핸들 삭제
}

// 비트맵 이미지를 로드
void Texture::Load(const wstring& _filePath, HDC _mainDC)
{
	// LoadImage -> 비트맵을 포함한 다양한 유형의 이미지를 로드
	/*
	LoadImageW(
		_In_opt_ HINSTANCE hInst, // 독립 리소스를 불러올 경우 nullptr
		_In_ LPCWSTR name, // 불러올 이미지의 경로 / LPCWSTR : CONST WCHAR*를 typedef
		_In_ UINT type, // 불러올 이미지의 종류
		_In_ int cx, // 너비 (0이고 fuLoad 인수에서 LR_DEFAULTSIZE가 지정되어있지 않으면 원본크기)
		_In_ int cy, // 높이 (0이고 fuLoad 인수에서 LR_DEFAULTSIZE가 지정되어있지 않으면 원본크기)
		_In_ UINT fuLoad); // 하나 이상의 플래그를 지정할 수 있습니다.
	*/
	// LR_LOADFROMFILE : lpszName 인수를 리소스 대신 파일 경로를 사용해 불러옵니다.
	// string.c_str() : 객체 string을 c스타일의 문자열 const char*로 변환해준다.
	m_hBit = (HBITMAP)LoadImage(nullptr, _filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	assert(m_hBit);

	// 호환 가능한 디바이스 컨텍스트(Compatible Device Context)를 생성
	// CreateCompatibleDC() 함수는 원본 디바이스 컨텍스트와 호환되는 새로운 디바이스 컨텍스트를 만듭니다.
	m_memDC = CreateCompatibleDC(_mainDC);

	// 불러온 비트맵과 DC 연결
	HBITMAP hPreBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hPreBit); // m_dc의 default 비트맵 (1 x 1)을 삭제

	// 비트맵 정보를 저장
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}

// 비트맵 이미지를 생성
void Texture::Create(UINT _width, UINT _height, HDC _mainDC)
{
	m_hBit = CreateCompatibleBitmap(_mainDC, _width, _height); // 호환 가능한 비트맵 생성
	m_memDC = CreateCompatibleDC(_mainDC); // 호환 가능한 디바이스 컨텍스트 생성

	// 생성한 비트맵과 DC 연결
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit); // m_dc의 default 비트맵 (1 x 1)을 삭제

	// 비트맵 정보를 저장
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}