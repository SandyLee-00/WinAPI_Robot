#pragma once


/// --------예상가능한 버그 적는 공간------------------------------------------------------------------------

/// 1. 헤더파일에서 다른 클래스를 사용하는 경우에는 헤더파일을 참조하지 않고 헤더파일 위에 class를 선언만 해주세요 !!!!!!!!

/// 2. 만약 한줄짜리 구현을 했는데 인자로 클래스를 사용하는경우 cpp파일에서 구현해야지 링크오류가 발생하지 않음

/// 3. 매크로 SINGLE을 사용하는 경우에는 CPP 파일에 생성자와 소멸자를 내가 직접 써야한다 !!
///   싱글 매크로함수를 사용해서 생성자와 소몀자를 구현하지 않으면 링크오류가 발생한다!!!!!!!

/// ----------------------------------------------------------------------------------------------------