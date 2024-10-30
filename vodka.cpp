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
using namespace std;
string verbose="e";
bool debugmode=false;
int x=0;
string last;
string sublast;
namespace vodka {
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
        class syscall_container {
            public:
                string thing;
                PRINT printele;
                ADD addele;
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
                        args=args+a;
                    }
                    return addele.info.name+" "+addele.output_uid+" "+args;
                } else {
                    return "error";
                }
            }
        };
    }
    namespace variables {
        class typess {
            public:
                string typenames;
                vector<string> method;
        };
        class variable {
            public:
                string varname;
                string uuid;
                bool consts;
                bool algo_dependant;
        };
        class vodint {
            public:
                string value;
                variable varinfo;
                typess typeinfo;
            vodint() {
                typeinfo.typenames="vodint";
                typeinfo.method={"add","sub","mul","div","dive","mod","abs","exp"};
            }
        };
        class element {
            public:
                string thing;
                vodint intele;
        };
    }
    namespace type {
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
                for (int i=0;i<value.length();++i) {
                    if (value[i]=='0' && reached==false) {
                        reached=false;
                    } else if (value[i]!='0' && reached==false) {
                        reached=true;
                        out=out+value[i];
                    } else {
                        out=out+value[i];
                    }
                }
                return out;
            }
            namespace method {
            }
        }
    }
}
void error(const string& error,const string& file,vector<string> lines_content,vector<int> lines,vector<size_t> indicator,vector<size_t> indic_len) {
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
    for (size_t y=0;y<indicator.size();++y) {
        cout<<"  "<<lines[y]<<" | "<<lines_content[y]<<endl;
        if (indicator[y]!=0) {
            cout<<" ";
            for (size_t i=0;i<indicator[y]+to_string(lines[0]).size()+3;++i) {
                cout<<" ";
            }
            for (size_t i=0;i<indic_len[y];++i) {
                cout<<"^";
            }
            cout<<endl;
        }
    }
    cout<<error<<endl;
}
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
                last="("+to_string(x)+"/15) "+texti;
                cout<<last;
            }
        } else {
            if (verbose=="a") {
                cout<<endl;
                last="("+to_string(x)+"/14) ";
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
void debuglog(const string& text,int line) {
    if (debugmode==true) {
        if (verbose=="e") {
            string texti=text.substr(1,text.length()-1);
            cout<<"Debug line "+to_string(line)+" : "+texti<<endl;
        } else if (verbose=="r") {
            string texti=text.substr(1,text.length()-1);
            cout<<endl<<"Debug line "+to_string(line)+" : "+texti<<endl;
        } else if (verbose=="a") {
            string texti=text.substr(1,text.length()-1);
            cout<<endl<<"Debug line "+to_string(line)+" : "+texti;
        }
    }
}
vector<string> mainargs;
vector<string> mainargsname;
map<string,string> mainargsdict;
vector<vodka::syscalls::syscall_container> instructions;
map<string,vodka::variables::element> variablesdict;
vector<string> variableslist;
vector<string> symbollist={"VODSTART","VODEND","VODIMPORT","VODTYPE","VODSTRUCT","VODCLASS","VODENDCLASS"};
vector<string> typelist={"app","command","shell","gui","logonui","logonshell","service"};
vector<string> final;
boost::uuids::uuid genuid() {
    boost::uuids::random_generator ran;
    boost::uuids::uuid uuid=ran();
    return uuid;
}
int main (int argc,char* argv[]) {
    string file;
    string output="";
    int option;
    string mode="compile";
    string finde;
    opterr=0;
    while ((option=getopt(argc,argv,"hdvVf:s:o:"))!=-1) {
        switch (option) {
        case 'h':
            cout<<"Vodka v0.1.1 - Vodka Objective Dictionary for Kernel Analyser\nHow to use : vodka [-h] [-f object_to_find (not working for the moment)] [-s source file] [-o output file] [-v set verbose mode to reduced] [-V set verbose mode to all] [-d enable debug mode]"<<endl;
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
        case '?':
            cout<<"Invalid argument."<<endl;
            return -1;
        default:
            return -1;;
        }
    }
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
                error("vodka.error.symbol.unknow_symbol : Unknow symbol found : "+temp.type,file,{line},{linenum},{2},{temp.type.length()});
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
    log("Looking for type symbol :");
    bool typefound=false;
    for (size_t i=0;i<symbols.size();++i) {
        log("Checking symbol type...",1,{(int)i+1},{symbols.size()});
        if (symbols[i].type=="VODTYPE") {
            if (typefound==false) {
                typefound=true;
            } else {
                error("vodka.error.vodtype.confusion : A vodka program can only contain one VODTYPE symbol.",file,{symbols[i].content},{symbols[i].line},{1},{symbols[i].content.size()-1});
                return -1;
            }
        }
        check();
    }
    if (typefound==false) {
        error("vodka.error.vodtype.not_found : Can't find VODTYPE symbol.",file,{"<no line affected>"},{0},{0},{0});
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
                error("vodka.error.vodtype.invalid_syntax : Invalid syntax for program type declaration.",file,{symbols[i].content},{symbols[i].line},{1},{symbols[i].content.size()-1});
                return -1;
            }
            check();
            log("Checking program type...",2,{(int)i+1,2},{symbols.size(),3});
            if (find(typelist.begin(),typelist.end(),ele[1])==typelist.end()) {
                error("vodka.error.vodtype.unknow_type : Unknow type : "+ele[1],file,{symbols[i].content},{symbols[i].line},{10},{ele[1].length()});
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
                    error("vodka.error.vodstart.name_not_found : Can't find cell's name.",file,{symbols[i].content},{symbols[i].line},{0},{0});
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
                    error("vodka.error.cell.confusion : An existing cell with the same name already exists.",file,contenterror,lineerror,indicatorpos,indicatorlen);
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
                error("vodka.error.vodend.not_found : Can't find corresponding VODEND symbol to the previous VODSTART.",file,{symbols[i].content},{symbols[i].line},{0},{0});
                return -1;
            }
        }
    }
    log("Verifying if program contain a main cell...");
    if (maincell.name!="main") {
        error("vodka.error.main.not_found : Can't find the main cell.",file,{"<no line affected>"},{0},{0},{0});
        return -1;
    }
    check();
    final.push_back("args:");
    log("Writing args section...");
    for (string argm:mainargs) {
        final.push_back(argm);
    }
    check();
    log("Started analysing main cell :");
    final.push_back("endargs");
    for (size_t i=0;i<maincell.content.size();++i) {
        string line=maincell.content[i];
        log("Analysing line "+to_string(i+1)+" :",1,{(int)i+1},{maincell.content.size()});
        if (line.substr(0,1)==">") {
            debuglog(line,maincell.start.line+(int)i+1);
        } else if (line.substr(0,5)=="vodka") {
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
                if (namepart.size()==2 && valuepart.size()==2) {
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
                        if (name.substr(0,1)=="$") {
                            var.consts=true;
                            var.algo_dependant=false;
                        } else {
                            var.consts=false;
                            var.algo_dependant=false;
                        }
                        for (auto const& emap:variablesdict) {
                            if (emap.first==name && emap.second.intele.varinfo.consts==true) {
                                error("vodka.error.variables.constant : Can't modify an constant.",file,{line},{maincell.start.line+(int)i+1},{0},{0});
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
                            error("vodka.error.vodint.invalid_value : Invalid value : "+valuepart[1],file,{line},{maincell.start.line+(int)i+1},{line.find(valuepart[1])+1},{valuepart[1].length()-1});
                            return -1;
                        }
                        check();
                        log("Saving variable...",2,{(int)i+1,8},{maincell.content.size(),8});
                        vodka::variables::element contvar;
                        contvar.intele=varint;
                        contvar.thing=typevar;
                        variablesdict[name]=contvar;
                        variableslist.push_back(name);
                        check();
                    } else {
                        error("vodka.error.variables.unknow_type : Unknow type : "+typevar,file,{line},{maincell.start.line+(int)i+1},{line.find(typevar)+1},{typevar.length()});
                        return -1;
                    }
                } else {
                    error("vodka.error.variables.invalid_syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1},{0},{0});
                    return -1;
                }
            } else {
                error("vodka.error.variables.invalid_syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1},{0},{0});
                return -1;
            }
        } else if (line.substr(0,12)=="kernel.print") {
            log("Checking system call syntax...",2,{(int)i+1,1},{maincell.content.size(),3});
            auto eles=split(line," ");
            if (eles.size()>=2) {
                check();
                log("Checking content...",2,{(int)i+1,2},{maincell.content.size(),3});
                vector<string> eltoprint(eles.begin()+1,eles.end());
                for (auto a:eltoprint) {
                    if (find(variableslist.begin(),variableslist.end(),a)==variableslist.end() && find(mainargsname.begin(),mainargsname.end(),a)==mainargsname.end()) {
                        error("vodka.error.variables.not_declared : "+a+" wasn't declared before vodprint's instruction.",file,{line},{maincell.start.line+(int)i+1},{line.find(a)+1},{a.length()});
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
                error("vodka.error.vodprint.invalid syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1},{0},{0});
                return -1;
            }
        } else {
            error("vodka.error.function.unknow : Unknow function.",file,{line},{maincell.start.line+(int)i+1},{1},{split(line," ")[0].length()});
            return -1;
        }
    }
    log("Writing data section :");
    final.push_back("data:");
    int a=1;
    for (auto i:variablesdict) {
        log("Writing "+i.second.intele.varinfo.uuid+"...",1,{a},{variablesdict.size()});
        if (i.second.intele.varinfo.algo_dependant==false) {
            final.push_back(i.second.intele.varinfo.uuid+"="+i.second.intele.value);
        }
        a=a+1;
        check();
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
    log("Writing in output file :");
    if (outputfile.is_open()) {
        a=1;
        for (const auto& line:final) {
            log("Writing line number "+to_string(a)+"...",1,{a},{final.size()});
            outputfile<<line<<"\n";
            a=a+1;
            check();
        }
        outputfile.close();
        if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file+" to "+output<<endl;}
    }
    return 0;
}