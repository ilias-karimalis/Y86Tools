# y86 Assembler


Language Choices:

Python: 
- Probably easier to implement
- Really doesn't need to be that fast

C++:
- This is a learning exercise and should probably be done in the Lang. I'm
trying to master.

What does Lexing actually look like?

First we Tokenize the file. That is we transform strings in the program to the
following structure:
```C
struct Token {
    Type of Token,		/// Some enum that signifies TokenType for the case of C++
    				/// we can combine it with the union and just use a variant
				/// type.
    Location in file,		/// FilePath, line number, column number
    union {
	long long value 	/// Value of int type, at most 8 bytes long so use long long
        const char * name	/// Unique name of Token so that we can compare files
				/// using pointer comparisons.
    }
}

```

Final decision just write it in C++.

Lets examine our grammar:
```asm
.pos 0
Init:
    irmovq Stack, %rbp
    irmovq Stack, %rsp

.pos 0x100
Stack:
```

First we make some assumptions, each instruction is on it's own line, but the 
labels `Init:` can be on the same line as an instruction. On the other hand,
assembler direvtives that are specified by `.` are each on their own line.
We'll know the size of .yo file we should return as the largest `.pos` plus an
offset for the code or data after it. 

Currently writing the Lexer.
Things that work:
- Integer Lexing
- Comment Lexing
- '.' Lexing

TODO:
- Keyword Lexing
- Identifier Lexing

Note I think that the above two are actually equivalent!