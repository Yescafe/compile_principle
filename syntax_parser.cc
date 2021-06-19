#include <iostream>
#include <string>
#include <cstdlib>

enum class Symbol {
  intw,              // int
  ident,             // 标识符
  semi,              // 分号
  ifw,               // if
  whilew,            // while
  forw,              // for
  readw,             // read
  writew,            // write
  lrp, rrp,          // 左右圆括号
  lcp, rcp,          // 左右花括号
  number,            // 数字
  times,             // 乘号
  slash,             // 斜杠
  add,               // 加号
  minus,             // 减号
  assign,            // 赋值（单等号）
  le, leq,           // 小于、小于等于
  gr, grq,           // 大于、大于等于
  eq, neq,           // 等于、不等于
  eof,               // 文件尾
  error = -1,        // 错误
};

Symbol sym;

void program();
void declaration_list();
void declaration_stat();
void statement_list();
void statement();
void if_stat();
void while_stat();
void for_stat();
void write_stat();
void read_stat();
void compound_stat();
void expression_stat();
void expression();
void bool_expr();
void additive_expr();
void term();
void factor();

bool accept(Symbol);
bool expect(Symbol);

void nextsym();
void error(const char* msg);
void info(const char* msg);

void program() {
  info("program");
  nextsym();
  declaration_list();
  statement_list();
}

void declaration_list() {
  info("declaration_list");
  if (sym == Symbol::intw) {
    declaration_stat();
    declaration_list();
  }
}

void declaration_stat() {
  info("declaration_stat");
  if (accept(Symbol::intw)) {
    expect(Symbol::ident);
    expect(Symbol::semi);
  }
}

void statement_list() {
  info("statement_list");
  if (sym == Symbol::ifw || sym == Symbol::whilew || sym == Symbol::forw ||
      sym == Symbol::readw || sym == Symbol::writew || sym == Symbol::lcp ||
      sym == Symbol::ident || sym == Symbol::semi) {
    statement();
    statement_list();
  }
}

void statement() {
  info("statement");
  if (sym == Symbol::ifw) {
    if_stat();
  } else if (sym == Symbol::whilew) {
    while_stat();
  } else if (sym == Symbol::forw) {
    for_stat();
  } else if (sym == Symbol::readw) {
    read_stat();
  } else if (sym == Symbol::writew) {
    write_stat();
  } else if (sym == Symbol::lcp) {
    compound_stat();
  } else if (sym != Symbol::eof) {
    expression();
  }
}

void if_stat() {
  info("if_stat");
  expect(Symbol::ifw);
  expect(Symbol::lrp);
  expression();
  expect(Symbol::rrp);
  if (sym == Symbol::lcp) {
    compound_stat();
  } else {
    statement();
  }
}

void while_stat() {
  info("while_stat");
  expect(Symbol::whilew);
  expect(Symbol::lrp);
  expression();
  expect(Symbol::rrp);
  if (sym == Symbol::lcp) {
    compound_stat();
  } else {
    statement();
  }
}

void for_stat() {
  info("for_stat");
  expect(Symbol::forw);
  expect(Symbol::lrp);
  expression();
  expect(Symbol::semi);
  expression();
  expect(Symbol::semi);
  expression();
  expect(Symbol::rrp);
  if (sym == Symbol::lcp) {
    compound_stat();
  } else {
    statement();
  }
}

void write_stat() {
  info("write_stat");
  expect(Symbol::writew);
  expression();
  expect(Symbol::semi);
}

void read_stat() {
  info("read_stat");
  expect(Symbol::readw);
  expect(Symbol::ident);
  expect(Symbol::semi);
}

void compound_stat() {
  info("compound_stat");
  expect(Symbol::lcp);
  statement_list();
  expect(Symbol::rcp);
}

void expression_stat() {
  info("expression_stat");
  if (sym == Symbol::ident) {
    expression();
  }
  expect(Symbol::semi);
}
void expression() {
  info("expression");
  if (sym == Symbol::lrp || sym == Symbol::number) {
    bool_expr();
  } else {
    expect(Symbol::ident);
    expect(Symbol::assign);
    additive_expr();
  }
}

void bool_expr() {
  info("bool_expr");
  additive_expr();
  if (sym == Symbol::le || sym == Symbol::gr || sym == Symbol::leq ||
      sym == Symbol::grq || sym == Symbol::eq || sym == Symbol::neq) {
    additive_expr();
  }
}

void additive_expr() {
  info("additive_expr");
  term();
  if (sym == Symbol::add || sym == Symbol::minus) {
    term();
  }
}

void term() {
  info("term");
  factor();
  if (sym == Symbol::times || sym == Symbol::slash) {
    factor();
  }
}

void factor() {
  info("factor");
  if (sym == Symbol::lrp) {
    expect(Symbol::lrp);
    expression();
    expect(Symbol::rrp);
  } else if (sym == Symbol::ident) {
    expect(Symbol::ident);
  } else {
    expect(Symbol::number);
  }
}

bool accept(Symbol s) {
  if (sym == s) {
    nextsym();
    return true;
  }
  return false;
}

bool expect(Symbol s) {
  if (accept(s))
    return true;
  error("expect: unexpected symbol");
  return false;
}

void nextsym()
{
  std::string token, type;
  if (std::cin >> token >> type) {
    std::cout << " " << token << " " << type << std::endl;
    if (token == "int") {
      sym = Symbol::intw;
    } else if (type == "identifier") {
      sym = Symbol::ident;
    } else if (token == ";") {
      sym = Symbol::semi;
    } else if (token == "if") {
      sym = Symbol::ifw;
    } else if (token == "while") {
      sym = Symbol::whilew;
    } else if (token == "for") {
      sym = Symbol::forw;
    } else if (token == "read") {
      sym = Symbol::readw;
    } else if (token == "write") {
      sym = Symbol::writew;
    } else if (token == "(") {
      sym = Symbol::lrp;
    } else if (token == ")") {
      sym = Symbol::rrp;
    } else if (token == "{") {
      sym = Symbol::lcp;
    } else if (token == "}") {
      sym = Symbol::rcp;
    } else if (type == "number") {
      sym = Symbol::number;
    } else if (token == "*") {
      sym = Symbol::times;
    } else if (token == "/") {
      sym = Symbol::slash;
    } else if (token == "+") {
      sym = Symbol::add;
    } else if (token == "-") {
      sym = Symbol::minus;
    } else if (token == "=") {
      sym = Symbol::assign;
    } else if (token == "<") {
      sym = Symbol::le;
    } else if (token == "<=") {
      sym = Symbol::leq;
    } else if (token == ">") {
      sym = Symbol::gr;
    } else if (token == ">=") {
      sym = Symbol::grq;
    } else if (token == "==") {
      sym = Symbol::eq;
    } else if (token == "!=") {
      sym = Symbol::neq;
    } else {
      sym = Symbol::error;
    }
  } else {
    sym = Symbol::eof;
  }
}

void error(const char* msg) {
  std::cerr << "Parse failed. " << msg << std::endl;
  std::exit(-1);
}

void info(const char* msg) {
  std::cout << "info: " << msg << std::endl;
}

int main() {
  program();
  std::cout << "Parse finish." << std::endl;
  return 0;
}