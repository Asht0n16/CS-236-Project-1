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

		// Skip whitespace
		if (isspace(c))
		{
			continue;
		}

		switch (c)
		{
		case '\n':
			lineNum++;	//FIXME!! this line is never hit
			break;

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

		case 'S':		//FIXME! using >> to get the rest of the word will skip whitespace and consume the next word
			in >> w;
			if (w == "chemes") {
				addToVector(Token(SCHEMES, "Schemes", lineNum));
			}
			else
			{
				addToVector(Token(UNDEFINED, "S", lineNum));
			}
			break;

		case 'F':
			in >> w;
			if (w == "acts") {
				addToVector(Token(FACTS, "Facts", lineNum));
			}
			else
			{
				addToVector(Token(UNDEFINED, "F", lineNum));
			}
			break;

		case 'R':
			in >> w;
			if (w == "ules") {
				addToVector(Token(RULES, "Rules", lineNum));
			}
			else
			{
				addToVector(Token(UNDEFINED, "R", lineNum));
			}
			break;

		case 'Q':
			in >> w;
			if (w == "ueries") {
				addToVector(Token(QUERIES, "Queries", lineNum));
			}
			else
			{
				addToVector(Token(UNDEFINED, "Q", lineNum));
			}
			break;
		}
	}
	addToVector(Token(END_OF_FILE, "", lineNum));
}