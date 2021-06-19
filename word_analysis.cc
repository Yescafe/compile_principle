#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cctype>
#include <iterator>
#include <cstdlib>
#include <utility>
#include <set>

const std::set<std::string> keywords {
  "if", "else", "for", "while", "do",
  "int", "write", "read",
};

enum class Id {
  Empty = 0,
  Identifier = 11,
  Number = 12,
  Delimiter = 13,
  Comment = 14,
  Keyword = 21,
  Error = 100,
};

std::vector<std::pair<std::string, ::Id>> splitLine(std::string);

int main()
{
  std::vector<std::pair<std::string, ::Id>> slices;

  std::string line;
  while (std::getline(std::cin, line)) {
    slices = splitLine(line);

    for (auto&& [token, type] : slices) {
      std::cout << token << " ";
      if (type == Id::Identifier) {
        std::cout << "identifier";
      } else if (type == Id::Number) {
        std::cout << "number";
      } else if (type == Id::Delimiter) {
        std::cout << "delimiter";
      } else if (type == Id::Keyword) {
        std::cout << "keyword";
      }
      std::cout << std::endl;
    }
  }
  return 0;
}

std::vector<std::pair<std::string, ::Id>> splitLine(std::string line)
{
  std::stringstream ss;
  std::string unit;
  std::vector<std::pair<std::string, ::Id>> r;

  for (int i = 0; i < line.length(); ) {
    auto& c = line[i];
    if (std::isalpha(c)) {
      ss << c;
      for (++i; i < line.length(); ++i) {
        if (isalnum(line[i]))
          ss << line[i];
        else
          break;
      }
      ss >> unit;
      ss.str(std::string{});
      ss.clear();
      if (keywords.find(unit) != keywords.end())
        r.push_back({unit, Id::Keyword});
      else
        r.push_back({unit, Id::Identifier});
    } else if (std::isdigit(c)) {
      ss << c;
      for (++i; i < line.length(); ++i) {
        if (isdigit(line[i]))
          ss << line[i];
        else
          break;
      }
      ss >> unit;
      ss.str(std::string{});
      ss.clear();
      r.push_back({unit, Id::Number});
    } else if (c == '+' || c == '-' || c == '*' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == ':' || c == ',') {
      ss << c;
      ss >> unit;
      ss.str(std::string{});
      ss.clear();
      r.push_back({unit, Id::Delimiter});
      ++i;
    } else if (c == '<' || c == '>' || c == '=' || c == '!') {
      ss << c;
      if (i + 1 < line.length() && line[i + 1] == '=') {    // for greater or equal, less or equal, equal, not equal
        ++i;
        ss << line[i];
      }
      ss >> unit;
      ss.str(std::string{});
      ss.clear();
      r.push_back({unit, Id::Delimiter});
      ++i;
    } else if (c == '/') {       // comment
      ss << c;
      if (i + 1 < line.length() && line[i + 1] == '*') {
        ++i;
        for (++i; i < line.length(); ++i)
          if (line[i - 1] == '*' && line[i] == '/')
            break;
        i += 2;
      } else {
        ss >> unit;
        ss.str(std::string{});
        ss.clear();
        r.push_back({unit, Id::Delimiter});
        ++i;
      }
    } else if (isspace(c)) {
      ++i;
    } else {
      // Error
      std::exit(-1);
    }
  }

  return r;
}
