#include "Json.h"

#include "JsonUtil.h"

#define READ_WHITESPACE() \
	if(!JsonUtil::ReadWhiteSpace(str, pos)) \
  { \
    std::cout << "EOF" << std::endl; \
    return false; \
	}

#define IF_READ_TOKEN(c) \
	if (!JsonUtil::ReadWhiteSpace(str, pos))\
	{ \
    std::cout << "EOF at reading token: " << c << std::endl; \
    return false;\
	} \
  if(CheckAndIncrement(str, pos, c))

#define IF_NOT_READ_TOKEN(c) \
	if (!JsonUtil::ReadWhiteSpace(str, pos))\
	{ \
    std::cout << "EOF at reading token: " << c << std::endl; \
    return false; \
	} \
	if(!CheckAndIncrement(str, pos, c))

bool CheckAndIncrement(const std::string& str, uint64_t& pos, char c)
{
	bool correct = str[pos] == c;
	if (correct) pos++;
	return correct;
}

void JsonObject::Add(const std::string& key, const Json& value)
{
	values.emplace(key, value);
}

Json& JsonObject::Reserve(const std::string& key)
{
	return values.emplace(key, Nullptr{}).first->second;
}

void JsonObject::Remove(const std::string& key)
{
	values.erase(values.find(key));
}

void JsonObject::Filter(const std::set<std::string>& set)
{
	for (auto it = values.begin(); it != values.end();)
	{
		if (set.find(it->first) != set.end())
			it++;
		else
			it = values.erase(it);
	}
}

bool JsonObject::Has(const std::string& key) const
{
	return values.find(key) != values.end();

}
const Json& JsonObject::Get(const std::string& key) const
{
	auto it = values.find(key);
	static Json null{Nullptr{}};
	if (it == values.end())
		return null;
	return it->second;
}

Json& JsonObject::Get(const std::string& key)
{
	auto it = values.find(key);
	static Json null{Nullptr{}};
	if (it == values.end())
		return null;
	return it->second;
}

std::ostream& operator<<(std::ostream& os, const JsonObject& object)
{
	size_t i = 0;
	os << "{";
	for (auto value : object.values)
	{
		os << "\"" << value.first << "\":" << value.second;
		if (i != object.values.size() - 1)
		{
			os << ",";
		}
		i++;
	}
	os << "}";
	return os;
}

void JsonArray::Add(const Json& value)
{
	values.emplace_back(value);
}

Json& JsonArray::Reserve()
{
	return values.emplace_back(Nullptr{});

}

const Json& JsonArray::Get(size_t i) const
{
	if (i < values.size())
		return values[i];
	static Json null{Nullptr{}};
	return null;
}

Json& JsonArray::Get(size_t i)
{
	if (i < values.size())
		return values[i];
	static Json null{Nullptr{}};
	return null;
}

size_t JsonArray::Size() const
{
	return values.size();
}

std::ostream& operator<<(std::ostream& os, const JsonArray& object)
{
	size_t i = 0;
	os << "[";
	for (auto& value : object.values)
	{
		os << value;
		if (i != object.values.size() - 1)
		{
			os << ",";
		}
		i++;
	}
	os << "]";
	return os;
}

bool Json::Read(const std::string& str, uint64_t& pos, Json& json)
{
	uint64_t startPos = pos;
	READ_WHITESPACE();
	if(str[pos] == '{')
	{
		pos++;
		JsonObject& object = json.value.emplace<JsonObject>();
		while (true)
		{
			IF_READ_TOKEN('}') 
			{
				break;
			}

			Json key;
			if (!JsonUtil::ReadString(str, pos, key))
			{
				std::cout << JsonUtil::GetStringPos(str, pos) << ": Failed reading string" << std::endl;
				return false;
			}

			IF_NOT_READ_TOKEN(':')
			{
				std::cout << JsonUtil::GetStringPos(str, pos) << ": Token after JsonObject key (\"" << key << "\") is not \':\'" << std::endl;
				return false;
			}

			Json& element = object.Reserve(key.GetString());

			uint64_t readPos = pos;
			if (!Read(str, pos, element))
			{
				std::cout << JsonUtil::GetStringPos(str, readPos) << ": Json type of JsonObject key (\"" << key << "\") failed" << std::endl;
				return false;
			}

			IF_NOT_READ_TOKEN(',')
			{
				IF_NOT_READ_TOKEN('}')
				{
					std::cout << JsonUtil::GetStringPos(str, pos) << ": expected token \",\" or \"}\"" << std::endl;
					return false;
				}
				break;
			}
		}
		return true;
	}
	else if (str[pos] == '[')
	{
		pos++;
		JsonArray& array = json.value.emplace<JsonArray>();
		while (true)
		{
			IF_READ_TOKEN(']')
			{
				break;
			}

			uint64_t readPos = pos;
			Json& element = array.Reserve();
			if (!Read(str, pos, element))
			{
				std::cout << JsonUtil::GetStringPos(str, readPos) << ": Json type of JsonArray failed" << std::endl;
				return false;
			}

			IF_NOT_READ_TOKEN(',')
			{
				IF_NOT_READ_TOKEN(']')
				{
					std::cout << JsonUtil::GetStringPos(str, pos) << ": expected token \",\" or \"]\"" << std::endl;
					return false;
				}
				break;
			}
		}

		return true;
	}
	else if (str[pos] == '\"')
	{
		return JsonUtil::ReadString(str, pos, json);
	}
	else if (JsonUtil::IsKeyword(str, pos, "true"))
	{
		json.value.emplace<bool>(true);
		return true;
	}
	else if (JsonUtil::IsKeyword(str, pos, "false"))
	{
		json.value.emplace<bool>(false);
		return true;
	}
	else if (JsonUtil::IsKeyword(str, pos, "null"))
	{
		json.value.emplace<Nullptr>();
		return true;
	}
	else if (JsonUtil::IsFloat(str, pos))
	{
		char* end;
		double d = std::strtod(str.c_str() + pos, &end);
		pos = end - str.c_str();
		json.value.emplace<double>(d);
		return true;
	}
	else if (JsonUtil::IsNumber(str, pos))
	{
		char* end;
		int64_t i = std::strtoll(str.c_str() + pos, &end, 10);
		pos = end - str.c_str();
		json.value.emplace<uint64_t>(i);
		return true;
	}
	return false;
}