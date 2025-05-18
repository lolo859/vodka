#include "vodka-lib.h"
#include "../dependencies/termcolor.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
#include <sys/resource.h>
#include <stdexcept>
#include <cstdlib>
using namespace std;
double vodka::utilities::get_process_time() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF,&usage)!=0) {
        throw std::runtime_error("Unable to get process time.");
    }
    double user_time=usage.ru_utime.tv_sec+usage.ru_utime.tv_usec/1e6;
    double sys_time=usage.ru_stime.tv_sec+usage.ru_stime.tv_usec/1e6;

    return user_time+sys_time;
}
//* String utilities
vector<string> vodka::utilities::split(string str,string delimiter) {
    vector<string> tokens;
    size_t start=0;
    size_t end=str.find(delimiter);
    while (end!=string::npos) {
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
void vodka::utilities::replaceall(string str,string from,string to) {
    size_t start_pos=0;
    while ((start_pos=str.find(from, start_pos))!=string::npos) {
        str.replace(start_pos,from.length(),to);
        start_pos+=to.length();
    }
}
//* UUID generator
boost::uuids::uuid vodka::utilities::genuid() {
    boost::uuids::random_generator ran;
    boost::uuids::uuid uuid=ran();
    return uuid;
}
//* Logs functions
void vodka::utilities::log(string text,int log_main_step,string last,int sublevel,vector<int> substep,vector<unsigned long> subtotal) {
    const char* format=getenv("VODKA_SHOW_LOG_TIME");
    string time;
    if (format!=nullptr && string(format)=="TRUE") {
        time="["+to_string(get_process_time())+"]";
    } else {
        time="";
    }
    format=getenv("VODKA_VERBOSE_MODE");
    string verbose;
    if (format!=nullptr) {
        if (string(format)=="e" || string(format)=="a" || string(format)=="r") {
            verbose=string(format);
        } else {
            verbose="e";
        }
    } else {
        verbose="e";
    }
    if (verbose=="a" || verbose=="r") {
        if (sublevel==0) {
            if (verbose=="a" || verbose=="r") {
                cout<<endl<<time+"[LOG]     ";
                string texti;
                if (verbose=="r" && text.substr(text.length()-1,1)==":") {
                    texti=text.substr(0,text.length()-2)+"...";
                } else {
                    texti=text;
                }
                last="("+to_string(log_main_step)+"/19) "+texti;
                cout<<last;
            }
        } else {
            if (verbose=="a") {
                cout<<endl<<time+"[LOG]     ";
                last="("+to_string(log_main_step)+"/19) ";
                for (int i=0;i<sublevel;++i) {
                    last=last+"("+to_string(substep[i])+"/"+to_string(subtotal[i])+") ";
                }
                last=last+text;
                cout<<last;
            }
        }
    }
}
void vodka::utilities::debuglog(string text,int line,string cell,string file,bool debug_info) {
    const char* format=getenv("VODKA_SHOW_LOG_TIME");
    string time;
    if (format!=nullptr && string(format)=="TRUE") {
        time="["+to_string(get_process_time())+"]";
    } else {
        time="";
    }
    format=getenv("VODKA_DEBUG_MODE");
    bool debug_mode;
    if (format!=nullptr && string(format)=="TRUE") {
        debug_mode=true;
    } else {
        debug_mode=false;
    }
    format=getenv("VODKA_VERBOSE_MODE");
    string verbose;
    if (format!=nullptr) {
        if (string(format)=="e" || string(format)=="a" || string(format)=="r") {
            verbose=string(format);
        } else {
            verbose="e";
        }
    } else {
        verbose="e";
    }
    if (debug_mode==true) {
        if (debug_info==true) {
            if (verbose=="e") {
                string texti=text.substr(1,text.length()-1);
                cout<<time+"[DEBUG]   Debug line "+to_string(line)+" in cell "<<termcolor::magenta<<termcolor::bold<<cell<<termcolor::reset<<" from file "<<termcolor::blue<<termcolor::bold<<filesystem::absolute(file).string()<<termcolor::reset<<" : "+texti<<endl;
            } else if (verbose=="r") {
                string texti=text.substr(1,text.length()-1);
                cout<<endl<<time+"[DEBUG]   Debug line "+to_string(line)+" in cell "<<termcolor::magenta<<termcolor::bold<<cell<<termcolor::reset<<" from file "<<termcolor::blue<<termcolor::bold<<filesystem::absolute(file).string()<<termcolor::reset<<" : "+texti<<endl;
            } else if (verbose=="a") {
                string texti=text.substr(1,text.length()-1);
                cout<<endl<<time+"[DEBUG]   Debug line "+to_string(line)+" in cell "<<termcolor::magenta<<termcolor::bold<<cell<<termcolor::reset<<" from file "<<termcolor::blue<<termcolor::bold<<filesystem::absolute(file).string()<<termcolor::reset<<" : "+texti;
            }
        } else {
            if (verbose=="e") {
                string texti=text.substr(2,text.length()-2);
                cout<<time+"[DEBUG]   "<<texti<<endl;
            } else if (verbose=="r") {
                string texti=text.substr(2,text.length()-2);
                cout<<endl<<time+"[DEBUG]   "<<texti<<endl;
            } else if (verbose=="a") {
                string texti=text.substr(2,text.length()-2);
                cout<<endl<<time+"[DEBUG]   "<<texti;
            }
        }
    }
}
void vodka::utilities::var_warning(string namevar,vodka::variables::VariableDatatype typevar,string namecell,string line) {
    const char* format=getenv("VODKA_SHOW_LOG_TIME");
    string time;
    if (format!=nullptr && string(format)=="TRUE") {
        time="["+to_string(get_process_time())+"]";
    } else {
        time="";
    }
    format=getenv("VODKA_SHOW_VAR_WARNING");
    bool var_warning_enabled;
    if (format!=nullptr && string(format)=="TRUE") {
        var_warning_enabled=true;
    } else {
        var_warning_enabled=false;
    }
    format=getenv("VODKA_VERBOSE_MODE");
    string verbose;
    if (format!=nullptr) {
        if (string(format)=="e" || string(format)=="a" || string(format)=="r") {
            verbose=string(format);
        } else {
            verbose="e";
        }
    } else {
        verbose="e";
    }
    if (var_warning_enabled==true) {
        if (verbose=="e") {
            cout<<time+"[WARNING] vodka.warnings.unused_variable : Variable "<<termcolor::bold<<namevar+" ("+vodka::variables::datatype_to_string(typevar)+")"<<termcolor::reset<<", declared line "<<termcolor::bold<<line<<termcolor::reset<<" in cell "<<termcolor::magenta<<termcolor::bold<<namecell<<termcolor::reset<<", isn't used anywhere and may take useless memory."<<endl;
        } else if (verbose=="r") {
            cout<<endl<<time+"[WARNING] vodka.warnings.unused_variable : Variable "<<termcolor::bold<<namevar+" ("+vodka::variables::datatype_to_string(typevar)+")"<<termcolor::reset<<", declared line "<<termcolor::bold<<line<<termcolor::reset<<" in cell "<<termcolor::magenta<<termcolor::bold<<namecell<<termcolor::reset<<", isn't used anywhere and may take useless memory."<<endl;
        } else if (verbose=="a") {
            cout<<endl<<time+"[WARNING] vodka.warnings.unused_variable : Variable "<<termcolor::bold<<namevar+" ("+vodka::variables::datatype_to_string(typevar)+")"<<termcolor::reset<<", declared line "<<termcolor::bold<<line<<termcolor::reset<<" in cell "<<termcolor::magenta<<termcolor::bold<<namecell<<termcolor::reset<<", isn't used anywhere and may take useless memory."<<endl;
        }
    }
}