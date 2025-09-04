#ifndef VODKA_LIB_H
#define VODKA_LIB_H
#include <string>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <functional>
#include "../dependencies/XoshiroCpp.hpp"
using namespace std;
//* Vodka standard utilities
//* For documentation, please refer to vodka-lib-usage.md
namespace vodka {
    const string LibraryVersion="0.5 beta 1";
    const string JsonVersion="5";
    //* Every library that has a reserved name inside the transcoder
    const vector<string> InternalLibraryList={"memory","conversions","math","vodstr"};
    //* Every functions for every internal library
    const map<string,vector<string>> InternalLibraryFunctions={{"memory",{"print","free","getmem"}},{"conversions",{"toint","todec","tostr"}},{"math",{"multiply","add","invert","abs","divmod","divide","mulint","muldec"}},{"vodstr",{"length","concat","substring","charat","reverse","escape","insert","find"}}};
    //* Every internal type
    const vector<string> InternalDatatypes={"vodint","vodec","vodstr","vodarg","vodka"};
    //* Every syscall
    const vector<string> InternalSyscalls={"PRINT","ADD","ASSIGN","FREE","INVERT","DUPLICATE","ABS","DIVMOD","TOINT","TODEC","MULINT","MULDEC","DIVIDE","LENGTH","CONCAT","SUBSTRING","CHARAT","REVERSE","ESCAPE","INSERT","FIND","GETMEM"};
    //* Indicate where to start datatype values replacement process (-1 mean instruction can't benefit of this feature)
    const map<string,int> IndexStartDatatypeReplacement={
        {"memory.print",1},
        {"memory.free",-1},
        {"conversions.toint",-1},
        {"conversions.todec",-1},
        {"conversions.tostr",-1},
        {"math.multiply",2},
        {"math.add",2},
        {"math.invert",-1},
        {"math.abs",-1},
        {"math.divmod",3},
        {"math.divide",2},
        {"math.mulint",2},
        {"math.muldec",2},
        {"vodstr.length",-1},
        {"vodstr.concat",-1},
        {"vodstr.substring",3},
        {"vodstr.charat",3},
        {"vodstr.reverse",-1},
        {"vodstr.escape",-1},
        {"vodstr.insert",-1},
        {"vodstr.find",-1},
        {"memory.getmem",2}
    };
    //* Every symbols
    const vector<string> InternalSymbol={"VODSTART","VODEND","VODIMPORT","VODTYPE","VODSTRUCT","VODCLASS","VODENDCLASS","VODEFINE"};
    //* Every program type
    const vector<string> InternalTypes={"app","command","shell","gui","logonui","logonshell","service"};
    //* Errors handling
    namespace errors {
        //* Contain the call stack of the error
        class SourcesStack {
            public:
                vector<string> source;
                vector<string> file;
                //* Add a element to the source stack
                void add(string src,string fil) {source.push_back(src);file.push_back(fil);}
        };
        //* Basic class for errors
        class ErrorContainer {
            public:
                string error;
                string file="";
                vector<string> lines_content={};
                vector<int> lines={};
                SourcesStack source;
                bool class_mode=false;
                ErrorContainer(string error,string file,vector<string> lines_content,vector<int> lines,SourcesStack source,bool cm=false) {
                    this->error=error;
                    this->file=file;
                    this->lines=lines;
                    this->lines_content=lines_content;
                    this->source=source;
                    this->class_mode=cm;
                }
        };
        //* Error raising function
        void raise(ErrorContainer element);
    }
    //* Syscall utilities
    namespace syscalls {
        //* Enum class for representating each syscall
        enum class SyscallsNames {
            PRINT,
            ADD,
            ASSIGN,
            FREE,
            INVERT,
            DUPLICATE,
            ABS,
            DIVMOD,
            TOINT,
            TODEC,
            MULDEC,
            MULINT,
            DIVIDE,
            LENGTH,
            CONCAT,
            SUBSTRING,
            CHARAT,
            REVERSE,
            ESCAPE,
            INSERT,
            FIND,
            GETMEM
        };
        //* Convert SyscallsNames object to string
        string syscall_to_string(SyscallsNames syscall);
        //* Each syscall has his own class without heritating from the base one
        class PRINT {
            public:
                string argument_uid;
                string name="PRINT";
        };
        class ADD {
            public:
                string output_uid;
                string first_uid;
                string second_uid;
                string name="ADD";
        };
        class ASSIGN {
            public:
                string value;
                string output_uid;
                string name="ASSIGN";
        };
        class FREE {
            public:
                string argument_uid;
                string name="FREE";
        };
        class INVERT {
            public:
                string source_uid;
                string output_uid;
                string name="INVERT";
        };
        class DUPLICATE {
            public:
                string source_uid;
                string output_uid;
                string name="DUPLICATE";
        };
        class ABS {
            public:
                string source_uid;
                string output_uid;
                string name="ABS";
        };
        class DIVMOD {
            public:
                string quotient_uid;
                string rest_uid;
                string divisor_uid;
                string dividend_uid;
                string name="DIVMOD";
        };
        class TOINT {
            public:
                string source_uid;
                string output_uid;
                string name="TOINT";
        };
        class TODEC {
            public:
                string source_uid;
                string output_uid;
                string name="TODEC";
        };
        class MULDEC {
            public:
                string output_uid;
                string first_uid;
                string second_uid;
                string precision_uid;
                string name="MULDEC";
        };
        class MULINT {
            public:
                string output_uid;
                string first_uid;
                string second_uid;
                string name="MULINT";
        };
        class DIVIDE {
            public:
                string output_uid;
                string first_uid;
                string second_uid;
                string precision_uid;
                string name="DIVIDE";
        };
        class LENGTH {
            public:
                string output_uid;
                string source_uid;
                string name="LENGTH";
        };
        class CONCAT {
            public:
                string output_uid;
                string first_uid;
                string second_uid;
                string name="CONCAT";
        };
        class SUBSTRING {
            public:
                string source_uid;
                string output_uid;
                string start_index_uid;
                string length_output_uid;
                string name="SUBSTRING";
        };
        class CHARAT {
            public:
                string output_uid;
                string input_uid;
                string char_uid;
                string name="CHARAT";
        };
        class REVERSE {
            public:
                string source_uid;
                string output_uid;
                string name="REVERSE";
        };
        class ESCAPE {
            public:
                string source_uid;
                string output_uid;
                string name="ESCAPE";
        };
        class INSERT {
            public:
                string source_uid;
                string output_uid;
                string index_uid;
                string string_to_insert_uid;
                string name="INSERT";
        };
        class FIND {
            public:
                string source_uid;
                string char_uid;
                string output_uid;
                string name="FIND";
        };
        class GETMEM {
            public:
                string source_uid;
                string output_uid;
                string name="GETMEM";
        };
        //* For registering a syscall, it need to be encapsuled into a SyscallContainer object
        class SyscallContainer {
            public:
                //* Type of syscall must be put into thing variable
                SyscallsNames thing;
                PRINT print_element;
                ADD add_element;
                ASSIGN assign_element;
                FREE free_element;
                INVERT invert_element;
                DUPLICATE duplicate_element;
                ABS abs_element;
                DIVMOD divmod_element;
                TOINT toint_element;
                TODEC todec_element;
                MULINT mulint_element;
                MULDEC muldec_element;
                DIVIDE divide_element;
                LENGTH length_element;
                CONCAT concat_element;
                SUBSTRING substring_element;
                CHARAT charat_element;
                REVERSE reverse_element;
                ESCAPE escape_element;
                INSERT insert_element;
                FIND find_element;
                GETMEM getmem_element;
            //* Function for getting the syntax of the syscall
            string syntax();
        };
    }
    //* Variables utilities
    namespace variables {
        //* Enum class for representating a datatype
        enum class VariableDatatype {
            vodint,
            vodec,
            vodstr,
            vodarg
        };
        //* Convert VariableDatatype object to string
        string datatype_to_string(VariableDatatype datatype);
        //* Base class for variables metadatas (excluding value)
        class VariableMetadata {
            public:
                string name;
                string uuid;
                bool is_vodka_constant;
                bool in_data_section=true;
                bool is_kernel_constant=false;
                bool is_null_as_declaration=false;
                bool algo_dependant;
        };
        //* Vodint type class : optimize for vodint internal structure
        class VodintVariable {
            public:
                string value;
        };
        //* Vodec type class : optimize for vodec internal structure
        class VodecVariable {
            public:
                string value;
        };
        //* Vodec type class : optimize for vodec internal structure
        class VodstrVariable {
            public:
                string value;
        };
        //* Vodarg type class : optimize for vodarg internal structure
        class VodargVariable {
            public:
        };
        //* For registering a variable, it need to be encapsuled into a element object
        class VariableContainer {
            public:
                //* Type of variable must be put into thing variable
                VariableDatatype thing;
                VariableMetadata variable_metadata;
                VodintVariable vodint_element;
                VodecVariable vodec_element;
                VodstrVariable vodstr_element;
                VodargVariable vodarg_element;
        };
    }
    //* General utilities
    namespace utilities {
        //* Vodka structure
        namespace structs {
            struct symbol {
                int line;
                string content;
                string type;
            };
            struct cell {
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
        }
        //* UUID generator
        std::string genvyid();
        //* Logs functions
        namespace output {
            void log(string text,int log_main_step,int sublevel=0,vector<int> substep={},vector<unsigned long> subtotal={});
            void debuglog(string text,int line,string cell,string file,bool debug_info=true);
            void var_warning(string namevar,vodka::variables::VariableDatatype typevar,string namecell,string line);
        }
        //* String utilities
        namespace string_utilities {
            vector<string> split(string str,string delimiter);
            void replaceall(string str,string from,string to);
            string strip(string text,string character);
        }
        double get_process_time();
        template<typename type,typename... Args> class SafeAttribute {
            private:
                type value;
                bool initialized=false;
                std::function<type(Args...)> executor;
            public:
                SafeAttribute(std::function<type(Args...)> exec):executor(exec) {}
                const type& run(Args... args) {
                    if (!initialized) {
                        value=executor(std::forward<Args>(args)...);
                        initialized=true;
                    }
                    return value;
                }
                bool is_initialized() const {return initialized;};
                const type& get() {return value;};
        };
        //* Encoding utilities
        namespace encoding {
            string hash_then_encode(string origin);
            string encode_to_bin(string input);
        }
    }
    //* Vodka analyser
    namespace analyser {
        using namespace vodka::errors;
        using namespace vodka::utilities;
        //* Return the list of variables passed in argument
        vector<string> get_arguments(string line);
        //* Base class to check if a line is conform to vodka syntax
        class LineSyntaxChecker {
            private:
                bool _check(vodka::errors::SourcesStack lclstack={});
                SafeAttribute<bool,SourcesStack> checked;
            public:
                LineSyntaxChecker():checked([this](SourcesStack lclstack){return _check(lclstack);}) {}
                string content;
                string file;
                int line_number;
                bool shoulb_be_skip=false;
                //* Check if the line is conform to vodka syntax (doesn't check the line argument)
                bool check(SourcesStack lclstack) {return checked.run(lclstack);}
                bool get_check_result() {return checked.is_initialized() && checked.get();}
        };
        //* Class to direct the analyse to a specialised analyser
        class LineTypeChecker {
            private:
                bool _line_type_analyse(vodka::errors::SourcesStack lclstack={});
                SafeAttribute<bool,SourcesStack> checked;
            public:
                LineTypeChecker():checked([this](SourcesStack lclstack){return _line_type_analyse(lclstack);}) {}
                LineSyntaxChecker line_checked;
                string type;
                string library_name;
                string instruction_name;
                //* Analyse the type of line (variable declaration, library instruction, debug line)
                bool line_type_analyse(SourcesStack lclstack) {return checked.run(lclstack);}
                bool get_analyse_result() {return checked.is_initialized() && checked.get();}
        };
        //* Class to parse a variable declaration
        class VariableDeclarationAnalyser {
            private:
                bool _parser(vodka::errors::SourcesStack lclstack={});
                bool _check_type_value(vector<string> context={},vodka::errors::SourcesStack lclstack={});
                bool _make_info(vodka::errors::SourcesStack lclstack={});
                bool _value_pre_treatement(vodka::errors::SourcesStack lclstack={});
                bool _make_output(vodka::errors::SourcesStack lclstack={});
                SafeAttribute<bool,SourcesStack> parsed;
                SafeAttribute<bool,vector<string>,SourcesStack> checked_type_value;
                SafeAttribute<bool,SourcesStack> info_done;
                SafeAttribute<bool,SourcesStack> pre_treated;
                SafeAttribute<bool,SourcesStack> output_done;
            public:
                VariableDeclarationAnalyser():
                parsed([this](SourcesStack srclclstack){return _parser(srclclstack);}),
                checked_type_value([this](vector<string> context={},SourcesStack srclclstack){return _check_type_value(context,srclclstack);}),
                info_done([this](SourcesStack srclclstack){return _make_info(srclclstack);}),
                pre_treated([this](SourcesStack srclclstack){return _value_pre_treatement(srclclstack);}),
                output_done([this](SourcesStack srclclstack){return _make_output(srclclstack);}) {}
                LineTypeChecker line_checked;
                string name;
                string datatype;
                string value;
                bool is_kernel_constant=false;
                bool is_vodka_const=false;
                vodka::variables::VariableMetadata variable_metadata;
                vodka::variables::VariableContainer variable_container;
                vodka::variables::VariableContainer duplication_source_variable;
                vodka::syscalls::SyscallContainer syscall_container;
                vector<string> variableslist_context;
                map<string,vodka::variables::VariableContainer> variablesdict_context;
                //* These functions should be used in this order
                //* Parse the variable declaration (name, datatype, value, constant)
                bool parser(SourcesStack lclstack) {return parsed.run(lclstack);}
                bool get_parser_result() {return parsed.is_initialized() && parsed.get();}
                //* Check the type and value of the variable (use vodka::type::<concerned type>::check_value(), if datatype is vodka, please include a list of already declared variables inside the context argument) 
                bool check_type_value(vector<string> context,SourcesStack lclstack) {return checked_type_value.run(context,lclstack);}
                bool get_check_type_value_result() {return checked_type_value.is_initialized() && checked_type_value.get();}
                //* Make the corresponding vodka::variables::VariableMetadata (please specifiy the original variable if datatype is vodka)
                bool make_info(SourcesStack lclstack) {return info_done.run(lclstack);}
                bool get_make_info_result() {return info_done.is_initialized() && info_done.get();}
                //* Make a pre-treatement of the value to store
                bool value_pre_treatement(SourcesStack lclstack) {return pre_treated.run(lclstack);}
                bool get_pre_treatement_result() {return pre_treated.is_initialized() && pre_treated.get();}
                //* Output the variable under a vodka::variable::VariableContainer object (please specifiy the original variable if datatype is vodka)
                bool make_output(SourcesStack lclstack) {return output_done.run(lclstack);}
                bool get_make_output_result() {return output_done.is_initialized() && output_done.get();}
        };
        //* Class to check the existence and datatype of each argument
        class ArgumentChecker {
            public:
                vector<string> variableslist_context;
                map<string,vodka::variables::VariableContainer> variablesdict_context;
                vector<vector<vodka::variables::VariableDatatype>> patern;
                vector<vodka::variables::VariableDatatype> lastest_allowed_type;
                vodka::analyser::LineTypeChecker line_content;
            //* Check the existence and datatype of each argument
            bool check(vodka::errors::SourcesStack lclstack={});
        };
    }
    //* Type utilities
    namespace type {
        //* Vodint utilities (see vodka-lib.cpp for more informations)
        namespace vodint {
            bool check_value(string value,vodka::analyser::LineSyntaxChecker context,vodka::errors::SourcesStack lclstack={});
            string remove_zero(string value,vodka::errors::SourcesStack lclstack={});
        }
        //* Vodec utilities (see vodka-lib.cpp for more informations)
        namespace vodec {
            bool check_value(string value,vodka::analyser::LineSyntaxChecker context,vodka::errors::SourcesStack lclstack={});
            string remove_zero(string value,vodka::errors::SourcesStack lclstack={});
        }
    }
    //* Json utilities
    namespace json {
        //* Kernel instructions json functions
        namespace kernel {
            //* Json container for kernel instructions
            class JsonContainer {
                public:
                    string type;
                    string instruction_name;
                    vector<string> args;
                map<string,string> syntax();
            };
        };
        //* Vodka instructions json functions
        namespace vodka {
            //* Json class for vodka instructions
            class VodkaInstruction {
                public:
                    string name;
                    string source="<builtin>";
                    string library;
                    string uid;
                    vector<string> args;
                map<string,string> syntax();
            };
            //* Json class for vodka symbols
            class VodkaSymbol {
                public:
                    string type;
                    string uid;
                    vector<string> args;
                map<string,string> syntax();
            };
            //* Json class for vodka variables declaration
            class VodkaVariableDeclaration {
                public:
                    string variable_name;
                    string variable_datatype;
                    string variable_value;
                    string uid;
                map<string,string> syntax();
            };
            //* Json class for vodka lines which contain variables declaration or instructions
            class VodkaLine {
                public:
                    string thing;
                    VodkaVariableDeclaration variable_declaration_element;
                    VodkaInstruction instruction_element;
                map<string,string> syntax();
            };
            //* Json class for vodka cells
            class VodkaCell {
                public:
                    string name;
                    string uid;
                    VodkaSymbol start;
                    VodkaSymbol end;
                    vector<VodkaLine> lines_content;
                map<string,map<string,string>> syntax();
            };
        };
    }
    //* Internal library
    namespace library {
        using namespace vodka::errors;
        using namespace vodka::utilities;
        //* Class for a line that call a function from an internal library
        class FunctionCall {
            public:
                vodka::analyser::LineTypeChecker line_checked;
                vodka::utilities::structs::cell cell_context;
                int iteration_number_context;
                string file_name_context;
                string verbose_context;
                int main_logstep_context;
                vector<string> variableslist_context;
                map<string,vodka::variables::VariableContainer> variablesdict_context;
        };
        //* Memory internal library
        namespace memory {
            //* Main class for parsing line that call memory internal library
            class CallTreatement {
                private:
                    string line;
                    //* Private functions for analysing each instructions
                    bool print_int(vodka::errors::SourcesStack lclstack={});
                    bool free_int(vodka::errors::SourcesStack lclstack={});
                    bool getmem_int(vodka::errors::SourcesStack lclstack={});
                    SafeAttribute<bool,SourcesStack> checked;
                    bool _call_treatement(vodka::errors::SourcesStack lclstack={});
                public:
                    CallTreatement():checked([this](SourcesStack lclstack){return _call_treatement(lclstack);}) {}
                    vodka::library::FunctionCall function_call;
                    vector<vodka::syscalls::SyscallContainer> syscalls_output;
                    bool var_flag=false;
                    //* Main function for parsing memory internal library
                    bool call_treatement(SourcesStack lclstack) {return checked.run(lclstack);}
                    bool get_call_treatement_result() {return checked.is_initialized() && checked.get();}
                private:
            };
        }
        //* Conversions internal library
        namespace conversions {
            //* Main class for parsing line that call converions internal library
            class CallTreatement {
                private:
                    string line;
                    //* Private functions for analysing each instructions
                    bool toint_int(vodka::errors::SourcesStack lclstack={});
                    bool todec_int(vodka::errors::SourcesStack lclstack={});
                    bool tostr_int(vodka::errors::SourcesStack lclstack={});
                    SafeAttribute<bool,SourcesStack> checked;
                    bool _call_treatement(vodka::errors::SourcesStack lclstack={});
                public:
                    CallTreatement():checked([this](SourcesStack lclstack){return _call_treatement(lclstack);}) {}
                    vodka::library::FunctionCall function_call;
                    vector<vodka::syscalls::SyscallContainer> syscalls_output;
                    bool var_flag=false;
                    //* Main function for parsing conversions internal library
                    bool call_treatement(SourcesStack lclstack) {return checked.run(lclstack);}
                    bool get_call_treatement_result() {return checked.is_initialized() && checked.get();}
            };
        }
        namespace math {
            //* Main class for parsing line that call converions internal library
            class CallTreatement {
                private:
                    string line;
                    //* Private functions for analysing each instructions
                    bool add_int(vodka::errors::SourcesStack lclstack={});
                    bool invert_int(vodka::errors::SourcesStack lclstack={});
                    bool abs_int(vodka::errors::SourcesStack lclstack={});
                    bool divmod_int(vodka::errors::SourcesStack lclstack={});
                    bool divide_int(vodka::errors::SourcesStack lclstack={});
                    bool mulint_int(vodka::errors::SourcesStack lclstack={});
                    bool muldec_int(vodka::errors::SourcesStack lclstack={});
                    bool multiply(vodka::errors::SourcesStack lclstack={});
                    SafeAttribute<bool,SourcesStack> checked;
                    bool _call_treatement(vodka::errors::SourcesStack lclstack={});
                public:
                    CallTreatement():checked([this](SourcesStack lclstack){return _call_treatement(lclstack);}) {}
                    vodka::library::FunctionCall function_call;
                    vector<vodka::syscalls::SyscallContainer> syscalls_output;
                    bool var_flag=false;
                    //* Main function for parsing math internal library
                    bool call_treatement(SourcesStack lclstack) {return checked.run(lclstack);}
                    bool get_call_treatement_result() {return checked.is_initialized() && checked.get();}
            };
        }
        namespace vodstr {
            //* Main class for parsing line that call converions internal library
            class CallTreatement {
                private:
                    string line;
                    //* Private functions for analysing each instructions
                    bool length_int(vodka::errors::SourcesStack lclstack={});
                    bool concat_int(vodka::errors::SourcesStack lclstack={});
                    bool substring_int(vodka::errors::SourcesStack lclstack={});
                    bool charat_int(vodka::errors::SourcesStack lclstack={});
                    bool reverse_int(vodka::errors::SourcesStack lclstack={});
                    bool escape_int(vodka::errors::SourcesStack lclstack={});
                    bool insert_int(vodka::errors::SourcesStack lclstack={});
                    bool find_int(vodka::errors::SourcesStack lclstack={});
                    SafeAttribute<bool,SourcesStack> checked;
                    bool _call_treatement(vodka::errors::SourcesStack lclstack={});
                public:
                    CallTreatement():checked([this](SourcesStack lclstack){return _call_treatement(lclstack);}) {}
                    vodka::library::FunctionCall function_call;
                    vector<vodka::syscalls::SyscallContainer> syscalls_output;
                    bool var_flag=false;
                    //* Main function for parsing math internal library
                    bool call_treatement(SourcesStack lclstack) {return checked.run(lclstack);}
                    bool get_call_treatement_result() {return checked.is_initialized() && checked.get();}
            };
        }
    }
    //* Compilation utilities
    namespace compilation {
        using namespace vodka::errors;
        using namespace vodka::utilities;
        //* The content of a .vod file, allow for removing inline comments
        class VodkaFile {
            private:
                bool _remove_comments();
                SafeAttribute<bool> removed_comments;
            public:
                VodkaFile():removed_comments([this](){return _remove_comments();}) {}
                vector<string> file_content;
                string file_source;
                //* Remove the inline comments
                bool remove_comments() {return removed_comments.run();}
                bool get_remove_comments() {return removed_comments.is_initialized() && removed_comments.get();}
        };
        //* The PreCompilation part, allow for structuring the compilation into symbols, cells and code pre-treatement
        class PreCompilation {
            private:
                SafeAttribute<bool,int&,SourcesStack> symbols_parsed;
                SafeAttribute<bool,int&,SourcesStack> type_found;
                SafeAttribute<bool,int&,SourcesStack> cells_done;
                SafeAttribute<bool,vector<string>&,bool,int&,SourcesStack> code_pretreatement_done;
                bool _parse_symbols(int& log_main_step,SourcesStack srclclstack);
                bool _detect_program_type(int& log_main_step,SourcesStack srclclstack);
                bool _detect_cells(int& log_main_step,SourcesStack srclclstack);
                bool _code_pretreatement(vector<string>& compiled_output,bool replace,int& log_main_step,SourcesStack srclclstack);
            public:
                PreCompilation():
                symbols_parsed([this](int& log_main_step,SourcesStack srclclstack){return _parse_symbols(log_main_step,srclclstack);}),
                type_found([this](int& log_main_step,SourcesStack srclclstack){return _detect_program_type(log_main_step,srclclstack);}),
                cells_done([this](int& log_main_step,SourcesStack srclclstack){return _detect_cells(log_main_step,srclclstack);}),
                code_pretreatement_done([this](vector<string>& compiled_output,bool replace,int& log_main_step,SourcesStack srclclstack){return _code_pretreatement(compiled_output,replace,log_main_step,srclclstack);}) {}
                map<string,vodka::variables::VariableContainer> maincell_args_dict;
                map<string,vodka::variables::VariableContainer> variables_dict;
                vector<string> maincell_args_list;
                vector<string> variables_list;
                VodkaFile file;
                map<string,string> replacement;
                vector<vodka::utilities::structs::symbol> symbols_list;
                string program_type;
                vector<structs::cell> cells_list;
                vector<string> cells_names;
                structs::cell maincell;
                //* Detect symbols and process symbols non-related to cells
                bool parse_symbol(int& log_main_step,SourcesStack srclclstack) {return symbols_parsed.run(log_main_step,srclclstack);};
                bool get_parse_symbol() {return symbols_parsed.is_initialized() && symbols_parsed.get();};
                //* Processing VODTYPE symbol
                bool detect_program_type(int& log_main_step,SourcesStack srclclstack) {return type_found.run(log_main_step,srclclstack);};
                bool get_detect_program_type() {return type_found.is_initialized() && type_found.get();};
                //* Process symbols related to cells
                bool detect_cells(int& log_main_step,SourcesStack srclclstack) {return cells_done.run(log_main_step,srclclstack);};
                bool get_detect_cells() {return cells_done.is_initialized() && cells_done.get();};
                //* Apply pre-treatement on the code
                bool code_pretreatement(vector<string>& compiled_output,bool replace,int& log_main_step,SourcesStack srclclstack) {return code_pretreatement_done.run(compiled_output,replace,log_main_step,srclclstack);};
                bool get_code_pretreatement() {return code_pretreatement_done.is_initialized() && code_pretreatement_done.get();};
        };
        class CompilationContext {
            public:
                PreCompilation file;
                map<string,vodka::variables::VariableContainer> maincell_args_dict;
                map<string,vodka::variables::VariableContainer> variables_dict;
                vector<string> maincell_args_list;
                vector<string> variables_list;
                //* Set all the variables to their expected values
                bool setup() {
                    if (file.get_code_pretreatement()==false) {
                        return false;
                    }
                    maincell_args_dict=file.maincell_args_dict;
                    maincell_args_list=file.maincell_args_list;
                    variables_dict=file.variables_dict;
                    variables_list=file.variables_list;
                    return true;
                }
        };
    }
}
#endif