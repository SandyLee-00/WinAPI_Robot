#pragma once

enum class TOPPING
{
	/// 추가 재료
	// 토핑 스프라이트시트와 enum 순서를 맞췄음
	// 순서 바꾸면 안돼용

	OLIVE,			// 올리브
	PEPPERONI,		// 페퍼로니
	PINEAPPLE,		// 파인애플
	POTATO,			// 감자
	SHIRIMP,		// 새우

	/// 기본 재료

	DOUGH,			// 도우(재료상태)
	SOURCE,			// 토마토 소스
	CHEEZE,			// 치즈

	NONE,
	END,
};

// TODO : 전체 헤더로 바꿔서 Boss에서 포함하지 않아도 되게 -- SANDY하기
enum class LOOKDIR
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

class Object_Food :
	public Object
{
protected:
	
	Object* m_owner;		// Food를 소유하고 있는 오브젝트
	Vector2 m_pos;			// Food의 포지션
	LOOKDIR m_lookdir;		// owner가 바라보는 방향

	int m_slice;			// 스프라이트 시트 자를 값
	Vector2 m_offset;		// 스프라이트 오프셋

	float m_animOffset;		 // IDLE Animation에 맞춰 y축 오프셋 애니메이션 주기
	double m_animOffsetTime; // DeltaTime 누적
	int m_animCount;

public:
	void SetOwner(Object* _owner) { m_owner = _owner; }

public:
	void Update(GameProcess* _gameProcess);
	void Render(HDC _dc);					// Owner가 없을 때 자기 자신 렌더
	virtual void OwnerRender(HDC _dc) = 0;	// Owner가 있을 때 Owner가 렌더

public:
	Object_Food();
	~Object_Food() override;
};

