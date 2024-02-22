#include "pch.h"
#include "Texture.h"
#include "GameProcess.h" // Core�� hdc�� �������� ����

// ������
Texture::Texture() : m_memDC(0), m_hBit(0), m_bitInfo()
{}

Texture::~Texture()
{
	DeleteDC(m_memDC); // Compatible Device Context�� ����
	DeleteObject(m_hBit); // ��Ʈ�� �ڵ� ����
}

// ��Ʈ�� �̹����� �ε�
void Texture::Load(const wstring& _filePath, HDC _mainDC)
{
	// LoadImage -> ��Ʈ���� ������ �پ��� ������ �̹����� �ε�
	/*
	LoadImageW(
		_In_opt_ HINSTANCE hInst, // ���� ���ҽ��� �ҷ��� ��� nullptr
		_In_ LPCWSTR name, // �ҷ��� �̹����� ��� / LPCWSTR : CONST WCHAR*�� typedef
		_In_ UINT type, // �ҷ��� �̹����� ����
		_In_ int cx, // �ʺ� (0�̰� fuLoad �μ����� LR_DEFAULTSIZE�� �����Ǿ����� ������ ����ũ��)
		_In_ int cy, // ���� (0�̰� fuLoad �μ����� LR_DEFAULTSIZE�� �����Ǿ����� ������ ����ũ��)
		_In_ UINT fuLoad); // �ϳ� �̻��� �÷��׸� ������ �� �ֽ��ϴ�.
	*/
	// LR_LOADFROMFILE : lpszName �μ��� ���ҽ� ��� ���� ��θ� ����� �ҷ��ɴϴ�.
	// string.c_str() : ��ü string�� c��Ÿ���� ���ڿ� const char*�� ��ȯ���ش�.
	m_hBit = (HBITMAP)LoadImage(nullptr, _filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	assert(m_hBit);

	// ȣȯ ������ ����̽� ���ؽ�Ʈ(Compatible Device Context)�� ����
	// CreateCompatibleDC() �Լ��� ���� ����̽� ���ؽ�Ʈ�� ȣȯ�Ǵ� ���ο� ����̽� ���ؽ�Ʈ�� ����ϴ�.
	m_memDC = CreateCompatibleDC(_mainDC);

	// �ҷ��� ��Ʈ�ʰ� DC ����
	HBITMAP hPreBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hPreBit); // m_dc�� default ��Ʈ�� (1 x 1)�� ����

	// ��Ʈ�� ������ ����
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}

// ��Ʈ�� �̹����� ����
void Texture::Create(UINT _width, UINT _height, HDC _mainDC)
{
	m_hBit = CreateCompatibleBitmap(_mainDC, _width, _height); // ȣȯ ������ ��Ʈ�� ����
	m_memDC = CreateCompatibleDC(_mainDC); // ȣȯ ������ ����̽� ���ؽ�Ʈ ����

	// ������ ��Ʈ�ʰ� DC ����
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit); // m_dc�� default ��Ʈ�� (1 x 1)�� ����

	// ��Ʈ�� ������ ����
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}