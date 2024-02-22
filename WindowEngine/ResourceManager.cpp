#include "pch.h"
#include "ResourceManager.h"

// ���ҽ��� �ٷ�� ���� �������
#include "PathManager.h" // ���� ���
#include "Texture.h" // ��Ʈ�� ����
#include "Res.h"

// ������
ResourceManager::ResourceManager()
{}

// �Ҹ���
ResourceManager::~ResourceManager()
{
	// ���ҽ� ���� map�� �� �޸� ����
	Safe_Delete_Map(m_mapTex);
}

// ��Ʈ�� �ε�
// ���ҽ� �̸��� ��θ� �̿��� map���� ��Ʈ�� ���ҽ��� �ε�
// Texture*�� ��ȯ
Texture* ResourceManager::LoadTexture(const wstring& _key, const wstring& _relativePath, PathManager* _pathManager, HDC _mainDC)
{
	// ���ҽ��� �̸� (key)�� �̿��� ��ųʸ����� ���ҽ��� �˻�
	// ���ҽ��� Texture*�� ����
	Texture* pTex = FindTexture(_key);

	if (pTex != nullptr)
		return pTex;
	else
	{
		// ���ҽ������� ��θ� ���� ����
		wstring resFilePath = _pathManager->GetContentPath();
		resFilePath += _relativePath;

		pTex = new Texture;
		pTex->Load(resFilePath, _mainDC); // �ش� ����� ���ҽ��� �� Load

		// map�� ����
		pTex->SetKey(_key);
		pTex->SetRelativePath(_relativePath);

		m_mapTex.insert(make_pair(_key, pTex));

		return pTex;
	}
}

// ��Ʈ�� ����
Texture* ResourceManager::CreateTexture(const wstring& _key, UINT _width, UINT _height, HDC _mainDC)
{
	// key�� �̿��� ���ҽ� Ž��
	Texture* pTex = FindTexture(_key);

	// pTex�� ���� ��������� �� ���ҽ��� return
	if (nullptr != pTex)
		return pTex;
	// pTex�� �ƹ��͵� ������ ���� ��Ʈ�� ����
	else
	{
		pTex = new Texture;
		pTex->Create(_width, _height, _mainDC);
		pTex->SetKey(_key);

		m_mapTex.insert(make_pair(_key, pTex));
		return pTex;
	}
}

// ��Ʈ�� �˻�
Texture* ResourceManager::FindTexture(const wstring& _key)
{
	// �ݺ���
	std::map<wstring, Res*>::iterator iter = m_mapTex.find(_key);

	// iter �ݺ��ڸ� �̿��� map�� ������ key�� Ž��
	// iter->second : map�� 2��° ���� value�� ��ȯ�ϴµ�
	// (Texture*)�� ��������� ĳ���� ���־�, "�ٿ�ĳ����"
	// iter->second�� ��ȯ���� Res*�̰� Res�� �Ļ�Ŭ������ Texture*�� �ٿ�ĳ���� ����
	if (iter != m_mapTex.end())
		return (Texture*)iter->second;
	else
		return nullptr; // ã�Ҵµ� ������ nullptr ��ȯ
}

void ResourceManager::Initialize()
{
}
