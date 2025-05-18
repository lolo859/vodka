#include "../vodka-lib.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
using namespace vodka::utilities;
using namespace vodka::errors;
//* Private functions for analysing each instructions
bool vodka::library::kernel::CallTreatement::print_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()>=2) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vector<string> eltoprint(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec,vodka::variables::VariableDatatype::vodarg};
        argcheck.patern={};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vector<string> uidtoprint;
        for (auto a:eltoprint) {
            uidtoprint.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::PRINT syscal;
        syscal.argument_uid=uidtoprint;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::PRINT;
        syscont.print_element=syscal;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.print.invalid syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::CallTreatement::add_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),1});
    auto eles=split(line," ");
    if (eles.size()>=4) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),2});
        vector<string> argsgive(eles.begin()+1,eles.end());
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
        for (int i=1;i<argsgive.size();++i) {
            if (function_call.variablesdict_context[argsgive[i-1]].thing!=function_call.variablesdict_context[argsgive[i]].thing) {
                raise(ErrorContainer("vodka.error.kernel.add.not_same_type : All arguments must be of the same type.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vector<string> uidargs;
        for (auto a:argsgive) {
            uidargs.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::ADD syscal;
        syscal.argument_uid=vector<string>(uidargs.begin()+1,uidargs.end());
        syscal.output_uid=uidargs[0];
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::ADD;
        syscont.add_element=syscal;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.add.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::CallTreatement::invert_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()==2) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        string arg=eles[1];
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
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
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,4},{function_call.cell_context.content.size(),3});
        string uidarg=function_call.variablesdict_context[arg].variable_metadata.uuid;
        vodka::syscalls::INVERT syscal;
        syscal.uid=uidarg;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::INVERT;
        syscont.invert_element=syscal;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.invert.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::CallTreatement::free_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()>=2) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        string arg;
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodec};
        argcheck.patern={};
        argcheck.line_content=function_call.line_checked;
        argcheck.variablesdict_context=function_call.variablesdict_context;
        argcheck.variableslist_context=function_call.variableslist_context;
        if (argcheck.check(srclclstack)==false) {
            return false;
        }
        for (int y=1;y<eles.size();++y) {
            arg=eles[y];
            if (eles[y].substr(0,2)=="$$") {
                raise(ErrorContainer("vodka.error.variables.constant : Can't delete an kernel constant.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vector<string> argsuid;
        for (auto a:vector<string>(eles.begin()+1,eles.end())) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::FREE freecall;
        freecall.argument_uid=argsuid;
        vodka::syscalls::SyscallContainer syscont;
        syscont.free_element=freecall;
        syscont.thing=vodka::syscalls::SyscallsNames::FREE;
        syscall_output.push_back(syscont);
        var_flag=true;
        auto args_erase=vector<string>(eles.begin()+1,eles.end());
        for (auto a:args_erase) {
            auto it=remove(function_call.variableslist_context.begin(),function_call.variableslist_context.end(),a);
            function_call.variableslist_context.erase(it,function_call.variableslist_context.end());
            function_call.variablesdict_context.erase(a);
        }
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.free.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::CallTreatement::abs_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()==2) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        string arg=eles[1];
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
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
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        string uidarg;
        uidarg=function_call.variablesdict_context[arg].variable_metadata.uuid;
        vodka::syscalls::ABS abscall;
        abscall.uid=uidarg;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::ABS;
        syscont.abs_element=abscall;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.abs.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::CallTreatement::divmod_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()==5) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
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
            raise(ErrorContainer("vodka.error.kernel.divmod.same_output : the two output variables can't be the same.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
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
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.divmod.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::CallTreatement::toint_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()==3) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodec,vodka::variables::VariableDatatype::vodarg}};
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
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vodka::syscalls::TOINT tointcall;
        tointcall.uid_source=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        tointcall.uid_output=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::TOINT;
        syscont.toint_element=tointcall;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.toint.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::CallTreatement::todec_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()==3) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodec},{vodka::variables::VariableDatatype::vodint,vodka::variables::VariableDatatype::vodarg}};
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
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vodka::syscalls::TODEC todeccall;
        todeccall.uid_source=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        todeccall.uid_output=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::TODEC;
        syscont.todec_element=todeccall;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.todec.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::CallTreatement::divide_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()==5) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
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
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vodka::syscalls::DIVIDE dividecall;
        dividecall.output_uid=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        dividecall.first_uid=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        dividecall.second_uid=function_call.variablesdict_context[arg[2]].variable_metadata.uuid;
        dividecall.precision_uid=function_call.variablesdict_context[arg[3]].variable_metadata.uuid;
        vodka::syscalls::SyscallContainer dividecont;
        dividecont.divide_element=dividecall;
        dividecont.thing=vodka::syscalls::SyscallsNames::DIVIDE;
        syscall_output.push_back(dividecont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.divide.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::CallTreatement::mulint_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()==4) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
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
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        vodka::syscalls::MULINT mulintcall;
        mulintcall.output_uid=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        mulintcall.first_uid=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        mulintcall.second_uid=function_call.variablesdict_context[arg[2]].variable_metadata.uuid;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::MULINT;
        syscont.mulint_element=mulintcall;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.mulint.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::CallTreatement::muldec_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()==5) {
        log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
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
        log("Registering system call.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,4},{function_call.cell_context.content.size(),3});
        vodka::syscalls::MULDEC muldeccall;
        muldeccall.output_uid=function_call.variablesdict_context[arg[0]].variable_metadata.uuid;
        muldeccall.first_uid=function_call.variablesdict_context[arg[1]].variable_metadata.uuid;
        muldeccall.second_uid=function_call.variablesdict_context[arg[2]].variable_metadata.uuid;
        muldeccall.precision_uid=function_call.variablesdict_context[arg[3]].variable_metadata.uuid;
        vodka::syscalls::SyscallContainer syscont;
        syscont.thing=vodka::syscalls::SyscallsNames::MULDEC;
        syscont.muldec_element=muldeccall;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.kernel.muldec.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
}
//* Main function for parsing kernel internal library
bool vodka::library::kernel::CallTreatement::call_treatement(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    line=function_call.line_checked.line_checked.content;
    if (function_call.line_checked.checked==false) {
        return false;
    }
    if (function_call.line_checked.type!="internal_library") {
        return false;
    }
    if (line.substr(0,12)=="kernel.print") {
        return print_int(srclclstack);
    } else if (line.substr(0,10)=="kernel.add") {
        return add_int(srclclstack);
    } else if (line.substr(0,13)=="kernel.invert") {
        return invert_int(srclclstack);
    } else if (line.substr(0,11)=="kernel.free") {
        return free_int(srclclstack);
    } else if (line.substr(0,10)=="kernel.abs") {
        return abs_int(srclclstack);
    } else if (line.substr(0,13)=="kernel.divmod") {
        return divmod_int(srclclstack);
    } else if (line.substr(0,12)=="kernel.toint") {
        return toint_int(srclclstack);
    } else if (line.substr(0,12)=="kernel.todec") {
        return todec_int(srclclstack);
    } else if (line.substr(0,13)=="kernel.divide") {
        return divide_int(srclclstack);
    } else if (line.substr(0,13)=="kernel.mulint") {
        return mulint_int(srclclstack);
    } else if (line.substr(0,13)=="kernel.muldec") {
        return muldec_int(srclclstack);
    }
    return false;
}