#include <string>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

void postfix(string s, string& theoutput, int& total);
bool checkNumber(char s);
int checkOperator(char s);
bool checkValid(string s, int total, int& theory);
int calculator(string s);

int main()
{
   string equation;
	string postfixa;
	int totala = 0;     //Total number of operator + operands.
	int theory = 0;    // The theory of which got proven in the checkValid function. (just for test)
	cout << "Please enter your equation." << endl;
	getline(cin, equation);
	bool exception = checkValid(equation, totala, theory);
	if(exception)
	{
		postfix(equation, postfixa, totala);
		int actualtotal = calculator(postfixa);
		cout << "THE ANSWER IS.......... " << actualtotal << "!!!!" << endl;
	}
	else
	{
		cout << "There was something wrong with your equation." << endl;
	}
		return 0;
}


void postfix(string s, string& theoutput,int& total)
{
	int totalaa = 0;
	//Just putting 20 because I don't expect to see more than 20 operators
	//In one stack.
	string thestack[20];
	int top = -1;
	string output;
	for(int i = 0; i < s.length(); i++)
	{
		string operand;
		while(checkNumber(s[i]))
		{
			operand = operand+s[i];
			i++;
		}
		//If operand was empty, it would've been weird to see
		//A space out of nowhere, so.....
		if(!(operand == ""))
		{
			output += operand + " ";
			totalaa++;
		}
		int oper = checkOperator(s[i]);
		bool inputted = false;
		while((!inputted))
		{
			switch(oper)
			{
				//(
				//It has to add always.
			case 1:
				top++;
				thestack[top] = s[i];
				inputted = true;
				break;
				//+ or -
			case 2: case 3:
				if(top == -1 || thestack[top] == "(")
				{
					top++;
					thestack[top] = s[i];
					inputted = true;
				}
				else
				{
					output += thestack[top] + " ";
					top--;
					totalaa++;
				}
				break;
				//* or /
			case 4: case 5:
				//cout << "trying to put  operator * or /" << endl;
				if(top == -1 || thestack[top] == "(")
				{
					top++;
					thestack[top] = s[i];
					inputted = true;
				}
				//* and / have higher priorities than + and - so
				//Stack if top = + or -
				else if(thestack[top] == "+" || thestack[top] == "-")
				{
					top++;
					thestack[top] = s[i];
					inputted = true;
				}
				else
				{
					//We gotta go again
					output += thestack[top] + " ";
					top--;
				}
				break;
				// )
				//Output everything until we see an opening parenthesis
			case 6:
				while(!(thestack[top] == "("))
				{
					output += thestack[top] + " ";
					top--;
					totalaa++;
				}
				//We need to discard the opening parenthesis so...
				//top-- to get the to next person in line.
				top--;
				inputted = true;
				break;
			default:
				inputted = true;
				break;
			}
		 }
	}
	while(top > -1)
	{
		output += thestack[top] + " ";
		top--;
		totalaa++;
	}
	cout << "postfix is: " << output << endl;
	theoutput = output;
	total = totalaa;
}

//There are several ways to test this.
//First, if there are even numbers of operands + operators, it is not possible.
//Ex: 1+, 2+2+, (6+13))
//But this will be proven through other ways so it's not really needed to be tested.
//Second, if 0 happens after /, it is not possible.
//Ex: 1/0 = not possible.
//Third, if there are two arithmetic operations in a row, it is not possible
//Ex: 1++
//Fourth, if there are no ending parenthesis while there is an opening parenthesis, it is not possible.
//So there should be the same number of opening parenthesis and ending parenthesis.
//Ex: (1+2+ <-- As you can see, there are odd numbers of operands+operators, but it is not possible.
//Fifth, if it ends in an arithmetic operator or a opening parenthesis (but already tested in fourth), it is not possible.
//Ex: 1+
bool checkValid(string s, int total, int& theory)
{
	//Second one.
	//So I run for loop until I find an operator which is /
	//I have set that / equals 5 in my checkOperator function.
	//And I compare the next one in the string and if that's a 0, which is 48
	//In hexadecimal, it will be false;
	for(int i = 0; i < s.length(); i++)
	{
		int oper = checkOperator(s[i]);
		if(oper == 5)
		{
			int nextnumber = (int)s[i+1];
			//Hexadecimal number of char 0 is 48.
			if(nextnumber == 48)
			{
				theory = 2;
				return false;
			}
		}
	}

	//Third one.
	//I run for loop until I find an arithmetic operator and if the next one
	//Is also an arithmetic operator, return false;
	for(int i = 0; i < s.length(); i++)
	{
		int oper = checkOperator(s[i]);
		//If between 1 and 6, it will be + - * or /
		if(oper > 1 && oper < 6)
		{
			//Checking the next one to see if it's also
			//An arithmetic operator OR a closing parenthesis
			int oper2 = checkOperator(s[i+1]);
			if(oper2 > 1 && oper2 < 7)
			{
				theory = 3;
				return false;
			}
		}
	}

	//Fourth one.
	bool ended = true;
	int opening = 0;
	int ending = 0;
	for(int i = 0; i < s.length(); i++)
	{
		int oper = checkOperator(s[i]);
		if(oper == 1)
		{
			ended = false;
			opening++;
		}
		if(oper == 6)
		{
			ended = true;
			ending++;
		}
	}
	//If operator never closed, it should return false;
	if(opening != ending)
	{
		if(ended == false)
		{
			theory = 4;
			return false;
		}
	}

	//Fifth one.
	int length = s.length();
	int oper = checkOperator(s[length-1]);
	if(oper > 1  && oper < 6)
	{
		theory = 5;
		return false;
	}

	//If all fails,
	return true;
}

//If the hexadecimal number is not between 48 and 57,,
//It's not a number.
bool checkNumber(char s)
{
	//cout << (int)s << endl;
	if(((int)s < 58) && ((int)s > 47))
		return true;
   else
		return false;
}

int checkOperator(char s)
{
	if(s == '(')
	{
		return 1;
	}
	else if(s == '+')
	{
		return 2;
	}
	else if(s == '-')
	{
		return 3;
	}
	else if(s == '*')
	{
		return 4;
	}
	else if(s == '/')
	{
		return 5;
	}
	else if(s == ')')
	{
		return 6;
	}
	else
		return 0;
}

int calculator(string s)
{
	int duhstack[10];
	int top = -1;
	int total = 0;
	for(int i = 0; i < s.length(); i++)
	{
		//could be double digit so..
		string operand;
		int opera = checkOperator(s[i]);
		if(opera > 1 && opera < 6)
		{
			int first = duhstack[top-1];
			int second = duhstack[top];
			if(opera == 2)
			{
				total = first + second;
			}
			if(opera == 3)
			{
				total = first - second;
			}
			if(opera == 4)
			{
				total = first * second;
			}
			if(opera == 5)
			{
				total = first / second;
			}
			top--;
			duhstack[top] = total;
		}
		if(checkNumber(s[i]) == true)
		{
			while(checkNumber(s[i]))
			{
				operand = operand+s[i];
				i++;
			}
			top++;
			duhstack[top] = atoi(operand.c_str());
		}
	}
	return total;
}
