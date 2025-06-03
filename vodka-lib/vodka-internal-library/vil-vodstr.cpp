#include "../vodka-lib.h"
#include <string>
#include <vector>
#include <map>
#include <boost/uuid/uuid_io.hpp>
using namespace std;
using namespace vodka::utilities;
using namespace vodka::syscalls;
using namespace vodka::errors;
//* Private functions for analysing each instructions
bool vodka::library::vodstr::CallTreatement::length_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==3) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodstr}};
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
        vector<string> argsuid;
        output::log("Registering systems call for this instruction.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        for (auto a:argsname) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::LENGTH lencall;
        lencall.source_uid=argsuid[1];
        lencall.output_uid=argsuid[0];
        vodka::syscalls::SyscallContainer syscont;
        syscont.length_element=lencall;
        syscont.thing=vodka::syscalls::SyscallsNames::LENGTH;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.vodstr.length.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
bool vodka::library::vodstr::CallTreatement::concat_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()>=4) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={vodka::variables::VariableDatatype::vodstr};
        argcheck.patern={{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodstr}};
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
        vector<string> argsuid;
        output::log("Registering systems call for this instruction.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        if (eles.size()==4) {
            for (auto a:argsname) {
                argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
            }
            vodka::syscalls::CONCAT concall;
            concall.first_uid=argsuid[1];
            concall.second_uid=argsuid[2];
            concall.output_uid=argsuid[0];
            vodka::syscalls::SyscallContainer syscont;
            syscont.concat_element=concall;
            syscont.thing=vodka::syscalls::SyscallsNames::CONCAT;
            syscalls_output.push_back(syscont);
            return true;
        } else {
            for (auto a:argsname) {
                argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
            }
            vodka::syscalls::CONCAT concall;
            concall.first_uid=argsuid[1];
            concall.second_uid=argsuid[2];
            concall.output_uid=argsuid[0];
            vodka::syscalls::SyscallContainer syscont;
            syscont.concat_element=concall;
            syscont.thing=vodka::syscalls::SyscallsNames::CONCAT;
            syscalls_output.push_back(syscont);
            for (int i=3;i<argsuid.size();i++) {
                vodka::syscalls::CONCAT concall;
                concall.first_uid=argsuid[0];
                concall.second_uid=argsuid[i];
                concall.output_uid=argsuid[0];
                vodka::syscalls::SyscallContainer syscont;
                syscont.concat_element=concall;
                syscont.thing=vodka::syscalls::SyscallsNames::CONCAT;
                syscalls_output.push_back(syscont);
            }
            return true;
        }
    } else {
        raise(ErrorContainer("vodka.error.library.vodstr.concat.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
bool vodka::library::vodstr::CallTreatement::substring_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==5) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodint}};
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
        vector<string> argsuid;
        output::log("Registering systems call for this instruction.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        for (auto a:argsname) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::SUBSTRING substringcall;
        substringcall.length_output_uid=argsuid[3];
        substringcall.start_index_uid=argsuid[2];
        substringcall.source_uid=argsuid[1];
        substringcall.output_uid=argsuid[0];
        vodka::syscalls::SyscallContainer syscont;
        syscont.substring_element=substringcall;
        syscont.thing=vodka::syscalls::SyscallsNames::SUBSTRING;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.vodstr.substring.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
bool vodka::library::vodstr::CallTreatement::charat_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==4) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodint}};
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
        vector<string> argsuid;
        output::log("Registering systems call for this instruction.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        for (auto a:argsname) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::CHARAT charatcall;
        charatcall.output_uid=argsuid[0];
        charatcall.input_uid=argsuid[1];
        charatcall.char_uid=argsuid[2];
        vodka::syscalls::SyscallContainer syscont;
        syscont.charat_element=charatcall;
        syscont.thing=vodka::syscalls::SyscallsNames::CHARAT;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.vodstr.charat.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
bool vodka::library::vodstr::CallTreatement::reverse_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==3) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodstr}};
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
        vector<string> argsuid;
        output::log("Registering systems call for this instruction.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        for (auto a:argsname) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::REVERSE reversecall;
        reversecall.output_uid=argsuid[0];
        reversecall.source_uid=argsuid[1];
        vodka::syscalls::SyscallContainer syscont;
        syscont.reverse_element=reversecall;
        syscont.thing=vodka::syscalls::SyscallsNames::REVERSE;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.vodstr.reverse.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
bool vodka::library::vodstr::CallTreatement::escape_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==3) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodstr}};
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
        vector<string> argsuid;
        output::log("Registering systems call for this instruction.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        for (auto a:argsname) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::ESCAPE escapecall;
        escapecall.output_uid=argsuid[0];
        escapecall.source_uid=argsuid[1];
        vodka::syscalls::SyscallContainer syscont;
        syscont.escape_element=escapecall;
        syscont.thing=vodka::syscalls::SyscallsNames::ESCAPE;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.vodstr.escape.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
bool vodka::library::vodstr::CallTreatement::insert_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==5) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodstr}};
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
        vector<string> argsuid;
        output::log("Registering systems call for this instruction.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        for (auto a:argsname) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::INSERT insertcall;
        insertcall.output_uid=argsuid[0];
        insertcall.source_uid=argsuid[1];
        insertcall.index_uid=argsuid[2];
        insertcall.string_to_insert_uid=argsuid[3];
        vodka::syscalls::SyscallContainer syscont;
        syscont.insert_element=insertcall;
        syscont.thing=vodka::syscalls::SyscallsNames::INSERT;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.vodstr.insert.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
bool vodka::library::vodstr::CallTreatement::find_int(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    output::log("Checking instruction syntax.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,1},{function_call.cell_context.content.size(),3});
    auto eles=string_utilities::split(line," ");
    if (eles.size()==4) {
        output::log("Checking content existence and datatype.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,2},{function_call.cell_context.content.size(),3});
        vodka::analyser::ArgumentChecker argcheck;
        argcheck.lastest_allowed_type={};
        argcheck.patern={{vodka::variables::VariableDatatype::vodint},{vodka::variables::VariableDatatype::vodstr},{vodka::variables::VariableDatatype::vodint}};
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
        vector<string> argsuid;
        output::log("Registering systems call for this instruction.",function_call.main_logstep_context,function_call.last_logstep_context,2,{(int)function_call.iteration_number_context+1,3},{function_call.cell_context.content.size(),3});
        for (auto a:argsname) {
            argsuid.push_back(function_call.variablesdict_context[a].variable_metadata.uuid);
        }
        vodka::syscalls::FIND findcall;
        findcall.output_uid=argsuid[0];
        findcall.source_uid=argsuid[1];
        findcall.char_uid=argsuid[2];
        vodka::syscalls::SyscallContainer syscont;
        syscont.find_element=findcall;
        syscont.thing=vodka::syscalls::SyscallsNames::FIND;
        syscalls_output.push_back(syscont);
        return true;
    } else {
        raise(ErrorContainer("vodka.error.library.vodstr.find.invalid_syntax : Invalid syntax.",function_call.file_name_context,{line},{function_call.cell_context.start.line+(int)function_call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
//* Main function for parsing vodka instruction
bool vodka::library::vodstr::CallTreatement::call_treatement(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    line=function_call.line_checked.line_checked.content;
    if (function_call.line_checked.checked==false) {
        return false;
    }
    if (function_call.line_checked.type!="internal_library") {
        return false;
    }
    if (line.substr(0,13)=="vodstr.length") {
        return length_int(srclclstack);
    } else if (line.substr(0,13)=="vodstr.concat") {
        return concat_int(srclclstack);
    } else if (line.substr(0,16)=="vodstr.substring") {
        return substring_int(srclclstack);
    } else if (line.substr(0,13)=="vodstr.charat") {
        return charat_int(srclclstack);
    } else if (line.substr(0,14)=="vodstr.reverse") {
        return reverse_int(srclclstack);
    } else if (line.substr(0,13)=="vodstr.escape") {
        return escape_int(srclclstack);
    } else if (line.substr(0,13)=="vodstr.insert") {
        return insert_int(srclclstack);
    } else if (line.substr(0,11)=="vodstr.find") {
        return find_int(srclclstack);
    }
    return false;
}