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
using namespace vodka::errors;
//* Some necessary functions
namespace inside_type {
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
using namespace inside_type;
//* Checking if the inputed value for vodint is correct
bool vodka::type::vodint::check_value(const string& value,vodka::analyser::line context,sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (value.empty() || value=="-") {
        raise(error_container("vodka.error.vodint.invalid_value : Invalid value for vodint : "+value,context.file,{context.content},{context.line},srclclstack));
        return false;
    }
    vector<char> num={'0','1','2','3','4','5','6','7','8','9','-'};
    for (auto v=0;v<value.length();++v) {
        if (v!=0 && value[v]=='-') {
            raise(error_container("vodka.error.vodint.invalid_value : Invalid value for vodint : "+value,context.file,{context.content},{context.line},srclclstack));
            return false;
        }
        if (find(num.begin(),num.end(),value[v])==num.end()) {
            raise(error_container("vodka.error.vodint.invalid_value : Invalid value for vodint : "+value,context.file,{context.content},{context.line},srclclstack));
            return false;
        }
    }
    return true;
}
//* Removing the zero at the beginning of the value inputed for vodint
string vodka::type::vodint::remove_zero(const string& value,sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    bool reached=false;
    string out;
    bool negative;
    string newvalue;
    if (value=="0" or value=="-0") {
        return "0";
    }
    if (value.substr(0,1)=="-") {
        negative=true;
        newvalue=value.substr(1,value.length()-1);
    } else {
        negative=false;
        newvalue=value;
    }
    if (newvalue.substr(0,1)=="0") {
        size_t pos=0;
        while (pos<newvalue.length() && newvalue[pos]==newvalue[0]) {
            ++pos;
        }
        out=newvalue.substr(pos);
    } else {
        out=newvalue;
    }
    if (negative==true) {
        out="-"+out;
    }
    return out;
}
//* Checking if the inputed value for vodec is correct
bool vodka::type::vodec::check_value(const string& value,vodka::analyser::line context,sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (value.empty() || value=="-") {
        raise(error_container("vodka.error.vodec.invalid_value : Invalid value for vodec : "+value,context.file,{context.content},{context.line},srclclstack));
        return false;
    }
    int occ=0;
    for (auto i:value) {
        if (i=='.') {
            ++occ;
        }
    }
    if (occ!=1) {
        raise(error_container("vodka.error.vodec.invalid_value : Invalid value for vodec : "+value,context.file,{context.content},{context.line},srclclstack));
        return false;
    }
    auto part=split(value,".");
    if (part.size()!=2 || part[0]=="-") {
        raise(error_container("vodka.error.vodec.invalid_value : Invalid value for vodec : "+value,context.file,{context.content},{context.line},srclclstack));
        return false;
    }
    vector<char> num={'0','1','2','3','4','5','6','7','8','9','-','.'};
    for (auto v=0;v<value.length();++v) {
        if (v!=0 && value[v]=='-') {
            raise(error_container("vodka.error.vodec.invalid_value : Invalid value for vodec : "+value,context.file,{context.content},{context.line},srclclstack));
            return false;
        }
        if (find(num.begin(),num.end(),value[v])==num.end()) {
            raise(error_container("vodka.error.vodec.invalid_value : Invalid value for vodec : "+value,context.file,{context.content},{context.line},srclclstack));
            return false;
        }
    }
    return true;
}
//* Removing the zero at the beginning of the value inputed for vodec
string vodka::type::vodec::remove_zero(const string& value,sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    string out;
    bool negative;
    string newvalue;
    if (value=="0.0" || value=="-0.0") {
        return "0.0";
    }
    if (value.substr(0,1)=="-") {
        negative=true;
        newvalue=value.substr(1,value.length()-1);
    } else {
        newvalue=value;
    }
    auto twopart=split(newvalue,".");
    string newintpart=vodka::type::vodint::remove_zero(twopart[0],srclclstack);
    auto decpart=twopart[1];
    string newdecpart;
    if (decpart!="") {
        reverse(decpart.begin(),decpart.end());
        newdecpart=vodka::type::vodint::remove_zero(decpart,srclclstack);
        reverse(newdecpart.begin(),newdecpart.end());
    } else {
        newdecpart="0";
    }
    out=newintpart+"."+newdecpart;
    if (negative==true) {
        out="-"+out;
    }
    return out;
}