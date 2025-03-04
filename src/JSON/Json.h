#ifndef JSON_H
#define JSON_H

#include <optional>

#include "JsonValue.h"

typedef struct {
  JsonValue parsed;
  std::string remaining;
} ParseResult;

class JSON{
private:
  std::optional<ParseResult> parseCharacterSequence(std::string lookFor,std::string data);
  std::optional<ParseResult> parseWhitespace(std::string data);
  std::optional<ParseResult> parseString(std::string data);
  std::optional<ParseResult> parseNumber(std::string data);
  std::optional<ParseResult> parseBool(std::string data);
  std::optional<ParseResult> parseArray(std::string data);
  std::optional<ParseResult> parseObject(std::string data);
public:
    static std::optional<JsonValue> Parse(std::string data);
};

#endif // JSON_H
