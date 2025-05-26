#include "vodka-lib.h"
#include <string>
#include <vector>
#include <map>
#include <boost/uuid/uuid_io.hpp>
using namespace std;
using namespace vodka::utilities;
using namespace vodka::syscalls;
using namespace vodka::errors;
//* Private functions for analysing each instructions
bool vodka::instructions::CallTreatement::multiply(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",instruction_call.main_logstep_context,instruction_call.last_logstep_context,2,{(int)instruction_call.iteration_number_context+1,1},{instruction_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==4) {
        output::log("Checking content existence and datatype.",instruction_call.main_logstep_context,instruction_call.last_logstep_context,2,{(int)instruction_call.iteration_number_context+1,2},{instruction_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec}};
        argcheck.line_content=instruction_call.line_checked;
        argcheck.variablesdict_context=instruction_call.variablesdict_context;
        argcheck.variableslist_context=instruction_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(ErrorContainer("vodka.error.variables.constant : Can't modify an constant.",instruction_call.file_name_context,{line},{instruction_call.cell_context.start.line+(int)instruction_call.iteration_number_context+1},srclclstack));
            return false;
        }
        vector<string> argsname(eles.begin()+1,eles.end());
        for (int i=1;i<argsname.size();++i) {
            if (instruction_call.variablesdict_context[argsname[i-1]].thing!=instruction_call.variablesdict_context[argsname[i]].thing) {
                raise(ErrorContainer("vodka.error.multiply.not_same_type : All arguments must be of the same type.",instruction_call.file_name_context,{line},{instruction_call.cell_context.start.line+(int)instruction_call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (instruction_call.variablesdict_context[argsname[0]].thing==vodka::variables::VariableDatatype::vodint) {
            output::log("Registering systems call for this instructions.",instruction_call.main_logstep_context,instruction_call.last_logstep_context,2,{(int)instruction_call.iteration_number_context+1,3},{instruction_call.cell_context.content.size(),3});
            vector<string> uidargs;
            for (auto a:argsname) {
                uidargs.push_back(instruction_call.variablesdict_context[a].variable_metadata.uuid);
            }
            MULINT mulintcall;
            mulintcall.output_uid=uidargs[0];
            mulintcall.first_uid=uidargs[1];
            mulintcall.second_uid=uidargs[2];
            SyscallContainer mulintcont;
            mulintcont.mulint_element=mulintcall;
            mulintcont.thing=vodka::syscalls::SyscallsNames::MULINT;
            syscalls_output.push_back(mulintcont);
            return true;
        } else if (instruction_call.variablesdict_context[argsname[0]].thing==vodka::variables::VariableDatatype::vodec) {
            output::log("Registering systems call for this instructions.",instruction_call.main_logstep_context,instruction_call.last_logstep_context,2,{(int)instruction_call.iteration_number_context+1,3},{instruction_call.cell_context.content.size(),3});
            vector<string> uidargs;
            for (auto a:argsname) {
                uidargs.push_back(instruction_call.variablesdict_context[a].variable_metadata.uuid);
            }
            string precision_uid=to_string(::genuid());
            ASSIGN asscall;
            asscall.output_uid=precision_uid;
            asscall.value="3";
            SyscallContainer asscont;
            asscont.assign_element=asscall;
            asscont.thing=vodka::syscalls::SyscallsNames::ASSIGN;
            syscalls_output.push_back(asscont);
            MULDEC muldeccall;
            muldeccall.output_uid=uidargs[0];
            muldeccall.first_uid=uidargs[1];
            muldeccall.second_uid=uidargs[2];
            muldeccall.precision_uid=precision_uid;
            SyscallContainer muldeccont;
            muldeccont.muldec_element=muldeccall;
            muldeccont.thing=vodka::syscalls::SyscallsNames::MULDEC;
            syscalls_output.push_back(muldeccont);
            FREE freecall;
            freecall.argument_uid={precision_uid};
            SyscallContainer freecont;
            freecont.thing=vodka::syscalls::SyscallsNames::FREE;
            freecont.free_element=freecall;
            syscalls_output.push_back(freecont);
            return true;
        }
    } else if (eles.size()==5) {
        output::log("Checking content existence.",instruction_call.main_logstep_context,instruction_call.last_logstep_context,2,{(int)instruction_call.iteration_number_context+1,2},{instruction_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint}};
        argcheck.line_content=instruction_call.line_checked;
        argcheck.variablesdict_context=instruction_call.variablesdict_context;
        argcheck.variableslist_context=instruction_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(ErrorContainer("vodka.error.variables.constant : Can't modify an constant.",instruction_call.file_name_context,{line},{instruction_call.cell_context.start.line+(int)instruction_call.iteration_number_context+1},srclclstack));
            return false;
        }
        vector<string> inoutargs(eles.begin()+1,eles.end()-1);
        output::log("Registering systems call for this instructions.",instruction_call.main_logstep_context,instruction_call.last_logstep_context,2,{(int)instruction_call.iteration_number_context+1,3},{instruction_call.cell_context.content.size(),3});
        vector<string> uidargs;
        for (auto a:inoutargs) {
            uidargs.push_back(instruction_call.variablesdict_context[a].variable_metadata.uuid);
        }
        uidargs.push_back(instruction_call.variablesdict_context[eles[4]].variable_metadata.uuid);
        MULDEC muldeccall;
        muldeccall.output_uid=uidargs[0];
        muldeccall.first_uid=uidargs[1];
        muldeccall.second_uid=uidargs[2];
        muldeccall.precision_uid=uidargs[3];
        SyscallContainer muldeccont;
        muldeccont.muldec_element=muldeccall;
        muldeccont.thing=vodka::syscalls::SyscallsNames::MULDEC;
        syscalls_output.push_back(muldeccont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.multiply.invalid_syntax : Invalid syntax.",instruction_call.file_name_context,{line},{instruction_call.cell_context.start.line+(int)instruction_call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
bool vodka::instructions::CallTreatement::tostr(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",instruction_call.main_logstep_context,instruction_call.last_logstep_context,2,{(int)instruction_call.iteration_number_context+1,1},{instruction_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==3) {
        output::log("Checking content existence and datatype.",instruction_call.main_logstep_context,instruction_call.last_logstep_context,2,{(int)instruction_call.iteration_number_context+1,2},{instruction_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec,vodka::variables::VariableDatatype::vodarg}};
        argcheck.line_content=instruction_call.line_checked;
        argcheck.variablesdict_context=instruction_call.variablesdict_context;
        argcheck.variableslist_context=instruction_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(ErrorContainer("vodka.error.variables.constant : Can't modify an constant.",instruction_call.file_name_context,{line},{instruction_call.cell_context.start.line+(int)instruction_call.iteration_number_context+1},srclclstack));
            return false;
        }
        vector<string> argsname(eles.begin()+1,eles.end());
        output::log("Registering systems call for this instructions.",instruction_call.main_logstep_context,instruction_call.last_logstep_context,2,{(int)instruction_call.iteration_number_context+1,3},{instruction_call.cell_context.content.size(),3});
        vector<string> uidargs;
        for (auto a:argsname) {
            uidargs.push_back(instruction_call.variablesdict_context[a].variable_metadata.uuid);
        }
        DUPLICATE duplicall;
        duplicall.output_uid=uidargs[0];
        duplicall.source_uid=uidargs[1];
        SyscallContainer duplicont;
        duplicont.duplicate_element=duplicall;
        duplicont.thing=vodka::syscalls::SyscallsNames::DUPLICATE;
        syscalls_output.push_back(duplicont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.tostr.invalid_syntax : Invalid syntax.",instruction_call.file_name_context,{line},{instruction_call.cell_context.start.line+(int)instruction_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
//* Main function for parsing vodka instruction
bool vodka::instructions::CallTreatement::call_treatement(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    line=instruction_call.line_checked.line_checked.content;
    if (instruction_call.line_checked.checked==false) {
        return false;
    }
    if (instruction_call.line_checked.type!="vodka_instruction") {
        return false;
    }
    if (line.substr(0,8)=="multiply") {
        return multiply(srclclstack);
    } else if (line.substr(0,5)=="tostr") {
        return tostr(srclclstack);
    }
    return false;
}