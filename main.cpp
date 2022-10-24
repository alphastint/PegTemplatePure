//#include "widget.h"

//#include <QApplication>

// int
// main(int argc, char* argv[])
//{
//  QApplication a(argc, argv);
//  Widget w;
//  w.show();
//  return a.exec();
//}

#include "peg_parser/generator.h"
#include "qglobal.h"
#include <cstdlib>
#include <iostream>
#include <string>

int
main(int argc, char* argv[])
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);
  std::vector<std::pair<std::string, float>> exprs = {
    { "1", 1. },
    { "1.1", 1.1 },
    { "1.01", 1.01 },
    { "+1.001", 1.001 },
    { "-1.001", -1.001 },
    { "1+1", 2 },
    { "1-1", 0 },
    { "1+2+3", 6 },
    { "1-1+2", 2 },
    { "(1)", 1 },
    { "(1+1)", 2 },
    { "(2-1)", 1 },
    { "-(2-1)", -1 },
    { "-(1)", -1 },
    { "-(-1)", 1 },
    { "-(1)-(-1)", 0 },
    { "-(1+2)-(2+2)", -7 },
    { "-(1+2)+(2+2)", 1 },
    { "1*1", 1 },
    { "1*2", 2 },
    { "1*(1+1)", 2 },
    { "(1+1)*(1+1)", 4 },
    { "-1*1", -1 },
    { "-(1+1)*(1+1)", -4 },
    { "-1*(-1)", 1 },
    { "(-1)*(-1)", 1 },
    { "(1)+(2)", 3 },
    { "(1)*(2)*(3)", 6 },
    { "(1+1)*(-1)", -2 },
    { "(1+2)*(1-2)*(2-1)-1*(-1)", -2 },
    { "1/2", 0.5 },
    { "1/(1+1)", 0.5 },
    { "(1-4*2)/4", -1.75 },
    { "((1))", 1 },
    { "(-(1))", -1 },
    { "(((-1)))", -1 },
    { "((-1)+1)", 0 },
    { "(7+1.44)*3-1.44*7+2*(3-1.4)", 18.44 },
    { "1(1+1)", 2 },
    { "(1+1)(1+1)", 4 },
    { "-1*1", -1 },
    { "-(1+1)(1+1)", -4 },
    { "-1(-1)", 1 },
    { "(-1)(-1)", 1 },
    { "(1)+(2)", 3 },
    { "(1)*(2)(3)", 6 },
    { "(1+1)(-1)", -2 },
    { "(1+2)*(1-2)(2-1)-1(-1)", -2 },
    { "(7+1.44)3-1.44*7+2(3-1.4)", 18.44 },
    { "1+(1+1)1.44*(7-1)", 18.28 }
  };

  peg_parser::ParserGenerator<float> g;

  //  g.setSeparator(g["Whitespace"] << "[\t \n]");

  g.setStart(g["Expression"]);

  for (const auto& expr : exprs) {
    std::cout << "_____________________________________________" << std::endl;
    try {
      std::cout << "expr = " << expr.first << std::endl;
      auto result = g.run(expr.first); // -> 5
      std::cout << expr.first << " = " << result << std::endl;
      if (std::fabs(result - expr.second) > 1.e-5) {
        std::cout << "expected: " << expr.second << std::endl;
        std::cout << "     got: " << result << std::endl;
        std::cout << "std::fabs(result - expr.second) = "
                  << std::fabs(result - expr.second) << std::endl;
      }
      Q_ASSERT(std::fabs(result - expr.second) < 1.e-5);
    } catch (peg_parser::SyntaxError& error) {
      auto syntax = error.syntax;
      std::cout << expr.first << std::endl;
      std::cout << std::string(syntax->begin, ' ');
      std::cout << std::string(syntax->length(), '~');
      std::cout << "^\n";
      std::cout << "  "
                << "Syntax error while parsing " << syntax->rule->name
                << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
