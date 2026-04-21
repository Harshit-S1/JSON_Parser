#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <memory>
#include <stdexcept>

using namespace std;

// Forward declaration for recursive data structures (Objects and Arrays)
struct JsonNode;

// Modern C++17 variant to safely hold any JSON data type
using JsonValue = variant<
    nullptr_t, 
    bool, 
    double, 
    string, 
    vector<shared_ptr<JsonNode>>,               // JSON Array
    map<string, shared_ptr<JsonNode>>           // JSON Object
>;

struct JsonNode {
    JsonValue value;
};

class JsonParser {
private:
    string input;
    size_t pos = 0;
    void skipWhitespace() {
        while (pos < input.length() && isspace(input[pos])) pos++;
    }

    string parseString() {
        string result = "";
        pos++; // Skip opening quote
        while (pos < input.length() && input[pos] != '"') {
            result += input[pos++];
        }
        pos++; // Skip closing quote
        return result;
    }

    double parseNumber() {
        size_t start = pos;
        while (pos < input.length() && (isdigit(input[pos]) || input[pos] == '.' || input[pos] == '-')) {
            pos++;
        }
        return stod(input.substr(start, pos - start));
    }

    shared_ptr<JsonNode> parseValue() {
        skipWhitespace();
        if (pos >= input.length()) throw runtime_error("Unexpected end of input");

        char current = input[pos];
        auto node = make_shared<JsonNode>();

        if (current == '"') {
            node->value = parseString();
        } else if (isdigit(current) || current == '-') {
            node->value = parseNumber();
        } else if (input.substr(pos, 4) == "true") {
            node->value = true; pos += 4;
        } else if (input.substr(pos, 5) == "false") {
            node->value = false; pos += 5;
        } else if (input.substr(pos, 4) == "null") {
            node->value = nullptr; pos += 4;
        } else if (current == '[') {
            node->value = parseArray();
        } else if (current == '{') {
            node->value = parseObject();
        } else {
            throw runtime_error("Invalid JSON syntax at position " + to_string(pos));
        }
        return node;
    }

    vector<shared_ptr<JsonNode>> parseArray() {
        vector<shared_ptr<JsonNode>> arr;
        pos++; // Skip '['
        skipWhitespace();
        if (input[pos] == ']') { pos++; return arr; } // Empty array

        while (true) {
            arr.push_back(parseValue());
            skipWhitespace();
            if (input[pos] == ']') { pos++; break; }
            if (input[pos] == ',') { pos++; } 
            else throw runtime_error("Expected ',' or ']' in array");
        }
        return arr;
    }

    map<string, shared_ptr<JsonNode>> parseObject() {
        map<string, shared_ptr<JsonNode>> obj;
        pos++; // Skip '{'
        skipWhitespace();
        if (input[pos] == '}') { pos++; return obj; } // Empty object

        while (true) {
            skipWhitespace();
            if (input[pos] != '"') throw runtime_error("Expected string key in object");
            string key = parseString();
            
            skipWhitespace();
            if (input[pos] != ':') throw runtime_error("Expected ':' after key");
            pos++; // Skip ':'
            
            obj[key] = parseValue();
            
            skipWhitespace();
            if (input[pos] == '}') { pos++; break; }
            if (input[pos] == ',') { pos++; } 
            else throw runtime_error("Expected ',' or '}' in object");
        }
        return obj;
    }

public:
    JsonParser(const string& json) : input(json) {}

    shared_ptr<JsonNode> parse() {
        return parseValue();
    }
};

int main() {
    // Test the parser with a complex nested JSON string
    string raw_json = R"({
        "name": "Harshit",
        "cgpa": 9.79,
        "is_student": true,
        "skills": ["C++", "Python", "Verilog"],
        "project": null
    })";

    try {
        cout << "Parsing JSON\n";
        JsonParser parser(raw_json);
        auto ast = parser.parse();
        cout << "Parsed into C++ Abstract Syntax Tree!\n";
        
        // In a full implementation, you would write a traverse/print function here.
        // For now, if it doesn't throw an error, the tree was built perfectly.
    } catch (const exception& e) {
        cerr << "Parsing Error: " << e.what() << "\n";
    }

    return 0;
}
