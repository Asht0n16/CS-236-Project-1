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
			scanStrings(c);
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

void Scanner::scanStrings(char curr)
{
	string str;

	// Scan next char until EOF is reached
	curr = in.get();
	while (curr != -1)
	{
		// If the current char is a single quote
		if (curr == '\'')
		{
			// If there are two adjacent single quotes (''), insert apostrophe to string
			if (in.peek() == '\'')
			{
				curr = in.get();		// Change if the output wants us to keep both apostrophes
				str += curr;
				curr = in.get();
				continue;
			}
			else // End of string: add to vector and return to main scanner
			{
				addToVector(Token(STRING, str, lineNum));
				return;
			}
		}

		// Add current character to string and get next char
		str += curr;
		curr = in.get();
	}

	// If EOF is reached, put string in Undefined Token
	addToVector(Token(UNDEFINED, str, lineNum));
}