#ifndef JSON_H
#define JSON_H

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
typedef enum JSONType
{
    JSON_NULL,
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_ARRAY,
    JSON_OBJECT
} JSONType;



class JSONValue;
using JsonValueInternal = std::variant<double,bool,std::string,std::vector<JSONValue>,std::map<std::string,JSONValue>>;
class JSONValue{
public:
  JSONValue(){
    type=JSON_NULL;
  }
  JSONType type;
  JsonValueInternal value;
  static JSONValue string(std::string value);
  static JSONValue number(double value);
  static JSONValue boolean(bool value);
  static JSONValue array(std::vector<JSONValue> value);
  static JSONValue object(std::map<std::string,JSONValue> value);
  static JSONValue null();
  // stream output operator for json value type

  friend std::ostream& operator<< (std::ostream& stream, const JSONValue& matrix);
};

typedef struct {
  JSONValue parsed;
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
    static std::optional<JSONValue> Parse(std::string data);
};

#endif // JSON_H
