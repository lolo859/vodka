#include "vodka-lib.h"
#include "../dependencies/termcolor.hpp"
#include "../dependencies/base85.h"
#include "../dependencies/XoshiroCpp.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <boost/crc.hpp>
#include <filesystem>
#include <sys/resource.h>
#include <stdexcept>
#include <cstdlib>
extern "C" {
    #include "../dependencies/xxhash.h"
};
using namespace std;
double vodka::utilities::get_process_time() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF,&usage)!=0) {
        throw runtime_error("Unable to get process time.");
    }
    double user_time=usage.ru_utime.tv_sec+usage.ru_utime.tv_usec/1e6;
    double sys_time=usage.ru_stime.tv_sec+usage.ru_stime.tv_usec/1e6;
    return user_time+sys_time;
}
//* String utilities
vector<string> vodka::utilities::string_utilities::split(string str,string delimiter) {
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
void vodka::utilities::string_utilities::replaceall(string str,string from,string to) {
    size_t start_pos=0;
    while ((start_pos=str.find(from, start_pos))!=string::npos) {
        str.replace(start_pos,from.length(),to);
        start_pos+=to.length();
    }
}
string vodka::utilities::string_utilities::strip(string text,string character) {
    if (character.size()!=1) {
        return nullptr;
    }
    char target=character[0];
    size_t i=0;
    while (i<text.size() && text[i]==target) {
        ++i;
    }
    return text.substr(i);
}
//* UUID generator
string vodka::utilities::genvyid(XoshiroCpp::Xoshiro256StarStar& gen,vodka::utilities::structs::random_values& rand) {
    uniform_int_distribution<uint64_t> dist(0,0xFFFFFFFFFFFFFFFFULL);
    rand.rand1=dist(gen);
    rand.rand2=dist(gen);
    rand.rand3=dist(gen);
    uint64_t seed=rand.rand1^rand.rand2^rand.rand3;
    char sign1[16];
    char sign2[16];
    char sign3[16];
    memcpy(sign1,&rand.rand2,8);
    memcpy(sign1+8,&rand.rand3,8);
    uint64_t sign1h=XXH3_64bits_withSeed(sign1,sizeof(sign1),seed);
    memcpy(sign2,&rand.rand1,8);
    memcpy(sign2+8,&rand.rand3,8);
    uint64_t sign2h=XXH3_64bits_withSeed(sign2,sizeof(sign2),seed);
    memcpy(sign3,&rand.rand1,8);
    memcpy(sign3+8,&rand.rand2,8);
    uint64_t sign3h=XXH3_64bits_withSeed(sign3,sizeof(sign3),seed);
    uint64_t lenc1=rand.rand1^sign1h;
    uint64_t lenc2=rand.rand2^sign2h;
    uint64_t lenc3=rand.rand3^sign3h;
    uint32_t enc1=lenc1>>32;
    uint32_t enc2=lenc2>>32;
    uint32_t enc3=lenc3>>32;
    char total[12];
    memcpy(total,&enc1,4);
    memcpy(total+4,&enc2,4);
    memcpy(total+8,&enc3,4);
    auto sumtotal=XXH3_64bits_withSeed(total,sizeof(total),seed);
    uint32_t trunctotal=sumtotal>>32;
    char grph[4];
    memcpy(grph,&trunctotal,4);
    auto sumgrp=XXH3_64bits_withSeed(grph,sizeof(grph),seed);
    uint32_t truncgrp=sumgrp>>32;
    char output[70];
    char grp[6];
    char hash[6];
    char enchar1[6];
    char enchar2[6];
    char enchar3[6];
    char rand1[11];
    char rand2[11];
    char rand3[11];
    base85::uint32_to_b85(truncgrp,grp);
    base85::uint32_to_b85(enc1,enchar1);
    base85::uint32_to_b85(enc2,enchar2);
    base85::uint32_to_b85(enc3,enchar3);
    base85::uint64_to_b85(rand.rand1,rand1);
    base85::uint64_to_b85(rand.rand2,rand2);
    base85::uint64_to_b85(rand.rand3,rand3);
    base85::uint32_to_b85(trunctotal,hash);
    int offset=0;
    output[offset++]='[';
    memcpy(output+offset,grp,5);
    offset+=5;
    output[offset++]='-';
    memcpy(output+offset,enchar1,5);
    offset+=5;
    memcpy(output+offset,rand1,10);
    offset+=10;
    output[offset++]='-';
    memcpy(output+offset,enchar2,5);
    offset+=5;
    memcpy(output+offset,rand2,10);
    offset+=10;
    output[offset++]='-';
    memcpy(output+offset,enchar3,5);
    offset+=5;
    memcpy(output+offset,rand3,10);
    offset+=10;
    output[offset++]='-';
    memcpy(output+offset,hash,5);
    offset+=5;
    output[offset++]=']';
    output[offset]='\0';
    return string(output);
}
string vodka::utilities::genvyid() {
    static thread_local XoshiroCpp::Xoshiro256StarStar ran = []() {random_device rd;
        mt19937_64 seeder(rd());
        array<uint64_t,4> seed_data={seeder(),seeder(),seeder(),seeder()};
        return XoshiroCpp::Xoshiro256StarStar(seed_data);
    }();
    static thread_local vodka::utilities::structs::random_values rands;
    return genvyid(ran,rands);
}
//* Logs functions
void vodka::utilities::output::log(string text,int log_main_step,string last,int sublevel,vector<int> substep,vector<unsigned long> subtotal) {
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
void vodka::utilities::output::debuglog(string text,int line,string cell,string file,bool debug_info) {
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
void vodka::utilities::output::var_warning(string namevar,vodka::variables::VariableDatatype typevar,string namecell,string line) {
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