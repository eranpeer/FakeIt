#pragma once

namespace testutils
{
	template<typename VecType>
	std::vector<VecType>& multi_emplace(std::vector<VecType>& vec)
	{
		return vec;
	}

	template<typename VecType>
	std::vector<VecType>&& multi_emplace(std::vector<VecType>&& vec)
	{
		return std::move(vec);
	}

	template<typename VecType, typename Head, typename... Tail>
	std::vector<VecType>& multi_emplace(std::vector<VecType>& vec, Head&& head, Tail&&... tail)
	{
	    vec.emplace_back(std::move(head));
	    return multi_emplace(vec, tail...);
	}

	template<typename VecType, typename Head, typename... Tail>
	std::vector<VecType>&& multi_emplace(std::vector<VecType>&& vec, Head&& head, Tail&&... tail)
	{
	    vec.emplace_back(std::move(head));
	    return std::move(multi_emplace(vec, tail...));
	}
}
