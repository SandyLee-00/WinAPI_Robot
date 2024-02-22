#include "pch.h" // include useful header & using namespace
#include "PathManager.h"

// no_argument constructor
// ������ �ʱ�ȭ ����Ʈ ��� (�� ������ ȿ�������� �ʱ�ȭ) or const ������� �ʱ�ȭ
PathManager::PathManager() : m_szContentPath{}, m_szSoundResPath{}
{}

// de - constructor
PathManager::~PathManager()
{}

void PathManager::Initialize()
{
	// ���� �۾� ���丮�� ��θ� �������� �Լ� (Windows.h�� �Լ�)
	// first : ����� �ִ� ����
	// second : ����� ���ڿ� ���� ����
	// m_szContentPath : L"C:\\Users\\USER-PC\\Documents\\GitHub\\WindowEngine\\WindowEngine"
	GetCurrentDirectory(255, m_szContentPath);
	GetCurrentDirectoryA(255, m_szSoundResPath);

	// ���� �۾� ���丮�� ����� ���� ���ϱ�
	// wcslen : strlen()�� wide ����
	int length = (int)wcslen(m_szContentPath);
	int sLength = (int)strlen(m_szSoundResPath);

	// ���� ������
	// ���� �۾� ���丮�� ��ΰ� ����Ǿ� �ִ� ���ڿ��� ������ index���� ù��° index���� ��ȸ
	for (int i = length - 1; i >= 0; i--)
	{
		// L"~\\WindowEngine\\WindowEngine"�� ������ ���� ����
		// L"~\\WindowEngine"
		if (m_szContentPath[i] == '\\') // '\' �� ���ڸ� ã����
		{
			m_szContentPath[i] = '\0'; // NULL ���� (���ڿ��� ��)
			break;
		}
	}
	for (int i = sLength - 1; i >= 0; i--)
	{
		// L"~\\WindowEngine\\WindowEngine"�� ������ ���� ����
		// L"~\\WindowEngine"
		if (m_szSoundResPath[i] == '\\') // '\' �� ���ڸ� ã����
		{
			m_szSoundResPath[i] = '\0'; // NULL ���� (���ڿ��� ��)
			break;
		}
	}

	// L"~\\WindowEngine"�� ������ ���� ����
	// L"~\\WindowEngine\\Resource"
	// wcscat_s : ���ڿ� ���ۿ� ���ڿ� �̾���̱�
	wcscat_s(m_szContentPath, 255, L"\\Resource");
	strcat_s(m_szSoundResPath, "\\Resource");
}