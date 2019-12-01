#include "YAMLStream.hpp"

YAMLStream::YAMLStream() : m_IsOpen{ false }, m_Emitter{ std::make_unique<YAML::Emitter>() } {}

YAMLStream::YAMLStream(std::string_view filepath, Type flag) : m_Emitter{ std::make_unique<YAML::Emitter>() }
{
	open(filepath, flag);
}

YAMLStream& YAMLStream::operator<<(const std::pair<std::string, std::unique_ptr<Configurable>>& node)
{
	*m_Emitter << YAML::Key << node.first;
	*m_Emitter << YAML::Value << YAML::Flow << node.second->encode();
	return *this;
}

void YAMLStream::open(std::string_view filePath, Type flag)
{
	switch (flag) {
	case in:
		try {
			m_Node.reset();
			m_Node = YAML::LoadFile(filePath.data());
			m_IsOpen = true;
		}
		catch (...) {
			m_IsOpen = false;
		}
		break;
	case out:
		m_OutFile.open(filePath.data());
		m_IsOpen = m_OutFile.is_open();
		(*m_Emitter) << YAML::BeginMap;
		break;
	}
}

void YAMLStream::close()
{
	if (m_OutFile.is_open()) {
		*m_Emitter << YAML::EndMap;
		m_OutFile << m_Emitter->c_str();
		m_OutFile.close();
		m_Emitter.reset(new YAML::Emitter);
	}
	m_IsOpen = false;
}

bool YAMLStream::isOpen()
{
	return m_IsOpen;
}

YAML::Node YAMLStream::operator[](std::string_view name)
{
	return m_Node[name.data()];
}