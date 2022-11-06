#include <iostream>
#include <fstream>
#include <thread>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "interpreter/interpreter.h"
#include "graph/engine.h"

Acalc::Interpreter interpreter;

void exec(const std::string& code) {
    Acalc::Lexer lexer(code);
    auto tokens = lexer.getTokens();

    Acalc::Parser parser(tokens);
    std::vector<std::unique_ptr<Acalc::Stmt>> stmts;

    std::unique_ptr<Acalc::Stmt> stmt = nullptr;

    try {
        while ((stmt = parser.parse()) != nullptr) {
            interpreter.interpret(stmt);
        }
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    } catch (const std::string& err) {
        std::cout << err << std::endl;
    } catch (const char* err) {
        std::cout << err << std::endl;
    }
}

void repl() {

    std::string line;

    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, line);
        exec(line);
    }
}

void read(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Couldn't read file " << filename << ".";
        return;
    }

    std::string code;
    std::string line;

    while (std::getline(file, line)) {
        code += line;
    }

    exec(code);
}

int main(int argc, char* argv[]) {

    std::thread thread1([argc, argv]() {
        switch (argc) {
            case 1: repl(); break;
            case 2: read(argv[1]); break;
            default: std::cerr << argv[0] << ": arguments passed were not understood";
        }

        Graph::Engine::exit();
    });

    std::thread thread2([]() {
        Graph::Engine::init();
        Graph::Engine::createWorld(800, 600, "Acalc");

        Graph::Engine::start();
    });

    thread1.join();
    thread2.join();

    return 0;
}
