#include "vodka-lib.h"
#include <string>
#include <vector>
using namespace std;
//* Syntax function for syscalls
string vodka::syscalls::SyscallContainer::syntax() {
    if (thing==vodka::syscalls::SyscallsNames::PRINT) {
        string args;
        for (auto a:print_element.argument_uid) {
            args=args+" "+a;
        }
        args=args.substr(1,args.size()-1);
        return print_element.name+" "+args;
    } else if (thing==vodka::syscalls::SyscallsNames::ADD) {
        string args;
        for (auto a:add_element.argument_uid) {
            args=args+" "+a;
        }
        args=args.substr(1,args.size()-1);
        return add_element.name+" "+add_element.output_uid+" "+args;
    } else if (thing==vodka::syscalls::SyscallsNames::ASSIGN) {
        return assign_element.name+" "+assign_element.output_uid+" "+assign_element.value;
    } else if (thing==vodka::syscalls::SyscallsNames::FREE) {
        string args;
        for (auto a:free_element.argument_uid) {
            args=args+" "+a;
        }
        args=args.substr(1,args.size()-1);
        return free_element.name+" "+args;
    } else if (thing==vodka::syscalls::SyscallsNames::INVERT) {
        return invert_element.name+" "+invert_element.uid;
    } else if (thing==vodka::syscalls::SyscallsNames::DUPLICATE) {
        return duplicate_element.name+" "+duplicate_element.output_uid+" "+duplicate_element.source_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::ABS) {
        return abs_element.name+" "+abs_element.uid;
    } else if (thing==vodka::syscalls::SyscallsNames::DIVMOD) {
        return divmod_element.name+" "+divmod_element.quotient_uid+" "+divmod_element.rest_uid+" "+divmod_element.dividend_uid+" "+divmod_element.divisor_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::TOINT) {
        return toint_element.name+" "+toint_element.uid_output+" "+toint_element.uid_source;
    } else if (thing==vodka::syscalls::SyscallsNames::TODEC) {
        return todec_element.name+" "+todec_element.uid_output+" "+todec_element.uid_source;
    } else if (thing==vodka::syscalls::SyscallsNames::MULINT) {
        return mulint_element.name+" "+mulint_element.output_uid+" "+mulint_element.first_uid+" "+mulint_element.second_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::MULDEC) {
        return muldec_element.name+" "+muldec_element.output_uid+" "+muldec_element.first_uid+" "+muldec_element.second_uid+" "+muldec_element.precision_uid;
    } else if (thing==vodka::syscalls::SyscallsNames::DIVIDE) {
        return divide_element.name+" "+divide_element.output_uid+" "+divide_element.first_uid+" "+divide_element.second_uid+" "+divide_element.precision_uid;
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
        default:return "Unknown";
    }
}