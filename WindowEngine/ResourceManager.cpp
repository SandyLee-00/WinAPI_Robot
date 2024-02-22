#include "pch.h"
#include "ResourceManager.h"

// 리소스를 다루기 위한 헤더파일
#include "PathManager.h" // 파일 경로
#include "Texture.h" // 비트맵 정보
#include "Res.h"

// 생성자
ResourceManager::ResourceManager()
{}

// 소멸자
ResourceManager::~ResourceManager()
{
	// 리소스 저장 map의 힙 메모리 해지
	Safe_Delete_Map(m_mapTex);
}

// 비트맵 로드
// 리소스 이름과 경로를 이용해 map에서 비트맵 리소스를 로드
// Texture*를 반환
Texture* ResourceManager::LoadTexture(const wstring& _key, const wstring& _relativePath, PathManager* _pathManager, HDC _mainDC)
{
	// 리소스의 이름 (key)을 이용해 딕셔너리에서 리소스를 검색
	// 리소스를 Texture*로 저장
	Texture* pTex = FindTexture(_key);

	if (pTex != nullptr)
		return pTex;
	else
	{
		// 리소스파일의 경로를 최종 결정
		wstring resFilePath = _pathManager->GetContentPath();
		resFilePath += _relativePath;

		pTex = new Texture;
		pTex->Load(resFilePath, _mainDC); // 해당 경로의 리소스를 찐 Load

		// map에 저장
		pTex->SetKey(_key);
		pTex->SetRelativePath(_relativePath);

		m_mapTex.insert(make_pair(_key, pTex));

		return pTex;
	}
}

// 비트맵 생성
Texture* ResourceManager::CreateTexture(const wstring& _key, UINT _width, UINT _height, HDC _mainDC)
{
	// key를 이용해 리소스 탐색
	Texture* pTex = FindTexture(_key);

	// pTex에 무언가 들어있으면 그 리소스를 return
	if (nullptr != pTex)
		return pTex;
	// pTex에 아무것도 없으면 직접 비트맵 생성
	else
	{
		pTex = new Texture;
		pTex->Create(_width, _height, _mainDC);
		pTex->SetKey(_key);

		m_mapTex.insert(make_pair(_key, pTex));
		return pTex;
	}
}

// 비트맵 검색
Texture* ResourceManager::FindTexture(const wstring& _key)
{
	// 반복자
	std::map<wstring, Res*>::iterator iter = m_mapTex.find(_key);

	// iter 반복자를 이용해 map을 끝까지 key를 탐색
	// iter->second : map의 2번째 인자 value를 반환하는데
	// (Texture*)를 명시적으로 캐스팅 해주어, "다운캐스팅"
	// iter->second의 반환형은 Res*이고 Res의 파생클래스인 Texture*로 다운캐스팅 해줌
	if (iter != m_mapTex.end())
		return (Texture*)iter->second;
	else
		return nullptr; // 찾았는데 없으면 nullptr 반환
}

void ResourceManager::Initialize()
{
}
