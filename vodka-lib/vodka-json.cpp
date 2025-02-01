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
namespace inside_json {
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
using namespace inside_json;
//* Syntax function for json kernel container
map<string,string> vodka::json::kernel::json_container::syntax() {
    map<string,string> out;
    out.insert(pair<string,string>("type",type));
    out.insert(pair<string,string>("intname",intname));
    for (int i=0;i<args.size();++i) {
        out.insert(pair<string,string>("arg"+to_string(i+1),args[i]));
    }
    return out;
};
//* Syntax function for json vodka instruction
map<string,string> vodka::json::vodka::instruction::syntax() {
    map<string,string> out;
    out.insert(pair<string,string>("name",name));
    out.insert(pair<string,string>("source",source));
    out.insert(pair<string,string>("library",library));
    out.insert(pair<string,string>("uid",uid));
    for (int i=0;i<args.size();++i) {
        out.insert(pair<string,string>("arg"+to_string(i+1),args[i]));
    }
    return out;
};
//* Syntax function for json vodka symbol
map<string,string> vodka::json::vodka::symbol::syntax() {
    map<string,string> out;
    out.insert(pair<string,string>("name",type));
    for (int i=0;i<args.size();++i) {
        out.insert(pair<string,string>("arg"+to_string(i+1),args[i]));
    }
    out.insert(pair<string,string>("uid",uid));
    return out;
};
//* Syntax function for json vodka variables declaration
map<string,string> vodka::json::vodka::var_declaration::syntax() {
    map<string,string> out;
    out.insert(pair<string,string>("name",name));
    out.insert(pair<string,string>("vartype",type));
    out.insert(pair<string,string>("decvalue",decvalue));
    out.insert(pair<string,string>("uid",uid));
    return out;
};
//* Syntax function for json vodka lines
map<string,string> vodka::json::vodka::line_container::syntax() {
    if (thing=="var") {
        auto out=varele.syntax();
        out.insert(pair<string,string>("type","var"));
        return out;
    } else if (thing=="int") {
        auto out=intele.syntax();
        out.insert(pair<string,string>("type","int"));
        return out;
    } else {
        return {};
    }
};
//* Syntax function for json vodka cells
map<string,map<string,string>> vodka::json::vodka::cell::syntax() {
    map<string,map<string,string>> out;
    out.insert(pair<string,map<string,string>>("name",{{"name",name}}));
    out.insert(pair<string,map<string,string>>("uid",{{"uid",uid}}));
    out.insert(pair<string,map<string,string>>("start",start.syntax()));
    out.insert(pair<string,map<string,string>>("end",end.syntax()));
    for (int i=0;i<lines.size();++i) {
        out.insert(pair<string,map<string,string>>("line"+to_string(i+1),lines[i].syntax()));
    }
    return out;
};