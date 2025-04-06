#include "vodka-lib.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
using namespace std;
using namespace vodka::utilities;
using namespace vodka::syscalls;
using namespace vodka::errors;
//* Some necessary functions
namespace inside_instruction {
    std::vector<std::string> split(const std::string& str,const std::string& delimiter) {
        std::vector<std::string> tokens;
        size_t start=0;
        size_t end=str.find(delimiter);
        while (end!=std::string::npos) {
            if (end>start) {
                tokens.push_back(str.substr(start,end-start));
            }
            start=end+delimiter.length();
            end=str.find(delimiter,start);
        }
        if (start<str.length()) {
            tokens.push_back(str.substr(start));
        }
        return tokens;
    }
    void replaceall(std::string &str,const std::string &from,const std::string &to) {
        size_t start_pos=0;
        while ((start_pos=str.find(from, start_pos))!=std::string::npos) {
            str.replace(start_pos,from.length(),to);
            start_pos+=to.length();
        }
    }
};
using namespace inside_instruction;
//* Private functions for analysing each instructions
bool vodka::instructions::instruction_traitement::multiply(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking instruction syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==4) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        string arg;
        for (int y=1;y<eles.size();++y) {
            arg=eles[y];
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg)==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify an constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        vector<string> argsname(eles.begin()+1,eles.end());
        for (auto a:argsname) {
            if (find(vodka::instructions::supported_type.at("multiply").begin(),vodka::instructions::supported_type.at("multiply").end(),call.variablesdict_context[a].thing)==vodka::instructions::supported_type.at("multiply").end()) {
                raise(error_container("vodka.error.instruction.multiply.wrong_type : "+a+" isn't vodint or vodec type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        for (int i=1;i<argsname.size();++i) {
            if (call.variablesdict_context[argsname[i-1]].thing!=call.variablesdict_context[argsname[i]].thing) {
                raise(error_container("vodka.error.instruction.multiply.not_same_type : All arguments msut be of the same type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (call.variablesdict_context[argsname[0]].thing=="vodint") {
            log("Registering systems call for this instructions.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
            vector<string> uidargs;
            for (auto a:argsname) {
                uidargs.push_back(call.variablesdict_context[a].intele.varinfo.uuid);
            }
            MULINT mulintcall;
            mulintcall.output_uid=uidargs[0];
            mulintcall.first_uid=uidargs[1];
            mulintcall.second_uid=uidargs[2];
            syscall_container mulintcont;
            mulintcont.mulintele=mulintcall;
            mulintcont.thing="MULINT";
            syscalls_output.push_back(mulintcont);
            return true;
        } else if (call.variablesdict_context[argsname[0]].thing=="vodec") {
            log("Registering systems call for this instructions.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
            vector<string> uidargs;
            for (auto a:argsname) {
                uidargs.push_back(call.variablesdict_context[a].decele.varinfo.uuid);
            }
            string precision_uid=to_string(::genuid());
            ASSIGN asscall;
            asscall.output_uid=precision_uid;
            asscall.value="3";
            syscall_container asscont;
            asscont.assignele=asscall;
            asscont.thing="ASSIGN";
            syscalls_output.push_back(asscont);
            MULDEC muldeccall;
            muldeccall.output_uid=uidargs[0];
            muldeccall.first_uid=uidargs[1];
            muldeccall.second_uid=uidargs[2];
            muldeccall.precision_uid=precision_uid;
            syscall_container muldeccont;
            muldeccont.muldecele=muldeccall;
            muldeccont.thing="MULDEC";
            syscalls_output.push_back(muldeccont);
            FREE freecall;
            freecall.argument_uid={precision_uid};
            syscall_container freecont;
            freecont.thing="FREE";
            freecont.freeele=freecall;
            syscalls_output.push_back(freecont);
            return true;
        }
    } else if (eles.size()==5) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        string arg;
        for (int y=1;y<eles.size();++y) {
            arg=eles[y];
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg)==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify an constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        vector<string> inoutargs(eles.begin()+1,eles.end()-1);
        for (auto a:inoutargs) {
            if (call.variablesdict_context[a].thing!="vodec") {
                raise(error_container("vodka.error.instruction.multiply.wrong_type : "+a+" isn't vodec type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (call.variablesdict_context[eles[4]].thing!="vodint") {
            raise(error_container("vodka.error.instruction.multiply.wrong_type : "+eles[4]+" isn't vodint type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Registering systems call for this instructions.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        vector<string> uidargs;
        for (auto a:inoutargs) {
            uidargs.push_back(call.variablesdict_context[a].decele.varinfo.uuid);
        }
        uidargs.push_back(call.variablesdict_context[eles[4]].intele.varinfo.uuid);
        MULDEC muldeccall;
        muldeccall.output_uid=uidargs[0];
        muldeccall.first_uid=uidargs[1];
        muldeccall.second_uid=uidargs[2];
        muldeccall.precision_uid=uidargs[3];
        syscall_container muldeccont;
        muldeccont.muldecele=muldeccall;
        muldeccont.thing="MULDEC";
        syscalls_output.push_back(muldeccont);
        return true;
    } else {
        raise(error_container("vodka.error.instruction.multiply.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
    return false;
}
//* Main function for parsing vodka instruction
bool vodka::instructions::instruction_traitement::traitement(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    line=call.type_analyser.line_analyse.content;
    if (call.type_analyser.checked==false) {
        return false;
    }
    if (call.type_analyser.type!="vodka_instruction") {
        return false;
    }
    if (line.substr(0,8)=="multiply") {
        return multiply(srclclstack);
    }
    return false;
}