#if defined(__GNUC__)
constexpr int gpp_major=__GNUC__;
constexpr int gpp_minor=__GNUC_MINOR__;
constexpr int gpp_patch=__GNUC_PATCHLEVEL__;
bool compiled_with_gpp=true;
#else
bool compiled_with_gpp=false;
#endif
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
#include <chrono>
#include <random>
#include <cstdlib>
#include "vodka-lib/vodka-lib.h"
#include "dependencies/json.hpp"
#include "dependencies/blake3.h"
#include "dependencies/xxhash.h"
#include "dependencies/XoshiroCpp.hpp"
//* Some necessary functions
using namespace std;
using namespace vodka::utilities;
using namespace vodka::errors;
//* Some variables
//* Variables for compilation and options
string verbose="e";
bool debug_mode=false;
bool var_warning_enabled=true;
bool disable_integrity_hash=false;
bool show_log_time=false;
int log_main_step=1;
string file_source;
SourcesStack srcstack;
vector<string> compiled_output;
map<string,map<string,string>> json_ints;
map<string,map<string,map<string,string>>> json_ints_v;
vodka::compilation::VodkaFile vodka_file;
vector<string> output_file_to_check;
//* Read file(s) and removing comments
bool read_file(string output,string mode,SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (output=="") {
        raise(ErrorContainer("vodka.error.file.output_not_provided : No output file was provided","<no file>",{"<no line affected>"},{0},lclstack));
        return false;
    }
    if (mode=="check") {
        output::log("Checking if source and output file exist.",log_main_step);
        if (filesystem::exists(file_source)==false) {
            raise(ErrorContainer("vodka.error.file.source_not_exist : Source file doesn't exist.",file_source,{"<no line affected>"},{0},lclstack));
            return false;
        }
        if (filesystem::exists(output)==false) {
            raise(ErrorContainer("vodka.error.file.output_not_exist : Output file doesn't exist.",file_source,{"<no line affected>"},{0},lclstack));
            return false;
        }
        log_main_step=log_main_step+1;
        output::log("Checking files readability.",log_main_step);
        ifstream file_source_stream(file_source.c_str());
        if (file_source_stream.is_open()==false) {
            raise(ErrorContainer("vodka.error.file.source_readability : Source file can't be open.",file_source,{"<no line affected>"},{0},lclstack));
            return false;
        }
        ifstream file_output_stream(output.c_str());
        if (file_output_stream.is_open()==false) {
            raise(ErrorContainer("vodka.error.file.output_readability : Output file can't be open.",output,{"<no line affected>"},{0},lclstack));
            return false;
        }
        log_main_step=log_main_step+1;
        output::log("Reading files.",log_main_step);
        string line_readed_source;
        while (getline(file_source_stream,line_readed_source)) {
            vodka_file.file_content.push_back(line_readed_source);
        }
        string line_readed_output;
        while (getline(file_output_stream,line_readed_output)) {
            output_file_to_check.push_back(line_readed_output);
        }
        log_main_step=log_main_step+1;
        output::log("Removing comments.",log_main_step);
        vodka_file.remove_comments();
        for (int i=0;i<output_file_to_check.size();++i) {
            if (output_file_to_check[i]!="") {
                if (output_file_to_check[i].find("§",0)==0) {
                    output_file_to_check[i]="";
                } else {
                    output_file_to_check[i]=string_utilities::split(output_file_to_check[i],"§")[0];
                }
            }
        }
        file_source_stream.close();
        vodka_file.file_source=file_source;
        return true;
    } else {
        output::log("Checking if source file exist.",log_main_step);
        if (filesystem::exists(file_source)==false) {
            raise(ErrorContainer("Source file doesn't exist.",file_source,{"<no line affected>"},{0},lclstack));
            return false;
        }
        log_main_step=log_main_step+1;
        output::log("Checking file readability.",log_main_step);
        ifstream file_source_stream(file_source.c_str());
        if (file_source_stream.is_open()==false) {
            raise(ErrorContainer("File can't be open.",file_source,{"<no line affected>"},{0},lclstack));
            return false;
        }
        log_main_step=log_main_step+1;
        output::log("Reading file.",log_main_step);
        string line_readed_source;
        while (getline(file_source_stream,line_readed_source)) {
            vodka_file.file_content.push_back(line_readed_source);
        }
        log_main_step=log_main_step+1;
        output::log("Removing comments.",log_main_step);
        vodka_file.remove_comments();
        file_source_stream.close();
        vodka_file.file_source=file_source;
        return true;
    }
}
//* Main function
int main (int argc,char* argv[]) {
    auto lclstack=srcstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    string output="";
    int option;
    bool replace=true;
    string mode="compile";
    string element_to_find;
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
        {"disable-variables-warnings",no_argument,nullptr,'!'},
        {"disable-integrity-hash",no_argument,nullptr,'H'},
        {"check-mode",no_argument,nullptr,'c'},
        {"show-log-time",no_argument,nullptr,'t'},
        {"version",no_argument,nullptr,'1'},
        {"license",no_argument,nullptr,'2'},
        {nullptr,0,nullptr,0}
    };
    //* Args management
    while ((option=getopt_long(argc,argv,"hHjJtrdvVcwf:s:o:",options,nullptr))!=-1) {
        switch (option) {
        case '!':
            var_warning_enabled=false;
        case '1':
            cout<<"Vodka transcoder version 0.5 beta 1"<<endl;
            cout<<"vodka-lib version "+vodka::LibraryVersion<<endl;
            cout<<"Json export format version 5"<<endl;
            cout<<"vodka-lib Json namespace version "+vodka::JsonVersion<<endl;
            cout<<"Dependencies:"<<endl;
            cout<<" - blake3 version "<<BLAKE3_VERSION_STRING<<endl;
            cout<<" - Json version "<<NLOHMANN_JSON_VERSION_MAJOR<<"."<<NLOHMANN_JSON_VERSION_MINOR<<"."<<NLOHMANN_JSON_VERSION_PATCH<<endl;
            cout<<" - Termcolor version 2.1.0"<<endl;
            cout<<" - xxHash version "<<XXH_VERSION_MAJOR<<"."<<XXH_VERSION_MINOR<<"."<<XXH_VERSION_RELEASE<<endl;
            cout<<" - XoshiroCpp"<<endl;
            if (compiled_with_gpp) {
                cout<<"g++ version used for compilation : "<<to_string(gpp_major)<<"."<<to_string(gpp_minor)<<"."<<to_string(gpp_patch)<<endl;
            } else {
                cout<<"Compiled with unknow compiler."<<endl;
            }
            return 0;
        case '2':
            cout<<"The vodka transcoder, vodka-lib, the vodka programming language and all the documentations and related elements of this ecosystem is under the GPL-3.0 license."<<endl;
            return 0;
        case 'h':
            cout<<"Vodka Objective Dictionary for Kernel Analyser\nOptions :\n  -h, --help :\n    show this help\n  -f, --find object_to_find :\n    (not working for the moment)\n  -s, --source-file source_file :\n    source file \n  -o, --output-file output_file :\n    output file\n  -v, --verbose-reduced :\n    set verbose mode to reduced\n  -V, --verbose-full :\n    set verbose mode to full\n  -d, --debug-lines :\n    enable debug mode\n  -j, --json-kernel :\n    export output to a json file specified with -o\n  -J, --json-vodka :\n    export .vod structure to a json file specified with -o\n  -r, --disable-replacements :\n    disable define replacement\n  -w, --disable-all-warnings :\n    disable warnings\n  --disable-variables-warnings :\n    disable variables warnings\n  -H, --disable-integrity-hash :\n    disable the integrity hash integration into the output\n  -c, --check-mode :\n    verify the integrity between the inputed file witn -s and the output file specified with -o\n  -t, --show-log-time :\n    show time in every log output, should be used alongside -v or -V\n  --version :\n    show version of every sub-section of the code, transcoder, dependencies, JSON and compiler\n  --license:\n    show the license"<<endl;
            return 0;
        case 'f':
            mode="find";
            element_to_find=optarg;
            break;
        case 's':
            file_source=optarg;
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
            debug_mode=true;
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
        case 't':
            show_log_time=true;
            break;
        case '?':
            cout<<"Invalid argument."<<endl;
            return -1;
        default:
            return -1;;
        }
    }
    if (show_log_time) {
        setenv("VODKA_SHOW_LOG_TIME","TRUE",1);
    } else {
        setenv("VODKA_SHOW_LOG_TIME","FALSE",1);
    }
    if (debug_mode) {
        setenv("VODKA_DEBUG_MODE","TRUE",1);
    } else {
        setenv("VODKA_DEBUG_MODE","FALSE",1);
    }
    if (var_warning_enabled) {
        setenv("VODKA_SHOW_VAR_WARNING","TRUE",1);
    } else {
        setenv("VODKA_SHOW_VAR_WARNING","FALSE",1);
    }
    setenv("VODKA_VERBOSE_MODE",verbose.c_str(),1);
    bool readed_file=read_file(output,mode,lclstack);
    if (readed_file==false) {
        return -1;
    }
    log_main_step=log_main_step+1;
    vodka::compilation::PreCompilation precompilation_engine;
    precompilation_engine.file=vodka_file;
    bool parse_symbols=precompilation_engine.parse_symbol(log_main_step,lclstack);
    if (parse_symbols==false) {
        return -1;
    }
    log_main_step=log_main_step+1;
    bool detected_program_type=precompilation_engine.detect_program_type(log_main_step,lclstack);
    if (detected_program_type==false) {
        return -1;
    }
    if (disable_integrity_hash) {
        compiled_output.push_back(encoding::encode_to_bin(output)+"type "+precompilation_engine.program_type);
    } else {
        compiled_output.push_back("type "+precompilation_engine.program_type);
    }
    log_main_step=log_main_step+1;
    bool detected_cells=precompilation_engine.detect_cells(log_main_step,lclstack);
    if (detected_cells==false) {
        return -1;
    }
    compiled_output.push_back("args:");
    log_main_step=log_main_step+1;
    bool code_pre_traited=precompilation_engine.code_pretreatement(compiled_output,replace,log_main_step,lclstack);
    if (code_pre_traited==false) {
        return -1;
    }
    log_main_step=log_main_step+1;
    vodka::compilation::CompilationContext context;
    context.file=precompilation_engine;
    if (context.setup()==false) {
        return -1;
    }
    output::log("Started analysing main cell :",log_main_step);
    //* Here happen all the magic
    compiled_output.push_back("endargs");
    vector<vodka::syscalls::SyscallContainer> instructions_main;
    map<string,vodka::variables::VariableContainer> main_variablesdict=context.variables_dict;
    vector<string> main_variableslist=context.variables_list;
    map<string,vector<string>> main_vars_used;
    for (size_t i=0;i<context.file.maincell.content.size();++i) {
        output::log("Analysing line "+to_string(i+1)+" :",log_main_step,1,{(int)i+1},{context.file.maincell.content.size()});
        string line=context.file.maincell.content[i];
        vodka::analyser::LineSyntaxChecker actual_line;
        actual_line.content=line;
        actual_line.line_number=context.file.maincell.start.line+(int)i+1;
        actual_line.file=file_source;
        actual_line.check(lclstack);
        if (actual_line.get_check_result()==false) {
            return -1;
        }
        if (actual_line.shoulb_be_skip==true) {
            continue;
        }
        vodka::analyser::LineTypeChecker type_analyser;
        type_analyser.line_checked=actual_line;
        type_analyser.line_type_analyse(lclstack);
        if (type_analyser.get_analyse_result()==false) {
            return -1;
        }
        //* Debug line processing
        if (type_analyser.type=="debug_one") {
            output::debuglog(line,context.file.maincell.start.line+(int)i+1,"main",file_source);
        } else if (type_analyser.type=="debug_two") {
            output::debuglog(line,context.file.maincell.start.line+(int)i+1,"main",file_source,false);
        } 
        //* Vodka instruction processing
        else if (type_analyser.type=="var") {
            output::log("Checking vodka declaration syntax.",log_main_step,2,{(int)i+1,1},{context.file.maincell.content.size(),6});
            vodka::analyser::VariableDeclarationAnalyser VariableDeclarationAnalyser;
            VariableDeclarationAnalyser.line_checked=type_analyser;
            VariableDeclarationAnalyser.variablesdict_context=main_variablesdict;
            VariableDeclarationAnalyser.variableslist_context=main_variableslist;
            VariableDeclarationAnalyser.parser(lclstack);
            if (VariableDeclarationAnalyser.get_parser_result()==false) {
                return -1;
            }
            if (VariableDeclarationAnalyser.name.substr(0,1)=="$" || VariableDeclarationAnalyser.name.substr(0,2)=="$$") {
                for (auto a:main_variableslist) {
                    if (a==VariableDeclarationAnalyser.name) {
                        raise(ErrorContainer("vodka.error.variables.constant : Can't modify a constant.",file_source,{line},{context.file.maincell.start.line+(int)i+1},lclstack));
                        return -1;
                    }
                }
            }
            output::log("Checking vodka declaration type and value.",log_main_step,2,{(int)i+1,2},{context.file.maincell.content.size(),6});
            VariableDeclarationAnalyser.check_type_value(main_variableslist,lclstack);
            if (VariableDeclarationAnalyser.get_check_type_value_result()==false) {
                return -1;
            }
            output::log("Making metadata about vodka declaration.",log_main_step,2,{(int)i+1,3},{context.file.maincell.content.size(),6});
            if (VariableDeclarationAnalyser.datatype=="vodka") {
                VariableDeclarationAnalyser.duplication_source_variable=main_variablesdict[VariableDeclarationAnalyser.value];
            }
            bool info=VariableDeclarationAnalyser.make_info(lclstack);
            if (info==false) {
                return -1;
            }
            output::log("Pre-treating vodka declaration value.",log_main_step,2,{(int)i+1,4},{context.file.maincell.content.size(),6});
            VariableDeclarationAnalyser.value_pre_treatement(lclstack);
            if (VariableDeclarationAnalyser.get_pre_treatement_result()==false) {
                return -1;
            }
            output::log("Compiling vodka declaration into variable container.",log_main_step,2,{(int)i+1,5},{context.file.maincell.content.size(),6});
            bool output=VariableDeclarationAnalyser.make_output(lclstack);
            if (output==false) {
                return -1;
            }
            output::log("Registering vodka declaration.",log_main_step,2,{(int)i+1,6},{context.file.maincell.content.size(),6});
            main_variablesdict[VariableDeclarationAnalyser.name]=VariableDeclarationAnalyser.variable_container;
            main_variableslist.push_back(VariableDeclarationAnalyser.name);
            if (!VariableDeclarationAnalyser.is_kernel_constant) {
                instructions_main.push_back(VariableDeclarationAnalyser.syscall_container);
                main_vars_used[VariableDeclarationAnalyser.name]={"f",to_string(actual_line.line_number)};
            }
            if (VariableDeclarationAnalyser.is_kernel_constant) {
                context.variables_dict[VariableDeclarationAnalyser.name]=VariableDeclarationAnalyser.variable_container;
                context.variables_list.push_back(VariableDeclarationAnalyser.name);   
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
            if (type_analyser.library_name=="memory") {
                vodka::library::FunctionCall fcall;
                fcall.verbose_context=verbose;
                fcall.main_logstep_context=log_main_step;
                fcall.variableslist_context=main_variableslist;
                fcall.cell_context=context.file.maincell;
                fcall.iteration_number_context=i;
                fcall.file_name_context=file_source;
                fcall.variablesdict_context=main_variablesdict;
                fcall.line_checked=type_analyser;
                vodka::library::memory::CallTreatement engine;
                engine.function_call=fcall;
                engine.call_treatement(lclstack);
                if (engine.get_call_treatement_result()==false) {
                    return -1;
                } else {
                    instructions_main.insert(instructions_main.end(),engine.syscalls_output.begin(),engine.syscalls_output.end());
                }
                if (engine.var_flag==true) {
                    main_variableslist=engine.function_call.variableslist_context;
                    main_variablesdict=engine.function_call.variablesdict_context;
                }
            } else if (type_analyser.library_name=="conversions") {
                vodka::library::FunctionCall fcall;
                fcall.verbose_context=verbose;
                fcall.main_logstep_context=log_main_step;
                fcall.variableslist_context=main_variableslist;
                fcall.cell_context=context.file.maincell;
                fcall.iteration_number_context=i;
                fcall.file_name_context=file_source;
                fcall.variablesdict_context=main_variablesdict;
                fcall.line_checked=type_analyser;
                vodka::library::conversions::CallTreatement engine;
                engine.function_call=fcall;
                engine.call_treatement(lclstack);
                if (engine.get_call_treatement_result()==false) {
                    return -1;
                } else {
                    instructions_main.insert(instructions_main.end(),engine.syscalls_output.begin(),engine.syscalls_output.end());
                }
                if (engine.var_flag==true) {
                    main_variableslist=engine.function_call.variableslist_context;
                    main_variablesdict=engine.function_call.variablesdict_context;
                }
            } else if (type_analyser.library_name=="math") {
                vodka::library::FunctionCall fcall;
                fcall.verbose_context=verbose;
                fcall.main_logstep_context=log_main_step;
                fcall.variableslist_context=main_variableslist;
                fcall.cell_context=context.file.maincell;
                fcall.iteration_number_context=i;
                fcall.file_name_context=file_source;
                fcall.variablesdict_context=main_variablesdict;
                fcall.line_checked=type_analyser;
                vodka::library::math::CallTreatement engine;
                engine.function_call=fcall;
                engine.call_treatement(lclstack);
                if (engine.get_call_treatement_result()==false) {
                    return -1;
                } else {
                    instructions_main.insert(instructions_main.end(),engine.syscalls_output.begin(),engine.syscalls_output.end());
                }
                if (engine.var_flag==true) {
                    main_variableslist=engine.function_call.variableslist_context;
                    main_variablesdict=engine.function_call.variablesdict_context;
                }
            } else if (type_analyser.library_name=="vodstr") {
                vodka::library::FunctionCall fcall;
                fcall.verbose_context=verbose;
                fcall.main_logstep_context=log_main_step;
                fcall.variableslist_context=main_variableslist;
                fcall.cell_context=context.file.maincell;
                fcall.iteration_number_context=i;
                fcall.file_name_context=file_source;
                fcall.variablesdict_context=main_variablesdict;
                fcall.line_checked=type_analyser;
                vodka::library::vodstr::CallTreatement engine;
                engine.function_call=fcall;
                engine.call_treatement(lclstack);
                if (engine.get_call_treatement_result()==false) {
                    return -1;
                } else {
                    instructions_main.insert(instructions_main.end(),engine.syscalls_output.begin(),engine.syscalls_output.end());
                }
                if (engine.var_flag==true) {
                    main_variableslist=engine.function_call.variableslist_context;
                    main_variablesdict=engine.function_call.variablesdict_context;
                }
            }
        } else {
            raise(ErrorContainer("vodka.error.function.unknow : Unknow function.",file_source,{line},{context.file.maincell.start.line+(int)i+1},lclstack));
            return -1;
        }
    }
    //* Indicating unused variables
    for (auto var:main_vars_used) {
        if (var.second[0]=="f") {
            vodka::utilities::output::var_warning(var.first,main_variablesdict[var.first].thing,"main",var.second[1]);
        }
    }
    //* Writing output file
    log_main_step=log_main_step+1;
    output::log("Writing variables :",log_main_step);
    compiled_output.push_back("data:");
    int a=1;
    for (auto i:context.variables_dict) {
        if (i.second.thing==vodka::variables::VariableDatatype::vodint) {
            if (i.second.variable_metadata.in_data_section==true) {
                output::log("Writing "+i.second.variable_metadata.uuid+".",log_main_step,1,{a},{context.variables_dict.size()});
                if (i.second.variable_metadata.is_kernel_constant==true) {
                    compiled_output.push_back(i.second.variable_metadata.uuid+" "+i.second.vodint_element.value);
                }
                a=a+1;
            }
        } else if (i.second.thing==vodka::variables::VariableDatatype::vodec) {
            if (i.second.variable_metadata.in_data_section==true) {
                output::log("Writing "+i.second.variable_metadata.uuid+".",log_main_step,1,{a},{context.variables_dict.size()});
                if (i.second.variable_metadata.is_kernel_constant==true) {
                    compiled_output.push_back(i.second.variable_metadata.uuid+" "+i.second.vodec_element.value);
                }
                a=a+1;
            }
        } else if (i.second.thing==vodka::variables::VariableDatatype::vodstr) {
            if (i.second.variable_metadata.in_data_section==true) {
                output::log("Writing "+i.second.variable_metadata.uuid+".",log_main_step,1,{a},{context.variables_dict.size()});
                if (i.second.variable_metadata.is_kernel_constant==true) {
                    compiled_output.push_back(i.second.variable_metadata.uuid+" "+i.second.vodstr_element.value);
                }
                a=a+1;
            }
        }
    }
    compiled_output.push_back("enddata");
    log_main_step=log_main_step+1;
    output::log("Writing main code section :",log_main_step);
    a=1;
    vector<string> kernel_sections={"main"};
    vector<int> kernel_sections_line={(int)compiled_output.size()};
    map<string,string> kernel_sections_content;
    map<string,string> vodka_cell_hash;
    compiled_output.push_back("main:");
    for (auto i:instructions_main) {
        output::log("Writing "+vodka::syscalls::syscall_to_string(i.thing)+" instruction.",log_main_step,1,{a},{instructions_main.size()});
        compiled_output.push_back(i.syntax());
        kernel_sections_content["main"]=kernel_sections_content["main"]+"\n"+i.syntax();
        a=a+1;
    }
    for (auto cell:context.file.cells_list) {
        string content;
        for (auto line:cell.content) {content=content+"\n"+line;}
        vodka_cell_hash[cell.name]=encoding::hash_then_encode(content);
    }
    if (disable_integrity_hash==false) {
        compiled_output.push_back(vodka_cell_hash["main"]+"endmain");
        for (int i=0;i<kernel_sections_line.size();++i) {
            compiled_output[kernel_sections_line[i]]=encoding::hash_then_encode(kernel_sections_content[kernel_sections[i]])+compiled_output[kernel_sections_line[i]];
        }
    } else {
        compiled_output.push_back("endmain");
    }
    if (mode=="check") {
        log_main_step=log_main_step+1;
        if (output_file_to_check[0].substr(0,3)==u8"\u200B" || output_file_to_check[0].substr(0,3)==u8"\u2063") {
            string binfile=string_utilities::split(output_file_to_check[0],"type")[0];
            if (encoding::encode_to_bin(output)==binfile) {
                cout<<"The output file hasn't been compiled with integrity checksum enabled. If you think there are checksum in the output file, remove all the invisibles caracters at the start of the first line and then retry."<<endl;
            } else {
                cout<<"The output file may have been compiled with integrity checksum enabled but someone tried to mask it by adding a wrong no-integrity tag. Remove all the invisibles caracters at the start of the first line and then retry."<<endl;
            }
        }
        output::log("Retrieving checksum.",log_main_step);
        map<string,string> vodka_checksum;
        map<string,string> checksum_line;
        vector<string> founded_sections;
        for (auto line:output_file_to_check) {
            for (auto cellcheck:context.file.cells_list) {
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
        log_main_step=log_main_step+1;
        output::log("Extracting checksum.",log_main_step);
        for (auto sect:founded_sections) {
            auto linet=checksum_line.at(sect);
            string checksum=linet.substr(0,1536);
            vodka_checksum[sect]=checksum;
        }
        log_main_step=log_main_step+1;
        output::log("Checking checksum.",log_main_step);
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
        log_main_step=log_main_step+1;
        output::log("Opening output file.",log_main_step);
        ofstream outputfile(output);
        log_main_step=log_main_step+1;
        output::log("Writing in output file :",log_main_step);
        if (outputfile.is_open()) {
            a=1;
            for (const auto& line:compiled_output) {
                output::log("Writing line number "+to_string(a)+".",log_main_step,1,{a},{compiled_output.size()});
                outputfile<<line<<"\n";
                a=a+1;
            }
            log_main_step=log_main_step+1;
            output::log("Closing output file.",log_main_step);
            outputfile.close();
            if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file_source+" to "+output<<endl;}
        }
    } else if (mode=="jsonkernel") {
        log_main_step=log_main_step+1;
        output::log("Opening output file.",log_main_step);
        ofstream outputfile(output);
        log_main_step=log_main_step+1;
        output::log("Converting kernel code to json format :",log_main_step);
        a=1;
        auto now=chrono::system_clock::now();
        time_t now_c=chrono::system_clock::to_time_t(now);
        tm utc=*std::gmtime(&now_c);
        stringstream ss;
        bool endargs=false;
        ss<<put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");
        json_ints["metadata"]={{"type","metadata"},{"vodka_version",vodka::LibraryVersion},{"json_version","5"},{"source_file",file_source},{"timestamp",ss.str()},{"json_type","kernel"}};
        vector<string> kernel_symbol={"args:","endargs","data:","enddata"};
        for (auto b:kernel_sections) {
            kernel_symbol.push_back(b+":");
            kernel_symbol.push_back("end"+b);
        }
        for (const string& line:compiled_output) {
            string actualcell="main";
            output::log("Converting line "+to_string(a)+".",log_main_step,1,{a},{compiled_output.size()});
            if (find(kernel_symbol.begin(),kernel_symbol.end(),line)==kernel_symbol.end()) {
                if (std::isalpha(line[0]) && std::isupper(line[0])) {
                    vodka::json::kernel::JsonContainer jsonth;
                    jsonth.type="system_call";
                    jsonth.instruction_name=string_utilities::split(line," ")[0];
                    auto eles=string_utilities::split(line," ");
                    jsonth.args=vector<string>(eles.begin()+1,eles.end());
                    json_ints[to_string(a)+" "+actualcell+" "+genvyid()]=jsonth.syntax();
                    a=a+1;
                } else {
                    vodka::json::kernel::JsonContainer jsonth;
                    if (endargs) {jsonth.type="constant";} else {jsonth.type="argument";};
                    if (a!=1) {jsonth.instruction_name=string_utilities::split(line,"=")[0];} else {jsonth.instruction_name="type";}
                    auto eles=string_utilities::split(line," ");
                    string otherside;
                    for (int i=1;i<eles.size();++i) {
                        if (i!=1) {
                            otherside=otherside+"="+eles[i];
                        } else {
                            otherside=eles[i];
                        }
                    }
                    jsonth.args={otherside};
                    json_ints[to_string(a)+" "+genvyid()]=jsonth.syntax();
                }
            } else if (line=="endargs") {
                endargs=true;
            }
            a=a+1;
        }
        log_main_step=log_main_step+1;
        output::log("Writing json file.",log_main_step);
        nlohmann::json j=json_ints;
        outputfile<<j.dump();
        outputfile.close();
        if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file_source+" to "+output<<endl;}
    } else if (mode=="jsonvodka") {
        log_main_step=log_main_step+1;
        output::log("Opening output file.",log_main_step);
        ofstream outputfile(output);
        log_main_step=log_main_step+1;
        output::log("Converting vodka code to json format :",log_main_step);
        auto now=chrono::system_clock::now();
        time_t now_c=chrono::system_clock::to_time_t(now);
        tm utc=*std::gmtime(&now_c);
        stringstream ss;
        ss<<put_time(&utc, "%Y-%m-%dT%H:%M:%SZ");
        json_ints_v["metadata"]={{"metadata",{{"type","metadata"},{"vodka_version",vodka::LibraryVersion},{"json_version","5"},{"source_file",file_source},{"timestamp",ss.str()},{"json_type","vodka"}}}};
        output::log("Converting symbols : ",log_main_step,1,{1},{2});
        json_ints_v["symbols"]={};
        bool cellstart=false;
        vodka::json::vodka::VodkaCell actualcell;
        vector<vodka::json::vodka::VodkaCell> idencell;
        int cellcount=0;
        for (int i=0;i<context.file.symbols_list.size();++i) {
            output::log("Converting \""+context.file.symbols_list[i].content+"\".",log_main_step,2,{1,i+1},{2,context.file.symbols_list.size()});
            vodka::json::vodka::VodkaSymbol symb;
            symb.type=context.file.symbols_list[i].type;
            auto args=string_utilities::split(context.file.symbols_list[i].content," ");
            symb.args=vector<string>(args.begin()+1,args.end());
            symb.uid=genvyid();
            if (symb.type=="VODSTART" && cellstart==false) {
                cellcount=cellcount+1;
                vodka::json::vodka::VodkaCell cell;
                cell.name=symb.args[0];
                cell.uid=to_string(cellcount)+":"+genvyid();
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
            json_ints_v["symbols"][to_string(i+1)+":"+genvyid()]=symb.syntax();
        }
        output::log("Converting cells : ",log_main_step,1,{2},{2});
        for (int i=0;i<idencell.size();++i) {
            output::log("Converting cell "+context.file.cells_list[i].name+".",log_main_step,2,{2,i+1},{2,context.file.cells_list.size()});
            auto cellcontent=context.file.cells_list[i].content;
            for (auto a:cellcontent) {
                if (a.substr(0,5)=="vodka") {
                    vodka::json::vodka::VodkaVariableDeclaration vardec;
                    vardec.uid=genvyid();
                    auto eles=string_utilities::split(a,"=");
                    auto namepart=eles[0];
                    auto valuepart=eles[1];
                    vardec.variable_name=string_utilities::split(namepart," ")[1];
                    vardec.variable_datatype=string_utilities::split(valuepart," ")[0];
                    auto vvaluepart=string_utilities::split(valuepart," ");
                    vector<string> valuev=vector<string>(vvaluepart.begin()+1,vvaluepart.end());
                    string value="";
                    for (int b=0;b<valuev.size();++b) {
                        if (b==0) {
                            value=valuev[b];
                        } else {
                            value=value+valuev[b];
                        }
                    }
                    vardec.variable_value=value;
                    vodka::json::vodka::VodkaLine linecont;
                    linecont.thing="variable_declaration";
                    linecont.variable_declaration_element=vardec;
                    idencell[i].lines_content.push_back(linecont);
                } else if (a.substr(0,6)=="kernel") {
                    vodka::json::vodka::VodkaInstruction instr;
                    instr.library="kernel";
                    instr.uid=genvyid();
                    instr.name=string_utilities::split(a," ")[0];
                    instr.source="<builtin>";
                    auto eles=string_utilities::split(a," ");
                    instr.args=vector<string>(eles.begin()+1,eles.end());
                    vodka::json::vodka::VodkaLine linecont;
                    linecont.thing="instruction";
                    linecont.instruction_element=instr;
                    idencell[i].lines_content.push_back(linecont);
                } else if (a.substr(0,11)=="conversions") {
                    vodka::json::vodka::VodkaInstruction instr;
                    instr.library="conversions";
                    instr.uid=genvyid();
                    instr.name=string_utilities::split(a," ")[0];
                    instr.source="<builtin>";
                    auto eles=string_utilities::split(a," ");
                    instr.args=vector<string>(eles.begin()+1,eles.end());
                    vodka::json::vodka::VodkaLine linecont;
                    linecont.thing="instruction";
                    linecont.instruction_element=instr;
                    idencell[i].lines_content.push_back(linecont);
                } else if (a.substr(0,4)=="math") {
                    vodka::json::vodka::VodkaInstruction instr;
                    instr.library="math";
                    instr.uid=genvyid();
                    instr.name=string_utilities::split(a," ")[0];
                    instr.source="<builtin>";
                    auto eles=string_utilities::split(a," ");
                    instr.args=vector<string>(eles.begin()+1,eles.end());
                    vodka::json::vodka::VodkaLine linecont;
                    linecont.thing="instruction";
                    linecont.instruction_element=instr;
                    idencell[i].lines_content.push_back(linecont);
                }
            }
            json_ints_v[idencell[i].uid]=idencell[i].syntax();
        }
        log_main_step=log_main_step+1;
        output::log("Writing json file.",log_main_step);
        nlohmann::json j=json_ints_v;
        outputfile<<j.dump();
        outputfile.close();
        if (verbose=="r" || verbose=="a") {cout<<"\nSucessfully compile "+file_source+" to "+output<<endl;}
    }
    unsetenv("VODKA_SHOW_LOG_TIME");
    unsetenv("VODKA_DEBUG_MODE");
    unsetenv("VODKA_VERBOSE_MODE");
    unsetenv("VODKA_SHOW_VAR_WARNING");
    return 0;
}