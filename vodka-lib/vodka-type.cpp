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
    if (value.substr(0,1)=="-") {
        negative=true;
        newvalue=value.substr(1,value.length()-1);
    } else {
        negative=false;
        newvalue=value;
    }
    if (newvalue!="0") {
        if (count(newvalue.begin(),newvalue.end(),'0')!=newvalue.length()) {
            for (int i=0;i<newvalue.length();++i) {
                if (newvalue[i]=='0' && reached==false) {
                    reached=false;
                } else if (newvalue[i]!='0' && reached==false) {
                    reached=true;
                    out=out+newvalue[i];
                } else {
                    out=out+newvalue[i];
                }
            }
        } else {
            out="0";
        }
    } else {
        out="0";
    }
    if (negative==true) {
        out="-"+out;
    }
    return out;
}
//* Invert value for vodint value
string vodka::type::vodint::invert_value(const string& value,sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    string out;
    if (value.substr(0,1)=="-") {
        out=value.substr(1,value.length()-1);
    } else {
        out="-"+value;
    }
    return out;
}
//* Calculate the sign of a multiplication/division between two vodint values
string vodka::type::vodint::calculate_sign(const string& value1,const string& value2,sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (value1.substr(0,1)!="-" && value2.substr(0,1)!="-") {
        return "";
    } else if (value1.substr(0,1)=="-" && value2.substr(0,1)=="-") {
        return "";
    } else {
        return "-";
    }
}
//* Calculate the absolute value of a vodint value
string vodka::type::vodint::abs(const string& value,sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (value.substr(0,1)=="-") {
        return value.substr(1,value.length()-1);
    } else {
        return value;
    }
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
    bool reached=false;
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
        negative=false;
        newvalue=value;
    }
    if (newvalue!="0") {
        if (count(newvalue.begin(),newvalue.end(),'0')!=newvalue.length()) {
            for (int i=0;i<newvalue.length();++i) {
                if (newvalue[i]=='0' && reached==false) {
                    reached=false;
                } else if (newvalue[i]!='0' && reached==false) {
                    reached=true;
                    out=out+newvalue[i];
                } else {
                    out=out+newvalue[i];
                }
            }
        } else {
            out="0";
        }
    } else {
        out="0";
    }
    if (negative==true) {
        out="-"+out;
    }
    return out;
}