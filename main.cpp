#include <iostream>
#include <fstream>
#include <stdint.h>
#include <string>
#include <vector>

namespace Fucking
{
	struct Instruction
	{
		uint8_t instruction;
		uint8_t param;

		Instruction(uint8_t _instruction, uint8_t _parameter)
			: instruction(_instruction), param(_parameter)
		{}
	};

	class Fucking
	{
	public:
		Fucking()
		{
			memset(stack, 0, 256 * sizeof(uint8_t));
			pointer = 0;
		}

	public:
		void executeProgram(std::vector<Instruction> instructions)
		{
			/*
			* 0x00 - push value onto the stack
			* 0x01 - pop value from the stack
			* 0x02 - print value from stack as ASCII character
			* 0x03 - add value with the currently selected one on the stack
			* 0x04 - substract value
			* 0x05 - if current value is not zero, continue execution as normal; else, skip over to the nearest 0x07
			* 0x06 - end if/ignore
			* 0x07 - move pointer to the right
			* 0x08 - move pointer to the left
			* 0x09 - ignore any incoming instructions till the next 0x07 (end if)
			*/
			bool ignore = false; // used for if statements
			for (auto instruction : instructions)
			{
				switch (instruction.instruction)
				{
				case 0x00:
					if (!ignore) stack[pointer] = instruction.param;
					break;

				case 0x01:
					if (!ignore) stack[pointer] = 0;
					break;

				case 0x02:
					if (!ignore) std::cout << (char)stack[pointer];
					break;

				case 0x03:
					if (!ignore) stack[pointer] += instruction.param;
					break;

				case 0x04:
					if (!ignore) stack[pointer] -= instruction.param;
					break;

				case 0x05:
					if (!ignore) if (stack[pointer] == 0) ignore = true;
					break;

				case 0x06:
					if (ignore) ignore = false;
					break;

				case 0x07:
					if (!ignore) pointer++;
					break;

				case 0x08:
					if (!ignore) pointer--;
					break;

				case 0x09:
					if (!ignore) ignore = true;

				default:
					break;
				}
			}
		}

	private:
		uint8_t stack[256];
		uint8_t pointer;
	};

	std::vector<Instruction> compileProgram(std::string source)
	{
		std::vector<Instruction> compiled;
		
		for (unsigned int i = 0; i < source.length() - 1; i++)
		{
			char instruction = source.at(i);
			char parameter = source.at(i + 1);
			compiled.emplace_back((uint8_t)instruction, (uint8_t)parameter);
		}

		return compiled;
	}
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cout << "No input files" << std::endl;
	}
	else
	{
		std::fstream file(argv[1], std::ios::in | std::ios::binary);
		if (!file.is_open())
		{
			std::cout << "Error while opening file (either the file doesn't exist or the program doesn't have enough permissions to open" << std::endl;
			return 1;
		}

		std::string program, line;
		 
		while (!file.eof())
		{
			std::getline(file, line);
			program.append(line);
		}
		
		// compile the program and run it
		Fucking::Fucking fucking{};
		fucking.executeProgram(Fucking::compileProgram(program));
		file.close();
	}
}
