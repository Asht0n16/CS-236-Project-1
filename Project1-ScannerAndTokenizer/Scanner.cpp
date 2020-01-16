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
			addToVector(Token(UNDEFINED, c, lineNum));
		}
	}
	addToVector(Token(END_OF_FILE, "", lineNum));
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
		addToVector(Token(SCHEMES, word, lineNum));
		return;
	}
	else if (word == "Facts")
	{
		addToVector(Token(FACTS, word, lineNum));
		return;
	}
	else if (word == "Rules")
	{
		addToVector(Token(RULES, word, lineNum));
		return;
	}
	else if (word == "Queries")
	{
		addToVector(Token(QUERIES, word, lineNum));
		return;
	}

	addToVector(Token(ID, word, lineNum));
}

void Scanner::scanStrings(char c)
{
	string str;
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
				c = in.get();		// Change if the output wants us to keep both apostrophes
				str += c;
				c = in.get();
				continue;
			}
			else // End of string: add to vector and return to main scanner
			{
				addToVector(Token(STRING, str, lineNum));
				return;
			}
		}
		else if (c == '\n')
		{
			lineNum++;
			str += ' ';
			c = in.get();
			continue;
		}

		// Add current character to string and get next char
		str += c;
		c = in.get();
	}

	// If EOF is reached, put string in Undefined Token
	addToVector(Token(UNDEFINED, str, lineNum));
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
			addToVector(Token(COMMENT, comm, ogLine));
			return;
		}
		// If a newline is found, increase linenum
		else if (c == '\n')
		{
			lineNum++;
			comm += ' ';
			// comm += c;	// Swap with line above if comment token has newline when stored
		}
		else
		{
			comm += c;
		}
		c = in.get();
	}

	// If EOF is reached, put comment in Undefined Token
	addToVector(Token(UNDEFINED, comm, ogLine));
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

	addToVector(Token(COMMENT, comm, lineNum));

	if (c == '\n') lineNum++;
}