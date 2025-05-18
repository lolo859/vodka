#include "vodka-lib.h"
#include <string>
#include <vector>
#include <map>
using namespace std;
//* Syntax function for json kernel container
map<string,string> vodka::json::kernel::JsonContainer::syntax() {
    map<string,string> out;
    out.insert(pair<string,string>("type",type));
    out.insert(pair<string,string>("instruction_name",instruction_name));
    for (int i=0;i<args.size();++i) {
        out.insert(pair<string,string>("arg"+to_string(i+1),args[i]));
    }
    return out;
};
//* Syntax function for json vodka instruction
map<string,string> vodka::json::vodka::VodkaInstruction::syntax() {
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
map<string,string> vodka::json::vodka::VodkaSymbol::syntax() {
    map<string,string> out;
    out.insert(pair<string,string>("name",type));
    for (int i=0;i<args.size();++i) {
        out.insert(pair<string,string>("arg"+to_string(i+1),args[i]));
    }
    out.insert(pair<string,string>("uid",uid));
    return out;
};
//* Syntax function for json vodka variables declaration
map<string,string> vodka::json::vodka::VodkaVariableDeclaration::syntax() {
    map<string,string> out;
    out.insert(pair<string,string>("variable_name",variable_name));
    out.insert(pair<string,string>("variable_type",variable_datatype));
    out.insert(pair<string,string>("variable_value",variable_value));
    out.insert(pair<string,string>("uid",uid));
    return out;
};
//* Syntax function for json vodka lines
map<string,string> vodka::json::vodka::VodkaLine::syntax() {
    if (thing=="variable_declaration") {
        auto out=variable_declaration_element.syntax();
        out.insert(pair<string,string>("type","variable_declaration"));
        return out;
    } else if (thing=="instruction") {
        auto out=instruction_element.syntax();
        out.insert(pair<string,string>("type","instruction"));
        return out;
    } else {
        return {};
    }
};
//* Syntax function for json vodka cells
map<string,map<string,string>> vodka::json::vodka::VodkaCell::syntax() {
    map<string,map<string,string>> out;
    out.insert(pair<string,map<string,string>>("name",{{"name",name}}));
    out.insert(pair<string,map<string,string>>("uid",{{"uid",uid}}));
    out.insert(pair<string,map<string,string>>("start",start.syntax()));
    out.insert(pair<string,map<string,string>>("end",end.syntax()));
    for (int i=0;i<lines_content.size();++i) {
        out.insert(pair<string,map<string,string>>("line"+to_string(i+1),lines_content[i].syntax()));
    }
    return out;
};