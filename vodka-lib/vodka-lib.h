#ifndef VODKA_LIB_H
#define VODKA_LIB_H
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
#include <iostream>
using namespace std;
//* Vodka standard utilities
//* For documentation, please refer to vodka-lib-usage.md
namespace vodka {
    const string LibraryVersion="0.4 beta 1";
    const string JsonVersion="4";
    //* Every library that has a reserved name inside the transcoder
    const vector<string> InternalLibraryList={"kernel"};
    //* Every functions for every internal library
    const map<string,vector<string>> InternalLibraryFunctions={{"kernel",{"print","add","assign","free","invert","back","duplicate","abs","divmod","toint","todec","divide","mulint","muldec"}}};
    //* Every internal type
    const vector<string> InternalDataypes={"vodint","vodec","vodarg","vodka"};
    //* Every syscall
    const vector<string> InternalSyscalls={"PRINT","ADD","ASSIGN","FREE","INVERT","DUPLICATE","ABS","DIVMOD","TOINT","TODEC","MULINT","MULDEC","DIVIDE"};
    //* Every vodka codebase instructions
    const vector<string> VodkaInstructions={"multiply"};
    //* Every conversions possible using syscalls
    const map<string,vector<string>> ConversionsSyscalls={{"TOINT",{"vodec","vodarg"}},{"TODEC",{"vodint","vodarg"}}};
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
    //* Syscall utilities (instruction starting with "kernel.<something>")
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
            DIVIDE
        };
        //* Convert SyscallsNames object to string
        string syscall_to_string(SyscallsNames syscall);
        //* Each syscall has his own class without heritating from the base one
        class PRINT {
            public:
                vector<string> argument_uid;
                string name="PRINT";
        };
        class ADD {
            public:
                vector<string> argument_uid;
                string output_uid;
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
                vector<string> argument_uid;
                string name="FREE";
        };
        class INVERT {
            public:
                string uid;
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
                string uid;
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
                string uid_source;
                string uid_output;
                string name="TOINT";
        };
        class TODEC {
            public:
                string uid_source;
                string uid_output;
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
            //* Analyse the type of line (variable declaration, vodka instruction, library instruction, debug line)
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
        //* UUID generator
        boost::uuids::uuid genuid();
        //* Logs functions
        void log(string text,int log_main_step,string last,int sublevel=0,vector<int> substep={},vector<unsigned long> subtotal={});
        void debuglog(string text,int line,string cell,string file,bool debug_info=true);
        void var_warning(string namevar,vodka::variables::VariableDatatype typevar,string namecell,string line);
        //* String utilities
        vector<string> split(string str,string delimiter);
        void replaceall(string str,string from,string to);
        double get_process_time();
    }
    //* Internal library
    namespace library {
        //* Class for a line that call a function from an internal library
        class FunctionCall {
            public:
                vodka::analyser::LineTypeChecker line_checked;
                vector<string> variableslist_context;
                vodka::utilities::cell cell_context;
                int iteration_number_context;
                string file_name_context;
                string verbose_context;
                int main_logstep_context;
                string last_logstep_context;
                map<string,vodka::variables::VariableContainer> variablesdict_context;
        };
        //* Kernel internal library
        namespace kernel {
            //* Main class for parsing line that call kernel internal library
            class CallTreatement {
                public:
                    vodka::library::FunctionCall function_call;
                    vector<vodka::syscalls::SyscallContainer> syscall_output;
                    bool checked=false;
                    bool var_flag=false;
                    //* Main function for parsing kernel internal library
                    bool call_treatement(vodka::errors::SourcesStack lclstack={});
                private:
                    //* Every datatypes supported by each function of kernel internal library
                    const map<string,vector<string>> supported_type={{"add",{"vodint","vodec"}},{"abs",{"vodint","vodec"}}};
                    string line;
                    //* Private functions for analysing each instructions
                    bool print_int(vodka::errors::SourcesStack lclstack={});
                    bool add_int(vodka::errors::SourcesStack lclstack={});
                    bool invert_int(vodka::errors::SourcesStack lclstack={});
                    bool free_int(vodka::errors::SourcesStack lclstack={});
                    bool abs_int(vodka::errors::SourcesStack lclstack={});
                    bool divmod_int(vodka::errors::SourcesStack lclstack={});
                    bool toint_int(vodka::errors::SourcesStack lclstack={});
                    bool todec_int(vodka::errors::SourcesStack lclstack={});
                    bool divide_int(vodka::errors::SourcesStack lclstack={});
                    bool mulint_int(vodka::errors::SourcesStack lclstack={});
                    bool muldec_int(vodka::errors::SourcesStack lclstack={});
            };
        }
    }
    //* Internal instructions
    namespace instructions {
        //* Class for a line that call a vodka instruction
        class InstructionCall {
            public:
                vodka::analyser::LineTypeChecker line_checked;
                vector<string> variableslist_context;
                vodka::utilities::cell cell_context;
                int iteration_number_context;
                string file_name_context;
                string verbose_context;
                int main_logstep_context;
                string last_logstep_context;
                map<string,vodka::variables::VariableContainer> variablesdict_context;
        };
        class CallTreatement {
            public:
                vodka::instructions::InstructionCall instruction_call;
                vector<vodka::syscalls::SyscallContainer> syscalls_output;
                bool checked=false;
                bool var_flag=false;
                //* Main function for parsing vodka instruction
                bool call_treatement(vodka::errors::SourcesStack lclstack={});
            private:
                string line;
                const map<string,vector<string>> supported_type={{"multiply",{"vodint","vodec"}}};
                //* Private functions for analysing each instructions
                bool multiply(vodka::errors::SourcesStack lclstack={});
        };
    }
}
#endif