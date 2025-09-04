#include "../vodka-lib.h"
#include <string>
#include <vector>
#include <map>
using namespace std;
using namespace vodka::utilities;
using namespace vodka::syscalls;
using namespace vodka::errors;
//* Private functions for analysing each instructions
bool vodka::library::math::CallTreatement::add_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),1});
    auto eles=string_utilities::split(line," ");
    if (eles.size()>=4) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),2});
        vector<string> argsname(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec}};
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
        for (int i=1;i<argsname.size();++i) {
            if (function_call.variablesdict_context[argsname[i-1]].thing!=function_call.variablesdict_context[argsname[i]].thing) {
                raise(ErrorContainer("vodka.error.library.math.add.not_same_type : All arguments must be of the same type.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        output::log("Registering system call.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vector<string> argsuid;
        if (eles.size()==4) {
            for (auto a:argsname) {
                argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
            }
            vodka::syscalls::ADD addcall;
            addcall.first_uid=argsuid[1];
            addcall.second_uid=argsuid[2];
            addcall.output_uid=argsuid[0];
            vodka::syscalls::SyscallContainer syscont;
            syscont.add_element=addcall;
            syscont.thing=vodka::syscalls::SyscallsNames::ADD;
            syscalls_output.push_back(syscont);
            return true;
        } else {
            for (auto a:argsname) {
                argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
            }
            vodka::syscalls::ADD addcall;
            addcall.first_uid=argsuid[1];
            addcall.second_uid=argsuid[2];
            addcall.output_uid=argsuid[0];
            vodka::syscalls::SyscallContainer syscont;
            syscont.add_element=addcall;
            syscont.thing=vodka::syscalls::SyscallsNames::ADD;
            syscalls_output.push_back(syscont);
            for (int i=3;i<argsuid.size();i++) {
                vodka::syscalls::ADD addcall;
                addcall.first_uid=argsuid[0];
                addcall.second_uid=argsuid[i];
                addcall.output_uid=argsuid[0];
                vodka::syscalls::SyscallContainer syscont;
                syscont.add_element=addcall;
                syscont.thing=vodka::syscalls::SyscallsNames::ADD;
                syscalls_output.push_back(syscont);
            }
            return true;
        }
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.math.add.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::math::CallTreatement::invert_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==3) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vector<string> argsname(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec}};
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
        for (int i=1;i<argsname.size();++i) {
            if (function_call.variablesdict_context[argsname[i-1]].thing!=function_call.variablesdict_context[argsname[i]].thing) {
                raise(ErrorContainer("vodka.error.library.math.invert.not_same_type : All arguments must be of the same type.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        output::log("Registering system call.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,4},{function_call.cell_context.content.size(),3});
        vector<string> uidargs;
        for (auto a:argsname) {
            uidargs.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::INVERT syscall;
        syscall.source_uid=uidargs[1];
        syscall.output_uid=uidargs[0];
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::INVERT;
        syscont.invert_element=syscall;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.math.invert.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::math::CallTreatement::abs_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==3) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vector<string> argsname(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec}};
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
        for (int i=1;i<argsname.size();++i) {
            if (function_call.variablesdict_context[argsname[i-1]].thing!=function_call.variablesdict_context[argsname[i]].thing) {
                raise(ErrorContainer("vodka.error.library.math.abs.not_same_type : All arguments must be of the same type.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        output::log("Registering system call.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vector<string> uidargs;
        for (auto a:argsname) {
            uidargs.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::ABS abscall;
        abscall.output_uid=uidargs[0];
        abscall.source_uid=uidargs[1];
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::ABS;
        syscont.abs_element=abscall;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.math.abs.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::math::CallTreatement::divmod_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==5) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodint}};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$" || eles[2].substr(0,2)=="$$" || eles[2].substr(0,1)=="$") {
            raise(ErrorContainer("vodka.error.variables.constant : Can't modify a constant.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
            return false;
        }
        if (arg[0]==arg[1]) {
            raise(ErrorContainer("vodka.error.library.math.divmod.same_output : the two output variables can't be the same.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
            return false;
        }
        output::log("Registering system call.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        string quouid=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        string resuid=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        string divid=function_call.variablesdict_context[arg[2]].variable_metadata.uuid;
        string divis=function_call.variablesdict_context[arg[3]].variable_metadata.uuid;
        vodka::syscalls::DIVMOD divmodcall;
        divmodcall.quotient_uid=quouid;
        divmodcall.rest_uid=resuid;
        divmodcall.dividend_uid=divid;
        divmodcall.divisor_uid=divis;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::DIVMOD;
        syscont.divmod_element=divmodcall;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.math.divmod.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::math::CallTreatement::divide_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==5) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint}};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$" || eles[2].substr(0,2)=="$$" || eles[2].substr(0,1)=="$") {
            raise(ErrorContainer("vodka.error.variables.constant : Can't modify a constant.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
            return false;
        }
        output::log("Registering system call.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vodka::syscalls::DIVIDE dividecall;
        dividecall.output_uid=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        dividecall.first_uid=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        dividecall.second_uid=function_call.variablesdict_context[arg[2]].variable_metadata.uuid;
        dividecall.precision_uid=function_call.variablesdict_context[arg[3]].variable_metadata.uuid;
        vodka::syscalls::SyscallContainer dividecont;
        dividecont.divide_element=dividecall;
        dividecont.thing=vodka::syscalls::SyscallsNames::DIVIDE;
        syscalls_output.push_back(dividecont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.math.divide.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::math::CallTreatement::mulint_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==4) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodint}};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$" || eles[2].substr(0,2)=="$$" || eles[2].substr(0,1)=="$") {
            raise(ErrorContainer("vodka.error.variables.constant : Can't modify a constant.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
            return false;
        }
        output::log("Registering system call.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vodka::syscalls::MULINT mulintcall;
        mulintcall.output_uid=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        mulintcall.first_uid=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        mulintcall.second_uid=function_call.variablesdict_context[arg[2]].variable_metadata.uuid;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::MULINT;
        syscont.mulint_element=mulintcall;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.math.mulint.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::math::CallTreatement::muldec_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking system call syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==5) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint}};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$" || eles[2].substr(0,2)=="$$" || eles[2].substr(0,1)=="$") {
            raise(ErrorContainer("vodka.error.variables.constant : Can't modify a constant.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
            return false;
        }
        output::log("Registering system call.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,4},{function_call.cell_context.content.size(),3});
        vodka::syscalls::MULDEC muldeccall;
        muldeccall.output_uid=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        muldeccall.first_uid=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        muldeccall.second_uid=function_call.variablesdict_context[arg[2]].variable_metadata.uuid;
        muldeccall.precision_uid=function_call.variablesdict_context[arg[3]].variable_metadata.uuid;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::MULDEC;
        syscont.muldec_element=muldeccall;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.math.muldec.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::math::CallTreatement::multiply(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==4) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec}};
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
        for (int i=1;i<argsname.size();++i) {
            if (function_call.variablesdict_context[argsname[i-1]].thing!=function_call.variablesdict_context[argsname[i]].thing) {
                raise(ErrorContainer("vodka.error.library.math.multiply.not_same_type : All arguments must be of the same type.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (function_call.variablesdict_context[argsname[0]].thing==vodka::variables::VariableDatatype::vodint) {
            output::log("Registering systems call for this instruction.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
            vector<string> uidargs;
            for (auto a:argsname) {
                uidargs.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
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
        } else if (function_call.variablesdict_context[argsname[0]].thing==vodka::variables::VariableDatatype::vodec) {
            output::log("Registering systems call for this instruction.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
            vector<string> uidargs;
            for (auto a:argsname) {
                uidargs.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
            }
            string precision_uid=genvyid();
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
        output::log("Checking content existence.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint}};
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
        vector<string> inoutargs(eles.begin()+1,eles.end()-1);
        output::log("Registering systems call for this instruction.",function_call.main_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vector<string> uidargs;
        for (auto a:inoutargs) {
            uidargs.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        uidargs.push_back(function_call.variablesdict_context[eles[4]].variable_metadata.uuid);
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
        raise(ErrorContainer("vodka.error.library.math.multiply.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
//* Main function for parsing vodka instruction
bool vodka::library::math::CallTreatement::_call_treatement(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    line=function_call.line_checked.line_checked.content;
    if (function_call.line_checked.get_analyse_result()==false) {
        return false;
    }
    if (function_call.line_checked.type!="internal_library") {
        return false;
    }
    if (line.substr(0,13)=="math.multiply") {
        return multiply(srclclstack);
    } else if (line.substr(0,8)=="math.add") {
        return add_int(srclclstack);
    } else if (line.substr(0,11)=="math.invert") {
        return invert_int(srclclstack);
    } else if (line.substr(0,8)=="math.abs") {
        return abs_int(srclclstack);
    } else if (line.substr(0,11)=="math.divmod") {
        return divmod_int(srclclstack);
    } else if (line.substr(0,11)=="math.divide") {
        return divide_int(srclclstack);
    } else if (line.substr(0,11)=="math.mulint") {
        return mulint_int(srclclstack);
    } else if (line.substr(0,11)=="math.muldec") {
        return muldec_int(srclclstack);
    }
    return false;
}