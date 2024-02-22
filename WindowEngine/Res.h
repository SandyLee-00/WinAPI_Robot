#pragma once

/// <summary>
/// ���� : ��� ���ҽ��� ���Ŭ����
/// ����� : ���ؿ�
/// </summary>
class Res
{
private:

	/// �������
	wstring m_key;          // ���ҽ� �̸� => Key
	wstring m_relativePath; // ���ҽ� ����� => Value

public:

	/// ��� �Լ�
	void SetKey(const wstring& _key) { m_key = _key; } // key setter
	void SetRelativePath(const wstring& _path) { m_relativePath = _path; } // path setter

	const wstring& GetKey() { return m_key; } // key getter
	const wstring& GetRelativePath() { return m_relativePath; } // path getter

public:
	
	/// ������
	Res();

	/// �Ҹ���
	virtual ~Res();
};

