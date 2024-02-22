#pragma once

enum class TOPPING
{
	/// �߰� ���
	// ���� ��������Ʈ��Ʈ�� enum ������ ������
	// ���� �ٲٸ� �ȵſ�

	OLIVE,			// �ø���
	PEPPERONI,		// ���۷δ�
	PINEAPPLE,		// ���ξ���
	POTATO,			// ����
	SHIRIMP,		// ����

	/// �⺻ ���

	DOUGH,			// ����(������)
	SOURCE,			// �丶�� �ҽ�
	CHEEZE,			// ġ��

	NONE,
	END,
};

// TODO : ��ü ����� �ٲ㼭 Boss���� �������� �ʾƵ� �ǰ� -- SANDY�ϱ�
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
	
	Object* m_owner;		// Food�� �����ϰ� �ִ� ������Ʈ
	Vector2 m_pos;			// Food�� ������
	LOOKDIR m_lookdir;		// owner�� �ٶ󺸴� ����

	int m_slice;			// ��������Ʈ ��Ʈ �ڸ� ��
	Vector2 m_offset;		// ��������Ʈ ������

	float m_animOffset;		 // IDLE Animation�� ���� y�� ������ �ִϸ��̼� �ֱ�
	double m_animOffsetTime; // DeltaTime ����
	int m_animCount;

public:
	void SetOwner(Object* _owner) { m_owner = _owner; }

public:
	void Update(GameProcess* _gameProcess);
	void Render(HDC _dc);					// Owner�� ���� �� �ڱ� �ڽ� ����
	virtual void OwnerRender(HDC _dc) = 0;	// Owner�� ���� �� Owner�� ����

public:
	Object_Food();
	~Object_Food() override;
};

