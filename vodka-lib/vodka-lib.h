#ifndef VODKA_LIB_H
#define VODKA_LIB_H
#include <string>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "../dependencies/XoshiroCpp.hpp"
using namespace std;
//* Vodka standard utilities
//* For documentation, please refer to vodka-lib-usage.md
namespace vodka {
    const string LibraryVersion="0.4";
    const string JsonVersion="4";
    //* Every library that has a reserved name inside the transcoder
    const vector<string> InternalLibraryList={"memory","conversions","math","vodstr"};
    //* Every functions for every internal library
    const map<string,vector<string>> InternalLibraryFunctions={{"memory",{"print","free","getmem"}},{"conversions",{"toint","todec","tostr"}},{"math",{"multiply","add","invert","abs","divmod","divide","mulint","muldec"}},{"vodstr",{"length","concat","substring","charat","reverse","escape","insert","find"}}};
    //* Every internal type
    const vector<string> InternalDataypes={"vodint","vodec","vodstr","vodarg","vodka"};
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
    //* Vodka analyser
    namespace analyser {
        //* Return the list of variables passed in argument
        vector<string> get_arguments(string line);
        //* Base class to check if a line is conform to vodka syntax
        class LineSyntaxChecker {
            public:
                string content;
                string file;
                int line_number;
                bool checked=false;
                bool shoulb_be_skip=false;
            //* Check if the line is conform to vodka syntax (doesn't check the line argument)
            bool check(vodka::errors::SourcesStack lclstack={});
        };
        //* Class to direct the analyse to a specialised analyser
        class LineTypeChecker {
            public:
                LineSyntaxChecker line_checked;
                bool checked=false;
                string type;
                string library_name;
                string instruction_name;
            //* Analyse the type of line (variable declaration, library instruction, debug line)
            bool line_type_analyse(vodka::errors::SourcesStack lclstack={});
        };
        //* Class to parse a variable declaration
        class VariableDeclarationAnalyser {
            public:
                LineTypeChecker line_checked;
                bool checked=false;
                bool checked_type_value=false;
                bool pre_treated=false;
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
            bool parser(vodka::errors::SourcesStack lclstack={});
            //* Check the type and value of the variable (use vodka::type::<concerned type>::check_value(), if datatype is vodka, please include a list of already declared variables inside the context argument) 
            bool check_type_value(vector<string> context={},vodka::errors::SourcesStack lclstack={});
            //* Make the corresponding vodka::variables::variable (please specifiy the original variable if datatype is vodka)
            bool make_info(vodka::errors::SourcesStack lclstack={});
            //* Make a pre-treatement of the value to store
            bool value_pre_treatement(vodka::errors::SourcesStack lclstack={});
            //* Output the variable under a vodka::variable::variable_container object (please specifiy the original variable if datatype is vodka)
            bool make_output(vodka::errors::SourcesStack lclstack={});
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
            struct random_values {
                uint64_t rand1;
                uint64_t rand2;
                uint64_t rand3;
            };
        }
        //* UUID generator
        std::string genvyid(XoshiroCpp::Xoshiro256StarStar& gen,vodka::utilities::structs::random_values& rand);
        std::string genvyid();
        //* Logs functions
        namespace output {
            void log(string text,int log_main_step,string last,int sublevel=0,vector<int> substep={},vector<unsigned long> subtotal={});
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
    }
    //* Internal library
    namespace library {
        //* Class for a line that call a function from an internal library
        class FunctionCall {
            public:
                vodka::analyser::LineTypeChecker line_checked;
                vodka::utilities::structs::cell cell_context;
                int iteration_number_context;
                string file_name_context;
                string verbose_context;
                int main_logstep_context;
                string last_logstep_context;
                vector<string> variableslist_context;
                map<string,vodka::variables::VariableContainer> variablesdict_context;
        };
        //* Memory internal library
        namespace memory {
            //* Main class for parsing line that call memory internal library
            class CallTreatement {
                public:
                    vodka::library::FunctionCall function_call;
                    vector<vodka::syscalls::SyscallContainer> syscalls_output;
                    bool checked=false;
                    bool var_flag=false;
                    //* Main function for parsing memory internal library
                    bool call_treatement(vodka::errors::SourcesStack lclstack={});
                private:
                    string line;
                    //* Private functions for analysing each instructions
                    bool print_int(vodka::errors::SourcesStack lclstack={});
                    bool free_int(vodka::errors::SourcesStack lclstack={});
                    bool getmem_int(vodka::errors::SourcesStack lclstack={});
            };
        }
        //* Conversions internal library
        namespace conversions {
            //* Main class for parsing line that call converions internal library
            class CallTreatement {
                public:
                    vodka::library::FunctionCall function_call;
                    vector<vodka::syscalls::SyscallContainer> syscalls_output;
                    bool checked=false;
                    bool var_flag=false;
                    //* Main function for parsing conversions internal library
                    bool call_treatement(vodka::errors::SourcesStack lclstack={});
                private:
                    string line;
                    //* Private functions for analysing each instructions
                    bool toint_int(vodka::errors::SourcesStack lclstack={});
                    bool todec_int(vodka::errors::SourcesStack lclstack={});
                    bool tostr_int(vodka::errors::SourcesStack lclstack={});
            };
        }
        namespace math {
            //* Main class for parsing line that call converions internal library
            class CallTreatement {
                public:
                    vodka::library::FunctionCall function_call;
                    vector<vodka::syscalls::SyscallContainer> syscalls_output;
                    bool checked=false;
                    bool var_flag=false;
                    //* Main function for parsing math internal library
                    bool call_treatement(vodka::errors::SourcesStack lclstack={});
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
            };
        }
        namespace vodstr {
            //* Main class for parsing line that call converions internal library
            class CallTreatement {
                public:
                    vodka::library::FunctionCall function_call;
                    vector<vodka::syscalls::SyscallContainer> syscalls_output;
                    bool checked=false;
                    bool var_flag=false;
                    //* Main function for parsing math internal library
                    bool call_treatement(vodka::errors::SourcesStack lclstack={});
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
            };
        }
    }
}
#endif