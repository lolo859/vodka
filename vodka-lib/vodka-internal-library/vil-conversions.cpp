#include "../vodka-lib.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
using namespace vodka::utilities;
using namespace vodka::errors;
//* Private functions for analysing each instructions
bool vodka::library::conversions::CallTreatement::toint_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==3) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodec,vodka::variables::VariableDatatype::vodstr,vodka::variables::VariableDatatype::vodarg}};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(ErrorContainer("vodka.error.variables.constant : Can't modify a constant.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
            return false;
        }
        output::log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vodka::syscalls::TOINT tointcall;
        tointcall.source_uid=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        tointcall.output_uid=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::TOINT;
        syscont.toint_element=tointcall;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.conversions.toint.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::conversions::CallTreatement::todec_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==3) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodstr,vodka::variables::VariableDatatype::vodarg}};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(ErrorContainer("vodka.error.variables.constant : Can't modify a constant.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
            return false;
        }
        output::log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vodka::syscalls::TODEC todeccall;
        todeccall.source_uid=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        todeccall.output_uid=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::TODEC;
        syscont.todec_element=todeccall;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.conversions.todec.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::conversions::CallTreatement::tostr_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==3) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec,vodka::variables::VariableDatatype::vodarg}};
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
        vector<string> argsname(eles.begin()+1,eles.end());
        output::log("Registering systems call for this instruction.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vector<string> uidargs;
        for (auto a:argsname) {
            uidargs.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::DUPLICATE duplicall;
        duplicall.output_uid=uidargs[0];
        duplicall.source_uid=uidargs[1];
        vodka::syscalls::SyscallContainer duplicont;
        duplicont.duplicate_element=duplicall;
        duplicont.thing=vodka::syscalls::SyscallsNames::DUPLICATE;
        syscalls_output.push_back(duplicont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.tostr.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
//* Main function for parsing kernel internal library
bool vodka::library::conversions::CallTreatement::call_treatement(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    line=function_call.line_checked.line_checked.content;
    if (function_call.line_checked.checked==false) {
        return false;
    }
    if (function_call.line_checked.type!="internal_library") {
        return false;
    }
    if (line.substr(0,17)=="conversions.toint") {
        return toint_int(srclclstack);
    } else if (line.substr(0,17)=="conversions.todec") {
        return todec_int(srclclstack);
    } else if (line.substr(0,17)=="conversions.tostr") {
        return tostr_int(srclclstack);
    }
    return false;
}