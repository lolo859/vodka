#include "vodka-lib.h"
#include <string>
#include <vector>
using namespace std;
//* Syntax function for syscalls
string vodka::syscalls::SyscallContainer::syntax() {
    if (thing==vodka::syscalls::SyscallsNames::PRINT) {
        return print_element.name+" "+print_element.argument_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::ADD) {
        return add_element.name+" "+add_element.output_uid+" "+add_element.first_uid+" "+add_element.second_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::ASSIGN) {
        return assign_element.name+" "+assign_element.output_uid+" "+assign_element.value;
    } else if (thing==vodka::syscalls::SyscallsNames::FREE) {
        return free_element.name+" "+free_element.argument_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::INVERT) {
        return invert_element.name+" "+invert_element.output_uid+" "+invert_element.source_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::DUPLICATE) {
        return duplicate_element.name+" "+duplicate_element.output_uid+" "+duplicate_element.source_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::ABS) {
        return abs_element.name+" "+abs_element.output_uid+" "+abs_element.source_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::DIVMOD) {
        return divmod_element.name+" "+divmod_element.quotient_uid+" "+divmod_element.rest_uid+" "+divmod_element.dividend_uid+" "+divmod_element.divisor_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::TOINT) {
        return toint_element.name+" "+toint_element.output_uid+" "+toint_element.source_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::TODEC) {
        return todec_element.name+" "+todec_element.output_uid+" "+todec_element.source_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::MULINT) {
        return mulint_element.name+" "+mulint_element.output_uid+" "+mulint_element.first_uid+" "+mulint_element.second_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::MULDEC) {
        return muldec_element.name+" "+muldec_element.output_uid+" "+muldec_element.first_uid+" "+muldec_element.second_uid+" "+muldec_element.precision_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::DIVIDE) {
        return divide_element.name+" "+divide_element.output_uid+" "+divide_element.first_uid+" "+divide_element.second_uid+" "+divide_element.precision_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::LENGTH) {
        return length_element.name+" "+length_element.output_uid+" "+length_element.source_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::CONCAT) {
        return concat_element.name+" "+concat_element.output_uid+" "+concat_element.first_uid+" "+concat_element.second_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::SUBSTRING) {
        return substring_element.name+" "+substring_element.output_uid+" "+substring_element.source_uid+" "+substring_element.start_index_uid+" "+substring_element.length_output_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::CHARAT) {
        return charat_element.name+" "+charat_element.output_uid+" "+charat_element.input_uid+" "+charat_element.char_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::REVERSE) {
        return reverse_element.name+" "+reverse_element.output_uid+" "+reverse_element.source_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::ESCAPE) {
        return escape_element.name+" "+escape_element.output_uid+" "+escape_element.source_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::INSERT) {
        return insert_element.name+" "+insert_element.output_uid+" "+insert_element.source_uid+" "+insert_element.index_uid+" "+insert_element.string_to_insert_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::FIND) {
        return find_element.name+" "+find_element.output_uid+" "+find_element.source_uid+" "+find_element.char_uid;
    } else {
        return "error";
    }
}
//* Convert SyscallsNames to string
string vodka::syscalls::syscall_to_string(SyscallsNames syscall) {
    switch (syscall) {
        case SyscallsNames::PRINT:return "PRINT";
        case SyscallsNames::ADD:return "ADD";
        case SyscallsNames::ASSIGN:return "ASSIGN";
        case SyscallsNames::FREE:return "FREE";
        case SyscallsNames::INVERT:return "INVERT";
        case SyscallsNames::DUPLICATE:return "DUPLICATE";
        case SyscallsNames::ABS:return "ABS";
        case SyscallsNames::DIVMOD:return "DIVMOD";
        case SyscallsNames::TOINT:return "TOINT";
        case SyscallsNames::TODEC:return "TODEC";
        case SyscallsNames::MULDEC:return "MULDEC";
        case SyscallsNames::MULINT:return "MULINT";
        case SyscallsNames::DIVIDE:return "DIVIDE";
        case SyscallsNames::LENGTH:return "LENGTH";
        case SyscallsNames::CONCAT:return "CONCAT";
        case SyscallsNames::SUBSTRING:return "SUBSTRING";
        case SyscallsNames::CHARAT:return "CHARAT";
        case SyscallsNames::REVERSE:return "REVERSE";
        case SyscallsNames::ESCAPE:return "ESCAPE";
        case SyscallsNames::INSERT:return "INSERT";
        case SyscallsNames::FIND:return "FIND";
        default:return "Unknown";
    }
}