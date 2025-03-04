#include "Json.h"
#include <cctype>
#include <optional>
#include <iostream>

#define FAIL() {std::cerr<<"failed! file: "<<__FILE__<<" and line: "<<__LINE__<<" and Data was='"<<data<<"'"<<std::endl; return std::nullopt;}

template<typename E, typename F>
std::optional<ParseResult> failExpectation(E expected,F found,int position=-1){
  std::cerr<<"Expected "<<expected<<" but found "<<found;
  if(position != -1)
    std::cerr<<" at position "<<position;
  std::cerr<<std::endl;
  return std::nullopt;
}
std::optional<ParseResult> parseArray(std::string data);
std::optional<ParseResult> parseObject(std::string data);


std::optional<ParseResult> parseString(std::string data){
  if(data[0]!= '"')
    return failExpectation('"',data[0]);
  size_t closing=0;
  while((closing = data.find('"',closing+1)) != std::string::npos && data[closing-1] == '\\');
  if(closing == std::string::npos)
    return failExpectation('"',"EOF");
  return ParseResult{JsonValue::string(data.substr(1,closing-1)),data.substr(closing+1)};
}

std::optional<ParseResult> parseNumber(std::string data){
  if(!std::isdigit(data[0]) && data[0] != '-')
    return failExpectation("digit or -",data[0]);
  size_t end = data.find_first_not_of("0123456789",1);
  if(end == std::string::npos)
    end = data.size();
  else if(data[end] == '.')
    end = data.find_first_not_of("0123456789",end+1);
  std::string remaining=end==std::string::npos?"":data.substr(end);
  return ParseResult{JsonValue::number(std::stod(data.substr(0,end))),remaining};
}

std::optional<ParseResult> parseBool(std::string data){
  if(data.find("true") == 0)
    return ParseResult{JsonValue::boolean(true),data.substr(4)};
  if(data.find("false") == 0)
    return ParseResult{JsonValue::boolean(false),data.substr(5)};
  return failExpectation("true or false",data.substr(0,5));
}

std::optional<ParseResult> parseNull(std::string data){
  if(data.find("null") == 0)
    return ParseResult{JsonValue::null(),data.substr(4)};
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
  std::vector<JsonValue> values;
  data = data.substr(1);
  consumeWhitespace(data);
  while(data[0] != ']'){
    consumeWhitespace(data);
    auto value = parseValue(data);
    if(!value)
      return failExpectation("Value",data);
    values.push_back(value->parsed);
    data = value->remaining;
    consumeWhitespace(data);
    if(data[0] != ','){
      if(data[0] != ']')
        return failExpectation(",",data[0]);
    }
    else
      data = data.substr(1);
  }
  return ParseResult{JsonValue::array(values),data.substr(1)};
}

std::optional<ParseResult> parseObject(std::string data){
  if(data[0] != '{')
  FAIL()
  std::map<std::string, JsonValue> values;
  data = data.substr(1);
  consumeWhitespace(data);
  while(data[0] != '}'){
    consumeWhitespace(data);
    auto key = parseString(data);
    if(!key)
      return failExpectation("string Key",data);
    data = key->remaining;
    consumeWhitespace(data);
    if(data[0] != ':')
      return failExpectation(":",data[0]);
    data = data.substr(1);
    consumeWhitespace(data);
    auto value = parseValue(data);
    if(!value)
      return failExpectation("Value",data);
    values[std::get<std::string>(key->parsed.value)] = value->parsed;
    data = value->remaining;
    consumeWhitespace(data);
    if(data[0] != ','){
      if(data[0] != '}')
        return failExpectation(",",data[0]);
    }
    else
      data = data.substr(1);
  }
  return ParseResult{JsonValue::object(values),data.substr(1)};
}

std::optional<JsonValue> JSON::Parse(std::string data){
  if(data.empty())
    return failExpectation("Non empty string",data)->parsed;
  auto result = parseValue(data);
  if(result->remaining.size() != 0)
    return failExpectation("EOF",result->remaining)->parsed;
  if(!result)
    return std::nullopt;
  return result->parsed;
}
