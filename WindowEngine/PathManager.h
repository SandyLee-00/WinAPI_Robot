#pragma once

/// <summary>
/// ���� : ���� ��� ���� (���ҽ� ������ ��θ� ����η� ��Ÿ��)
/// ����� : ���ؿ�
/// </summary>
class PathManager
{
	PathManager();
	~PathManager();

	friend class GameProcess;

private: // member variable

	// sz : NULL�� ������ ���ڿ� (�밡���� ǥ���)
	// wchar_t : 2����Ʈ�� ũ��, �����ڵ� ǥ������ (��ο� �ѱ��� ���ԵǾ� ���� �� �ֱ� ����..?)
	wchar_t m_szContentPath[256];
	char m_szSoundResPath[256]; // ���� ��� ����

public: // member func

	// Path Manager �ʱ�ȭ �Լ�
	void Initialize();

	// m_szContentPath�� getter
	const wchar_t* GetContentPath() { return m_szContentPath; }
	const char* GetSoundResPath() { return m_szSoundResPath; }
};