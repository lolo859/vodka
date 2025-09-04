#include "../vodka-lib.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
using namespace vodka::utilities;
using namespace vodka::errors;
//* Private functions for analysing each instructions
bool vodka::library::memory::CallTreatement::print_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()>=2) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vector<string> argsname(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec,vodka::variables::VariableDatatype::vodstr,vodka::variables::VariableDatatype::vodarg};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec,vodka::variables::VariableDatatype::vodstr,vodka::variables::VariableDatatype::vodarg}};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        output::log("Registering system call.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vector<string> argsuid;
        for (auto a:argsname) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        for (int i=0;i<argsuid.size();i++) {
            vodka::syscalls::PRINT printcall;
            printcall.argument_uid=argsuid[i];
            vodka::syscalls::SyscallContainer syscont;
            syscont.print_element=printcall;
            syscont.thing=vodka::syscalls::SyscallsNames::PRINT;
            syscalls_output.push_back(syscont);
        }
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.memory.print.invalid syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::memory::CallTreatement::free_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()>=2) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vector<string> argsname(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec,vodka::variables::VariableDatatype::vodstr};
        argcheck.patern={};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        string arg;
        for (int y=1;y<eles.size();++y) {
            arg=eles[y];
            if (eles[y].substr(0,2)=="$$") {
                raise(ErrorContainer("vodka.error.variables.constant : Can't delete an kernel constant.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        output::log("Registering system call.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vector<string> argsuid;
        for (auto a:argsname) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        for (int i=0;i<argsuid.size();i++) {
            vodka::syscalls::FREE freecall;
            freecall.argument_uid=argsuid[i];
            vodka::syscalls::SyscallContainer syscont;
            syscont.free_element=freecall;
            syscont.thing=vodka::syscalls::SyscallsNames::FREE;
            syscalls_output.push_back(syscont);
        }
        auto args_erase=vector<string>(eles.begin()+1,eles.end());
        for (auto a:args_erase) {
            auto it=remove(function_call.variableslist_context.begin(),function_call.variableslist_context.end(),a);
            function_call.variableslist_context.erase(it,function_call.variableslist_context.end());
            function_call.variablesdict_context.erase(a);
        }
        var_flag=true;
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.memory.free.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::memory::CallTreatement::getmem_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==3) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vector<string> argsname(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodarg,vodka::variables::VariableDatatype::vodec,vodka::variables::VariableDatatype::vodstr}};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(ErrorContainer("vodka.error.variables.constant : Can't modify an constant.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
            return false;
        }
        output::log("Registering system call.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vector<string> argsuid;
        for (auto a:argsname) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::GETMEM getmemcall;
        getmemcall.source_uid=argsuid[1];
        getmemcall.output_uid=argsuid[0];
        vodka::syscalls::SyscallContainer syscont;
        syscont.getmem_element=getmemcall;
        syscont.thing=vodka::syscalls::SyscallsNames::GETMEM;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.memory.free.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}

//* Main function for parsing memory internal library
bool vodka::library::memory::CallTreatement::_call_treatement(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    line=function_call.line_checked.line_checked.content;
    if (function_call.line_checked.get_analyse_result()==false) {
        return false;
    }
    if (function_call.line_checked.type!="internal_library") {
        return false;
    }
    if (line.substr(0,12)=="memory.print") {
        return print_int(srclclstack);
    } else if (line.substr(0,11)=="memory.free") {
        return free_int(srclclstack);
    } else if (line.substr(0,13)=="memory.getmem") {
        return getmem_int(srclclstack);
    }
    return false;
}