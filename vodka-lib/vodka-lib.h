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
namespace vodka {
    const string version="0.3 beta 2";
    const string json_version="3";
    //* Every library that has a reserved name inside the transcoder
    const vector<string> internal_library={"kernel"};
    //* Every functions for every internal library
    const map<string,vector<string>> internal_library_functions={{"kernel",{"print","add","assign","free","invert","back","duplicate","abs","divmod","toint","todec"}}};
    //* Every internal type
    const vector<string> internal_type={"vodint","vodec","vodarg","vodka"};
    //* Every syscall
    const vector<string> internal_syscall={"PRINT","ADD","ASSIGN","FREE","INVERT","BACK","DUPLICATE","ABS","DIVMOD","TOINT","TODEC"};
    //* Every vodka codebase instructions
    const vector<string> vodka_instruction={"multiply"};
    //* Every conversions possible using syscalls
    const map<string,vector<string>> conversion_syscall={{"TOINT",{"vodec","vodarg"}},{"TODEC",{"vodint","vodarg"}}};
    //* Errors handling
    namespace errors {
        //* Contain the call stack of the error
        class sources_stack {
            public:
                vector<string> source;
                vector<string> file;
                //* Add a element to the source stack
                void add(const string& src,const string& fil) {source.push_back(src);file.push_back(fil);}
        };
        //* Basic class for errors
        class error_container {
            public:
                string error;
                string file="";
                vector<string> lines_content={};
                vector<int> lines={};
                sources_stack source;
                bool class_mode=false;
                error_container(string error,string file,vector<string> lines_content,vector<int> lines,sources_stack source,bool cm=false) {
                    this->error=error;
                    this->file=file;
                    this->lines=lines;
                    this->lines_content=lines_content;
                    this->source=source;
                    this->class_mode=cm;
                }
        };
        //* Error raising function
        void raise(error_container element);
    }
    //* Syscall utilities (instruction starting with "kernel.<something>")
    namespace syscalls {
        //* Base class for syscalls, doesn't modify when inside a syscall_class !
        class syscall {
            public:
                string name;
                bool support_multiple_argument;
        };
        //* Each syscall has his own class without heritating from the base one
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
                vector<string> argument_uid;
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
                string uid;
                syscall info;
            ABS() {
                info.name="ABS";
                info.support_multiple_argument=true;
            }
        };
        class DIVMOD {
            public:
                string quotient_uid;
                string rest_uid;
                string divisor_uid;
                string dividend_uid;
                syscall info;
            DIVMOD() {
                info.name="DIVMOD";
                info.support_multiple_argument=true;
            }
        };
        class TOINT {
            public:
                string uid_source;
                string uid_output;
                syscall info;
            TOINT() {
                info.name="TOINT";
                info.support_multiple_argument=true;
            }
        };
        class TODEC {
            public:
                string uid_source;
                string uid_output;
                syscall info;
            TODEC() {
                info.name="TODEC";
                info.support_multiple_argument=true;
            }
        };
        //* For registering a syscall, it need to be encapsuled into a syscall_container object
        class syscall_container {
            public:
                //* Type of syscall must be put into thing variable
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
                TOINT tointele;
                TODEC todecele;
            //* Function for getting the syntax of the syscall
            string syntax();
        };
    }
    //* Variables utilities
    namespace variables {
        //* Base class for datatypes
        class typess {
            public:
                string typenames;
        };
        //* Base class for variables metadatas (excluding value)
        class variable {
            public:
                string varname;
                string uuid;
                bool consts;
                bool write=true;
                bool define=false;
                bool algo_dependant;
        };
        //* Vodint type class : optimize for vodint internal structure
        class vodint {
            public:
                string value;
                variable varinfo;
                typess typeinfo;
            vodint() {
                typeinfo.typenames="vodint";
            }
        };
        //* Vodec type class : optimize for vodec internal structure
        class vodec {
            public:
                string value;
                variable varinfo;
                typess typeinfo;
            vodec() {
                typeinfo.typenames="vodec";
            }
        };
        //* Vodarg type class : optimize for vodarg internal structure
        class vodarg {
            public:
                variable varinfo;
                typess typeinfo;
            vodarg() {
                typeinfo.typenames="vodarg";
            }
        };
        //* For registering a variable, it need to be encapsuled into a element object
        class element {
            public:
                //* Type of variable must be put into thing variable
                string thing="";
                vodint intele;
                vodec decele;
                vodarg argele;
        };
    }
    //* Vodka analyser
    namespace analyser {
        using namespace vodka::errors;
        //* Base class to check if a line is conform to vodka syntax
        class line {
            public:
                string content;
                string file;
                int line;
                bool checked=false;
                bool skip=false;
            //* Check if the line is conform to vodka syntax (doesn't check the line argument)
            bool check(sources_stack lclstack={});
        };
        //* Class to direct the analyse to a specialised analyser
        class type_analyser {
            public:
                line line_analyse;
                bool checked=false;
                string type;
                string library_name;
                string instruction_name;
            //* Analyse the type of line (variable declaration, vodka instruction, library instruction, debug line)
            bool line_type_analyse(sources_stack lclstack={});
        };
        //* Class to parse a variable declaration
        class var_dec_analyser {
            public:
                type_analyser line_analyse;
                bool checked=false;
                bool checked_type_value=false;
                bool pre_treated=false;
                string name;
                string datatype;
                string value;
                bool is_kernel_const=false;
                bool is_vodka_const=false;
                vodka::variables::variable var;
                vodka::variables::element var_object;
                vodka::variables::element source_duplication;
                vodka::syscalls::syscall_container vodka_object;
            //* These functions should be used in this order
            //* Parse the variable declaration (name, datatype, value, constant)
            bool var_dec_analyse(sources_stack lclstack={});
            //* Check the type and value of the variable (use vodka::type::<concerned type>::check_value(), if datatype is vodka, please include a list of already declared variables inside the context argument) 
            bool check_type_value(vector<string> context={},sources_stack lclstack={});
            //* Make the corresponding vodka::variables::variable (please specifiy the original variable if datatype is vodka)
            bool make_info(sources_stack lclstack={});
            //* Make a pre-treatement of the value to store
            bool pre_treatement(sources_stack lclstack={});
            //* Output the variable under a vodka::variable::variable_container object (please specifiy the original variable if datatype is vodka)
            bool output(sources_stack lclstack={});
        };
    }
    //* Type utilities
    namespace type {
        using namespace vodka::errors;
        //* Vodint utilities (see vodka-lib.cpp for more informations)
        namespace vodint {
            bool check_value(const string& value,vodka::analyser::line context,sources_stack lclstack={});
            string remove_zero(const string& value,sources_stack lclstack={});
            string invert_value(const string& value,sources_stack lclstack={});
            string calculate_sign(const string& value1,const string& value2,sources_stack lclstack={});
            string abs(const string& value,sources_stack lclstack={});
        }
        //* Vodec utilities (see vodka-lib.cpp for more informations)
        namespace vodec {
            bool check_value(const string& value,vodka::analyser::line context,sources_stack lclstack={});
            string remove_zero(const string& value,sources_stack lclstack={});
        }
    }
    //* Json utilities
    namespace json {
        //* Kernel instructions json functions
        namespace kernel {
            //* Json container for kernel instructions
            class json_container {
                public:
                    string type;
                    string intname;
                    vector<string> args;
                map<string,string> syntax();
            };
        };
        //* Vodka instructions json functions
        namespace vodka {
            //* Json class for vodka instructions
            class instruction {
                public:
                    string name;
                    string source="<vodka>";
                    string library;
                    string uid;
                    vector<string> args;
                map<string,string> syntax();
            };
            //* Json class for vodka symbols
            class symbol {
                public:
                    string type;
                    string uid;
                    vector<string> args;
                map<string,string> syntax();
            };
            //* Json class for vodka variables declaration
            class var_declaration {
                public:
                    string name;
                    string type;
                    string decvalue;
                    string uid;
                map<string,string> syntax();
            };
            //* Json class for vodka lines which contain variables declaration or instructions
            class line_container {
                public:
                    string thing;
                    var_declaration varele;
                    instruction intele;
                map<string,string> syntax();
            };
            //* Json class for vodka cells
            class cell {
                public:
                    string name;
                    string uid;
                    symbol start;
                    symbol end;
                    vector<line_container> lines;
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
        //* UUID generator
        boost::uuids::uuid genuid();
        //* Logs functions
        void log(const string& text,string verbose,int x,string last,int sublevel=0,vector<int> substep={},vector<unsigned long> subtotal={});
        void debuglog(const string& text,int line,const string& cell,bool debugmode,string verbose,string file,bool debug_info=true);
    }
    //* Internal library
    namespace library {
        //* Class for a line that call a function from an internal library
        class function_call {
            public:
                vodka::analyser::type_analyser type_analyser;
                vector<string> variableslist_context;
                vodka::utilities::cellule cell_context;
                int iteration_number_context;
                string file_name_context;
                string verbose_context;
                int main_logstep_context;
                string last_logstep_context;
                map<string,vodka::variables::element> variablesdict_context;
                bool conversions_checked;
                string conversion_error_code;
        };
        //* Kernel internal library
        namespace kernel {
            using namespace vodka::errors;
            //* Main class for parsing line that call kernel internal library
            class traitement {
                public:
                    vodka::library::function_call call;
                    vector<vodka::syscalls::syscall_container> syscall_output;
                    bool checked=false;
                    bool var_flag=false;
                    //* Main function for parsing kernel internal library
                    bool kernel_traitement(sources_stack lclstack={});
                private:
                    string line;
                    //* Private functions for analysing each instructions
                    bool print_int(sources_stack lclstack={});
                    bool add_int(sources_stack lclstack={});
                    bool invert_int(sources_stack lclstack={});
                    bool free_int(sources_stack lclstack={});
                    bool abs_int(sources_stack lclstack={});
                    bool divmod_int(sources_stack lclstack={});
                    bool toint_int(sources_stack lclstack={});
                    bool todec_int(sources_stack lclstack={});
            };
        }
    }
    //* Internal instructions
    namespace instructions {
        using namespace vodka::errors;
        //* Class for a line that call a vodka instruction
        class instruction_call {
            public:
                vodka::analyser::type_analyser type_analyser;
                vector<string> variableslist_context;
                vodka::utilities::cellule cell_context;
                int iteration_number_context;
                string file_name_context;
                string verbose_context;
                int main_logstep_context;
                string last_logstep_context;
                map<string,vodka::variables::element> variablesdict_context;
        };
        class instruction_traitement {
            public:
                vodka::instructions::instruction_call call;
                vector<vodka::syscalls::syscall_container> syscalls_output;
                bool checked=false;
                bool var_flag=false;
                //* Main function for parsing vodka instruction
                bool traitement(sources_stack lclstack={});
            private:
                string line;
                //* Private functions for analysing each instructions
                bool multiply(sources_stack lclstack={});
        };
    }
}
#endif