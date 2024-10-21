#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>
#include <sstream>

using namespace std;

enum TokenType {
    T_INT, T_FLOAT, T_DOUBLE, T_STRING, T_BOOL, T_CHAR, T_ID, T_NUM, T_IF, T_ELSE, T_RETURN, T_WHILE, T_FOR, T_ASSIGN, T_PLUS, T_MINUS, T_MUL, T_DIV, 
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE, T_SEMICOLON, T_GT, T_LT, T_EQ, T_NEQ, T_AND, T_OR, T_AAGAR, T_EOF, T_COMMENT,T_INCREMENT
};

struct Token {
    TokenType type;
    string value;
    int line;
    Token(TokenType t, const string& v, int l) : type(t), value(v), line(l) {}
};

string tokenTypeToString(TokenType type) {
    switch (type) {
        case T_INT: return "T_INT";
        case T_FLOAT: return "T_FLOAT";
        case T_DOUBLE: return "T_DOUBLE";
        case T_STRING: return "T_STRING";
        case T_BOOL: return "T_BOOL";
        case T_CHAR: return "T_CHAR";
        case T_ID: return "T_ID";
        case T_NUM: return "T_NUM";
        case T_IF: return "T_IF";
        case T_ELSE: return "T_ELSE";
        case T_RETURN: return "T_RETURN";
        case T_WHILE: return "T_WHILE";
        case T_FOR: return "T_FOR";
        case T_ASSIGN: return "T_ASSIGN";
        case T_PLUS: return "T_PLUS";
        case T_MINUS: return "T_MINUS";
        case T_MUL: return "T_MUL";
        case T_DIV: return "T_DIV";
        case T_LPAREN: return "T_LPAREN";
        case T_RPAREN: return "T_RPAREN";
        case T_LBRACE: return "T_LBRACE";
        case T_RBRACE: return "T_RBRACE";
        case T_SEMICOLON: return "T_SEMICOLON";
        case T_GT: return "T_GT";
        case T_LT: return "T_LT";
        case T_EQ: return "T_EQ";
        case T_NEQ: return "T_NEQ";
        case T_AND: return "T_AND";
        case T_OR: return "T_OR";
        case T_AAGAR: return "T_AAGAR";  //custom keyword
        case T_EOF: return "T_EOF";
        case T_COMMENT: return "T_COMMENT";
        case T_INCREMENT: return "T_INCREMENT";
        default: return "UNKNOWN";
    }
}

class Lexer {
private:
    string src;
    size_t pos;
    int line; //error logging with line number
public:
    Lexer(const string &src) {
        this->src = src;
        this->pos = 0;
        this->line = 1;
    }

    string consumeNumber() {
        size_t start = pos;
        bool hasDecimal = false;
        while (pos < src.size() && (isdigit(src[pos]) || src[pos] == '.')) {
            if (src[pos] == '.') {
                if (hasDecimal) {
                    cout << "Syntax error: unexpected '.' in number at position " << pos << endl;
                    exit(1);
                }
                hasDecimal = true;
            }
            pos++;
        }
        return src.substr(start, pos - start);
    }


    string consumeWord() {
        size_t start = pos;
        while (pos < src.size() && isalnum(src[pos])) pos++;
        return src.substr(start, pos - start);
    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        while (pos < src.size()) {
            char current = src[pos];
            if (current == '\n') line++;
            if (isspace(current)) {
                pos++;
                continue;
            }
            //handle comments
            if (current == '/' && src[pos + 1] == '/') {
                while (pos < src.size() && src[pos] != '\n') pos++;
                line++; pos++;
                continue;
            }
            if (isdigit(current)) {
                tokens.push_back(Token(T_NUM, consumeNumber(), line));
                continue;
            }

            if (isalpha(current)) {
                string word = consumeWord();
                if (word == "int") tokens.push_back(Token(T_INT, word, line));
                else if (word == "float") tokens.push_back(Token(T_FLOAT, word, line));
                else if (word == "double") tokens.push_back(Token(T_DOUBLE, word, line));
                else if (word == "string") tokens.push_back(Token(T_STRING, word, line));
                else if (word == "bool") tokens.push_back(Token(T_BOOL, word, line));
                else if (word == "char") tokens.push_back(Token(T_CHAR, word, line));
                else if (word == "agar") tokens.push_back(Token(T_AAGAR, word, line));
                else if (word == "else") tokens.push_back(Token(T_ELSE, word, line));
                else if (word == "return") tokens.push_back(Token(T_RETURN, word, line));
                else if (word == "while") tokens.push_back(Token(T_WHILE, word, line));
                else if (word == "for") tokens.push_back(Token(T_FOR, word, line));
                else tokens.push_back(Token(T_ID, word, line));
                continue;
            }

            switch (current) {
                
                case '-': tokens.push_back(Token(T_MINUS, "-", line)); break;
                case '*': tokens.push_back(Token(T_MUL, "*", line)); break;
                case '/': tokens.push_back(Token(T_DIV, "/", line)); break;
                case '(': tokens.push_back(Token(T_LPAREN, "(", line)); break;
                case ')': tokens.push_back(Token(T_RPAREN, ")", line)); break;
                case '{': tokens.push_back(Token(T_LBRACE, "{", line)); break;
                case '}': tokens.push_back(Token(T_RBRACE, "}", line)); break;
                case ';': tokens.push_back(Token(T_SEMICOLON, ";", line)); break;
                case '>': tokens.push_back(Token(T_GT, ">", line)); break;
                case '<': tokens.push_back(Token(T_LT, "<", line)); break;
                case '+': 
                    if (src[pos + 1] == '+') {
                        tokens.push_back(Token(T_INCREMENT, "++", line));
                        pos++;
                    }
                    else{
                        tokens.push_back(Token(T_PLUS, "+", line)); break;
                    }
                    break;
                case '=': 
                    if (src[pos + 1] == '=') {
                        tokens.push_back(Token(T_EQ, "==", line));
                        pos++;
                    }
                    else{
                        tokens.push_back(Token(T_ASSIGN, "=", line)); break;
                    }
                    break;
                case '!':
                    if (src[pos + 1] == '=') {
                        tokens.push_back(Token(T_NEQ, "!=", line));
                        pos++;
                    } else {
                        cout << "Unexpected character: " << current << " at line " << line << endl; exit(1);
                    }
                    break;
                case '&':
                    if (src[pos + 1] == '&') {
                        tokens.push_back(Token(T_AND, "&&", line));
                        pos++;
                    } else {
                        cout << "Unexpected character: " << current << " at line " << line << endl; exit(1);
                    }
                    break;
                case '|':
                    if (src[pos + 1] == '|') {
                        tokens.push_back(Token(T_OR, "||", line));
                        pos++;
                    } else {
                        cout << "Unexpected character: " << current << " at line " << line << endl; exit(1);
                    }
                    break;
                default: 
                    cout << "Unexpected character: " << current << " at line " << line << endl; 
                    exit(1);
            }
            pos++;
        }
        tokens.push_back(Token(T_EOF, "", line));
        return tokens;
    }
};

class SymbolTable {
public:
    unordered_map<string, string> table;

    void insert(const string& id, const string& type) {
        table[id] = type;
    }

    bool exists(const string& id) {
        return table.find(id) != table.end();
    }

    string getType(const string& id) {
        return table[id];
    }
};

class Parser {
public:
    Parser(const vector<Token> &tokens) {
        this->tokens = tokens;
        this->pos = 0;
        this->symbolTable = SymbolTable();
    }

    void parseProgram() {
        while (tokens[pos].type != T_EOF) {
            parseStatement();
        }
    }

private:
    vector<Token> tokens;
    size_t pos;
    SymbolTable symbolTable;

    Token consume(TokenType type) {
        if (tokens[pos].type == type) {
            return tokens[pos++];
        } else {
            cout << "Syntax error: expected " << tokenTypeToString(type) << " but got " 
                 << tokenTypeToString(tokens[pos].type) << " at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseStatement() {
        if (tokens[pos].type == T_INT || tokens[pos].type == T_FLOAT || tokens[pos].type == T_DOUBLE ||
            tokens[pos].type == T_STRING || tokens[pos].type == T_BOOL || tokens[pos].type == T_CHAR) {
            parseDeclaration();
        } else if (tokens[pos].type == T_AAGAR) {
            parseIfStatement();
        } else if (tokens[pos].type == T_WHILE) {
            parseWhileLoop();
        } else if (tokens[pos].type == T_FOR) {
            parseForLoop();
        } else {
            cout << "Unexpected statement at line " << tokens[pos].line << endl;
            exit(1);
        }
    }

    void parseDeclaration() {
        Token typeToken = tokens[pos++];
        Token idToken = consume(T_ID);

        if (symbolTable.exists(idToken.value)) {
            cout << "Error: Variable '" << idToken.value << "' already declared at line " << idToken.line << endl;
            exit(1);
        }

        symbolTable.insert(idToken.value, tokenTypeToString(typeToken.type));
        consume(T_ASSIGN);
        parseExpression();
        consume(T_SEMICOLON);
    }

void parseExpression() {
    Token left = tokens[pos++];

    if (left.type != T_NUM && left.type != T_ID) {
        cout << "Expected a number or identifier at line " << left.line << endl;
        exit(1);
    }

    while (tokens[pos].type == T_EQ || tokens[pos].type == T_NEQ ||
           tokens[pos].type == T_LT || tokens[pos].type == T_GT ||
           tokens[pos].type == T_AND || tokens[pos].type == T_OR) {
        
        Token op = tokens[pos++];
        Token right = tokens[pos++];
        
        if (right.type != T_NUM && right.type != T_ID) {
            cout << "Expected a number or identifier after operator at line " << right.line << endl;
            exit(1);
        }
    }
}

    void parseIfStatement() {
        consume(T_AAGAR); 
        consume(T_LPAREN);
        parseExpression();
        consume(T_RPAREN);
        consume(T_LBRACE);
        parseStatement();
        consume(T_RBRACE);
    }

    void parseWhileLoop() {
        consume(T_WHILE);
        consume(T_LPAREN);
        parseExpression();
        consume(T_RPAREN);
        consume(T_LBRACE);
        parseStatement();
        consume(T_RBRACE);
    }

    void parseForLoop() {
    consume(T_FOR);
    consume(T_LPAREN);

    //initialization
    parseDeclaration();
    //condition
    parseExpression();
    consume(T_SEMICOLON);
    //increment
    parseIncrement();
    consume(T_RPAREN);
    consume(T_LBRACE);
    parseStatement();
    consume(T_RBRACE);
}

void parseIncrement() {
    Token incrementToken = tokens[pos++]; 
    if (incrementToken.type == T_ID) { 
        Token nextToken = tokens[pos++];
        if (nextToken.type == T_PLUS || nextToken.type == T_INCREMENT || nextToken.type == T_EQ) {
            if (nextToken.type == T_PLUS) {
                Token incrementValue = tokens[pos++];
                if (incrementValue.type != T_NUM) {
                    cout << "Expected number after increment operator at line " << incrementValue.line << endl;
                    exit(1);
                }
            }
            else if (nextToken.type == T_INCREMENT) {

            }
            else if (nextToken.type == T_EQ) {
                Token nextAfterEq = tokens[pos++];
                if (nextAfterEq.type != T_NUM) {
                    cout << "Expected number after '+=' operator at line " << nextAfterEq.line << endl;
                    exit(1);
                }
            }
        } else {
            cout << "Expected increment operator at line " << incrementToken.line << endl;
            exit(1);
        }
    } else {
        cout << "Expected identifier for increment at line " << incrementToken.line << endl;
        exit(1);
    }
}

};

void runCompiler(const string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << fileName << endl;
        return;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string sourceCode = buffer.str();
    
    Lexer lexer(sourceCode);
    vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    parser.parseProgram();

    cout << "Compilation successful!" << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: compiler <source_file>" << endl;
        return 1;
    }

    string fileName = argv[1];
    runCompiler(fileName);

    return 0;
}
