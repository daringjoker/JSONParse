#include "Json.h"
#include <cctype>
#include <optional>
#include <iostream>

#define FAIL() {std::cout<<"failed! file: "<<__FILE__<<" and line: "<<__LINE__<<" and Data was='"<<data<<"'"<<std::endl; return std::nullopt;}

std::optional<ParseResult> parseArray(std::string data);
std::optional<ParseResult> parseObject(std::string data);

JSONValue JSONValue::null(){
  JSONValue nullValue;
  nullValue.type = JSONType::JSON_NULL;
  return nullValue;
}

JSONValue JSONValue::string(std::string value){
  JSONValue stringValue;
  stringValue.type = JSONType::JSON_STRING;
  stringValue.value = value;
  return stringValue;
}

JSONValue JSONValue::number(double value){
  JSONValue numberValue;
  numberValue.type = JSONType::JSON_NUMBER;
  numberValue.value = value;
  return numberValue;
}

JSONValue JSONValue::boolean(bool value){
  JSONValue booleanValue;
  booleanValue.type = JSONType::JSON_BOOL;
  booleanValue.value = value;
  return booleanValue;
}

JSONValue JSONValue::array(std::vector<JSONValue> value){
  JSONValue arrayValue;
  arrayValue.type = JSONType::JSON_ARRAY;
  arrayValue.value = value;
  return arrayValue;
}

JSONValue JSONValue::object(std::map<std::string, JSONValue> value){
  JSONValue objectValue;
  objectValue.type = JSONType::JSON_OBJECT;
  objectValue.value = value;
  return objectValue;
}

std::ostream& operator<<(std::ostream& stream, const JSONValue& value){
  bool requireComma = false;
  switch(value.type){
    case JSONType::JSON_NULL:
      stream << "null";
      break;
    case JSONType::JSON_STRING:
      stream << '"' << std::get<std::string>(value.value) << '"';
      break;
    case JSONType::JSON_NUMBER:
      stream<<std::fixed<<std::get<double>(value.value);
      break;
    case JSONType::JSON_BOOL:
      stream << (std::get<bool>(value.value) ? "true" : "false");
      break;
    case JSONType::JSON_OBJECT:
      stream << "{\n";
      for(auto& [key, element] : std::get<std::map<std::string, JSONValue>>(value.value)){
        if(requireComma){
          stream << ",\n";
        }
        requireComma = true;
        stream << '"' << key << "\": " << element;
      }
      stream << "\n}";
      break;
    case JSONType::JSON_ARRAY:
      auto& array = std::get<std::vector<JSONValue>>(value.value);
      stream << '[';
      for(auto& element : array){
        if(requireComma){
          stream << ", ";
        }
        requireComma = true;
        stream << element;
      }
      stream << ']'<<std::endl;
      break;
  }
  return stream;

}

std::optional<ParseResult> parseString(std::string data){
  if(data[0]!= '"')
  FAIL()
  size_t closing=0;
  while((closing = data.find('"',closing+1)) != std::string::npos && data[closing-1] == '\\');
  if(closing == std::string::npos)
  FAIL()
  return ParseResult{JSONValue::string(data.substr(1,closing-1)),data.substr(closing+1)};
}

std::optional<ParseResult> parseNumber(std::string data){
  if(!std::isdigit(data[0]) && data[0] != '-')
  FAIL()
  size_t end = data.find_first_not_of("0123456789",1);
  if(end == std::string::npos)
    end = data.size();
  else if(data[end] == '.')
    end = data.find_first_not_of("0123456789",end+1);

  std::string remaining=end==std::string::npos?"":data.substr(end);
  return ParseResult{JSONValue::number(std::stod(data.substr(0,end))),remaining};
  FAIL()
}

std::optional<ParseResult> parseBool(std::string data){
  if(data.find("true") == 0)
    return ParseResult{JSONValue::boolean(true),data.substr(4)};
  if(data.find("false") == 0)
    return ParseResult{JSONValue::boolean(false),data.substr(5)};
  FAIL()
}

std::optional<ParseResult> parseNull(std::string data){
  if(data.find("null") == 0)
    return ParseResult{JSONValue::null(),data.substr(4)};
  FAIL()
}
void consumeWhitespace(std::string& data){
  while(std::isspace(data[0]))
    data = data.substr(1);
}

std::optional<ParseResult> parseValue(std::string data){
  consumeWhitespace(data);
  if(data.empty())
  FAIL()
  if(data[0] == '"')
    return parseString(data);
  if(std::isdigit(data[0]) || data[0] == '-')
    return parseNumber(data);
  if(data.find("true") == 0 || data.find("false") == 0)
    return parseBool(data);
  if(data.find("null") == 0)
    return parseNull(data);
  if(data[0] == '[')
    return parseArray(data);
  if(data[0] == '{')
    return parseObject(data);
  FAIL()
}

std::optional<ParseResult> parseArray(std::string data){
  if(data[0] != '[')
  FAIL()
  std::vector<JSONValue> values;
  data = data.substr(1);
  consumeWhitespace(data);
  while(data[0] != ']'){
    consumeWhitespace(data);
    auto value = parseValue(data);
    if(!value)
    FAIL()
    values.push_back(value->parsed);
    data = value->remaining;
    consumeWhitespace(data);
    if(data[0] == ',')
      data = data.substr(1);
  }
  return ParseResult{JSONValue::array(values),data.substr(1)};
}

std::optional<ParseResult> parseObject(std::string data){
  if(data[0] != '{')
  FAIL()
  std::map<std::string, JSONValue> values;
  data = data.substr(1);
  consumeWhitespace(data);
  while(data[0] != '}'){
    consumeWhitespace(data);
    auto key = parseString(data);
    if(!key)
    FAIL()
    data = key->remaining;
    consumeWhitespace(data);
    if(data[0] != ':')
    FAIL()
    data = data.substr(1);
    consumeWhitespace(data);
    auto value = parseValue(data);
    if(!value)
    FAIL()
    values[std::get<std::string>(key->parsed.value)] = value->parsed;
    data = value->remaining;
    consumeWhitespace(data);
    if(data[0] == ',')
      data = data.substr(1);
  }
  return ParseResult{JSONValue::object(values),data.substr(1)};
}

std::optional<JSONValue> JSON::Parse(std::string data){
  if(data.empty())
  FAIL()
  auto result = parseValue(data);
  if(!result)
  FAIL()
  return result->parsed;
}
