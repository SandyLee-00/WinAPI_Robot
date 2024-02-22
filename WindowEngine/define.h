#pragma once

enum class OBJECT_TYPE
{
	DEFAULT,
	BACKGROUND,

	FOOD,
	INTERACTABLE_OBJECT,

	PLAYER,
	BOSS,

	SYSTEM,

	UI =31,
	END = 32,
};

enum class SCENE_TYPE
{
	MAIN,
	OPTION,
	TUTORIAL,
	STAGE01,
	CARTOON,
	ENDING,

	EXITGAME,
	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	CHANGE_SCENE,


	END,
};

enum class BRUSH_TYPE
{
	// ���� ������Ʈ
	HOLLOW,
	BLACK,

	// ���� �����ؾ��ϴ� BRUSH
	RED,
	GREEN,
	BLUE,
	SKKBLUE,
	BROWN,
	MAGENTA,
	END,
};

enum class PEN_TYPE
{
	// ���� ������Ʈ
	HOLLOW,
	WHITE,

	// ���� �����ؾ��ϴ� PEN
	RED,
	GREEN,
	BLUE,
	SKKBLUE,
	BROWN,
	MAGENTA,
	END,
};

enum class FONT_TYPE
{
	DOSPILGI_FONT,
	GALMURI7_FONT,
	GALMURI9_FONT,
	GALMURI9_FONT_SMALL,

	END,
};

/// <summary>
///  ���� ����
/// </summary>
enum class SOUND_TYPE
{
	PLAYER,
	BOSS,

	ORDER,

	OVEN1,
	OVEN2,
	OVEN3,

	BGM,
	UI,
	BOSS_HMM,

	PAGE,
	OVERCOOK,
	GAMEOVER,



	END
};

enum class PATH_TYPE
{
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEEN,
	EIGHT,
	NINE,
	TEN,
	ELEVEN,


	END
};