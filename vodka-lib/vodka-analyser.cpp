#include "vodka-lib.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/uuid/uuid_io.hpp>
using namespace std;
using namespace vodka::errors;
using namespace vodka::utilities;
//* Return the list of variables passed in argument
vector<string> vodka::analyser::get_arguments(string line) {
    auto eles=string_utilities::split(line," ");
    return vector<string>(eles.begin()+1,eles.end());
}
//* Checking if the line is conform to vodka syntax (doesn't check the line argument)
bool vodka::analyser::LineSyntaxChecker::check(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (content=="") {
        shoulb_be_skip=true;
        return true;
    }
    if (content.substr(0,1)==">" || content.substr()==">>") {
        return true;
    } else if (content.substr(0,6)=="vodka ") {
        return true;
    }
    for (auto a:vodka::InternalLibraryList) {
        if (content.substr(0,a.size())==a && string_utilities::split(content,".").size()>=2) {
            auto functions=vodka::InternalLibraryFunctions.at(a);
            for (auto b:functions) {
                if (string_utilities::split(content,".")[1].substr(0,b.size())==b) {
                    return true;
                }
            }
        }
    }
    for (auto a:vodka::VodkaInstructions) {
        if (content.substr(0,a.size())==a) {
            return true;
        }
    }
    raise(ErrorContainer("vodka.error.analyser.syntax : Invalid syntax.",file,{content},{line_number},srclclstack,true));
    return false;
}
//* Analyse the type of line (variable declaration, vodka instruction, library instruction, debug line)
bool vodka::analyser::LineTypeChecker::line_type_analyse(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (line_checked.checked==false) {
        raise(ErrorContainer("vodka.error.analyser.chain_error : Previous treatement hasn't been well executed.",line_checked.file,{line_checked.content},{line_checked.line_number},srclclstack));
        return false;
    } else {
        if (line_checked.content.substr(0,2)==">>") {
            type="debug_two";
            return true;
        } else if (line_checked.content.substr(0,1)==">" && line_checked.content.substr(0,2)!=">>") {
            type="debug_one";
            return true;
        } else if (line_checked.content.substr(0,6)=="vodka ") {
            type="var";
            return true;
        }
        for (auto a:vodka::InternalLibraryList) {
            if (line_checked.content.substr(0,a.size())==a) {
                type="internal_library";
                library_name=a;
                return true;
            }
        }
        for (auto a:vodka::VodkaInstructions) {
            if (line_checked.content.substr(0,a.size())==a) {
                type="vodka_instruction";
                instruction_name=a;
                return true;
            }
        }
    }
    raise(ErrorContainer("vodka.error.analyser.unknow_line_type : Can't identify the type of line.",line_checked.file,{line_checked.content},{line_checked.line_number},srclclstack));
    return false;
}
//* Parse the variable declaration (name, datatype, value, constant)
bool vodka::analyser::VariableDeclarationAnalyser::parser(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (line_checked.checked==false || line_checked.line_checked.checked==false || line_checked.type!="var") {
        raise(ErrorContainer("vodka.error.analyser.chain_error : Previous treatement hasn't been well executed.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
        return false;
    }
    if (line_checked.line_checked.content.substr(0,5)=="vodka") {
        auto eles=string_utilities::split(line_checked.line_checked.content,"=");
        if (eles.size()>=2) {
            auto namepart=string_utilities::split(eles[0]," ");
            if (namepart.size()==2) {
                name=namepart[1];
                is_kernel_constant=name.substr(0,2)=="$$";
                is_vodka_const=name.substr(0,1)=="$";
            } else {
                raise(ErrorContainer("vodka.error.variables.invalid_syntax : Invalid syntax.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
                return false;
            }
            if (name.substr(0,1)=="#" || name.substr(0,1)=="%") {
                raise(ErrorContainer("vodka.error.variables.invalid_name : Can't create variable starting with # or %.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
                return false;
            }
            string valueside=line_checked.line_checked.content.substr(eles[0].size());
            valueside=string_utilities::strip(valueside," ");
            valueside=string_utilities::strip(valueside,"=");
            auto valuepart=string_utilities::split(valueside," ");
            if (valuepart.size()>=2) {
                datatype=valuepart[0];
                string valuevec=valueside.substr(datatype.size()+1);
                value=valuevec;
                if (datatype=="vodka" && name.substr(0,2)=="$$") {
                    raise(ErrorContainer("vodka.error.variables.duplicate_kernel_constant : Can't create kernel constant from duplication.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
                    return false;
                } else {
                    return true;
                }
            } else {
                raise(ErrorContainer("vodka.error.variables.invalid_syntax : Invalid syntax.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
                return false;
            }
        } else {
            raise(ErrorContainer("vodka.error.variables.invalid_syntax : Invalid syntax.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
            return false;
        }
    } else {
        raise(ErrorContainer("vodka.error.variables.invalid_syntax : Invalid syntax.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
        return false;
    }
}
//* Check the type and value of the variable (use vodka::type::<concerned type>::check_value(), if datatype is vodka, please include a list of already declared variables inside the context argument)
bool vodka::analyser::VariableDeclarationAnalyser::check_type_value(vector<string> context,SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (checked) {
        bool f=false;
        for (auto a:vodka::InternalDataypes){
            if (datatype==a) {
                f=true;
                break;
            } else {
                f=false;
            }
        }
        if (!f) {
            raise(ErrorContainer("vodka.error.variables.unknown_type : Unknow type : "+datatype,line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
            return false;
        }
        if (datatype=="vodint") {
            return vodka::type::vodint::check_value(value,line_checked.line_checked,srclclstack);
        } else if (datatype=="vodec") {
            return vodka::type::vodec::check_value(value,line_checked.line_checked,srclclstack);
        } else if (datatype=="vodstr") {
            return true;
        } else if (datatype=="vodka") {
            if (name.substr(0,2)=="$$") {
                raise(ErrorContainer("vodka.error.variables.duplicate_kernel_constant : Can't duplicate kernel constant.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
                return false;
            }
            bool fo=false;
            for (auto a:context) {
                if (value==a) {
                    fo=true;
                    return true;
                } else {
                    fo=false;
                }
            }
            if (!fo) {
                raise(ErrorContainer("vodka.error.variables.unknown_variable : Unknow variable : "+value,line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
                return false;
            }
            return true;
        } else {
            raise(ErrorContainer("vodka.error.variables.unknown_type : Unknow type : "+datatype,line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
            return false;
        }
    } else {
        raise(ErrorContainer("vodka.error.analyser.chain_error : Previous treatement hasn't been well executed.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
        return false;
    }
}
//* Make the corresponding vodka::variables::variable
bool vodka::analyser::VariableDeclarationAnalyser::make_info(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (checked_type_value) {
        if (datatype!="vodka") {
            variable_metadata.algo_dependant=false;
            variable_metadata.is_vodka_constant=is_vodka_const;
            variable_metadata.is_kernel_constant=is_kernel_constant;
            variable_metadata.uuid=to_string(vodka::utilities::genuid());
            variable_metadata.name=name;
            return true;
        } else {
            if (vodka::variables::datatype_to_string(duplication_source_variable.thing)!="") {
                if (vodka::variables::datatype_to_string(duplication_source_variable.thing)=="vodint") {
                    variable_metadata.algo_dependant=duplication_source_variable.variable_metadata.algo_dependant;
                    variable_metadata.is_vodka_constant=is_vodka_const;
                    variable_metadata.in_data_section=false;
                    variable_metadata.uuid=to_string(vodka::utilities::genuid());
                    variable_metadata.name=name;
                    return true;
                } else if (vodka::variables::datatype_to_string(duplication_source_variable.thing)=="vodec") {
                    variable_metadata.algo_dependant=duplication_source_variable.variable_metadata.algo_dependant;
                    variable_metadata.is_vodka_constant=is_vodka_const;
                    variable_metadata.in_data_section=false;
                    variable_metadata.uuid=to_string(vodka::utilities::genuid());
                    variable_metadata.name=name;
                    return true;
                } else if (vodka::variables::datatype_to_string(duplication_source_variable.thing)=="vodstr") {
                    variable_metadata.algo_dependant=duplication_source_variable.variable_metadata.algo_dependant;
                    variable_metadata.is_vodka_constant=is_vodka_const;
                    variable_metadata.in_data_section=false;
                    variable_metadata.uuid=to_string(vodka::utilities::genuid());
                    variable_metadata.name=name;
                    return true;
                } else {
                    raise(ErrorContainer("vodka.error.analyser.datatype_error : Can't duplicate this type : "+vodka::variables::datatype_to_string(duplication_source_variable.thing),line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
                    return false;
                }
            } else {
                raise(ErrorContainer("vodka.error.analyser.argument_error : If datatype is vodka, please provide the duplicated variable in the duplication_source_variable arg.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
                return false;
            }
        }
    } else {
        raise(ErrorContainer("vodka.error.analyser.chain_error : Previous treatement hasn't been well executed.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
        return false;
    }
}
//* Make a pre-treatement of the value to store
bool vodka::analyser::VariableDeclarationAnalyser::value_pre_treatement(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (checked_type_value) {
        if (datatype=="vodint") {
            value=vodka::type::vodint::remove_zero(value,srclclstack);
            return true;
        } else if (datatype=="vodec") {
            value=vodka::type::vodec::remove_zero(value,srclclstack);
            return true;
        } else if (datatype=="vodstr") {
            return true;
        } else if (datatype=="vodka") {
            return true;
        } else {
            raise(ErrorContainer("vodka.error.variables.unknown_type : Unknow type : "+datatype,line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
            return false;
        }
    } else {
        raise(ErrorContainer("vodka.error.analyser.chain_error : Previous treatement hasn't been well executed.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
        return false;
    }
}
//* Output the variable under a vodka::variable::variable_container object
bool vodka::analyser::VariableDeclarationAnalyser::make_output(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (pre_treated) {
        if (datatype=="vodint") {
            vodka::variables::VodintVariable varr;
            variable_container.variable_metadata=variable_metadata;
            varr.value=value;
            variable_container.vodint_element=varr;
            variable_container.thing=vodka::variables::VariableDatatype::vodint;
            vodka::syscalls::ASSIGN asscall;
            asscall.output_uid=variable_metadata.uuid;
            asscall.value=value;
            vodka::syscalls::SyscallContainer asscont;
            asscont.thing=vodka::syscalls::SyscallsNames::ASSIGN;
            asscont.assign_element=asscall;
            syscall_container=asscont;
            return true;
        } else if (datatype=="vodec") {
            vodka::variables::VodecVariable varr;
            variable_container.variable_metadata=variable_metadata;
            varr.value=value;
            variable_container.vodec_element=varr;
            variable_container.thing=vodka::variables::VariableDatatype::vodec;
            vodka::syscalls::ASSIGN asscall;
            asscall.output_uid=variable_metadata.uuid;
            asscall.value=value;
            vodka::syscalls::SyscallContainer asscont;
            asscont.thing=vodka::syscalls::SyscallsNames::ASSIGN;
            asscont.assign_element=asscall;
            syscall_container=asscont;
            return true;
        } else if (datatype=="vodstr") {
            vodka::variables::VodstrVariable varr;
            variable_container.variable_metadata=variable_metadata;
            varr.value=value;
            variable_container.vodstr_element=varr;
            variable_container.thing=vodka::variables::VariableDatatype::vodstr;
            vodka::syscalls::ASSIGN asscall;
            asscall.output_uid=variable_metadata.uuid;
            asscall.value=value;
            vodka::syscalls::SyscallContainer asscont;
            asscont.thing=vodka::syscalls::SyscallsNames::ASSIGN;
            asscont.assign_element=asscall;
            syscall_container=asscont;
            return true;
        } else if (datatype=="vodka") {
            if (vodka::variables::datatype_to_string(duplication_source_variable.thing)!="") {
                if (vodka::variables::datatype_to_string(duplication_source_variable.thing)=="vodint") {
                    vodka::variables::VodintVariable varr;
                    variable_container.variable_metadata=variable_metadata;
                    varr.value=duplication_source_variable.vodint_element.value;
                    variable_container.vodint_element=varr;
                    variable_container.thing=vodka::variables::VariableDatatype::vodint;
                    vodka::syscalls::DUPLICATE dupcall;
                    dupcall.output_uid=variable_metadata.uuid;
                    dupcall.source_uid=duplication_source_variable.variable_metadata.uuid;
                    vodka::syscalls::SyscallContainer dupcont;
                    dupcont.thing=vodka::syscalls::SyscallsNames::DUPLICATE;
                    dupcont.duplicate_element=dupcall;
                    syscall_container=dupcont;
                    return true;
                } else if (vodka::variables::datatype_to_string(duplication_source_variable.thing)=="vodec") {
                    vodka::variables::VodecVariable varr;
                    variable_container.variable_metadata=variable_metadata;
                    varr.value=duplication_source_variable.vodec_element.value;
                    variable_container.vodec_element=varr;
                    variable_container.thing=vodka::variables::VariableDatatype::vodec;
                    vodka::syscalls::DUPLICATE dupcall;
                    dupcall.output_uid=variable_metadata.uuid;
                    dupcall.source_uid=duplication_source_variable.variable_metadata.uuid;
                    vodka::syscalls::SyscallContainer dupcont;
                    dupcont.thing=vodka::syscalls::SyscallsNames::DUPLICATE;
                    dupcont.duplicate_element=dupcall;
                    syscall_container=dupcont;
                    return true;
                } else if (vodka::variables::datatype_to_string(duplication_source_variable.thing)=="vodstr") {
                    vodka::variables::VodstrVariable varr;
                    variable_container.variable_metadata=variable_metadata;
                    varr.value=duplication_source_variable.vodec_element.value;
                    variable_container.vodstr_element=varr;
                    variable_container.thing=vodka::variables::VariableDatatype::vodstr;
                    vodka::syscalls::DUPLICATE dupcall;
                    dupcall.output_uid=variable_metadata.uuid;
                    dupcall.source_uid=duplication_source_variable.variable_metadata.uuid;
                    vodka::syscalls::SyscallContainer dupcont;
                    dupcont.thing=vodka::syscalls::SyscallsNames::DUPLICATE;
                    dupcont.duplicate_element=dupcall;
                    syscall_container=dupcont;
                    return true;
                } else {
                    raise(ErrorContainer("vodka.error.analyser.argument_error : If datatype is vodka, please provide the duplicated variable in the duplication_source_variable arg.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
                    return false;
                }
            } else {
                raise(ErrorContainer("vodka.error.analyser.argument_error : If datatype is vodka, please provide the duplicated variable in the duplication_source_variable arg.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
                return false;
            }
        } else {
            raise(ErrorContainer("vodka.error.variables.unknown_type : Unknow type : "+datatype,line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
            return false;
        }
    } else {
        raise(ErrorContainer("vodka.error.analyser.chain_error : Previous treatement hasn't been well executed.",line_checked.line_checked.file,{line_checked.line_checked.content},{line_checked.line_checked.line_number},srclclstack));
        return false;
    }
}
//* Check the existence and datatype of each argument
bool vodka::analyser::ArgumentChecker::check(SourcesStack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (line_content.checked) {
        if (line_content.type=="internal_library" || line_content.type=="vodka_instruction") {
            auto argsname=get_arguments(line_content.line_checked.content);
            for (auto arg:argsname) {
                if (find(variableslist_context.begin(),variableslist_context.end(),arg)==variableslist_context.end()) {
                    raise(ErrorContainer("vodka.error.variables.not_declared : "+arg+" isn't declared before instruction.",line_content.line_checked.file,{line_content.line_checked.content},{line_content.line_checked.line_number},srclclstack));
                    return false;
                }
            }
            for (int i=0;i<argsname.size();++i) {
                if (i+1<=(patern.size())) {
                    if (find(patern[i].begin(),patern[i].end(),variablesdict_context.at(argsname[i]).thing)==patern[i].end()) {
                        raise(ErrorContainer("vodka.error."+string_utilities::split(line_content.line_checked.content," ")[0]+".wrong_type : "+argsname[i]+" (argument number "+to_string(i+1)+") isn't of the expected datatype.",line_content.line_checked.file,{line_content.line_checked.content},{line_content.line_checked.line_number},srclclstack));
                        return false;
                    }
                } else {
                    if (find(lastest_allowed_type.begin(),lastest_allowed_type.end(),variablesdict_context.at(argsname[i]).thing)==lastest_allowed_type.end()) {
                        raise(ErrorContainer("vodka.error."+string_utilities::split(line_content.line_checked.content," ")[0]+".wrong_type : "+argsname[i]+" (argument number "+to_string(i+1)+") isn't of the expected datatype.",line_content.line_checked.file,{line_content.line_checked.content},{line_content.line_checked.line_number},srclclstack));
                        return false;
                    }
                }
            }
        } else {
            raise(ErrorContainer("vodka.error.analyser.chain_error : Line should be a function or instruction call.",line_content.line_checked.file,{line_content.line_checked.content},{line_content.line_checked.line_number},srclclstack));
            return false;
        }
    } else {
        raise(ErrorContainer("vodka.error.analyser.chain_error : Previous treatement hasn't been well executed.",line_content.line_checked.file,{line_content.line_checked.content},{line_content.line_checked.line_number},srclclstack));
        return false;
    }
    return true;
}