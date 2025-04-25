# Kernel code language codebase and syntax

## Introduction

Kernel code is the low-level, intermediate language designed to be the final target of Vodka code transcoding. It features a minimalistic but precise instruction set, focused on execution rather than readability. Kernel code is what actually gets interpreted and run. Please note that this documentation only covers currently implemented features (including beta-stage ones), and does not include any planned or theoretical additions.

## Index

- [Sections](#sections)
- [Arguments](#arguments)
- [Datas](#datas)
- [Variables](#variables)
- [Instructions](#instructions)
  - [PRINT](#print)
  - [ADD](#add)
  - [ASSIGN](#assign)
  - [FREE](#free)
  - [INVERT](#invert)
  - [DUPLICATE](#duplicate)
  - [ABS](#abs)
  - [DIVMOD](#divmod)
  - [TOINT](#toint)
  - [TODEC](#todec)
  - [MULDEC](#muldec)
  - [MULINT](#mulint)
  - [DIVIDE](#divide)

## Sections

The kernel code is divided into sections define with their name followed by `:`. A section is ended by the keyword `end` followed immediately by the name of the section. Sections can't have any arguments. 

Code example:
```
main:
<some code>
endmain
```

The name of the first section to be executed is always `main`. Sections can't be named `args` or `data`

## Arguments

Arguments are stored in the order they are passed to the processus. The variable UID where they are stored is located in the `args` section. If too many arguments are passed to the processus, they will ignored.

Code example:
```
args:
<uid_1>
<uid_2>
endargs
```

## Datas

Datas are basically [kernel constants](vodka-codebase.md#kernel-constants). These are read-only variables that can't be deleted with [FREE](#free) or added during runtime. They are accessible to all sections inside the file and can be duplicated. They can't be overwritten by the [ASSIGN](#assign) or [DUPLICATE](#duplicate) instructions. They are stored inside the `data` section.

Code example:
```
data:
<uid>=<value>
<uid>=<value>
enddata
```

## Variables

Variables are the core of the kernel code. They are declared using the [ASSIGN](#assign) syscall. Variables are identified by UID in order to be unique and can contain a infinity of caracters. There is no distinction between local or global variables. A section can totally access a variable declared by another section.

Code example:
```
ASSIGN <uid> <value>
```

## Instructions

Instructions (or syscalls) are always written in capital letter and can't be added outside of section. All arguments are variables UID except for the value in the `ASSIGN` instruction. All arguments are separated by spaces. Here is the list of instructions:
- [PRINT](#print)
- [ADD](#add)
- [ASSIGN](#assign)
- [FREE](#free)
- [INVERT](#invert)
- [DUPLICATE](#duplicate)
- [ABS](#abs)
- [DIVMOD](#divmod)
- [TOINT](#toint)
- [TODEC](#todec)
- [MULDEC](#muldec)
- [MULINT](#mulint)
- [DIVIDE](#divide)

A lot of these instructions are accessible using the [kernel](vodka-codebase.md#kernel) internal library.

### PRINT

The `PRINT` instruction output variables to standard output.

Syntax: `PRINT <uid_1> <uid_2> ... <uid_n>`

### ADD

The `ADD` instruction adds variables together. All arguments except the output must have either a integer or decimal number syntax.

Syntax: `ADD <output_uid> <first_term> <second_term> ... <n_term>`

### ASSIGN

The `ASSIGN` instruction creates a variable. The value is the rest of the line.

Syntax: `ASSIGN <uid> value`

### FREE

The `FREE` instruction deletes variables.

Syntax: `FREE <uid_1> <uid_2> ... <uid_n>`

### INVERT

The `INVERT` instruction invert the sign of a integer or decimal number. The argument must have either a integer or decimal number syntax.

Syntax: `INVERT <uid>`

### DUPLICATE

The `DUPLICATE` instruction duplicates variables.

Syntax: `DUPLICATE <output_uid> <source_uid>`

### ABS

The `ABS` instruction set a integer to positive. The argument must have a integer number syntax.

Syntax: `ABS <uid>`

### DIVMOD

The `DIVMOD` instruction divide two integers and save both the quotient and the rest. The first and second term must have a integer number syntax.

Syntax: `DIVMOD <output_uid> <rest_uid> <first_term> <second_term>`

### TOINT

The `TOINT` instruction remove the decimal part of a decimal number. The source variable must have a decimal number syntax.

Syntax: `TOINT <output_uid> <source_uid>`

### TODEC

The `TODEC` instruction add a empty decimal part to a integer number. The source variable must have a integer number syntax.

Syntax: `TODEC <output_uid> <source_uid>`

### MULINT

The `MULINT` instruction multiply two integers numbers together. The first and second term must have a integer number syntax.

Syntax: `MULINT <output_uid> <first_term> <second_term>`

### MULDEC

The `MULDEC` instruction multiply two decimals numbers together. The first and second term must have a decimal number syntax and the precision must have a integer number syntax.

Syntax: `MULDEC <output_uid> <first_term> <second_term> <precision_uid>`

### DIVIDE

The `DIVIDE` instruction divide two decimals numbers together. The first and second term must have a decimal number syntax and the precision must have a integer number syntax.

Syntax: `DIVIDE <output_uid> <first_term> <second_term> <precision_uid>`