#include <iostream>

#include "Scanner.h"
#include "Token.h"

using namespace std;

void Scanner::scanInput()
{
	while (in.peek() != -1)
	{
		char c = in.get();
		
		// Increase line number if newline
		if (c == '\n')
		{
			lineNum++;
			continue;
		}

		// Skip all other whitespace
		if (isspace(c)) continue;

		// If the char is the beginning of a word token, switch to word scanner
		if (isalpha(c))
		{
			scanWords(c);
			continue;
		}

		switch (c)
		{
		case ',':
			addToDeque(Token(COMMA, ",", lineNum));
			break;
			
		case '.':
			addToDeque(Token(PERIOD, ".", lineNum));
			break;

		case '?':
			addToDeque(Token(Q_MARK, "?", lineNum));
			break;

		case '(':
			addToDeque(Token(LEFT_PAREN, "(", lineNum));
			break;

		case ')':
			addToDeque(Token(RIGHT_PAREN, ")", lineNum));
			break;

		case ':':
			if (in.peek() == '-')
			{
				c = in.get();
				addToDeque(Token(COLON_DASH, ":-", lineNum));
			}
			else
			{
				addToDeque(Token(COLON, ":", lineNum));
			}
			break;

		case '*':
			addToDeque(Token(MULTIPLY, "*", lineNum));
			break;

		case '+':
			addToDeque(Token(ADD, "+", lineNum));
			break;

		case '\'':
			scanStrings(c);
			break;

		case '#':
			// If there's a bar, it's a block comment
			if (in.peek() == '|')
			{
				scanBComment(c);
			}
			// If not, it's a line comment
			else
			{
				scanLComment(c);
			}
			break;

		default:
			addToDeque(Token(UNDEFINED, c, lineNum));
		}
	}
	addToDeque(Token(END_OF_FILE, "", lineNum));
}

void Scanner::scanWords(char c)
{
	string word;
	word += c;

	// Scan in entire word and check for invalid chars
	c = in.peek();
	while (isalnum(c))
	{
		c = in.get();
		word += c;
		c = in.peek();
	}

	// Check for Keywords
	if (word == "Schemes")
	{
		addToDeque(Token(SCHEMES, word, lineNum));
		return;
	}
	else if (word == "Facts")
	{
		addToDeque(Token(FACTS, word, lineNum));
		return;
	}
	else if (word == "Rules")
	{
		addToDeque(Token(RULES, word, lineNum));
		return;
	}
	else if (word == "Queries")
	{
		addToDeque(Token(QUERIES, word, lineNum));
		return;
	}

	addToDeque(Token(ID, word, lineNum));
}

void Scanner::scanStrings(char c)
{
	string str;
	str += c;
	int ogLine = lineNum;

	// Scan next char until EOF is reached
	c = in.get();
	while (c != -1)
	{
		// If the current char is a single quote
		if (c == '\'')
		{
			// If there are two adjacent single quotes (''), insert apostrophe to string
			if (in.peek() == '\'')
			{
				str += c;		// Add both apostrophes
				c = in.get();
				str += c;
				c = in.get();
				continue;
			}
			else // End of string: add to vector and return to main scanner
			{
				str += c;
				addToDeque(Token(STRING, str, ogLine));
				return;
			}
		}
		else if (c == '\n')
		{
			lineNum++;
			str += c;
			c = in.get();
			continue;
		}

		// Add current character to string and get next char
		str += c;
		c = in.get();
	}

	// If EOF is reached, put string in Undefined Token
	addToDeque(Token(UNDEFINED, str, ogLine));
}

void Scanner::scanBComment(char c)
{
	string comm;
	comm += c;

	int ogLine = lineNum;
	// Scan next char until EOF is found
	c = in.get();
	while (c != -1)
	{
		// If the end of the comment is found, add comment to Token
		if (c == '|' && in.peek() == '#')
		{
			comm += c;
			comm += in.get();
			// addToVector(Token(COMMENT, comm, ogLine));	Removed with addition of Parser
			return;
		}
		// If a newline is found, increase linenum
		else if (c == '\n')
		{
			lineNum++;
			//comm += ' ';
			comm += c;	// Swap with line above if comment token has newline when stored
		}
		else
		{
			comm += c;
		}
		c = in.get();
	}

	// If EOF is reached, put comment in Undefined Token
	addToDeque(Token(UNDEFINED, comm, ogLine));
}

void Scanner::scanLComment(char c)
{
	string comm;
	comm += c;

	// Scan next char until EOF or newline is reached
	c = in.get();
	while (c != -1 && c != '\n')
	{
		comm += c;
		c = in.get();
	}

	// addToVector(Token(COMMENT, comm, lineNum));		Removed with addition of Parser

	if (c == '\n') lineNum++;
}