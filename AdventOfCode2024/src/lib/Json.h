#pragma once

#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <variant>
#include <vector>

class Json;

class JsonObject
{
private:
	std::map<std::string, Json> values;

public:
	void Add(const std::string& key, const Json& value);
	Json& Reserve(const std::string& key);
	void Remove(const std::string& key);
	void Filter(const std::set<std::string>& set);
	bool Has(const std::string& key) const;
	const Json& Get(const std::string& key) const;
	Json& Get(const std::string& key);
	friend std::ostream& operator<<(std::ostream& os, const JsonObject& object);

	std::map<std::string, Json>::iterator begin() { return values.begin(); }
	std::map<std::string, Json>::iterator end() { return values.end(); }
	std::map<std::string, Json>::const_iterator begin() const { return values.cbegin(); }
	std::map<std::string, Json>::const_iterator end() const { return values.cend(); }
};

class JsonArray
{
private:
	std::vector<Json> values;

public:
	void Add(const Json& value);
	Json& Reserve();
	const Json& Get(size_t i) const;
	Json& Get(size_t i);
	size_t Size() const;

	friend std::ostream& operator<<(std::ostream& os, const JsonArray& object);

	std::vector<Json>::iterator begin() { return values.begin(); }
	std::vector<Json>::iterator end() { return values.end(); }
	std::vector<Json>::const_iterator begin() const { return values.cbegin(); }
	std::vector<Json>::const_iterator end() const { return values.cend(); }
};

class Nullptr
{
	friend std::ostream& operator<<(std::ostream& os, const Nullptr& object) { return os << "null"; }
};

class Json
{
public:
	friend class JsonUtil;
	using JsonVariant = std::variant<JsonObject, JsonArray, std::string, double, uint64_t, bool, Nullptr>;
private:
	JsonVariant value;

public:
	Json() : value{Nullptr{}} {}
	Json(const JsonObject& value) : value{value} {}
	Json(const JsonArray& value) : value{value} {}
	Json(const std::string& value) : value{value} {}
	Json(double value) : value{value} {}
	Json(float value) : value{(double)value} {}
	Json(uint64_t value) : value{value} {}
	Json(int64_t value) : value{(uint64_t)value} {}
	Json(uint32_t value) : value{(uint64_t)value} {}
	Json(int32_t value) : value{(uint64_t)value} {}
	Json(uint16_t value) : value{(uint64_t)value} {}
	Json(int16_t value) : value{(uint64_t)value} {}
	Json(uint8_t value) : value{(uint64_t)value} {}
	Json(int8_t value) : value{(uint64_t)value} {}
	Json(bool value) : value{value} {}
	Json(Nullptr value) : value{value} {}

	const JsonObject& GetJsonObject() const
	{
		if (IsJsonObject())
		{
			return std::get<JsonObject>(value);
		}
		static JsonObject ref;
		return ref;
	}

	JsonObject& GetJsonObject()
	{
		if (IsJsonObject())
		{
			return std::get<JsonObject>(value);
		}
		static JsonObject ref;
		return ref;
	}

	const JsonArray& GetJsonArray() const
	{
		if (IsJsonArray())
		{
			return std::get<JsonArray>(value);
		}
		static JsonArray ref;
		return ref;
	}

	JsonArray& GetJsonArray() 
	{
		if (IsJsonArray())
		{
			return std::get<JsonArray>(value);
		}
		static JsonArray ref;
		return ref;
	}

	const std::string& GetString() const
	{
		if (IsString())
		{
			return std::get<std::string>(value);
		}
		static std::string ref;
		return ref;
	}

	std::string& GetString()
	{
		if (IsString())
		{
			return std::get<std::string>(value);
		}
		static std::string ref;
		return ref;
	}

	float GetFloat() const
	{
		if (IsFloat())
		{
			return (float)std::get<double>(value);
		}
		return 0.0f;
	}

	double GetDouble() const
	{
		if (IsString())
		{
			return std::get<double>(value);
		}
		return 0.0;
	}

	uint64_t GetUInt64() const
	{
		if (IsInt())
		{
			return std::get<uint64_t>(value);
		}
		return 0;
	}

	int64_t GetInt64() const
	{
		if (IsInt())
		{
			return (int64_t)std::get<uint64_t>(value);
		}
		return 0;
	}

	int32_t GetUInt32() const
	{
		if (IsInt())
		{
			return (uint32_t)std::get<uint64_t>(value);
		}
		return 0;
	}

	int32_t GetInt32() const
	{
		if (IsInt())
		{
			return (int32_t)std::get<uint64_t>(value);
		}
		return 0;
	}

	uint16_t GetUInt16() const
	{
		if (IsInt())
		{
			return (uint16_t)std::get<uint64_t>(value);
		}
		return 0;
	}

	int16_t GetInt16() const
	{
		if (IsInt())
		{
			return (int16_t)std::get<uint64_t>(value);
		}
		return 0;
	}

	uint8_t GetUInt8() const
	{
		if (IsInt())
		{
			return (uint8_t)std::get<uint64_t>(value);
		}
		return 0;
	}

	int8_t GetInt8() const
	{
		if (IsInt())
		{
			return (int8_t)std::get<uint64_t>(value);
		}
		return 0;
	}

	bool GetBool() const
	{
		if (IsBool())
		{
			return std::get<bool>(value);
		}
		return false;
	}

	bool IsJsonObject() const
	{
		return std::holds_alternative<JsonObject>(value);
	}

	bool IsJsonArray() const
	{
		return std::holds_alternative<JsonArray>(value);
	}

	bool IsString() const
	{
		return std::holds_alternative<std::string>(value);
	}

	bool IsFloat() const
	{
		return std::holds_alternative<double>(value);
	}

	bool IsInt() const
	{
		return std::holds_alternative<uint64_t>(value);
	}

	bool IsBool() const
	{
		return std::holds_alternative<bool>(value);
	}

	bool IsNull() const
	{
		return std::holds_alternative<Nullptr>(value);
	}

	friend std::ostream& operator<<(std::ostream& os, const Json& json)
	{
		if (json.IsJsonObject())
			os << json.GetJsonObject();
		else if (json.IsJsonArray())
			os << json.GetJsonArray();
		else if (json.IsString())
			os << "\"" << json.GetString() << "\"";
		else if (json.IsFloat())
			os << json.GetFloat();
		else if (json.IsInt())
			os << json.GetInt64();
		else if (json.IsBool())
			os << json.GetBool() ? "true" : "false";
		else if (json.IsNull())
			os << "null";
		return os;
	}

	static std::pair<bool, Json> FromString(const std::string& str)
	{
		uint64_t pos = 0;
		Json json;
		bool res = Read(str, pos, json);
		return {res, json};
	}

	static std::pair<bool, Json> FromStream(std::istream& stream)
	{
		std::string s(std::istreambuf_iterator<char>(stream), {});
		uint64_t pos = 0;
		Json json;
		bool res = Read(s, pos, json);
		return {res, json};
	}

	static bool Read(const std::string& str, uint64_t& pos, Json& json);
};