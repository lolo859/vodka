#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <unistd.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <nlohmann/json.hpp>
#include <cctype>
#include <cstring>
#include <chrono>
//* Some variables
using namespace std;
string verbose="e";
bool debugmode=false;
int x=0;
string last;
string sublast;
string file;
//* Vodka standard utilities
namespace vodka {
    //* Syscall utilities (instruction starting with "kernel.<something>")
    namespace syscalls {
        class syscall {
            public:
                string name;
                bool support_multiple_argument;
        };
        class PRINT {
            public:
                vector<string> argument_uid;
                syscall info;
            PRINT() {
                info.name="PRINT";
                info.support_multiple_argument=true;
            }
        };
        class ADD {
            public:
                vector<string> argument_uid;
                string output_uid;
                syscall info;
            ADD() {
                info.name="ADD";
                info.support_multiple_argument=true;
            }
        };
        class ASSIGN {
            public:
                string value;
                string output_uid;
                syscall info;
            ASSIGN() {
                info.name="ASSIGN";
                info.support_multiple_argument=true;
            }
        };
        class FREE {
            public:
                vector<string> args_uid;
                syscall info;
            FREE() {
                info.name="FREE";
                info.support_multiple_argument=true;
            }
        };
        class INVERT {
            public:
                string uid;
                syscall info;
            INVERT() {
                info.name="INVERT";
                info.support_multiple_argument=false;
            }
        };
        class BACK {
            public:
                string var_uid;
                string const_uid;
                string back_uid;
                syscall info;
            BACK() {
                info.name="BACK";
                info.support_multiple_argument=true;
            }
        };
        class DUPLICATE {
            public:
                string source_uid;
                string output_uid;
                syscall info;
            DUPLICATE() {
                info.name="DUPLICATE";
                info.support_multiple_argument=true;
            }
        };
        class ABS {
            public:
                string args_uid;
                syscall info;
            ABS() {
                info.name="ABS";
                info.support_multiple_argument=true;
            }
        };
        class DIVMOD {
            public:
                string quouid;
                string resuid;
                string divis;
                string divid;
                syscall info;
            DIVMOD() {
                info.name="DIVMOD";
                info.support_multiple_argument=true;
            }
        };
        class syscall_container {
            public:
                string thing;
                PRINT printele;
                ADD addele;
                ASSIGN assignele;
                FREE freeele;
                INVERT invertele;
                BACK backele;
                DUPLICATE duplicateele;
                ABS absele;
                DIVMOD divmodele;
            string syntax() {
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
                    for (auto a:freeele.args_uid) {
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
                    return absele.info.name+" "+absele.args_uid;
                } else if (thing=="DIVMOD") {
                    return divmodele.info.name+" "+divmodele.quouid+" "+divmodele.resuid+" "+divmodele.divid+" "+divmodele.divis;
                } else {
                    return "error";
                }
            }
        };
    }
    //* Variables utilities
    namespace variables {
        class typess {
            public:
                string typenames;
        };
        class variable {
            public:
                string varname;
                string uuid;
                bool consts;
                bool write=true;
                bool define=false;
                bool algo_dependant;
        };
        class vodint {
            public:
                string value;
                variable varinfo;
                typess typeinfo;
            vodint() {
                typeinfo.typenames="vodint";
            }
        };
        class element {
            public:
                string thing;
                vodint intele;
        };
    }
    //* Type utilities
    namespace type {
        //* Vodint utilities
        namespace vodint {
            bool check_value(const string& value) {
                if (value.empty() || value=="-") {
                    return false;
                }
                vector<char> num={'0','1','2','3','4','5','6','7','8','9','-'};
                for (auto v=0;v<value.length();++v) {
                    if (v!=0 && value[v]=='-') {
                        return false;
                    }
                    if (find(num.begin(),num.end(),value[v])==num.end()) {
                        return false;
                    }
                }
                return true;
            }
            string remove_zero(const string& value) {
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
            string invert_value(const string& value) {
                string out;
                if (value.substr(0,1)=="-") {
                    out=value.substr(1,value.length()-1);
                } else {
                    out="-"+value;
                }
                return out;
            }
            string calculate_sign(const string& value1,const string& value2) {
                if (value1.substr(0,1)!="-" && value2.substr(0,1)!="-") {
                    return "";
                } else if (value1.substr(0,1)=="-" && value2.substr(0,1)=="-") {
                    return "";
                } else {
                    return "-";
                }
            }
            string abs(const string& value) {
                if (value.substr(0,1)=="-") {
                    return value.substr(1,value.length()-1);
                } else {
                    return value;
                }
            }
        }
    }
    //* Json utilities
    namespace json {
        class json_container {
            public:
                string type;
                string intname;
                vector<string> args;
            map<string,string> syntax() {
                map<string,string> out;
                out.insert(pair<string,string>("type",type));
                out.insert(pair<string,string>("intname",intname));
                for (int i=0;i<args.size();++i) {
                    out.insert(pair<string,string>("arg"+to_string(i+1),args[i]));
                }
                return out;
            };
        };
    }
}
//* Error function
void error(const string& error,const string& file,vector<string> lines_content,vector<int> lines) {
    string linestr="";
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
    cout<<"\nFile "+file+":"+to_string(lines[0])+", line(s) "+linestr+" an error occured :"<<endl;
    for (size_t y=0;y<lines.size();++y) {
        cout<<"  "<<lines[y]<<" | "<<lines_content[y]<<endl;
    }
    cout<<error<<endl;
}
//* Utilities
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
//* Vodka structure
struct symbol {
    int line;
    string content;
    string type;
};
struct cellule {
    vector<string> content;
    string name;
    vector<string> args;
    vector<string> outs;
    symbol start;
    symbol end;
};
struct import {
    string file;
    string type;
    string importas;
    vector<string> content;
};
boost::uuids::uuid genuid() {
    boost::uuids::random_generator ran;
    boost::uuids::uuid uuid=ran();
    return uuid;
}
//* Logs functions
void log(const string& text,int sublevel=0,vector<int> substep={},vector<unsigned long> subtotal={}) {
    if (verbose=="a" || verbose=="r") {
        if (sublevel==0) {
            if (verbose=="a" || verbose=="r") {
                string texti;
                if (verbose=="r" && text.substr(text.length()-1,1)==":") {
                    texti=text.substr(0,text.length()-2)+"...";
                } else {
                    texti=text;
                }
                if (x!=0) {
                    cout<<endl<<endl;
                }
                x=x+1;
                last="("+to_string(x)+"/18) "+texti;
                cout<<last;
            }
        } else {
            if (verbose=="a") {
                cout<<endl;
                last="("+to_string(x)+"/18) ";
                for (int i=0;i<sublevel;++i) {
                    last=last+"("+to_string(substep[i])+"/"+to_string(subtotal[i])+") ";
                }
                last=last+text;
                cout<<last;
            }
        }
    }
}
void check() {
    if (verbose=="r" || verbose=="a") {
        cout<<"\r"<<last<<" Done";
    }
}
void debuglog(const string& text,int line,const string& cell,bool debug_info=true) {
    if (debugmode==true) {
        if (debug_info==true) {
            if (verbose=="e") {
                string texti=text.substr(1,text.length()-1);
                cout<<"Debug line "+to_string(line)+" in cell "+cell+" from file "+filesystem::absolute(file).string()+" : "+texti<<endl;
            } else if (verbose=="r") {
                string texti=text.substr(1,text.length()-1);
                cout<<endl<<"Debug line "+to_string(line)+" in cell "+cell+" "+filesystem::absolute(file).string()+" : "+texti<<endl;
            } else if (verbose=="a") {
                string texti=text.substr(1,text.length()-1);
                cout<<endl<<"Debug line "+to_string(line)+" in cell "+cell+" "+filesystem::absolute(file).string()+" : "+texti;
            }
        } else {
            if (verbose=="e") {
                string texti=text.substr(2,text.length()-2);
                cout<<texti<<endl;
            } else if (verbose=="r") {
                string texti=text.substr(2,text.length()-2);
                cout<<endl<<texti<<endl;
            } else if (verbose=="a") {
                string texti=text.substr(2,text.length()-2);
                cout<<endl<<texti;
            }
        }
    }
}
//* Some vectors and maps
vector<string> mainargs;
vector<string> mainargsname;
map<string,string> mainargsdict;
vector<vodka::syscalls::syscall_container> instructions;
map<string,vodka::variables::element> variablesdict;
map<string,string> replacement;
vector<string> variableslist;
vector<string> symbollist={"VODSTART","VODEND","VODIMPORT","VODTYPE","VODSTRUCT","VODCLASS","VODENDCLASS","VODEFINE"};
vector<string> typelist={"app","command","shell","gui","logonui","logonshell","service"};
vector<string> final;
map<string,map<string,string>> json_ints;
//* Main function
int main (int argc,char* argv[]) {
    string output="";
    int option;
    bool replace=true;
    string mode="compile";
    string finde;
    opterr=0;
    //* Args management
    while ((option=getopt(argc,argv,"hjrdvVf:s:o:"))!=-1) {
        switch (option) {
        case 'h':
            cout<<"Vodka v0.2 beta 6 - Vodka Objective Dictionary for Kernel Analyser\nHow to use : vodka [-h] [-f object_to_find (not working for the moment)] [-s source file] [-o output file] [-v set verbose mode to reduced] [-V set verbose mode to all] [-d enable debug mode] [-j export output to a json file specified with -o] [-r disable define replacement]"<<endl;
            return 0;
        case 'f':
            mode="find";
            finde=optarg;
            break;
        case 's':
            file=optarg;
            break;
        case 'o':
            output=optarg;
            break;
        case 'v':
            verbose="r";
            break;
        case 'V':
            verbose="a";
            break;
        case 'd':
            debugmode=true;
            break;
        case 'j':
            mode="json";
            break;
        case 'r':
            replace=false;
            break;
        case '?':
            cout<<"Invalid argument."<<endl;
            return -1;
        default:
            return -1;;
        }
    }
    //* Source file fetching
    if (output=="" && mode=="compile") {
        cout<<"Please specify an output file for compiled code."<<endl;
        return -1;
    }
    log("Checking if source file exist...");
    if (filesystem::exists(file)==false) {
        cout<<"Source file doesn't exitst."<<endl;
        return -1;
    }
    check();
    log("Checking file readability...");
    ifstream filee(file.c_str());
    if (filee.is_open()==false) {
        cout<<"File can't be open."<<endl;
        return -1;
    }
    check();
    log("Reading file...");
    string lineread;
    vector<string> content;
    while (getline(filee,lineread)) {
        content.push_back(lineread);
    }
    check();
    log("Removing comments...");
    for (int i=0;i<content.size();++i) {
        content[i]=split(content[i],"§")[0];
    }
    check();
    filee.close();
    log("Detecting symbols...");
    vector<symbol> symbols;
    for (size_t i=0;i<content.size();++i) {
        string line=content[i];
        log("Detecting if line "+to_string(i+1)+" contain symbol :",1,{(int)i+1},{content.size()});
        if (line.rfind("£",0)==0) {
            log("Allocating memory...",2,{(int)i+1,1},{content.size(),3});
            symbol temp;
            temp.line=i+1;
            temp.content=line;
            auto ele=split(line," ");
            temp.type=ele[0].substr(2,ele[0].size());
            check();
            log("Checking symbol...",2,{(int)i+1,2},{content.size(),3});
            if (find(symbollist.begin(),symbollist.end(),temp.type)==symbollist.end()) {
                int linenum=i+1;
                error("vodka.error.symbol.unknow_symbol : Unknow symbol found : "+temp.type,file,{line},{linenum});
                return -1;
            } else {
                check();
                log("Saving symbol...",2,{(int)i+1,3},{content.size(),3});
                symbols.push_back(temp);
                check();
            }
        } else {
            log("No symbol detected.",2,{(int)i+1,1},{content.size(),1});
        }
    }
    log("Searching define replacement instruction :");
    for (size_t i=0;i<symbols.size();++i) {
        log("Checking symbol type :",1,{(int)i+1},{symbols.size()});
        if (symbols[i].type=="VODEFINE") {
            auto eles=split(symbols[i].content," ");
            if (eles.size()==3) {
                replacement[eles[1]]=eles[2];
                log("Replacement found. "+eles[1]+" will be replaced with "+eles[2]+".",2,{(int)i+1,1},{symbols.size(),1});
            } else {
                error("vodka.error.vodefine.invalid_syntax : Invalid syntax for define replacement declaration.",file,{symbols[i].content},{symbols[i].line});
                return -1;
            }
        } else {
            log("No VODEFINE symbol detected.",2,{(int)i+1,1},{symbols.size(),1});
        }
    }
    log("Looking for type symbol :");
    bool typefound=false;
    for (size_t i=0;i<symbols.size();++i) {
        log("Checking symbol type...",1,{(int)i+1},{symbols.size()});
        if (symbols[i].type=="VODTYPE") {
            if (typefound==false) {
                typefound=true;
            } else {
                error("vodka.error.vodtype.confusion : A vodka program can only contain one VODTYPE symbol.",file,{symbols[i].content},{symbols[i].line});
                return -1;
            }
        }
        check();
    }
    if (typefound==false) {
        error("vodka.error.vodtype.not_found : Can't find VODTYPE symbol.",file,{"<no line affected>"},{0});
        return -1;
    }
    log("Detecting program type :");
    string program_type;
    for (size_t i=0;i<symbols.size();++i) {
        log("Checking if line contain type symbol :",1,{(int)i+1},{symbols.size()});
        if (symbols[i].type=="VODTYPE") {
            log("Checking syntax...",2,{(int)i+1,1},{symbols.size(),3});
            auto ele=split(symbols[i].content," ");
            if (ele.size()!=2) {
                error("vodka.error.vodtype.invalid_syntax : Invalid syntax for program type declaration.",file,{symbols[i].content},{symbols[i].line});
                return -1;
            }
            check();
            log("Checking program type...",2,{(int)i+1,2},{symbols.size(),3});
            if (find(typelist.begin(),typelist.end(),ele[1])==typelist.end()) {
                error("vodka.error.vodtype.unknow_type : Unknow type : "+ele[1],file,{symbols[i].content},{symbols[i].line});
                return -1;
            }
            check();
            log("Saving program type...",2,{(int)i+1,3},{symbols.size(),3});
            program_type=ele[1];
            check();
        } else {
            log("The symbol isn't the type symbol.",2,{(int)i+1,1},{symbols.size(),1});
        }
    }
    final.push_back("type "+program_type);
    log("Detecting cells :");
    vector<cellule> cells;
    vector<string> cellnames;
    cellule maincell;
    for (size_t i=0;i<symbols.size();++i) {
        if (symbols[i].type=="VODSTART") {
            log("Checking start symbol syntax...",1,{1},{10});
            if (symbols.size()>=i+2 && symbols[i+1].type=="VODEND") {
                check();
                log("Allocating memory...",1,{2},{10});
                cellule temp;
                check();
                log("Detecting cell start and end symbol...",1,{3},{10});
                temp.start=symbols[i];
                temp.end=symbols[i+1];
                check();
                log("Detecting cell name...",1,{4},{10});
                auto args=split(symbols[i].content," ");
                if (args.size()>1) {
                    temp.name=args[1];
                    check();
                } else {
                    error("vodka.error.vodstart.name_not_found : Can't find cell's name.",file,{symbols[i].content},{symbols[i].line});
                    return -1;
                }
                log("Checking if an cell with the same name already exist in the program...",1,{5},{10});
                auto it=find(cellnames.begin(),cellnames.end(),temp.name);
                vector<string> contenterror;
                vector<int> lineerror;
                vector<size_t> indicatorpos;
                vector<size_t> indicatorlen;
                if (it!=cellnames.end()) {
                    for (size_t y=0;y<cells.size();++y) {
                        if (cells[y].name==temp.name) {
                            contenterror.push_back(cells[y].start.content);
                            lineerror.push_back(cells[y].start.line);
                            indicatorpos.push_back(11+to_string(cells[y].start.line).length()-1);
                            indicatorlen.push_back(args[1].length());
                        }
                    } 
                    contenterror.push_back(temp.start.content);
                    lineerror.push_back(temp.start.line);
                    indicatorpos.push_back(11+to_string(temp.start.line).length()-1);
                    indicatorlen.push_back(args[1].length());
                    error("vodka.error.cell.confusion : An existing cell with the same name already exists.",file,contenterror,lineerror);
                    return -1;
                }
                check();
                log("Detecting cell argument...",1,{6},{10});
                if (args.size()>2) {
                    temp.args=vector<string>(args.begin()+2,args.end());
                }
                check();
                log("Detecting cell output...",1,{7},{10});
                auto outs=split(temp.end.content," ");
                if (outs.size()>1) {
                    temp.outs=vector<string>(outs.begin()+1,outs.end());
                }
                check();
                log("Saving cell content...",1,{8},{10});
                int startline=temp.start.line;
                int endline=temp.end.line-2;
                for (int i=startline;i<endline+1;++i) {
                    temp.content.push_back(content[i]);
                }
                check();
                log("Saving cell...",1,{9},{10});
                cells.push_back(temp);
                cellnames.push_back(temp.name);
                check();
                log("Detecting if cell is the main cell...",1,{10},{10});
                if (temp.name=="main") {
                    maincell=temp;
                    for (auto a:maincell.args) {
                        auto uid=to_string(genuid());
                        mainargs.push_back(uid);
                        mainargsdict[a]=uid;
                        mainargsname.push_back(a);
                    }
                }
                check();
            } else {
                error("vodka.error.vodend.not_found : Can't find corresponding VODEND symbol to the previous VODSTART.",file,{symbols[i].content},{symbols[i].line});
                return -1;
            }
        }
    }
    log("Verifying if program contain a main cell...");
    if (maincell.name!="main") {
        error("vodka.error.main.not_found : Can't find the main cell.",file,{"<no line affected>"},{0});
        return -1;
    }
    check();
    final.push_back("args:");
    log("Writing args section...");
    for (string argm:mainargs) {
        final.push_back(argm);
    }
    check();
    if (replace==false) {
        log("Skipping replacement step because of -r.");
    } else {
        log("Started define replacing process...");
        for (int i=0;i<maincell.content.size();++i) {
            for (auto y:replacement) {
                replaceall(maincell.content[i],y.first,y.second);
            }
        }
        for (auto u:cells) {
            for (int i=0;i<u.content.size();++i) {
                for (auto y:replacement) {
                    replaceall(maincell.content[i],y.first,y.second);
                }
            }
        }
        check();
    }
    log("Started analysing main cell :");
    //* Here happen all the magic
    final.push_back("endargs");
    for (size_t i=0;i<maincell.content.size();++i) {
        string line=maincell.content[i];
        log("Analysing line "+to_string(i+1)+" :",1,{(int)i+1},{maincell.content.size()});
        //* Debug line processing
        if (line.substr(0,1)==">" && line.substr(0,2)!=">>") {
            debuglog(line,maincell.start.line+(int)i+1,"main");
        } else if (line.substr(0,2)==">>") {
            debuglog(line,maincell.start.line+(int)i+1,"main",false);
        } 
        //* Vodka instruction processing
        else if (line.substr(0,5)=="vodka") {
            log("Checking vodka declaration syntax...",2,{(int)i+1,1},{maincell.content.size(),8});
            auto eles=split(line,"=");
            if (eles.size()==2) {
                check();
                log("Parsing vodka declaration...",2,{(int)i+1,2},{maincell.content.size(),8});
                auto namepart=split(eles[0]," ");
                auto valuepart=split(eles[1]," ");
                if (namepart.size()==3) {
                    namepart.pop_back();
                }
                if (valuepart[0]=="") {
                    valuepart.erase(valuepart.begin());
                }
                check();
                log("Parsing variable information...",2,{(int)i+1,3},{maincell.content.size(),8});
                if (namepart.size()==2 && valuepart.size()>=2) {
                    string name=namepart[1];
                    string typevar=valuepart[0];
                    check();
                    log("Allocating memory for variable...",2,{(int)i+1,4},{maincell.content.size(),8});
                    vodka::variables::variable var;
                    check();
                    log("Detecting type...",2,{(int)i+1,5},{maincell.content.size(),8});
                    if (typevar=="vodint") {
                        check();
                        log("Detecting variable context...",2,{(int)i+1,6},{maincell.content.size(),8});
                        var.uuid=to_string(genuid());
                        if (name.substr(0,2)=="$$") {
                            var.consts=true;
                            var.define=true;
                            var.algo_dependant=false;
                        } else if (name.substr(0,1)=="$") {
                            var.consts=true;
                            var.algo_dependant=false;
                        } else {
                            var.consts=false;
                            var.algo_dependant=false;
                        }
                        for (auto const& emap:variablesdict) {
                            if (emap.first==name && emap.second.intele.varinfo.consts==true) {
                                error("vodka.error.variables.constant : Can't modify an constant.",file,{line},{maincell.start.line+(int)i+1});
                                return -1;
                            }
                        }
                        check();
                        log("Checking value...",2,{(int)i+1,7},{maincell.content.size(),8});
                        vodka::variables::vodint varint;
                        varint.varinfo=var;
                        if (vodka::type::vodint::check_value(valuepart[1])) {
                            varint.value=vodka::type::vodint::remove_zero(valuepart[1]);
                        } else {
                            error("vodka.error.vodint.invalid_value : Invalid value : "+valuepart[1],file,{line},{maincell.start.line+(int)i+1});
                            return -1;
                        }
                        check();
                        log("Saving variable...",2,{(int)i+1,8},{maincell.content.size(),8});
                        if (find(variableslist.begin(),variableslist.end(),name)==variableslist.end()) {
                            vodka::variables::element contvar;
                            contvar.intele=varint;
                            contvar.thing=typevar;
                            variablesdict[name]=contvar;
                            variableslist.push_back(name);
                            if (var.define==false) {
                                vodka::syscalls::ASSIGN asscall;
                                asscall.output_uid=var.uuid;
                                asscall.value=varint.value;
                                vodka::syscalls::syscall_container asscont;
                                asscont.thing="ASSIGN";
                                asscont.assignele=asscall;
                                instructions.push_back(asscont);
                            }
                        } else {
                            auto varuid=variablesdict[name].intele.varinfo.uuid;
                            vodka::syscalls::ASSIGN asscall;
                            asscall.output_uid=varuid;
                            asscall.value=varint.value;
                            vodka::syscalls::syscall_container syscont;
                            syscont.thing="ASSIGN";
                            syscont.assignele=asscall;
                            instructions.push_back(syscont);
                        }
                        check();
                    } else if (typevar=="vodka") {
                        check();
                        log("Verifying content existence...",2,{(int)i+1,6},{maincell.content.size(),8});
                        if (valuepart.size()!=2) {
                            error("vodka.error.variables.multiples_variables : vodka type can only duplicate one variable at the time.",file,{line},{maincell.start.line+(int)i+1});
                            return -1;
                        }
                        if (find(variableslist.begin(),variableslist.end(),valuepart[1])==variableslist.end()) {
                            error("vodka.error.variables.not_declared : "+valuepart[1]+" wasn't declared before declaration.",file,{line},{maincell.start.line+(int)i+1});
                            return -1;
                        }
                        vodka::variables::variable var;
                        var.uuid=to_string(genuid());
                        var.write=false;
                        if (name.substr(0,2)=="$$") {
                            error("vodka.error.variables.kernel_constant : Can't create kernel constant from variables duplication.",file,{line},{maincell.start.line+(int)i+1});
                        } else if (name.substr(0,1)=="$") {
                            var.consts=true;
                        } else {
                            var.consts=false;
                        }
                        var.varname=name;
                        if (variablesdict[valuepart[1]].thing=="vodint") {
                            var.algo_dependant=variablesdict[valuepart[1]].intele.varinfo.algo_dependant;
                            vodka::variables::vodint varint;
                            varint.varinfo=var;
                            varint.value=variablesdict[valuepart[1]].intele.value;
                            vodka::variables::element varcont;
                            varcont.thing="vodint";
                            varcont.intele=varint;
                            variablesdict[name]=varcont;
                            variableslist.push_back(name);
                            vodka::syscalls::DUPLICATE dupcall;
                            dupcall.output_uid=var.uuid;
                            dupcall.source_uid=variablesdict[valuepart[1]].intele.varinfo.uuid;
                            vodka::syscalls::syscall_container dupcont;
                            dupcont.thing="DUPLICATE";
                            dupcont.duplicateele=dupcall;
                            instructions.push_back(dupcont);
                        }
                    } else {
                        error("vodka.error.variables.unknow_type : Unknow type : "+typevar,file,{line},{maincell.start.line+(int)i+1});
                        return -1;
                    }
                } else {
                    error("vodka.error.variables.invalid_syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1});
                    return -1;
                }
            } else {
                error("vodka.error.variables.invalid_syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1});
                return -1;
            }
        //* Kernel function analysis
        } else if (line.substr(0,12)=="kernel.print") {
            log("Checking system call syntax...",2,{(int)i+1,1},{maincell.content.size(),3});
            auto eles=split(line," ");
            if (eles.size()>=2) {
                check();
                log("Checking content existence...",2,{(int)i+1,2},{maincell.content.size(),3});
                vector<string> eltoprint(eles.begin()+1,eles.end());
                for (auto a:eltoprint) {
                    if (find(variableslist.begin(),variableslist.end(),a)==variableslist.end() && find(mainargsname.begin(),mainargsname.end(),a)==mainargsname.end()) {
                        error("vodka.error.variables.not_declared : "+a+" wasn't declared before instruction.",file,{line},{maincell.start.line+(int)i+1});
                        return -1;
                    }
                }
                check();
                log("Registering system call...",2,{(int)i+1,3},{maincell.content.size(),3});
                vector<string> uidtoprint;
                for (auto a:eltoprint) {
                    if (find(mainargsname.begin(),mainargsname.end(),a)==mainargsname.end()) {
                        uidtoprint.push_back(variablesdict[a].intele.varinfo.uuid);
                    } else {
                        uidtoprint.push_back(mainargsdict[a]);
                    }
                }
                vodka::syscalls::PRINT syscal;
                syscal.argument_uid=uidtoprint;
                vodka::syscalls::syscall_container syscont;
                syscont.thing="PRINT";
                syscont.printele=syscal;
                instructions.push_back(syscont);
                check();
            } else {
                error("vodka.error.kernel.print.invalid syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1});
                return -1;
            }
        } else if (line.substr(0,10)=="kernel.add") {
            log("Checking system call syntax...",2,{(int)i+1,1},{maincell.content.size(),4});
            auto eles=split(line," ");
            if (eles.size()>=4) {
                check();
                log("Checking content existence...",2,{(int)i+1,2},{maincell.content.size(),4});
                vector<string> argsgive(eles.begin()+1,eles.end());
                for (auto a:argsgive) {
                    if (find(variableslist.begin(),variableslist.end(),a)==variableslist.end()) {
                        error("vodka.error.variables.not_declared : "+a+" wasn't declared before instruction.",file,{line},{maincell.start.line+(int)i+1});
                        return -1;
                    }
                }
                if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
                    error("vodka.error.variables.constant : Can't modify an constant.",file,{line},{maincell.start.line+(int)i+1});
                }
                check();
                log("Checking content datatype...",2,{(int)i+1,3},{maincell.content.size(),4});
                for (auto a:argsgive) {
                    if (variablesdict[a].thing!="vodint" || variablesdict[a].intele.typeinfo.typenames!="vodint") {
                        error("vodka.error.kernel.add.wrong_type : "+a+" isn't vodint type.",file,{line},{maincell.start.line+(int)i+1});
                        return -1;
                    }
                }
                check();
                log("Registering system call...",2,{(int)i+1,4},{maincell.content.size(),4});
                vector<string> uidargs;
                for (auto a:argsgive) {
                    uidargs.push_back(variablesdict[a].intele.varinfo.uuid);
                }
                vodka::syscalls::ADD syscal;
                syscal.argument_uid=vector<string>(uidargs.begin()+1,uidargs.end());
                syscal.output_uid=uidargs[0];
                vodka::syscalls::syscall_container syscont;
                syscont.thing="ADD";
                syscont.addele=syscal;
                instructions.push_back(syscont);
                check();
            } else {
                error("vodka.error.kernel.add.invalid_syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1});
                return -1;
            }
        } else if (line.substr(0,13)=="kernel.invert") {
            log("Checking system call syntax...",2,{(int)i+1,1},{maincell.content.size(),4});
            auto eles=split(line," ");
            if (eles.size()==2) {
                check();
                log("Checking content existence...",2,{(int)i+1,2},{maincell.content.size(),4});
                string arg=eles[1];
                if (find(variableslist.begin(),variableslist.end(),arg)==variableslist.end()) {
                    error("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",file,{line},{maincell.start.line+(int)i+1});
                    return -1;
                }
                if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
                    error("vodka.error.variables.constant : Can't modify an constant.",file,{line},{maincell.start.line+(int)i+1});
                }
                check();
                log("Checking content datatype...",2,{(int)i+1,3},{maincell.content.size(),4});
                if (variablesdict[arg].thing!="vodint" || variablesdict[arg].intele.typeinfo.typenames!="vodint") {
                    error("vodka.error.kernel.invert.wrong_type : "+arg+" isn't vodint type.",file,{line},{maincell.start.line+(int)i+1});
                    return -1;
                }
                check();
                log("Registering system call...",2,{(int)i+1,4},{maincell.content.size(),4});
                string uidarg=variablesdict[arg].intele.varinfo.uuid;
                vodka::syscalls::INVERT syscal;
                syscal.uid=uidarg;
                vodka::syscalls::syscall_container syscont;
                syscont.thing="INVERT";
                syscont.invertele=syscal;
                instructions.push_back(syscont);
                check();
            } else {
                error("vodka.error.kernel.invert.invalid_syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1});
                return -1;
            }
        } else if (line.substr(0,8)=="multiply") {
            log("Checking instruction syntax...",2,{(int)i+1,1},{maincell.content.size(),4});
            auto eles=split(line," ");
            if (eles.size()==4) {
                check();
                log("Checking content existence...",2,{(int)i+1,2},{maincell.content.size(),4});
                string arg;
                for (int y=1;y<eles.size();++y) {
                    arg=eles[y];
                    if (find(variableslist.begin(),variableslist.end(),arg)==variableslist.end()) {
                        error("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",file,{line},{maincell.start.line+(int)i+1});
                        return -1;
                    }
                }
                if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
                    error("vodka.error.variables.constant : Can't modify an constant.",file,{line},{maincell.start.line+(int)i+1});
                }
                check();
                log("Checking content datatype...",2,{(int)i+1,3},{maincell.content.size(),4});
                vector<string> argsname(eles.begin()+1,eles.end());
                for (auto a:argsname) {
                    if (variablesdict[a].thing!="vodint" || variablesdict[a].intele.typeinfo.typenames!="vodint") {
                        error("vodka.error.instruction.multiply.wrong_type : "+a+" isn't vodint type.",file,{line},{maincell.start.line+(int)i+1});
                        return -1;
                    }
                }
                check();
                if (variablesdict[eles[2]].intele.varinfo.algo_dependant==false && variablesdict[eles[3]].intele.varinfo.algo_dependant==false) {
                    log("Registering systems call for this instructions...",2,{(int)i+1,4},{maincell.content.size(),4});
                    vector<string> uidargs;
                    for (auto a:argsname) {
                        uidargs.push_back(variablesdict[a].intele.varinfo.uuid);
                    }
                    string outputuid=uidargs[0];
                    vodka::syscalls::ASSIGN assigncall;
                    assigncall.output_uid=outputuid;
                    assigncall.value="0";
                    vodka::syscalls::syscall_container syscont;
                    syscont.thing="ASSIGN";
                    syscont.assignele=assigncall;
                    instructions.push_back(syscont);
                    if (variablesdict[argsname[2]].intele.value!="0") {
                        if (variablesdict[argsname[1]].intele.value.substr(0,1)=="-") {
                            vodka::syscalls::INVERT invertcall;
                            invertcall.uid=uidargs[1];
                            vodka::syscalls::syscall_container syscont;
                            syscont.invertele=invertcall;
                            syscont.thing="INVERT";
                            instructions.push_back(syscont);
                        }
                        if (variablesdict[argsname[2]].intele.value.substr(0,1)=="-") {
                            vodka::syscalls::INVERT invertcall;
                            invertcall.uid=uidargs[2];
                            vodka::syscalls::syscall_container syscont;
                            syscont.invertele=invertcall;
                            syscont.thing="INVERT";
                            instructions.push_back(syscont);
                        }
                        vodka::syscalls::ASSIGN assigncall;
                        string countuid=to_string(genuid());
                        assigncall.output_uid=countuid;
                        assigncall.value="0";
                        vodka::syscalls::syscall_container syscont;
                        syscont.thing="ASSIGN";
                        syscont.assignele=assigncall;
                        instructions.push_back(syscont);
                        vodka::syscalls::ASSIGN assigncall2;
                        string one_const=to_string(genuid());
                        assigncall2.output_uid=one_const;
                        assigncall2.value="1";
                        vodka::syscalls::syscall_container syscont2;
                        syscont2.thing="ASSIGN";
                        syscont2.assignele=assigncall2;
                        instructions.push_back(syscont2);
                        vodka::syscalls::ASSIGN assigncall3;
                        string two_const=to_string(genuid());
                        assigncall3.output_uid=two_const;
                        assigncall3.value="2";
                        vodka::syscalls::syscall_container syscont3;
                        syscont3.thing="ASSIGN";
                        syscont3.assignele=assigncall3;
                        instructions.push_back(syscont3);
                        vector<string> uidtoadd={outputuid,uidargs[1]};
                        vodka::syscalls::ADD addcall;
                        addcall.output_uid=outputuid;
                        addcall.argument_uid=uidtoadd;
                        vodka::syscalls::syscall_container syscont4;
                        syscont4.thing="ADD";
                        syscont4.addele=addcall;
                        instructions.push_back(syscont4);
                        vector<string> uidtoadd2={countuid,one_const};
                        vodka::syscalls::ADD addcall2;
                        addcall2.output_uid=countuid;
                        addcall2.argument_uid=uidtoadd2;
                        vodka::syscalls::syscall_container syscont5;
                        syscont5.thing="ADD";
                        syscont5.addele=addcall2;
                        instructions.push_back(syscont5);
                        vodka::syscalls::BACK backcall;
                        backcall.back_uid=two_const;
                        backcall.var_uid=countuid;
                        backcall.const_uid=uidargs[2];
                        vodka::syscalls::syscall_container syscont6;
                        syscont6.thing="BACK";
                        syscont6.backele=backcall;
                        instructions.push_back(syscont6);
                        vodka::syscalls::FREE freecall;
                        freecall.args_uid={countuid,one_const,two_const};
                        vodka::syscalls::syscall_container syscont7;
                        syscont7.thing="FREE";
                        syscont7.freeele=freecall;
                        instructions.push_back(syscont7);
                        if (vodka::type::vodint::calculate_sign(variablesdict[argsname[1]].intele.value,variablesdict[argsname[2]].intele.value).substr(0,1)=="-") {
                            vodka::syscalls::INVERT invertcall;
                            invertcall.uid=outputuid;
                            vodka::syscalls::syscall_container syscont;
                            syscont.invertele=invertcall;
                            syscont.thing="INVERT";
                            instructions.push_back(syscont);
                        }
                        if (variablesdict[argsname[1]].intele.value.substr(0,1)=="-") {
                            vodka::syscalls::INVERT invertcall;
                            invertcall.uid=uidargs[1];
                            vodka::syscalls::syscall_container syscont;
                            syscont.invertele=invertcall;
                            syscont.thing="INVERT";
                            instructions.push_back(syscont);
                        }
                        if (variablesdict[argsname[2]].intele.value.substr(0,1)=="-") {
                            vodka::syscalls::INVERT invertcall;
                            invertcall.uid=uidargs[2];
                            vodka::syscalls::syscall_container syscont;
                            syscont.invertele=invertcall;
                            syscont.thing="INVERT";
                            instructions.push_back(syscont);
                        }
                    }
                    check();
                } else {
                    error("vodka.error.instruction.multiply.not_constance : Can't multiply variable without knowing their values.",file,{line},{maincell.start.line+(int)i+1});
                    return -1;
                }
            } else {
                error("vodka.error.instruction.multiply.invalid_syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1});
                return -1;
            }        
        } else if (line.substr(0,11)=="kernel.free") {
            log("Checking system call syntax...",2,{(int)i+1,1},{maincell.content.size(),2});
            auto eles=split(line," ");
            if (eles.size()>=2) {
                check();
                log("Checking content existence...",2,{(int)i+1,2},{maincell.content.size(),2});
                string arg;
                for (int y=1;y<eles.size();++y) {
                    arg=eles[y];
                    if (find(variableslist.begin(),variableslist.end(),arg)==variableslist.end()) {
                        error("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",file,{line},{maincell.start.line+(int)i+1});
                        return -1;
                    }
                }
                if (eles[1].substr(0,2)=="$$") {
                    error("vodka.error.variables.constant : Can't delete an constant.",file,{line},{maincell.start.line+(int)i+1});
                }
                check();
                log("Registering system call...",2,{(int)i+1,4},{maincell.content.size(),3});
                vector<string> argsuid;
                for (auto a:vector<string>(eles.begin()+1,eles.end())) {
                    if (variablesdict[a].thing=="vodint") {
                        argsuid.push_back(variablesdict[a].intele.varinfo.uuid);
                    }
                }
                vodka::syscalls::FREE freecall;
                freecall.args_uid=argsuid;
                vodka::syscalls::syscall_container syscont;
                syscont.freeele=freecall;
                syscont.thing="FREE";
                instructions.push_back(syscont);
                for (auto a:freecall.args_uid) {variablesdict.erase(a);variableslist.erase(remove(variableslist.begin(),variableslist.end(),a),variableslist.end());};
                check();
            } else {
                error("vodka.error.kernel.free.invalid_syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1});
                return -1;
            }
        } else if (line.substr(0,10)=="kernel.abs") {
            log("Checking system call syntax...",2,{(int)i+1,1},{maincell.content.size(),4});
            auto eles=split(line," ");
            if (eles.size()==2) {
                check();
                log("Checking content existence...",2,{(int)i+1,2},{maincell.content.size(),4});
                string arg=eles[1];
                if (find(variableslist.begin(),variableslist.end(),arg)==variableslist.end()) {
                    error("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",file,{line},{maincell.start.line+(int)i+1});
                    return -1;
                }
                if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
                    error("vodka.error.variables.constant : Can't modify an constant.",file,{line},{maincell.start.line+(int)i+1});
                }
                check();
                log("Checking content datatype...",2,{(int)i+1,3},{maincell.content.size(),4});
                if (variablesdict[arg].thing!="vodint" || variablesdict[arg].intele.typeinfo.typenames!="vodint") {
                    error("vodka.error.kernel.abs.wrong_type : "+arg+" isn't vodint type.",file,{line},{maincell.start.line+(int)i+1});
                    return -1;
                }
                check();
                log("Registering system call...",2,{(int)i+1,4},{maincell.content.size(),4});
                string uidarg=variablesdict[arg].intele.varinfo.uuid;
                vodka::syscalls::ABS abscall;
                abscall.args_uid=uidarg;
                vodka::syscalls::syscall_container syscont;
                syscont.thing="ABS";
                syscont.absele=abscall;
                instructions.push_back(syscont);
                check();
            } else {
                error("vodka.error.kernel.abs.invalid_syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1});
                return -1;
            }
        } else if (line.substr(0,13)=="kernel.divmod") {
            log("Checking system call syntax...",2,{(int)i+1,1},{maincell.content.size(),4});
            auto eles=split(line," ");
            if (eles.size()==5) {
                check();
                log("Checking content existence...",2,{(int)i+1,2},{maincell.content.size(),4});
                auto arg=vector<string>(eles.begin()+1,eles.end());
                for (int y=0;y<arg.size();++y) {
                    if (find(variableslist.begin(),variableslist.end(),arg[y])==variableslist.end()) {
                        error("vodka.error.variables.not_declared : "+arg[y]+" wasn't declared before instruction.",file,{line},{maincell.start.line+(int)i+1});
                        return -1;
                    }
                }
                if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$" || eles[2].substr(0,2)=="$$" || eles[2].substr(0,1)=="$") {
                    error("vodka.error.variables.constant : Can't modify an constant.",file,{line},{maincell.start.line+(int)i+1});
                }
                check();
                log("Checking content datatype...",2,{(int)i+1,3},{maincell.content.size(),4});
                for (int y=0;y<arg.size();++y) {
                    if (variablesdict[arg[y]].thing!="vodint" || variablesdict[arg[y]].intele.typeinfo.typenames!="vodint") {
                        error("vodka.error.kernel.divmod.wrong_type : "+arg[y]+" isn't vodint type.",file,{line},{maincell.start.line+(int)i+1});
                        return -1;
                    }
                }
                if (arg[0]==arg[1]) {
                    error("vodka.error.kernel.divmod.same_output : the two output variables can't be the same.",file,{line},{maincell.start.line+(int)i+1});
                    return -1;
                }
                check();
                log("Registering system call...",2,{(int)i+1,4},{maincell.content.size(),4});
                string quouid=variablesdict[arg[0]].intele.varinfo.uuid;
                string resuid=variablesdict[arg[1]].intele.varinfo.uuid;
                string divid=variablesdict[arg[2]].intele.varinfo.uuid;
                string divis=variablesdict[arg[3]].intele.varinfo.uuid;
                vodka::syscalls::DIVMOD divmodcall;
                divmodcall.quouid=quouid;
                divmodcall.resuid=resuid;
                divmodcall.divid=divid;
                divmodcall.divis=divis;
                vodka::syscalls::syscall_container syscont;
                syscont.thing="DIVMOD";
                syscont.divmodele=divmodcall;
                instructions.push_back(syscont);
                check();
            } else {
                error("vodka.error.kernel.abs.invalid_syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1});
                return -1;
            }
        } else {
            error("vodka.error.function.unknow : Unknow function.",file,{line},{maincell.start.line+(int)i+1});
            return -1;
        }
    }
    //* Writing output file
    log("Writing variables :");
    final.push_back("data:");
    int a=1;
    for (auto i:variablesdict) {
        if (i.second.thing=="vodint") {
            if (i.second.intele.varinfo.write==true) {
                log("Writing "+i.second.intele.varinfo.uuid+"...",1,{a},{variablesdict.size()});
                if (i.second.intele.varinfo.define==true) {
                    final.push_back(i.second.intele.varinfo.uuid+"="+i.second.intele.value);
                }
                a=a+1;
                check();
            }
        }
    }
    final.push_back("enddata");
    log("Writing code section :");
    a=1;
    final.push_back("code:");
    for (auto i:instructions) {
        log("Writing "+i.thing+" instruction...",1,{a},{instructions.size()});
        final.push_back(i.syntax());
        a=a+1;
        check();
    }
    final.push_back("endcode");
    log("Opening output file...");
    ofstream outputfile(output);
    check();
    if (mode=="compile") {
        log("Writing in output file :");
        if (outputfile.is_open()) {
            a=1;
            for (const auto& line:final) {
                log("Writing line number "+to_string(a)+"...",1,{a},{final.size()});
                outputfile<<line<<"\n";
                a=a+1;
                check();
            }
            log("Closing output file...");
            outputfile.close();
            check();
            if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file+" to "+output<<endl;}
        }
    } else if (mode=="json") {
        log("Converting to json format :");
        a=1;
        for (const string& line:final) {
            auto now=chrono::system_clock::now();
            time_t now_c=chrono::system_clock::to_time_t(now);
            tm utc=*std::gmtime(&now_c);
            stringstream ss;
            ss<<put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");
            log("Converting line "+to_string(a)+"...",1,{a},{final.size()});
            json_ints["metadata"]={{"type","metadata"},{"vodka_version","0.2 beta 5"},{"json_version","2"},{"source_file",file},{"timestamp",ss.str()}};
            vector<string> kernel_symbol={"code:","endcode","args:","endargs","data:","enddata"};
            if (find(kernel_symbol.begin(),kernel_symbol.end(),line)==kernel_symbol.end() && line.substr(0,4)!="type") {
                if (std::isalpha(line[0]) && std::isupper(line[0])) {
                    vodka::json::json_container jsonth;
                    jsonth.type="system_call";
                    jsonth.intname=split(line," ")[0];
                    auto eles=split(line," ");
                    jsonth.args=vector<string>(eles.begin()+1,eles.end());
                    json_ints[to_string(a)+":"+to_string(genuid())]=jsonth.syntax();
                    a=a+1;
                } else {
                    vodka::json::json_container jsonth;
                    jsonth.type="variable";
                    jsonth.intname=split(line,"=")[0];
                    auto eles=split(line,"=");
                    string otherside;
                    for (int i=1;i<eles.size();++i) {
                        otherside=otherside+eles[i];
                    }
                    jsonth.args={otherside};
                    json_ints[to_string(a)+":"+to_string(genuid())]=jsonth.syntax();
                    a=a+1;
                }
            }
            check();
        }
        log("Writing json file...");
        nlohmann::json j=json_ints;
        outputfile<<j.dump();
        outputfile.close();
        check();
        if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file+" to "+output<<endl;}
    }
    return 0;
}