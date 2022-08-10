//
// Created by Дима Шестаков on 08.08.2022.
//

#include "myloxx/Scanner.h"

namespace myloxx {

	 Scanner::Scanner(std::string source) : source_(source) {
		start_ = source_.begin();
		current_ = source_.begin();
		line_ = 1;
	}

	[[nodiscard]] Token Scanner::ScanToken()
	{
		SkipWhitespace();
		start_ = current_;

		if(IsAtEnd()) return MakeToken(TokenType::TOKEN_EOF);

		char c = Advance();
		if(IsAlpha(c)) return Identifier();
		if(IsDigit(c)) return Number();

		switch(c) {
		case '(': return MakeToken(TokenType::LEFT_PAREN);
		case ')': return MakeToken(TokenType::RIGHT_PAREN);
		case '{': return MakeToken(TokenType::LEFT_BRACE);
		case '}': return MakeToken(TokenType::RIGHT_BRACE);
		case ';': return MakeToken(TokenType::SEMICOLON);
		case ',': return MakeToken(TokenType::COMMA);
		case '.': return MakeToken(TokenType::DOT);
		case '-': return MakeToken(TokenType::MINUS);
		case '+': return MakeToken(TokenType::PLUS);
		case '/': return MakeToken(TokenType::SLASH);
		case '*': return MakeToken(TokenType::STAR);
		case '!' :
			return MakeToken(Match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
		case '=':
			return MakeToken(Match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
		case '<':
			return MakeToken(Match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
		case '>':
			return MakeToken(Match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
		case '"':
			return String();
		}

		return ErrorToken("Unexpected character");
	}

	[[nodiscard]] bool Scanner::IsAtEnd() const noexcept
	{
		return current_ == source_.end();
	}

	[[nodiscard]] Token Scanner::MakeToken(TokenType type) const noexcept
	{
		Token token;
		token.type_ = type;
		token.start_ = start_;
		token.length_ = std::distance(start_, current_);
		token.line_ = line_;
		return token;
	}

	[[nodiscard]] Token Scanner::ErrorToken(std::string message) const noexcept
	{
		Token token;
		token.type_ = TokenType::ERROR;
		token.source_ = message;
		token.start_ = token.source_.begin();
		token.length_ = token.source_.size();
		token.line_ = line_;
		return token;
	}

	char Scanner::Advance()
	{
		auto ret = *current_;
		current_++;
		return ret;
	}

	[[nodiscard]] bool Scanner::Match(char expected)
	{
		if(IsAtEnd()) return false;

		if(*current_ != expected) return false;
		current_++;
		return true;
	}

	void Scanner::SkipWhitespace()
	{
		while(true) {
			char c = Peek();
			switch(c) {
			case ' ':
			case '\r':
			case '\t':
				Advance();
				break;
			case '\n':
				line_++;
				Advance();
				break;
			case '/':
				if(PeekNext() == '/')
					while(Peek() != '\n' && !IsAtEnd()) Advance();
			default:
				return;
			}
		}
	}

	[[nodiscard]] char Scanner::Peek() const
	{
		return *current_;
	}

	[[nodiscard]] char Scanner::PeekNext() const{
		if(IsAtEnd()) return '\0';
		return *(current_+1);
	}

	[[nodiscard]] Token Scanner::String()
	{
		while(Peek() != '"' && !IsAtEnd()) {
			if(Peek() == '\n') line_++;
			Advance();
		}

		if(IsAtEnd()) return ErrorToken("Unterminated string");

		Advance();
		return MakeToken(TokenType::STRING);
	}

	[[nodiscard]] bool Scanner::IsDigit(char c)
	{
		return c >= '0' && c <= '9';
	}

	[[nodiscard]] Token Scanner::Number()
	{
		while(IsDigit(Peek())) Advance();

		if(Peek() == '.' && IsDigit(PeekNext())) {
			Advance();
			while(IsDigit(Peek())) Advance();
		}

		return MakeToken(TokenType::NUMBER);
	}

	[[nodiscard]] bool Scanner::IsAlpha(char c)
	{
		return (c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			c == '_';
	}

	[[nodiscard]] Token Scanner::Identifier()
	{
		while(IsAlpha(Peek()) || IsDigit(Peek())) Advance();
		return MakeToken(IdentifierType());
	}

	[[nodiscard]] TokenType Scanner::IdentifierType()
	{
		switch(*start_) {
		case 'a': return CheckKeyword(1, 2, "nd", TokenType::AND);
		case 'c': return CheckKeyword(1, 4, "lass", TokenType::CLASS);
		case 'e': return CheckKeyword(1, 3, "lse", TokenType::ELSE);
		case 'f':
			if(current_ - start_ > 1) {
				switch(*(start_ + 1)) {
				case 'a' : return CheckKeyword(2, 3, "lse", TokenType::FALSE);
				case 'o': return CheckKeyword(2, 1, "r", TokenType::FOR);
				case 'u': return CheckKeyword(2, 1, "n", TokenType::FUN);
				}
			}
			break;
		case 'i': return CheckKeyword(1, 1, "f", TokenType::IF);
		case 'n': return CheckKeyword(1, 2, "il", TokenType::NIL);
		case 'o': return CheckKeyword(1, 1, "r", TokenType::OR);
		case 'p': return CheckKeyword(1, 4, "rint", TokenType::PRINT);
		case 'r': return CheckKeyword(1, 5, "eturn", TokenType::RETURN);
		case 's': return CheckKeyword(1, 4, "uper", TokenType::SUPER);
		case 't':
			if(current_ - start_ > 1) {
				switch(*(start_ + 1)) {
				case 'h': return CheckKeyword(2, 2, "is", TokenType::THIS);
				case 'r': return CheckKeyword(2, 2, "ue", TokenType::TRUE);
				}
			}
			break;
		case 'v': return CheckKeyword(1, 2, "ar", TokenType::VAR);
		case 'w': return CheckKeyword(1, 4, "hile", TokenType::WHILE);
		}
		return TokenType::IDENTIFIER;
	}

	[[nodiscard]] TokenType Scanner::CheckKeyword(size_t begin, size_t length, std::string rest, TokenType type) const
	{
		if((size_t)std::distance(start_, current_) == begin + length
			&& source_.substr((start_ - source_.begin()) + begin, length) == rest)
			return type;
		return TokenType::IDENTIFIER;
	}

	std::string Token::NameOfType() noexcept
	{
		switch(type_) {

		case TokenType::LEFT_PAREN:
			return "LEFT_PAREN";
		case TokenType::RIGHT_PAREN:
			return "RIGHT_PAREN";
		case TokenType::LEFT_BRACE:
			return "LEFT_BRACE";
		case TokenType::RIGHT_BRACE:
			return "RIGHT_BRACE";
		case TokenType::COMMA:
			return "COMMA";
		case TokenType::DOT:
			return "DOT";
		case TokenType::MINUS:
			return "MINUS";
		case TokenType::PLUS:
			return "PLUS";
		case TokenType::SEMICOLON:
			return "SEMICOLON";
		case TokenType::SLASH:
			return "SLASH";
		case TokenType::STAR:
			return "STAR";
		case TokenType::BANG:
			return "BANG";
		case TokenType::BANG_EQUAL:
			return "BANG_EQUAL";
		case TokenType::EQUAL:
			return "EQUAL";
		case TokenType::EQUAL_EQUAL:
			return "EQUAL_EQUAL";
		case TokenType::GREATER:
			return "GREATER";
		case TokenType::GREATER_EQUAL:
			return "GREATER_EQUAL";
		case TokenType::LESS:
			return "LESS";
		case TokenType::LESS_EQUAL:
			return "LESS_EQUAL";
		case TokenType::IDENTIFIER:
			return "IDENTIFIER";
		case TokenType::STRING:
			return "STRING";
		case TokenType::NUMBER:
			return "NUMBER";
		case TokenType::AND:
			return "AND";
		case TokenType::CLASS:
			return "CLASS";
		case TokenType::ELSE:
			return "ELSE";
		case TokenType::FALSE:
			return "FALSE";
		case TokenType::FOR:
			return "FOR";
		case TokenType::FUN:
			return "FUN";
		case TokenType::IF:
			return "IF";
		case TokenType::NIL:
			return "NIL";
		case TokenType::OR:
			return "OR";
		case TokenType::PRINT:
			return "PRINT";
		case TokenType::RETURN:
			return "RETURN";
		case TokenType::SUPER:
			return "SUPER";
		case TokenType::THIS:
			return "THIS";
		case TokenType::TRUE:
			return "TRUE";
		case TokenType::VAR:
			return "VAR";
		case TokenType::WHILE:
			return "WHILE";
		case TokenType::ERROR:
			return "ERROR";
		case TokenType::TOKEN_EOF:
			return "EOF";
		}
	}
}