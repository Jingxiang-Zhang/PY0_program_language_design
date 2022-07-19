### Introduction

According to python's grammar, I designed this PY0 language by C++ as Fundamentals of Compiling course final project. It took me several weeks to design and program it. PY0 is a interpreted language, therefore, the efficiency of this compiler is relatively low. But by reading the source code of this program, I hope reader could have a better understanding of the compiling process.

### Grammar Design

PY0's grammar simulates Python's gramar. So, it is not difficult for you to understand if you know Python.

```
#1. the start of the input
<file_input> := (NEWLINE | <stmt>)* ENDMARKER

#2. statement and simple statement
<stmt> := <simple_stmt> | <compound_stmt>
<simple_stmt> := <small_stmt> NEWLINE
<small_stmt> := (<del_stmt> | <expr_stmt> | <pass_stmt> | <flow_stmt> | <import_stmt>)

<expr_stmt> := <or_test> | (NAME ((<augassign> <or_test>) | ('=' <or_test>)))
<augassign> := ('+=' | '-=' | '*=' | '/=')

<del_stmt> := 'del' NAME
<pass_stmt> := 'pass'
<flow_stmt> := <break_stmt> | <continue_stmt> | <return_stmt>
<break_stmt> := 'break'
<continue_stmt> := 'continue'
<return_stmt> := 'return' or_test
<import_stmt> := 'import' ( STRING | NAME )


#3. compound statement
<compound_stmt> := <if_stmt> | <while_stmt> | <for_stmt> | <funcdef>
<if_stmt> := 'if' <or_test> ':' <suite> ('elif' <or_test> ':' <suite>)*['else' ':' <suite>]
<while_stmt> := 'while' <or_test> ':' <suite>
<for_stmt> := 'for' NAME 'in' <or_test> ':' <suite>

<suite> := NEWLINE INDENT <file_input> DEDENT
<funcdef> := 'def' NAME <parameters> ':' <suite>
<parameters> := '(' [typedargslist] ')'
<typedargslist> := NAME (',' NAME )*


<or_test> := <and_test> ('or' <and_test>)*
<and_test> := <not_test> ('and' <not_test>)*
<not_test>:= ['not'] <comparison>
<comparison> := <expr> [<comp_op> <expr>]

<comp_op> := '<' | '>' | '==' | '>=' | '<='
<expr> := <term> (('+'|'-') <term>)*
<term> := <factor> (('*' | '/') <factor>)*
<factor> ::= <atom> <trailer>*
<atom> := <atom_base>  | <array> | '(' or_test ')'
<atom_base> := NAME | NUMBER | STRING
<array> := '(' <or_test> (',' <or_test>)* ')'

<trailer> := '(' [<arglist>] ')' | '[' <subscript> ']'
<subscript> := <or_test>
<arglist> := <or_test> (',' <or_test>)*
```

### Project Architecture

The project architecture of all modules are as follows. In order to avoid circular reference, I split some modules. All modules shown below are header files and are not class inheritance. In the following, I will explain the function of each module and the design of all functions in detail.

<div align="center">
  <img src="https://github.com/Jingxiang-Zhang/PY0_program_language_design/blob/main/img/architecture.png">
</div>

There are totally 9 parts of this project, and next, I will introduce it one by one.

#### Symbol Table

One of the most basic modules. It doesn't call any other modules, and almost all modules call the contents of this module, converting abstract numbers to enumerations. This was the first module I designed, and it hasn't changed much since.

<div align="center">
  <img src="https://github.com/Jingxiang-Zhang/PY0_program_language_design/blob/main/img/symbol.png">
</div>

#### Error Handler

It does not call any other module, used to display error information, and gives error handling. It needs to be called by other modules. Due to the limited time, I only made a very brief description of the key errors, and the error report content is relatively simple.

<div align="center">
  <img src="https://github.com/Jingxiang-Zhang/PY0_program_language_design/blob/main/img/error_msg.png">
</div>

#### Lexical Analysis

This module is used for word segmentation. It was originally designed and then added additional functions to meet more requirements. The following functions are provided:

- Partition the input file by the filename
- Get the next word
- Get the last word
- Skip a code segment
- Get the code length
- Get the current code location
- Skip the cursor into appointed location

#### Program Runtime Base & Grammatical Analysis Base

The reason of separating these two parts from its main part, it to avoid circular reference. Here are some segment code of these two parts:

<div align="center">
  <img src="https://github.com/Jingxiang-Zhang/PY0_program_language_design/blob/main/img/structure_base.png">
</div>

#### Program Runtime Management

In this part, It finish the following works:

- Add variables to the variable table
- Get the variable object in the variable table
- Increase or decrease the level of code segment
- Keep the variables when enter a new function
- Put the variables into current variable table when exit a function
- Delete variables
- Record import list
- Show the whole variable table

#### Built-in Function Initiation

This module is initialized for functions and is called in the main function. To satisfy the hierarchical structure of the program, I have divided functions into two categories: 1. Build-in functions, such as print and input functions. 2. User-defined functions. This function is used to initialize all the built-in functions. All built-in functions use function pointers and store the pointer to the function in the symbol table for invocation. Function pointer types are defined as follows:

```
typedef phrase::return_value(*funpointer)(phrase::infunction_type * infunction); 
```

There are several built-in function:

<div align="center">
  <img src="https://github.com/Jingxiang-Zhang/PY0_program_language_design/blob/main/img/built_in_function.png">
</div>

#### Semantic and Grammatical Analysis

Analyse all the code, which include:

- **float, int, string** types of variable
- **break, continue** grammar in loop
- **del** grammar to delete variable
- vector operation
- **import** grammar
- **if, elif, else** branch statement
- **while, for** loop structure
- **def** user define function, and function call
- build-in function call


#### Main Function

Display the interface and the basic function. This PY0 project has two methods to use, one is to use the command line, or drag the file to open, passed in a PY0 file path as the command line parameters; The second option is to enter the command operation interface without passing in command line parameters.

### Demonstration

<div align="center">
  <img src="https://github.com/Jingxiang-Zhang/PY0_program_language_design/blob/main/img/demonstration.png">
</div>
