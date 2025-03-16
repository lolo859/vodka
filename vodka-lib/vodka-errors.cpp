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
namespace inside_errors {
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
using namespace inside_errors;
//* Error output function
//* Error output function
void vodka::errors::raise(error_container element) {
    auto error=element.error;
    auto file=element.file;
    auto lines=element.lines;
    auto lines_content=element.lines_content;
    auto source=element.source.source;
    auto filesource=element.source.file;
    for (int i=0;i<filesource.size();++i) {
        string filepath=filesource[i];
        replaceall(filepath,"/",">");
        filepath=filepath+">";
        auto namepath=source[i];
        auto namepathsplit=split(namepath," ")[1];
        auto namewithoutpar=split(namepathsplit,"(")[0];
        source[i]=filepath+namewithoutpar;
    }
    string linestr="";
    if (source.size()!=0) {
        for (int i=0;i<source.size();i++) {
            auto path=split(source[i],">");
            auto sourcename=path[path.size()-1];
            path.pop_back();
            auto sourcenamepath=split(sourcename,"::");
            auto functionname=sourcenamepath[sourcenamepath.size()-1];
            if (i!=0) {
                cout<<"["+to_string(i+1)+"] Called function "<<termcolor::magenta<<termcolor::bold<<functionname<<termcolor::reset<<", located in :"<<endl;
            } else {
                cout<<endl<<"[1] Function "<<termcolor::magenta<<termcolor::bold<<functionname<<termcolor::reset<<", located in :"<<endl;
            }
            for (int y=0;y<path.size();++y) {
                if (y==path.size()-1) {
                    cout<<" ├─ "<<termcolor::cyan<<termcolor::bold<<path[y]<<termcolor::reset<<" (file)"<<endl;
                } else {
                    cout<<" ├─ "<<termcolor::blue<<termcolor::bold<<path[y]<<termcolor::reset<<" (directory)"<<endl;
                }
            }
            for (int y=0;y<sourcenamepath.size();++y) {
                if (sourcenamepath[y]==functionname) {
                    cout<<" └─ "<<termcolor::magenta<<termcolor::bold<<sourcenamepath[y]<<termcolor::reset<<" (function)"<<endl;
                } else {
                    if (y==sourcenamepath.size()-2) {
                        cout<<" ├─ "<<termcolor::yellow<<termcolor::bold<<sourcenamepath[y]<<termcolor::reset<<" (class/final namespace)"<<endl;
                    } else {
                        cout<<" ├─ "<<termcolor::green<<termcolor::bold<<sourcenamepath[y]<<termcolor::reset<<" (namespace)"<<endl;
                    }
                }
            }
        }
        cout<<endl<<"Raised the following exception :"<<endl;
    }
    if (lines.size()!=0) {
        if (lines.size()>1) {
            for (size_t i=0;i<lines.size();++i) {
                if (i==lines.size()-1) {
                    linestr=linestr+to_string(lines[i])+",";
                } else {
                    linestr=linestr+to_string(lines[i])+", ";
                }
            }
        } else {
            linestr=to_string(lines[0]);
        }
    }
    if (file!="") {
        cout<<"\nFile "<<termcolor::cyan<<termcolor::bold<<file<<termcolor::reset<<":"+to_string(lines[0])+", line(s) "+linestr+" an error occured :"<<endl;
    }
    if (lines_content.size()!=0 && lines_content.size()!=0) {
        for (size_t y=0;y<lines.size();++y) {
            cout<<"  "<<lines[y]<<" | "<<lines_content[y]<<endl;
        }
    }
    cout<<termcolor::red<<termcolor::bold<<error<<termcolor::reset<<endl;
}