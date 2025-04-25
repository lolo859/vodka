#include "vodka-lib.h"
#include "../dependencies/termcolor.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
#include <cstring>
using namespace std;
//* Some necessary functions
namespace inside_utils {
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
using namespace inside_utils;
//* Utilities
//* UUID generator
boost::uuids::uuid vodka::utilities::genuid() {
    boost::uuids::random_generator ran;
    boost::uuids::uuid uuid=ran();
    return uuid;
}
//* Logs functions
void vodka::utilities::log(const string& text,string verbose,int x,string last,int sublevel,vector<int> substep,vector<unsigned long> subtotal) {
    if (verbose=="a" || verbose=="r") {
        if (sublevel==0) {
            if (verbose=="a" || verbose=="r") {
                cout<<endl<<"[LOG]     ";
                string texti;
                if (verbose=="r" && text.substr(text.length()-1,1)==":") {
                    texti=text.substr(0,text.length()-2)+"...";
                } else {
                    texti=text;
                }
                last="("+to_string(x)+"/19) "+texti;
                cout<<last;
            }
        } else {
            if (verbose=="a") {
                cout<<endl<<"[LOG]     ";
                last="("+to_string(x)+"/19) ";
                for (int i=0;i<sublevel;++i) {
                    last=last+"("+to_string(substep[i])+"/"+to_string(subtotal[i])+") ";
                }
                last=last+text;
                cout<<last;
            }
        }
    }
}
void vodka::utilities::debuglog(const string& text,int line,const string& cell,bool debugmode,string verbose,string file,bool debug_info) {
    if (debugmode==true) {
        if (debug_info==true) {
            if (verbose=="e") {
                string texti=text.substr(1,text.length()-1);
                cout<<"[DEBUG]   Debug line "+to_string(line)+" in cell "<<termcolor::magenta<<termcolor::bold<<cell<<termcolor::reset<<" from file "<<termcolor::blue<<termcolor::bold<<filesystem::absolute(file).string()<<termcolor::reset<<" : "+texti<<endl;
            } else if (verbose=="r") {
                string texti=text.substr(1,text.length()-1);
                cout<<endl<<"[DEBUG]   Debug line "+to_string(line)+" in cell "<<termcolor::magenta<<termcolor::bold<<cell<<termcolor::reset<<" from file "<<termcolor::blue<<termcolor::bold<<filesystem::absolute(file).string()<<termcolor::reset<<" : "+texti<<endl;
            } else if (verbose=="a") {
                string texti=text.substr(1,text.length()-1);
                cout<<endl<<"[DEBUG]   Debug line "+to_string(line)+" in cell "<<termcolor::magenta<<termcolor::bold<<cell<<termcolor::reset<<" from file "<<termcolor::blue<<termcolor::bold<<filesystem::absolute(file).string()<<termcolor::reset<<" : "+texti;
            }
        } else {
            if (verbose=="e") {
                string texti=text.substr(2,text.length()-2);
                cout<<"[DEBUG]   "<<texti<<endl;
            } else if (verbose=="r") {
                string texti=text.substr(2,text.length()-2);
                cout<<endl<<"[DEBUG]   "<<texti<<endl;
            } else if (verbose=="a") {
                string texti=text.substr(2,text.length()-2);
                cout<<endl<<"[DEBUG]   "<<texti;
            }
        }
    }
}
void vodka::utilities::var_warning(string namevar,string typevar,string namecell,string line,bool var_warning_enabled,string verbose) {
    if (var_warning_enabled==true) {
        if (verbose=="e") {
            cout<<"[WARNING] vodka.warnings.unused_variable : Variable "<<termcolor::bold<<namevar+" ("+typevar+")"<<termcolor::reset<<", declared line "<<termcolor::bold<<line<<termcolor::reset<<" in cell "<<termcolor::magenta<<termcolor::bold<<namecell<<termcolor::reset<<", isn't used anywhere and may take useless memory."<<endl;
        } else if (verbose=="r") {
            cout<<endl<<"[WARNING] vodka.warnings.unused_variable : Variable "<<termcolor::bold<<namevar+" ("+typevar+")"<<termcolor::reset<<", declared line "<<termcolor::bold<<line<<termcolor::reset<<" in cell "<<termcolor::magenta<<termcolor::bold<<namecell<<termcolor::reset<<", isn't used anywhere and may take useless memory."<<endl;
        } else if (verbose=="a") {
            cout<<endl<<"[WARNING] vodka.warnings.unused_variable : Variable "<<termcolor::bold<<namevar+" ("+typevar+")"<<termcolor::reset<<", declared line "<<termcolor::bold<<line<<termcolor::reset<<" in cell "<<termcolor::magenta<<termcolor::bold<<namecell<<termcolor::reset<<", isn't used anywhere and may take useless memory."<<endl;
        }
    }
}