
/// pch.h: 미리 컴파일된 헤더 파일입니다.
/// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
/// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
/// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
/// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

/// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include "Resource.h"

#include <Windows.h>
#include <iostream>
#pragma comment(lib, "Msimg32.lib") // TransBlt 함수을 실제로 구현한 장소

// 메모릭 추적
#include <crtdbg.h>

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <array>
using std::array;


#include <map>
using std::map;
using std::make_pair;

#include <string>
using std::string;
using std::wstring;

// < cmath > 헤더 파일이 <math.h> 헤더 파일과 비교했을 때, C++에서 수학 함수를 사용할 때 더욱 호환성을 고려한 방식으로 구현되어 있기 때문에 일반적으로 더욱 추천됩니다.
// <cmath> 헤더 파일은 <math.h> 헤더 파일을 확장한 것이며, C++에서 추가된 기능을 포함하고 있습니다.예를 들어, <cmath> 헤더 파일에는 C++에서 추가된 타입인 long long과 long double에 대한 수학 함수들이 포함되어 있습니다.
// 또한, <cmath> 헤더 파일에는 C++11에서 추가된 std::isnan과 같은 함수들도 포함되어 있습니다.
// 하지만, <math.h> 헤더 파일은 C 언어와 호환성을 유지하기 위해 설계되었기 때문에, C++에서 사용할 때 일부 함수들이 C 언어와 다른 방식으로 동작할 수 있습니다.따라서, 호환성을 고려할 때에는 <cmath> 헤더 파일을 사용하는 것이 좋습니다.
#include <cmath>
#include <assert.h>
#include <random>

#include "define.h"
#include "Vector2.h"
#include "PathPoint.h"
#include "Math.h"
#include "func.h"

#include "fmod.hpp"

// FMOD
#ifndef _WIN64
#include "../inc/fmod.hpp"
#pragma comment (lib, "../lib/x86/fmod_vc.lib")
using namespace FMOD;
#endif
#ifdef _WIN64
#include "fmod/inc/fmod.hpp"
#pragma comment (lib, "fmod/lib/fmod_vc.lib")
using namespace FMOD;
#endif

#include "GameProcess.h"

#endif //PCH_H
