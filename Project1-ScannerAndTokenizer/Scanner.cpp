#include <iostream>

#include "Scanner.h"
#include "Token.h"

using namespace std;

void Scanner::scanInput()
{
	while (in.peek() != -1)
	{
		char c = in.get();
		string w;
		
		// Increase line number
		if (c == '\n')
		{
			lineNum++;
			continue;
		}

		// Skip whitespace
		if (isspace(c)) continue;

		switch (c)
		{
		case ',':
			addToVector(Token(COMMA, ",", lineNum));
			break;
			
		case '.':
			addToVector(Token(PERIOD, ".", lineNum));
			break;

		case '?':
			addToVector(Token(Q_MARK, "?", lineNum));
			break;

		case '(':
			addToVector(Token(LEFT_PAREN, "(", lineNum));
			break;

		case ')':
			addToVector(Token(RIGHT_PAREN, ")", lineNum));
			break;

		case ':':
			if (in.peek() == '-')
			{
				c = in.get();
				addToVector(Token(COLON_DASH, ":-", lineNum));
			}
			else
			{
				addToVector(Token(COLON, ":", lineNum));
			}
			break;

		case '*':
			addToVector(Token(MULTIPLY, "*", lineNum));
			break;

		case '+':
			addToVector(Token(ADD, "+", lineNum));
			break;

		case '\'':
			addToVector(Token(STRING, scanStrings(), lineNum));
			break;

		case '#':
			// Go to comment scanner --- Maybe distinguish between line comment and block comment here
			addToVector(Token(COMMENT, "Default comment", lineNum));
			break;

		default:
			addToVector(Token(UNDEFINED, c, lineNum));
		}
	}
	addToVector(Token(END_OF_FILE, "", lineNum));
}

void Scanner::scanChars()
{

}

string Scanner::scanStrings()
{

	return "Default String";
}