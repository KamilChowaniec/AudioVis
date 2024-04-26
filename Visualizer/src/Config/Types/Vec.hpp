#pragma once
#include "Config/Types/Configurable.hpp"
#include "Config/Viewers/NullViewer.hpp"
#include "Logger.hpp"
#include <glm/glm.hpp>

template<size_t N, typename T, template<typename> typename Viewer = NullViewer>
class Vec;

template<typename T, template<typename> typename Viewer = NullViewer>
using Number = Vec<1, T, Viewer>;
template<typename T, template<typename> typename Viewer = NullViewer>
using Vec2 = Vec<2, T, Viewer>;
template<typename T, template<typename> typename Viewer = NullViewer>
using Vec3 = Vec<3, T, Viewer>;
template<typename T, template<typename> typename Viewer = NullViewer>
using Vec4 = Vec<4, T, Viewer>;
template<typename T, template<typename> typename Viewer = NullViewer>
using Color = Vec4<T, Viewer>;


template<size_t N, typename T, template<typename> typename Viewer>
class Vec : public Configurable, public glm::vec<N, T> {
public:
	Vec(glm::vec<N, T> vec, Viewer<T>&& viewer = Viewer<T>())
		: glm::vec<N, T>{ vec }, m_Viewer{ viewer }
	{}

	template<int U = N>
	Vec(typename std::enable_if_t<U == 1, T> val, Viewer<T>&& viewer = Viewer<T>())
		: glm::vec<N, T>{ val }, m_Viewer{ viewer }
	{}

	template<int U = N>
	operator std::enable_if_t<U == 1, T>() const { return 0; }

private:
	Viewer<T> m_Viewer;

	void show(std::string_view name) override { m_Viewer.show(name, &this->x); }
	
	YAML::Node encode() override {
		YAML::Node node;
		if constexpr(N == 1) node = (*this)[0];
		else for (int i = 0; i < N; ++i) node.push_back((*this)[i]);
		return node;
	}

	void decode(const YAML::Node& node) override {
		if constexpr (N == 1)
		{
			if (!node.IsScalar() || node.size() != N)
				LOG_ERROR("Wrong type to decode!");
			(*this)[0] = node.as<T>();
		}
		else {
			if (!node.IsSequence() || node.size() != N)
				LOG_ERROR("Wrong type to decode!");
			for (int i = 0; i < N; ++i) (*this)[i] = node[i].as<T>();
		}
	}
};
