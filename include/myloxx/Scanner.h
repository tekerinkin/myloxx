#pragma once
#include <string_view>
#include <string>

namespace myloxx {
	enum class TokenType : uint8_t {
		// Single-character tokens.
		LEFT_PAREN, RIGHT_PAREN,
		LEFT_BRACE, RIGHT_BRACE,
		COMMA, DOT, MINUS, PLUS,
		SEMICOLON, SLASH, STAR,
		// One or two character tokens.
		BANG, BANG_EQUAL,
		EQUAL, EQUAL_EQUAL,
		GREATER, GREATER_EQUAL,
		LESS, LESS_EQUAL,
		// Literals.
		IDENTIFIER, STRING, NUMBER,
		// Keywords.
		AND, CLASS, ELSE, FALSE,
		FOR, FUN, IF, NIL, OR,
		PRINT, RETURN, SUPER, THIS,
		TRUE, VAR, WHILE,
		
		ERROR, TOKEN_EOF
	};

	struct Token {
		TokenType type_;
		std::string source_;
		std::string::iterator start_;
		size_t length_;
		int line_ = 1;

		Token(TokenType type, std::string source, size_t line) noexcept
					:type_(type), source_(source), start_(source_.begin()),
						length_(source_.size()), line_(line)
		{}

		Token() noexcept : type_(TokenType::TOKEN_EOF) {};

		std::string NameOfType() noexcept;

	};

	class Scanner
	{
	public:
		explicit Scanner(std::string source);

		[[nodiscard]] Token ScanToken();

	private:
		std::string::iterator start_;
		std::string::iterator current_;
		size_t line_;
		std::string source_;

		[[nodiscard]] Token ErrorToken(std::string message) const noexcept;
		[[nodiscard]] Token MakeToken(TokenType type) const noexcept;

		char Advance();

		[[nodiscard]] bool IsAtEnd() const noexcept;
		[[nodiscard]] bool Match(char expected);
		[[nodiscard]] char Peek() const;
		[[nodiscard]] char PeekNext() const;
		[[nodiscard]] Token String();
		[[nodiscard]] Token Number();
		[[nodiscard]] bool IsDigit(char c);
		[[nodiscard]] bool IsAlpha(char c);
		[[nodiscard]] Token Identifier();
		[[nodiscard]] TokenType IdentifierType();
		[[nodiscard]] TokenType CheckKeyword(size_t begin, size_t length, std::string rest, TokenType type) const;
		void SkipWhitespace();

	};
}


