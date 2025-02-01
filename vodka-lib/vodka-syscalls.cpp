#include "vodka-lib.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
using namespace std;
//* Some necessary functions
namespace inside_syscalls {
    std::vector<std::string> split(const std::string& str,const std::string& delimiter) {
        std::vector<std::string> tokens;
        size_t start=0;
        size_t end=str.find(delimiter);
        while (end!=std::string::npos) {
            if (end>start) {
                tokens.push_back(str.substr(start,end-start));
            }
            start=end+delimiter.length();
            end=str.find(delimiter,start);
        }
        if (start<str.length()) {
            tokens.push_back(str.substr(start));
        }
        return tokens;
    }
    void replaceall(std::string &str,const std::string &from,const std::string &to) {
        size_t start_pos=0;
        while ((start_pos=str.find(from, start_pos))!=std::string::npos) {
            str.replace(start_pos,from.length(),to);
            start_pos+=to.length();
        }
    }
}
using namespace inside_syscalls;
//* Syntax function for syscalls
string vodka::syscalls::syscall_container::syntax() {
    if (thing=="PRINT") {
        string args;
        for (auto a:printele.argument_uid) {
            args=args+" "+a;
        }
        args=args.substr(1,args.size()-1);
        return printele.info.name+" "+args;
    } else if (thing=="ADD") {
        string args;
        for (auto a:addele.argument_uid) {
            args=args+" "+a;
        }
        args=args.substr(1,args.size()-1);
        return addele.info.name+" "+addele.output_uid+" "+args;
    } else if (thing=="ASSIGN") {
        return assignele.info.name+" "+assignele.output_uid+" "+assignele.value;
    } else if (thing=="FREE") {
        string args;
        for (auto a:freeele.argument_uid) {
            args=args+" "+a;
        }
        args=args.substr(1,args.size()-1);
        return freeele.info.name+" "+args;
    } else if (thing=="INVERT") {
        return invertele.info.name+" "+invertele.uid;
    } else if (thing=="BACK") {
        return backele.info.name+" "+backele.var_uid+" "+backele.const_uid+" "+backele.back_uid;
    } else if (thing=="DUPLICATE") {
        return duplicateele.info.name+" "+duplicateele.output_uid+" "+duplicateele.source_uid;
    } else if (thing=="ABS") {
        return absele.info.name+" "+absele.uid;
    } else if (thing=="DIVMOD") {
        return divmodele.info.name+" "+divmodele.quotient_uid+" "+divmodele.rest_uid+" "+divmodele.dividend_uid+" "+divmodele.divisor_uid;
    } else {
        return "error";
    }
}