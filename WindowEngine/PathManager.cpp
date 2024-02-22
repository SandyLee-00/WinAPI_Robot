#include "pch.h" // include useful header & using namespace
#include "PathManager.h"

// no_argument constructor
// 생성자 초기화 리스트 사용 (더 빠르고 효율적으로 초기화) or const 멤버변수 초기화
PathManager::PathManager() : m_szContentPath{}, m_szSoundResPath{}
{}

// de - constructor
PathManager::~PathManager()
{}

void PathManager::Initialize()
{
	// 현재 작업 디렉토리의 경로를 가져오는 함수 (Windows.h의 함수)
	// first : 경로의 최대 길이
	// second : 저장될 문자열 버퍼 공간
	// m_szContentPath : L"C:\\Users\\USER-PC\\Documents\\GitHub\\WindowEngine\\WindowEngine"
	GetCurrentDirectory(255, m_szContentPath);
	GetCurrentDirectoryA(255, m_szSoundResPath);

	// 현재 작업 디렉토리의 경로의 길이 구하기
	// wcslen : strlen()의 wide 버전
	int length = (int)wcslen(m_szContentPath);
	int sLength = (int)strlen(m_szSoundResPath);

	// 상위 폴더로
	// 현재 작업 디렉토리의 경로가 저장되어 있는 문자열의 마지막 index부터 첫번째 index까지 순회
	for (int i = length - 1; i >= 0; i--)
	{
		// L"~\\WindowEngine\\WindowEngine"을 다음과 같이 변경
		// L"~\\WindowEngine"
		if (m_szContentPath[i] == '\\') // '\' 이 문자를 찾으면
		{
			m_szContentPath[i] = '\0'; // NULL 삽입 (문자열의 끝)
			break;
		}
	}
	for (int i = sLength - 1; i >= 0; i--)
	{
		// L"~\\WindowEngine\\WindowEngine"을 다음과 같이 변경
		// L"~\\WindowEngine"
		if (m_szSoundResPath[i] == '\\') // '\' 이 문자를 찾으면
		{
			m_szSoundResPath[i] = '\0'; // NULL 삽입 (문자열의 끝)
			break;
		}
	}

	// L"~\\WindowEngine"을 다음과 같이 변경
	// L"~\\WindowEngine\\Resource"
	// wcscat_s : 문자열 버퍼에 문자열 이어붙이기
	wcscat_s(m_szContentPath, 255, L"\\Resource");
	strcat_s(m_szSoundResPath, "\\Resource");
}