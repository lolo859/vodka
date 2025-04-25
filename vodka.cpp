#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <bitset>
#include <unistd.h>
#include <cctype>
#include <getopt.h>
#include <cstring>
#include <chrono>
#include <boost/hash2/sha3.hpp>
#include "vodka-lib/vodka-lib.h"
#include "dependencies/json.hpp"
//* Some necessary functions
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
string hash_then_encode(string origin) {
    boost::hash2::sha3_512 hasher;
    hasher.update(origin.data(),origin.size());
    auto digest=hasher.result();
    string out;
    for (auto byte:digest) {
        bitset<8> bits(static_cast<unsigned char>(byte));
        auto str=bits.to_string();
        for (auto a:str) {
            if (a=='0') {
                out=out+u8"\u200B";
            } else {
                out=out+u8"\u2063";
            }
        }
    }
    return out;
}
string encode_to_bin(string input) {
    string binar;
    for (unsigned char a:input) {
        bitset<8> bits(a);
        binar=binar+bits.to_string();
    }
    string out;
    for (auto a:binar) {
        if (a=='0') {
            out=out+u8"\u200B";
        } else {
            out=out+u8"\u2063";
        }
    }
    return hash_then_encode(out);
}
//* Some variables
using namespace std;
using namespace vodka::utilities;
using namespace vodka::errors;
//* Variables for compilation and options
string verbose="e";
bool debugmode=false;
bool var_warning_enabled=true;
bool disable_integrity_hash=false;
int x=1;
string last;
string file;
sources_stack srcstack;
vector<string> final;
map<string,map<string,string>> json_ints;
map<string,map<string,map<string,string>>> json_ints_v;
//* Vectors and maps for code and code structure analysis
vector<string> mainargslist;
map<string,vodka::variables::element> mainargsdict;
map<string,vodka::variables::element> variablesdict;
vector<string> variableslist;
map<string,string> replacement;
vector<string> symbollist={"VODSTART","VODEND","VODIMPORT","VODTYPE","VODSTRUCT","VODCLASS","VODENDCLASS","VODEFINE"};
vector<string> typelist={"app","command","shell","gui","logonui","logonshell","service"};
vector<string> content;
vector<string> output_file_to_check;
vector<symbol> symbols;
symbol temp;
bool typefound=false;
string program_type;
vector<cellule> cells;
vector<string> cellnames;
cellule maincell;
//* Read file
int read_file(string output,string mode,sources_stack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    //* Source file fetching
    if (output=="") {
        raise(error_container("vodka.error.file.output_not_provided","<no file>",{"<no line affected>"},{0},lclstack));
        return -1;
    }
    if (mode=="check") {
        log("Checking if source and output file exist.",verbose,x,last);
        if (filesystem::exists(file)==false) {
            raise(error_container("Source file doesn't exist.",file,{"<no line affected>"},{0},lclstack));
            return -1;
        }
        if (filesystem::exists(output)==false) {
            raise(error_container("Output file doesn't exist.",file,{"<no line affected>"},{0},lclstack));
            return -1;
        }
        x=x+1;
        log("Checking files readability.",verbose,x,last);
        ifstream filee(file.c_str());
        if (filee.is_open()==false) {
            raise(error_container("File can't be open.",file,{"<no line affected>"},{0},lclstack));
            return -1;
        }
        ifstream fileee(output.c_str());
        if (fileee.is_open()==false) {
            raise(error_container("File can't be open.",output,{"<no line affected>"},{0},lclstack));
            return -1;
        }
        x=x+1;
        log("Reading files.",verbose,x,last);
        string lineread;
        while (getline(filee,lineread)) {
            content.push_back(lineread);
        }
        string linereade;
        while (getline(fileee,linereade)) {
            output_file_to_check.push_back(linereade);
        }
        x=x+1;
        log("Removing comments.",verbose,x,last);
        for (int i=0;i<content.size();++i) {
            if (content[i]!="") {
                if (content[i].find("§",0)==0) {
                    content[i]="";
                } else {
                    content[i]=split(content[i],"§")[0];
                }
            }
        }
        for (int i=0;i<output_file_to_check.size();++i) {
            if (output_file_to_check[i]!="") {
                if (output_file_to_check[i].find("§",0)==0) {
                    output_file_to_check[i]="";
                } else {
                    output_file_to_check[i]=split(output_file_to_check[i],"§")[0];
                }
            }
        }
        filee.close();
        return 0;
    } else {
        log("Checking if source file exist.",verbose,x,last);
        if (filesystem::exists(file)==false) {
            raise(error_container("Source file doesn't exist.",file,{"<no line affected>"},{0},lclstack));
            return -1;
        }
        x=x+1;
        log("Checking file readability.",verbose,x,last);
        ifstream filee(file.c_str());
        if (filee.is_open()==false) {
            raise(error_container("File can't be open.",file,{"<no line affected>"},{0},lclstack));
            return -1;
        }
        x=x+1;
        log("Reading file.",verbose,x,last);
        string lineread;
        while (getline(filee,lineread)) {
            content.push_back(lineread);
        }
        x=x+1;
        log("Removing comments.",verbose,x,last);
        for (int i=0;i<content.size();++i) {
            if (content[i]!="") {
                if (content[i].find("§",0)==0) {
                    content[i]="";
                } else {
                    content[i]=split(content[i],"§")[0];
                }
            }
        }
        filee.close();
        return 0;
    }
}
//* Detect symbols 
int detect_symbol(sources_stack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Detecting symbols.",verbose,x,last);
    for (size_t i=0;i<content.size();++i) {
        string line=content[i];
        log("Detecting if line "+to_string(i+1)+" contain symbol :",verbose,x,last,1,{(int)i+1},{content.size()});
        if (line.rfind("£",0)==0) {
            log("Allocating memory.",verbose,x,last,2,{(int)i+1,1},{content.size(),3});
            temp.line=i+1;
            temp.content=line;
            auto ele=split(line," ");
            temp.type=ele[0].substr(2,ele[0].size());
            log("Checking symbol.",verbose,x,last,2,{(int)i+1,2},{content.size(),3});
            if (find(symbollist.begin(),symbollist.end(),temp.type)==symbollist.end()) {
                int linenum=i+1;
                raise(error_container("vodka.error.symbol.unknow_symbol : Unknow symbol found : "+temp.type,file,{line},{linenum},lclstack));
                return -1;
            } else {
                log("Saving symbol.",verbose,x,last,2,{(int)i+1,3},{content.size(),3});
                symbols.push_back(temp);
            }
        } else {
            log("No symbol detected.",verbose,x,last,2,{(int)i+1,1},{content.size(),1});
        }
    }
    x=x+1;
    log("Searching define replacement instruction :",verbose,x,last);
    for (size_t i=0;i<symbols.size();++i) {
        log("Checking symbol type :",verbose,x,last,1,{(int)i+1},{symbols.size()});
        if (symbols[i].type=="VODEFINE") {
            auto eles=split(symbols[i].content," ");
            if (eles.size()==3) {
                replacement[eles[1]]=eles[2];
                log("Replacement found. "+eles[1]+" will be replaced with "+eles[2]+".",verbose,x,last,2,{(int)i+1,1},{symbols.size(),1});
            } else {
                raise(error_container("vodka.error.vodefine.invalid_syntax : Invalid syntax for define replacement declaration.",file,{symbols[i].content},{symbols[i].line},lclstack));
                return -1;
            }
        } else {
            log("No VODEFINE symbol detected.",verbose,x,last,2,{(int)i+1,1},{symbols.size(),1});
        }
    }
    x=x+1;
    log("Looking for type symbol :",verbose,x,last);
    for (size_t i=0;i<symbols.size();++i) {
        log("Checking symbol type.",verbose,x,last,1,{(int)i+1},{symbols.size()});
        if (symbols[i].type=="VODTYPE") {
            if (typefound==false) {
                typefound=true;
            } else {
                raise(error_container("vodka.error.vodtype.confusion : A vodka program can only contain one VODTYPE symbol.",file,{symbols[i].content},{symbols[i].line},lclstack));
                return -1;
            }
        }
    }
    if (typefound==false) {
        raise(error_container("vodka.error.vodtype.not_found : Can't find VODTYPE symbol.",file,{"<no line affected>"},{0},lclstack));
        return -1;
    }
    return 0;
}
int detect_program_type(string output,sources_stack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Detecting program type :",verbose,x,last);
    for (size_t i=0;i<symbols.size();++i) {
        log("Checking if line contain type symbol :",verbose,x,last,1,{(int)i+1},{symbols.size()});
        if (symbols[i].type=="VODTYPE") {
            log("Checking syntax.",verbose,x,last,2,{(int)i+1,1},{symbols.size(),3});
            auto ele=split(symbols[i].content," ");
            if (ele.size()!=2) {
                raise(error_container("vodka.error.vodtype.invalid_syntax : Invalid syntax for program type declaration.",file,{symbols[i].content},{symbols[i].line},lclstack));
                return -1;
            }
            log("Checking program type.",verbose,x,last,2,{(int)i+1,2},{symbols.size(),3});
            if (find(typelist.begin(),typelist.end(),ele[1])==typelist.end()) {
                raise(error_container("vodka.error.vodtype.unknow_type : Unknow type : "+ele[1],file,{symbols[i].content},{symbols[i].line},lclstack));
                return -1;
            }
            log("Saving program type.",verbose,x,last,2,{(int)i+1,3},{symbols.size(),3});
            program_type=ele[1];
        } else {
            log("The symbol isn't the type symbol.",verbose,x,last,2,{(int)i+1,1},{symbols.size(),1});
        }
    }
    if (disable_integrity_hash) {
        final.push_back(encode_to_bin(output)+"type "+program_type);
    } else {
        final.push_back("type "+program_type);
    }
    return 0;
}
int detect_cells(sources_stack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Detecting cells :",verbose,x,last);
    for (size_t i=0;i<symbols.size();++i) {
        if (symbols[i].type=="VODSTART") {
            log("Checking start symbol syntax.",verbose,x,last,1,{1},{10});
            if (symbols.size()>=i+2 && symbols[i+1].type=="VODEND") {
                log("Allocating memory.",verbose,x,last,1,{2},{10});
                cellule temp;
                log("Detecting cell start and end symbol.",verbose,x,last,1,{3},{10});
                temp.start=symbols[i];
                temp.end=symbols[i+1];
                log("Detecting cell name.",verbose,x,last,1,{4},{10});
                auto args=split(symbols[i].content," ");
                if (args.size()>1) {
                    temp.name=args[1];
                } else {
                    raise(error_container("vodka.error.vodstart.name_not_found : Can't find cell's name.",file,{symbols[i].content},{symbols[i].line},lclstack));
                    return -1;
                }
                log("Checking if an cell with the same name already exist in the program.",verbose,x,last,1,{5},{10});
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
                    raise(error_container("vodka.error.cell.confusion : An existing cell with the same name already exists.",file,contenterror,lineerror,lclstack));
                    return -1;
                }
                log("Detecting cell argument.",verbose,x,last,1,{6},{10});
                if (args.size()>2) {
                    temp.args=vector<string>(args.begin()+2,args.end());
                }
                log("Detecting cell output.",verbose,x,last,1,{7},{10});
                auto outs=split(temp.end.content," ");
                if (outs.size()>1) {
                    temp.outs=vector<string>(outs.begin()+1,outs.end());
                }
                log("Saving cell content.",verbose,x,last,1,{8},{10});
                int startline=temp.start.line;
                int endline=temp.end.line-2;
                for (int i=startline;i<endline+1;++i) {
                    temp.content.push_back(content[i]);
                }
                log("Saving cell.",verbose,x,last,1,{9},{10});
                cells.push_back(temp);
                cellnames.push_back(temp.name);
                log("Detecting if cell is the main cell.",verbose,x,last,1,{10},{10});
                if (temp.name=="main") {
                    maincell=temp;
                    for (auto a:maincell.args) {
                        auto uid=to_string(genuid());
                        vodka::variables::variable argsinfo;
                        argsinfo.algo_dependant=true;
                        argsinfo.consts=false;
                        argsinfo.define=false;
                        argsinfo.write=false;
                        argsinfo.uuid=to_string(genuid());
                        argsinfo.varname=a;
                        vodka::variables::vodarg argscont;
                        argscont.varinfo=argsinfo;
                        vodka::variables::element varcont;
                        varcont.thing="vodarg";
                        varcont.argele=argscont;
                        mainargsdict[a]=varcont;
                        mainargslist.push_back(a);
                        variableslist.push_back(a);
                        variablesdict[a]=varcont;
                    }
                }
            } else {
                raise(error_container("vodka.error.vodend.not_found : Can't find corresponding VODEND symbol to the previous VODSTART.",file,{symbols[i].content},{symbols[i].line},lclstack));
                return -1;
            }
        }
    }
    return 0;
}
int verify_main_cell(sources_stack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Verifying if program contain a main cell.",verbose,x,last);
    if (maincell.name!="main") {
        raise(error_container("vodka.error.main.not_found : Can't find the main cell.",file,{"<no line affected>"},{0},lclstack));
        return -1;
    }
    return 0;
}
int code_pre_treatement(bool replace,sources_stack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Writing args section.",verbose,x,last);
    for (auto argm:mainargsdict) {
        final.push_back(argm.second.argele.varinfo.uuid);
    }
    if (replace==false) {
        x=x+1;
        log("Skipping replacement step because of -r.",verbose,x,last);
    } else {
        x=x+1;
        log("Started define replacing process.",verbose,x,last);
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
    }
    x=x+1;
    log("Started datatypes replacement process.",verbose,x,last);
    vector<string> directs_data;
    for (int i=0;i<maincell.content.size();++i) {
        vodka::analyser::line localline;
        localline.line=maincell.start.line+i+1;
        localline.file=file;
        localline.content=maincell.content[i];
        auto direct_declared_data=split(maincell.content[i]," ");
        if (direct_declared_data.size()>2) {
            if (direct_declared_data[0]!="vodka") {
                direct_declared_data.erase(direct_declared_data.begin(),direct_declared_data.begin()+1);
                for (auto arg:direct_declared_data) {
                    if (arg.substr(0,1)=="#" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                        if (vodka::type::vodint::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                            vodka::variables::vodint intele;
                            intele.varinfo.algo_dependant=false;
                            intele.varinfo.consts=true;
                            intele.varinfo.write=true;
                            intele.varinfo.define=true;
                            intele.varinfo.uuid=to_string(vodka::utilities::genuid());
                            intele.value=vodka::type::vodint::remove_zero(arg.substr(1,arg.size()-1));
                            vodka::variables::element contele;
                            contele.intele=intele;
                            contele.thing="vodint";
                            variablesdict[arg]=contele;
                            variableslist.push_back(arg);
                            directs_data.push_back(arg);
                        } else {
                            return -1;
                        }
                    } else if (arg.substr(0,1)=="%" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                        if (vodka::type::vodec::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                            vodka::variables::vodec decele;
                            decele.varinfo.algo_dependant=false;
                            decele.varinfo.consts=true;
                            decele.varinfo.write=true;
                            decele.varinfo.define=true;
                            decele.varinfo.uuid=to_string(vodka::utilities::genuid());
                            decele.value=vodka::type::vodec::remove_zero(arg.substr(1,arg.size()-1));
                            vodka::variables::element contele;
                            contele.decele=decele;
                            contele.thing="vodec";
                            variablesdict[arg]=contele;
                            variableslist.push_back(arg);
                            directs_data.push_back(arg);
                        } else {
                            return -1;
                        }
                    }
                }
            }
        }
    }
    for (auto cell:cells) {
        for (int i=0;i<cell.content.size();++i) {
            vodka::analyser::line localline;
            localline.line=cell.start.line+i+1;
            localline.file=file;
            localline.content=cell.content[i];
            auto direct_declared_data=split(cell.content[i]," ");
            if (direct_declared_data.size()>2) {
                if (direct_declared_data[0]!="vodka") {
                    direct_declared_data.erase(direct_declared_data.begin(),direct_declared_data.begin()+1);
                    for (auto arg:direct_declared_data) {
                        if (arg.substr(0,1)=="#" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                            if (vodka::type::vodint::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                                vodka::variables::vodint intele;
                                intele.varinfo.algo_dependant=false;
                                intele.varinfo.consts=true;
                                intele.varinfo.write=true;
                                intele.varinfo.define=true;
                                intele.varinfo.uuid=to_string(vodka::utilities::genuid());
                                intele.value=vodka::type::vodint::remove_zero(arg.substr(1,arg.size()-1));
                                vodka::variables::element contele;
                                contele.intele=intele;
                                contele.thing="vodint";
                                variablesdict[arg]=contele;
                                variableslist.push_back(arg);
                                directs_data.push_back(arg);
                            } else {
                                return -1;
                            }
                        } else if (arg.substr(0,1)=="%" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                            if (vodka::type::vodec::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                                vodka::variables::vodec decele;
                                decele.varinfo.algo_dependant=false;
                                decele.varinfo.consts=true;
                                decele.varinfo.write=true;
                                decele.varinfo.define=true;
                                decele.varinfo.uuid=to_string(vodka::utilities::genuid());
                                decele.value=vodka::type::vodec::remove_zero(arg.substr(1,arg.size()-1));
                                vodka::variables::element contele;
                                contele.decele=decele;
                                contele.thing="vodec";
                                variablesdict[arg]=contele;
                                variableslist.push_back(arg);
                                directs_data.push_back(arg);
                            } else {
                                return -1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
//* Main function
int main (int argc,char* argv[]) {
    auto lclstack=srcstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    string output="";
    int option;
    bool replace=true;
    string mode="compile";
    string finde;
    opterr=0;
    struct option options[] = {
        {"help",no_argument,nullptr,'h'},
        {"find",required_argument,nullptr,'f'},
        {"source-file",required_argument,nullptr,'s'},
        {"output-file",required_argument,nullptr,'o'},
        {"verbose-reduced",no_argument,nullptr,'v'},
        {"verbose-full",no_argument,nullptr,'V'},
        {"debug-lines",no_argument,nullptr,'d'},
        {"json-kernel",no_argument,nullptr,'j'},
        {"json-vodka",no_argument,nullptr,'J'},
        {"disable-replacements",no_argument,nullptr,'r'},
        {"disable-all-warnings",no_argument,nullptr,'w'},
        {"disable-variables-warnings",no_argument,nullptr,0},
        {"disable-integrity-hash",no_argument,nullptr,'H'},
        {"check-mode",no_argument,nullptr,'c'},
        {nullptr, 0, nullptr, 0}
    };
    //* Args management
    while ((option=getopt_long(argc,argv,"hHjJrdvVcwf:s:o:",options,nullptr))!=-1) {
        switch (option) {
        case 0:
            if (string(argv[optind-1])=="disable-variables-warnings") {
                var_warning_enabled=false;
            }
            break;
        case 'h':
            cout<<"Vodka v0.3 - Vodka Objective Dictionary for Kernel Analyser\nOptions :\n  -h, --help :\n    show this help\n  -f, --find object_to_find :\n    (not working for the moment)\n  -s, --source-file source_file :\n    source file \n  -o, --output-file output_file :\n    output file\n  -v, --verbose-reduced :\n    set verbose mode to reduced\n  -V, --verbose-full :\n    set verbose mode to full\n  -d, --debug-lines :\n    enable debug mode\n  -j, --json-kernel :\n    export output to a json file specified with -o\n  -J, --json-vodka :\n    export .vod structure to a json file specified with -o\n  -r, --disable-replacements :\n    disable define replacement\n  -w, --disable-all-warnings :\n    disable warnings\n  --disable-variables-warnings :\n    disable variables warnings\n  -H, --disable-integrity-hash :\n    disable the integrity hash integration into the output\n  -c, --check-mode :\n    verify the integrity between the inputed file witn -s and the output file specified with -o"<<endl;
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
            mode="jsonkernel";
            break;
        case 'J':
            mode="jsonvodka";
            break;
        case 'r':
            replace=false;
            break;
        case 'w':
            var_warning_enabled=false;
            break;
        case 'H':
            disable_integrity_hash=true;
            break;
        case 'c':
            mode="check";
            break;
        case '?':
            cout<<"Invalid argument."<<endl;
            return -1;
        default:
            return -1;;
        }
    }
    int readedFile=read_file(output,mode,lclstack);
    if (readedFile==-1) {
        return -1;
    }
    x=x+1;
    int detectedSymbols=detect_symbol(lclstack);
    if (detectedSymbols==-1) {
        return -1;
    }
    x=x+1;
    int detectedProgramType=detect_program_type(output,lclstack);
    if (detectedProgramType==-1) {
        return -1;
    }
    x=x+1;
    int detectedCells=detect_cells(lclstack);
    if (detectedCells==-1) {
        return -1;
    }
    x=x+1;
    int verifiedMainCell=verify_main_cell(lclstack);
    if (verifiedMainCell==-1) {
        return -1;
    }
    final.push_back("args:");
    x=x+1;
    int codePreTreatement=code_pre_treatement(replace,lclstack);
    if (codePreTreatement==-1) {
        return -1;
    }
    x=x+1;
    log("Started analysing main cell :",verbose,x,last);
    //* Here happen all the magic
    final.push_back("endargs");
    vector<vodka::syscalls::syscall_container> instructions_main;
    map<string,vodka::variables::element> main_variablesdict=variablesdict;
    vector<string> main_variableslist=variableslist;
    map<string,vector<string>> main_vars_used;
    for (size_t i=0;i<maincell.content.size();++i) {
        log("Analysing line "+to_string(i+1)+" :",verbose,x,last,1,{(int)i+1},{maincell.content.size()});
        string line=maincell.content[i];
        vodka::analyser::line actual_line;
        actual_line.content=line;
        actual_line.line=maincell.start.line+(int)i+1;
        actual_line.file=file;
        actual_line.checked=actual_line.check(lclstack);
        if (actual_line.checked==false) {
            return -1;
        }
        if (actual_line.skip==true) {
            continue;
        }
        vodka::analyser::type_analyser type_analyser;
        type_analyser.line_analyse=actual_line;
        type_analyser.checked=type_analyser.line_type_analyse(lclstack);
        if (type_analyser.checked==false) {
            return -1;
        }
        //* Debug line processing
        if (type_analyser.type=="debug_one") {
            debuglog(line,maincell.start.line+(int)i+1,"main",debugmode,verbose,file);
        } else if (type_analyser.type=="debug_two") {
            debuglog(line,maincell.start.line+(int)i+1,"main",debugmode,verbose,file,false);
        } 
        //* Vodka instruction processing
        else if (type_analyser.type=="var") {
            log("Checking vodka declaration syntax.",verbose,x,last,2,{(int)i+1,1},{maincell.content.size(),6});
            vodka::analyser::var_dec_analyser var_dec_analyser;
            var_dec_analyser.line_analyse=type_analyser;
            var_dec_analyser.checked=var_dec_analyser.var_dec_analyse(lclstack);
            if (var_dec_analyser.checked==false) {
                return -1;
            }
            if (var_dec_analyser.name.substr(0,1)=="$" || var_dec_analyser.name.substr(0,2)=="$$") {
                for (auto a:main_variableslist) {
                    if (a==var_dec_analyser.name) {
                        raise(error_container("vodka.error.variables.constant : Can't modify a constant.",file,{line},{maincell.start.line+(int)i+1},lclstack));
                        return -1;
                    }
                }
            }
            log("Checking vodka declaration type and value.",verbose,x,last,2,{(int)i+1,2},{maincell.content.size(),6});
            var_dec_analyser.checked_type_value=var_dec_analyser.check_type_value(main_variableslist,lclstack);
            if (var_dec_analyser.checked_type_value==false) {
                return -1;
            }
            log("Making metadata about vodka declaration.",verbose,x,last,2,{(int)i+1,3},{maincell.content.size(),6});
            if (var_dec_analyser.datatype=="vodka") {
                var_dec_analyser.source_duplication=main_variablesdict[var_dec_analyser.value];
            }
            bool info=var_dec_analyser.make_info(lclstack);
            if (info==false) {
                return -1;
            }
            log("Pre-treating vodka declaration value.",verbose,x,last,2,{(int)i+1,4},{maincell.content.size(),6});
            var_dec_analyser.pre_treated=var_dec_analyser.pre_treatement(lclstack);
            if (var_dec_analyser.pre_treated==false) {
                return -1;
            }
            log("Compiling vodka declaration into variable container.",verbose,x,last,2,{(int)i+1,5},{maincell.content.size(),6});
            bool output=var_dec_analyser.output(lclstack);
            if (output==false) {
                return -1;
            }
            log("Registering vodka declaration.",verbose,x,last,2,{(int)i+1,6},{maincell.content.size(),6});
            main_variablesdict[var_dec_analyser.name]=var_dec_analyser.var_object;
            main_variableslist.push_back(var_dec_analyser.name);
            if (!var_dec_analyser.is_kernel_const) {
                instructions_main.push_back(var_dec_analyser.vodka_object);
                main_vars_used[var_dec_analyser.name]={"f",to_string(actual_line.line)};
            }
            if (var_dec_analyser.is_kernel_const) {
                variablesdict[var_dec_analyser.name]=var_dec_analyser.var_object;
                variableslist.push_back(var_dec_analyser.name);   
            }
        //* Kernel function analysis
        } else if (type_analyser.type=="internal_library") {
            auto args=vodka::analyser::get_arguments(line);
            for (auto arg:args) {
                try {
                    main_vars_used.at(arg)={"t",main_vars_used.at(arg)[1]};
                } catch (const std::out_of_range& e) {
                    //* Error is raised later
                }
            }
            if (type_analyser.library_name=="kernel") {
                vodka::library::function_call fcall;
                fcall.verbose_context=verbose;
                fcall.main_logstep_context=x;
                fcall.last_logstep_context=last;
                fcall.variableslist_context=main_variableslist;
                fcall.cell_context=maincell;
                fcall.iteration_number_context=i;
                fcall.file_name_context=file;
                fcall.variablesdict_context=main_variablesdict;
                fcall.type_analyser=type_analyser;
                vodka::library::kernel::treatement engine;
                engine.call=fcall;
                engine.checked=engine.kernel_treatement(lclstack);
                if (engine.checked==false) {
                    return -1;
                } else {
                    instructions_main.insert(instructions_main.end(),engine.syscall_output.begin(),engine.syscall_output.end());
                }
                if (engine.var_flag==true) {
                    main_variableslist=engine.call.variableslist_context;
                    main_variablesdict=engine.call.variablesdict_context;
                }
            }
        } else if (type_analyser.type=="vodka_instruction") {
            auto args=vodka::analyser::get_arguments(line);
            for (auto arg:args) {
                try {
                    main_vars_used.at(arg)={"t",main_vars_used.at(arg)[1]};
                } catch (const std::out_of_range& e) {
                    //* Error is raised later
                }
            }
            vodka::instructions::instruction_call icall;
            icall.verbose_context=verbose;
            icall.main_logstep_context=x;
            icall.last_logstep_context=last;
            icall.variableslist_context=main_variableslist;
            icall.cell_context=maincell;
            icall.iteration_number_context=i;
            icall.file_name_context=file;
            icall.variablesdict_context=main_variablesdict;
            icall.type_analyser=type_analyser;
            vodka::instructions::instruction_treatement engine;
            engine.call=icall;
            engine.checked=engine.treatement(lclstack);
            if (engine.checked==false) {
                return -1;
            } else {
                for (auto a:engine.syscalls_output) {
                    instructions_main.push_back(a);
                }
            }
            if (engine.var_flag==true) {
                main_variablesdict=engine.call.variablesdict_context;
                main_variableslist=engine.call.variableslist_context;
            }
        } else {
            raise(error_container("vodka.error.function.unknow : Unknow function.",file,{line},{maincell.start.line+(int)i+1},lclstack));
            return -1;
        }
    }
    //* Indicating unused variables
    for (auto var:main_vars_used) {
        if (var.second[0]=="f") {
            vodka::utilities::var_warning(var.first,main_variablesdict[var.first].thing,"main",var.second[1],var_warning_enabled,verbose);
        }
    }
    //* Writing output file
    x=x+1;
    log("Writing variables :",verbose,x,last);
    final.push_back("data:");
    int a=1;
    for (auto i:variablesdict) {
        if (i.second.thing=="vodint") {
            if (i.second.intele.varinfo.write==true) {
                log("Writing "+i.second.intele.varinfo.uuid+".",verbose,x,last,1,{a},{variablesdict.size()});
                if (i.second.intele.varinfo.define==true) {
                    final.push_back(i.second.intele.varinfo.uuid+"="+i.second.intele.value);
                }
                a=a+1;
            }
        } else if (i.second.thing=="vodec") {
            if (i.second.decele.varinfo.write==true) {
                log("Writing "+i.second.decele.varinfo.uuid+".",verbose,x,last,1,{a},{variablesdict.size()});
                if (i.second.decele.varinfo.define==true) {
                    final.push_back(i.second.decele.varinfo.uuid+"="+i.second.decele.value);
                }
                a=a+1;
            }
        }
    }
    final.push_back("enddata");
    x=x+1;
    log("Writing main code section :",verbose,x,last);
    a=1;
    vector<string> kernel_sections={"main"};
    vector<int> kernel_sections_line={(int)final.size()};
    map<string,string> kernel_sections_content;
    map<string,string> vodka_cell_hash;
    final.push_back("main:");
    for (auto i:instructions_main) {
        log("Writing "+i.thing+" instruction.",verbose,x,last,1,{a},{instructions_main.size()});
        final.push_back(i.syntax());
        kernel_sections_content["main"]=kernel_sections_content["main"]+"\n"+i.syntax();
        a=a+1;
    }
    for (auto cell:cells) {
        string content;
        for (auto line:cell.content) {content=content+"\n"+line;}
        vodka_cell_hash[cell.name]=hash_then_encode(content);
    }
    if (disable_integrity_hash==false) {
        final.push_back(vodka_cell_hash["main"]+"endmain");
        for (int i=0;i<kernel_sections_line.size();++i) {
            final[kernel_sections_line[i]]=hash_then_encode(kernel_sections_content[kernel_sections[i]])+final[kernel_sections_line[i]];
        }
    } else {
        final.push_back("endmain");
    }
    if (mode=="check") {
        x=x+1;
        if (output_file_to_check[0].substr(0,3)==u8"\u200B" || output_file_to_check[0].substr(0,3)==u8"\u2063") {
            string binfile=split(output_file_to_check[0],"type")[0];
            if (encode_to_bin(output)==binfile) {
                cout<<"The output file hasn't been compiled with integrity checksum enabled. If you think there are checksum in the output file, remove all the invisibles caracters at the start of the first line and then retry."<<endl;
            } else {
                cout<<"The output file may have been compiled with integrity checksum enabled but someone tried to mask it by adding a wrong no-integrity tag. Remove all the invisibles caracters at the start of the first line and then retry."<<endl;
            }
        }
        log("Retrieving checksum.",verbose,x,last);
        map<string,string> vodka_checksum;
        map<string,string> checksum_line;
        vector<string> founded_sections;
        for (auto line:output_file_to_check) {
            for (auto cellcheck:cells) {
                if (line.substr(line.size()-cellcheck.name.size(),cellcheck.name.size())==cellcheck.name) {
                    checksum_line[cellcheck.name]=line;
                    founded_sections.push_back(cellcheck.name);
                    break;
                }
            }
        }
        if (find(founded_sections.begin(),founded_sections.end(),"main")==founded_sections.end()) {
            cout<<"Can't found main section.";
            return -1;
        } else if (founded_sections.size()==0) {
            cout<<"Doesn't found any section.";
            return -1;
        }
        x=x+1;
        log("Extracting checksum.",verbose,x,last);
        for (auto sect:founded_sections) {
            auto linet=checksum_line.at(sect);
            string checksum=linet.substr(0,1536);
            vodka_checksum[sect]=checksum;
        }
        x=x+1;
        log("Checking checksum.",verbose,x,last);
        cout<<endl<<"Check result:"<<endl;
        for (auto checksum:vodka_checksum) {
            cout<<" - "+checksum.first+": ";
            if (checksum.second==vodka_cell_hash.at(checksum.first)) {
                cout<<"pass"<<endl;
            } else {
                cout<<"failed"<<endl;
            }
        }
    } else if (mode=="compile") {
        x=x+1;
        log("Opening output file.",verbose,x,last);
        ofstream outputfile(output);
        x=x+1;
        log("Writing in output file :",verbose,x,last);
        if (outputfile.is_open()) {
            a=1;
            for (const auto& line:final) {
                log("Writing line number "+to_string(a)+".",verbose,x,last,1,{a},{final.size()});
                outputfile<<line<<"\n";
                a=a+1;
            }
            x=x+1;
            log("Closing output file.",verbose,x,last);
            outputfile.close();
            if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file+" to "+output<<endl;}
        }
    } else if (mode=="jsonkernel") {
        x=x+1;
        log("Opening output file.",verbose,x,last);
        ofstream outputfile(output);
        x=x+1;
        log("Converting kernel code to json format :",verbose,x,last);
        a=1;
        auto now=chrono::system_clock::now();
        time_t now_c=chrono::system_clock::to_time_t(now);
        tm utc=*std::gmtime(&now_c);
        stringstream ss;
        bool endargs=false;
        ss<<put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");
        json_ints["metadata"]={{"type","metadata"},{"vodka_version","0.3"},{"json_version","4"},{"source_file",file},{"timestamp",ss.str()},{"json_type","kernel"}};
        vector<string> kernel_symbol={"args:","endargs","data:","enddata"};
        for (auto b:kernel_sections) {
            kernel_symbol.push_back(b+":");
            kernel_symbol.push_back("end"+b);
        }
        for (const string& line:final) {
            string actualcell="main";
            log("Converting line "+to_string(a)+".",verbose,x,last,1,{a},{final.size()});
            if (find(kernel_symbol.begin(),kernel_symbol.end(),line)==kernel_symbol.end() && line.substr(0,4)!="type") {
                if (std::isalpha(line[0]) && std::isupper(line[0])) {
                    vodka::json::kernel::json_container jsonth;
                    jsonth.type="system_call";
                    jsonth.intname=split(line," ")[0];
                    auto eles=split(line," ");
                    jsonth.args=vector<string>(eles.begin()+1,eles.end());
                    json_ints[to_string(a)+"@"+actualcell+":"+to_string(genuid())]=jsonth.syntax();
                    a=a+1;
                } else {
                    vodka::json::kernel::json_container jsonth;
                    if (endargs) {jsonth.type="constant";} else {jsonth.type="argument";};
                    jsonth.intname=split(line,"=")[0];
                    auto eles=split(line,"=");
                    string otherside;
                    for (int i=1;i<eles.size();++i) {
                        if (i!=1) {
                            otherside=otherside+"="+eles[i];
                        } else {
                            otherside=eles[i];
                        }
                    }
                    jsonth.args={otherside};
                    json_ints[to_string(a)+":"+to_string(genuid())]=jsonth.syntax();
                }
            } else if (line=="endargs") {
                endargs=true;
            }
            a=a+1;
        }
        x=x+1;
        log("Writing json file.",verbose,x,last);
        nlohmann::json j=json_ints;
        outputfile<<j.dump();
        outputfile.close();
        if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file+" to "+output<<endl;}
    } else if (mode=="jsonvodka") {
        x=x+1;
        log("Opening output file.",verbose,x,last);
        ofstream outputfile(output);
        x=x+1;
        log("Converting vodka code to json format :",verbose,x,last);
        auto now=chrono::system_clock::now();
        time_t now_c=chrono::system_clock::to_time_t(now);
        tm utc=*std::gmtime(&now_c);
        stringstream ss;
        ss<<put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");
        json_ints_v["metadata"]={{"metadata",{{"type","metadata"},{"vodka_version","0.3"},{"json_version","4"},{"source_file",file},{"timestamp",ss.str()},{"json_type","vodka"}}}};
        log("Converting symbols : ",verbose,x,last,1,{1},{2});
        json_ints_v["symbols"]={};
        bool cellstart=false;
        vodka::json::vodka::cell actualcell;
        vector<vodka::json::vodka::cell> idencell;
        int cellcount=0;
        for (int i=0;i<symbols.size();++i) {
            log("Converting \""+symbols[i].content+"\".",verbose,x,last,2,{1,i+1},{2,symbols.size()});
            vodka::json::vodka::symbol symb;
            symb.type=symbols[i].type;
            auto args=split(symbols[i].content," ");
            symb.args=vector<string>(args.begin()+1,args.end());
            symb.uid=to_string(genuid());
            if (symb.type=="VODSTART" && cellstart==false) {
                cellcount=cellcount+1;
                vodka::json::vodka::cell cell;
                cell.name=symb.args[0];
                cell.uid=to_string(cellcount)+":"+to_string(genuid());
                cell.start=symb;
                cellstart=true;
                actualcell=cell;
            }
            if (symb.type=="VODEND" && cellstart==true) {
                actualcell.end=symb;
                actualcell.end.uid=symb.uid;
                cellstart=false;
                idencell.insert(idencell.begin(),actualcell);
            }
            json_ints_v["symbols"][to_string(i+1)+":"+to_string(genuid())]=symb.syntax();
        }
        log("Converting cells : ",verbose,x,last,1,{2},{2});
        vector<string> vodkaints=vodka::vodka_instruction;
        for (int i=0;i<idencell.size();++i) {
            log("Converting cell "+cells[i].name+".",verbose,x,last,2,{2,i+1},{2,cells.size()});
            auto cellcontent=cells[i].content;
            for (auto a:cellcontent) {
                if (a.substr(0,5)=="vodka") {
                    vodka::json::vodka::var_declaration vardec;
                    vardec.uid=to_string(genuid());
                    auto eles=split(a,"=");
                    auto namepart=eles[0];
                    auto valuepart=eles[1];
                    vardec.name=split(namepart," ")[1];
                    vardec.type=split(valuepart," ")[0];
                    auto vvaluepart=split(valuepart," ");
                    vector<string> valuev=vector<string>(vvaluepart.begin()+1,vvaluepart.end());
                    string value="";
                    for (int b=0;b<valuev.size();++b) {
                        if (b==0) {
                            value=valuev[b];
                        } else {
                            value=value+valuev[b];
                        }
                    }
                    vardec.decvalue=value;
                    vodka::json::vodka::line_container linecont;
                    linecont.thing="var";
                    linecont.varele=vardec;
                    idencell[i].lines.push_back(linecont);
                } else if (a.substr(0,6)=="kernel") {
                    vodka::json::vodka::instruction instr;
                    instr.library="kernel";
                    instr.uid=to_string(genuid());
                    instr.name=split(a," ")[0];
                    instr.source="<builtin>";
                    auto eles=split(a," ");
                    instr.args=vector<string>(eles.begin()+1,eles.end());
                    vodka::json::vodka::line_container linecont;
                    linecont.thing="int";
                    linecont.intele=instr;
                    idencell[i].lines.push_back(linecont);
                } else if (find(vodkaints.begin(),vodkaints.end(),split(a," ")[0])!=vodkaints.end()) {
                    vodka::json::vodka::instruction instr;
                    instr.library="<no_library>";
                    instr.uid=to_string(genuid());
                    instr.name=split(a," ")[0];
                    instr.source="<builtin>";
                    auto eles=split(a," ");
                    instr.args=vector<string>(eles.begin()+1,eles.end());
                    vodka::json::vodka::line_container linecont;
                    linecont.thing="int";
                    linecont.intele=instr;
                    idencell[i].lines.push_back(linecont);
                }
            }
            json_ints_v[idencell[i].uid]=idencell[i].syntax();
        }
        x=x+1;
        log("Writing json file.",verbose,x,last);
        nlohmann::json j=json_ints_v;
        outputfile<<j.dump();
        outputfile.close();
        if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file+" to "+output<<endl;}
    }
    return 0;
}