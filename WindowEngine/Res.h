#pragma once

/// <summary>
/// 역할 : 모든 리소스의 기반클래스
/// 담당자 : 공준열
/// </summary>
class Res
{
private:

	/// 멤버변수
	wstring m_key;          // 리소스 이름 => Key
	wstring m_relativePath; // 리소스 상대경로 => Value

public:

	/// 멤버 함수
	void SetKey(const wstring& _key) { m_key = _key; } // key setter
	void SetRelativePath(const wstring& _path) { m_relativePath = _path; } // path setter

	const wstring& GetKey() { return m_key; } // key getter
	const wstring& GetRelativePath() { return m_relativePath; } // path getter

public:
	
	/// 생성자
	Res();

	/// 소멸자
	virtual ~Res();
};

