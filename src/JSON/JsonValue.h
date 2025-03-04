#ifndef  JSONVALUE_H
#define JSONVALUE_H
#include <map>
#include <string>
#include <variant>
#include <vector>
#include <iostream>

typedef enum JsonType
{
    JSON_NULL,
    JSON_STRING,
    JSON_NUMBER,
    JSON_BOOL,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;



class JsonValue;
using JsonValueUnion = std::variant<double,bool,std::string,std::vector<JsonValue>,std::map<std::string,JsonValue>>;
class JsonValue{
public:
  JsonType type;
  JsonValueUnion value;
  static JsonValue string(std::string value);
  static JsonValue number(double value);
  static JsonValue boolean(bool value);
  static JsonValue array(std::vector<JsonValue> value);
  static JsonValue object(std::map<std::string,JsonValue> value);
  static JsonValue null();

  friend std::ostream& operator<< (std::ostream& stream, const JsonValue& matrix);
};
#endif // JSONVALUE_H
