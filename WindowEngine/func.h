#pragma once

class Object;


template<typename T>
void Safe_Delete_Vec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
			delete _vec[i];
	}
	_vec.clear();
}


template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	// 탬플릿 내부에서 이너클래스에 접근하기 위해서는 앞에 typename을 붙여야 한다.
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		if (nullptr != iter->second)
			delete iter->second;
	}

	_map.clear();
}