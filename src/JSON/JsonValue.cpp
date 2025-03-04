#include "JsonValue.h"

JsonValue JsonValue::null(){
  JsonValue nullValue;
  nullValue.type = JsonType::JSON_NULL;
  return nullValue;
}

JsonValue JsonValue::string(std::string value){
  JsonValue stringValue;
  stringValue.type = JsonType::JSON_STRING;
  stringValue.value = value;
  return stringValue;
}

JsonValue JsonValue::number(double value){
  JsonValue numberValue;
  numberValue.type = JsonType::JSON_NUMBER;
  numberValue.value = value;
  return numberValue;
}

JsonValue JsonValue::boolean(bool value){
  JsonValue booleanValue;
  booleanValue.type = JsonType::JSON_BOOL;
  booleanValue.value = value;
  return booleanValue;
}

JsonValue JsonValue::array(std::vector<JsonValue> value){
  JsonValue arrayValue;
  arrayValue.type = JsonType::JSON_ARRAY;
  arrayValue.value = value;
  return arrayValue;
}

JsonValue JsonValue::object(std::map<std::string, JsonValue> value){
  JsonValue objectValue;
  objectValue.type = JsonType::JSON_OBJECT;
  objectValue.value = value;
  return objectValue;
}

std::ostream& operator<<(std::ostream& stream, const JsonValue& value){
  bool requireComma = false;
  switch(value.type){
    case JsonType::JSON_NULL:
      stream << "null";
      break;
    case JsonType::JSON_STRING:
      stream << '"' << std::get<std::string>(value.value) << '"';
      break;
    case JsonType::JSON_NUMBER:
      stream<<std::fixed<<std::get<double>(value.value);
      break;
    case JsonType::JSON_BOOL:
      stream << (std::get<bool>(value.value) ? "true" : "false");
      break;
    case JsonType::JSON_OBJECT:
      stream << "{\n";
      for(auto& [key, element] : std::get<std::map<std::string, JsonValue>>(value.value)){
        if(requireComma){
          stream << ",\n";
        }
        requireComma = true;
        stream << '"' << key << "\": " << element;
      }
      stream << "\n}";
      break;
    case JsonType::JSON_ARRAY:
      auto& array = std::get<std::vector<JsonValue>>(value.value);
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
