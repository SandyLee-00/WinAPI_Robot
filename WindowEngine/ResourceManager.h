#pragma once

// Res�� Texture Ŭ������ ���� ����
class Res;
class Texture;
class  PathManager;

/// <summary>
/// ���� : ���ҽ� ��� �Ѱ� (�ε�, ����, �˻�)
/// ����� : ���ؿ�
/// </summary>
class ResourceManager
{
	ResourceManager();
	~ResourceManager();

	friend class GameProcess;



private:

	/// ��� ����
	// ���ҽ��� key - value ���·� ����
	// key : ���ҽ��� "�̸�" (wstring)
	// value : ���ҽ��� "������" (Res*)
	map<wstring, Res*> m_mapTex;


public:
	Texture* LoadTexture(const wstring& _key, const wstring& _relativePath, PathManager* _pathManager, HDC _mainDC);
	Texture* CreateTexture(const wstring& _key, UINT _width, UINT _height, HDC _mainDC);
	Texture* FindTexture(const wstring& _key);

	void Initialize();

};