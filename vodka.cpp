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
                info.support_multiple_argument="yes";
            }
        };
        class syscall_container {
            public:
                string thing;
                PRINT printele;
            string syntax() {
                if (thing=="PRINT") {
                    string args;
                    for (auto a:printele.argument_uid) {
                        args=args+a;
                    }
                    return printele.info.name+" "+args;
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
                vector<char> num={'0','1','2','3','4','5','6','7','8','9'};
                for (auto v:value) {
                    if (find(num.begin(),num.end(),v)==num.end()) {
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
    cout<<"File "+file+":"+to_string(lines[0])+", line(s) "+linestr+" an error occured :"<<endl;
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
    while ((option=getopt(argc,argv,"hf:s:o:"))!=-1) {
        switch (option) {
        case 'h':
            cout<<"Vodka v0.1 - Vodka Objective Dictionary for Kernel Analyser\nHow to use : vodka [-h] [-f object_to_find] [-s source file]"<<endl;
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
    if (filesystem::exists(file)==false) {
        cout<<"Source file doesn't exitst."<<endl;
        return -1;
    }
    ifstream filee(file.c_str());
    if (filee.is_open()==false) {
        cout<<"File can't be open."<<endl;
        return -1;
    }
    string lineread;
    vector<string> content;
    while (getline(filee,lineread)) {
        content.push_back(lineread);
    }
    filee.close();
    vector<symbol> symbols;
    for (size_t i=0;i<content.size();++i) {
        string line=content[i];
        if (line.rfind("£",0)==0) {
            symbol temp;
            temp.line=i+1;
            temp.content=line;
            auto ele=split(line," ");
            temp.type=ele[0].substr(2,ele[0].size());
            if (find(symbollist.begin(),symbollist.end(),temp.type)==symbollist.end()) {
                int linenum=i+1;
                error("vodka.error.symbol.unknow_symbol : Unknow symbol found : "+temp.type,file,{line},{linenum},{2},{temp.type.length()});
                return -1;
            } else {
                symbols.push_back(temp);
            }
        }
    }
    bool typefound=false;
    for (size_t i=0;i<symbols.size();++i) {
        if (symbols[i].type=="VODTYPE") {
            if (typefound==false) {
                typefound=true;
            } else {
                error("vodka.error.vodtype.confusion : A vodka program can only contain one VODTYPE symbol.",file,{symbols[i].content},{symbols[i].line},{1},{symbols[i].content.size()-1});
                return -1;
            }
        }
    }
    if (typefound==false) {
        error("vodka.error.vodtype.not_found : Can't find VODTYPE symbol.",file,{"<no line affected>"},{0},{0},{0});
        return -1;
    }
    string program_type;
    for (size_t i=0;i<symbols.size();++i) {
        if (symbols[i].type=="VODTYPE") {
            auto ele=split(symbols[i].content," ");
            if (ele.size()!=2) {
                error("vodka.error.vodtype.invalid_syntax : Invalid syntax for program type declaration.",file,{symbols[i].content},{symbols[i].line},{1},{symbols[i].content.size()-1});
                return -1;
            }
            if (find(typelist.begin(),typelist.end(),ele[1])==typelist.end()) {
                error("vodka.error.vodtype.unknow_type : Unknow type : "+ele[1],file,{symbols[i].content},{symbols[i].line},{10},{ele[1].length()});
                return -1;
            }
            program_type=ele[1];
        }
    }
    final.push_back("type "+program_type);
    vector<cellule> cells;
    vector<string> cellnames;
    cellule maincell;
    for (size_t i=0;i<symbols.size();++i) {
        if (symbols[i].type=="VODSTART") {
            if (symbols.size()>=i+2 && symbols[i+1].type=="VODEND") {
                cellule temp;
                temp.start=symbols[i];
                temp.end=symbols[i+1];
                auto args=split(symbols[i].content," ");
                if (args.size()>1) {
                    temp.name=args[1];
                } else {
                    error("vodka.error.vodstart.name_not_found : Can't find cell's name.",file,{symbols[i].content},{symbols[i].line},{0},{0});
                    return -1;
                }
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
                if (args.size()>2) {
                    temp.args=vector<string>(args.begin()+2,args.end());
                }
                auto outs=split(temp.end.content," ");
                if (outs.size()>1) {
                    temp.outs=vector<string>(outs.begin()+1,outs.end());
                }
                int startline=temp.start.line;
                int endline=temp.end.line-2;
                for (int i=startline;i<endline+1;++i) {
                    temp.content.push_back(content[i]);
                }
                cells.push_back(temp);
                cellnames.push_back(temp.name);
                if (temp.name=="main") {
                    maincell=temp;
                    for (auto a:maincell.args) {
                        auto uid=to_string(genuid());
                        mainargs.push_back(uid);
                        mainargsdict[a]=uid;
                        mainargsname.push_back(a);
                    }
                }
            } else {
                error("vodka.error.vodend.not_found : Can't find corresponding VODEND symbol to the previous VODSTART.",file,{symbols[i].content},{symbols[i].line},{0},{0});
                return -1;
            }
        }
    }
    if (maincell.name!="main") {
        error("vodka.error.main.not_found : Can't find the main cell.",file,{"<no line affected>"},{0},{0},{0});
        return -1;
    }
    final.push_back("args:");
    for (string argm:mainargs) {
        final.push_back(argm);
    }
    final.push_back("endargs");
    for (size_t i=0;i<maincell.content.size();++i) {
        string line=maincell.content[i];
        if (line.substr(0,5)=="vodka") {
            auto eles=split(line,"=");
            if (eles.size()==2) {
                auto namepart=split(eles[0]," ");
                auto valuepart=split(eles[1]," ");
                if (namepart.size()==3) {
                    namepart.pop_back();
                }
                if (valuepart[0]=="") {
                    valuepart.erase(valuepart.begin());
                }
                if (namepart.size()==2 && valuepart.size()==2) {
                    string name=namepart[1];
                    string typevar=valuepart[0];
                    vodka::variables::variable var;
                    if (typevar=="vodint") {
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
                        vodka::variables::vodint varint;
                        varint.varinfo=var;
                        if (vodka::type::vodint::check_value(valuepart[1])) {
                            varint.value=vodka::type::vodint::remove_zero(valuepart[1]);
                        } else {
                            error("vodka.error.vodint.invalid_value : Invalid value : "+valuepart[1],file,{line},{maincell.start.line+(int)i+1},{line.find(valuepart[1])+1},{valuepart[1].length()-1});
                            return -1;
                        }
                        vodka::variables::element contvar;
                        contvar.intele=varint;
                        contvar.thing=typevar;
                        variablesdict[name]=contvar;
                        variableslist.push_back(name);
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
        } else if (line.substr(0,8)=="vodprint") {
            auto eles=split(line," ");
            if (eles.size()>=2) {
                vector<string> eltoprint(eles.begin()+1,eles.end());
                for (auto a:eltoprint) {
                    if (find(variableslist.begin(),variableslist.end(),a)==variableslist.end() && find(mainargsname.begin(),mainargsname.end(),a)==mainargsname.end()) {
                        error("vodka.error.variables.not_declared : "+a+" wasn't declared before vodprint's instruction.",file,{line},{maincell.start.line+(int)i+1},{line.find(a)+1},{a.length()});
                        return -1;
                    }
                }
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
            } else {
                error("vodka.error.vodprint.invalid syntax : Invalid syntax.",file,{line},{maincell.start.line+(int)i+1},{0},{0});
                return -1;
            }
        }
    }
    final.push_back("data:");
    for (auto i:variablesdict) {
        if (i.second.intele.varinfo.algo_dependant==false) {
            final.push_back(i.second.intele.varinfo.uuid+"="+i.second.intele.value);
        }
    }
    final.push_back("enddata");
    final.push_back("code:");
    for (auto i:instructions) {
        final.push_back(i.syntax());
    }
    final.push_back("endcode");
    ofstream outputfile(output);
    if (outputfile.is_open()) {
        for (const auto& line:final) {
            outputfile<<line<<"\n";
        }
        outputfile.close();
        cout<<"Sucessfully compile "+file+" to "+output<<endl;
    }
    return 0;
}