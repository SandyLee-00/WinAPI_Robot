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
	// 스톡 오브젝트
	HOLLOW,
	BLACK,

	// 직접 해제해야하는 BRUSH
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
	// 스톡 오브젝트
	HOLLOW,
	WHITE,

	// 직접 해제해야하는 PEN
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
///  사운드 종류
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