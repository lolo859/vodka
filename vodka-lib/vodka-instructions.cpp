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
using namespace vodka::variables;
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
            if (call.variablesdict_context[a].thing!="vodint" || call.variablesdict_context[a].intele.typeinfo.typenames!="vodint") {
                raise(error_container("vodka.error.instruction.multiply.wrong_type : "+a+" isn't vodint type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (call.variablesdict_context[eles[2]].intele.varinfo.algo_dependant==false && call.variablesdict_context[eles[3]].intele.varinfo.algo_dependant==false) {
            log("Registering systems call for this instructions.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
            vector<string> uidargs;
            for (auto a:argsname) {
                uidargs.push_back(call.variablesdict_context[a].intele.varinfo.uuid);
            }
            string outputuid=uidargs[0];
            vodka::syscalls::ASSIGN assigncall;
            assigncall.output_uid=outputuid;
            assigncall.value="0";
            vodka::syscalls::syscall_container syscont;
            syscont.thing="ASSIGN";
            syscont.assignele=assigncall;
            syscalls_output.push_back(syscont);
            if (call.variablesdict_context.at(argsname[2]).intele.value!="0") {
                if (call.variablesdict_context[argsname[1]].intele.value.substr(0,1)=="-") {
                    vodka::syscalls::INVERT invertcall;
                    invertcall.uid=uidargs[1];
                    vodka::syscalls::syscall_container syscont;
                    syscont.invertele=invertcall;
                    syscont.thing="INVERT";
                    syscalls_output.push_back(syscont);
                }
                if (call.variablesdict_context[argsname[2]].intele.value.substr(0,1)=="-") {
                    vodka::syscalls::INVERT invertcall;
                    invertcall.uid=uidargs[2];
                    vodka::syscalls::syscall_container syscont;
                    syscont.invertele=invertcall;
                    syscont.thing="INVERT";
                    syscalls_output.push_back(syscont);
                }
                vodka::syscalls::ASSIGN assigncall;
                string countuid=to_string(genuid());
                assigncall.output_uid=countuid;
                assigncall.value="0";
                vodka::syscalls::syscall_container syscont;
                syscont.thing="ASSIGN";
                syscont.assignele=assigncall;
                syscalls_output.push_back(syscont);
                vodka::syscalls::ASSIGN assigncall2;
                string one_const=to_string(genuid());
                assigncall2.output_uid=one_const;
                assigncall2.value="1";
                vodka::syscalls::syscall_container syscont2;
                syscont2.thing="ASSIGN";
                syscont2.assignele=assigncall2;
                syscalls_output.push_back(syscont2);
                vodka::syscalls::ASSIGN assigncall3;
                string two_const=to_string(genuid());
                assigncall3.output_uid=two_const;
                assigncall3.value="2";
                vodka::syscalls::syscall_container syscont3;
                syscont3.thing="ASSIGN";
                syscont3.assignele=assigncall3;
                syscalls_output.push_back(syscont3);
                vector<string> uidtoadd={outputuid,uidargs[1]};
                vodka::syscalls::ADD addcall;
                addcall.output_uid=outputuid;
                addcall.argument_uid=uidtoadd;
                vodka::syscalls::syscall_container syscont4;
                syscont4.thing="ADD";
                syscont4.addele=addcall;
                syscalls_output.push_back(syscont4);
                vector<string> uidtoadd2={countuid,one_const};
                vodka::syscalls::ADD addcall2;
                addcall2.output_uid=countuid;
                addcall2.argument_uid=uidtoadd2;
                vodka::syscalls::syscall_container syscont5;
                syscont5.thing="ADD";
                syscont5.addele=addcall2;
                syscalls_output.push_back(syscont5);
                vodka::syscalls::BACK backcall;
                backcall.back_uid=two_const;
                backcall.var_uid=countuid;
                backcall.const_uid=uidargs[2];
                vodka::syscalls::syscall_container syscont6;
                syscont6.thing="BACK";
                syscont6.backele=backcall;
                syscalls_output.push_back(syscont6);
                vodka::syscalls::FREE freecall;
                freecall.argument_uid={countuid,one_const,two_const};
                vodka::syscalls::syscall_container syscont7;
                syscont7.thing="FREE";
                syscont7.freeele=freecall;
                syscalls_output.push_back(syscont7);
                if (vodka::type::vodint::calculate_sign(call.variablesdict_context[argsname[1]].intele.value,call.variablesdict_context[argsname[2]].intele.value,srclclstack).substr(0,1)=="-") {
                    vodka::syscalls::INVERT invertcall;
                    invertcall.uid=outputuid;
                    vodka::syscalls::syscall_container syscont;
                    syscont.invertele=invertcall;
                    syscont.thing="INVERT";
                    syscalls_output.push_back(syscont);
                }
                if (call.variablesdict_context[argsname[1]].intele.value.substr(0,1)=="-") {
                    vodka::syscalls::INVERT invertcall;
                    invertcall.uid=uidargs[1];
                    vodka::syscalls::syscall_container syscont;
                    syscont.invertele=invertcall;
                    syscont.thing="INVERT";
                    syscalls_output.push_back(syscont);
                }
                if (call.variablesdict_context[argsname[2]].intele.value.substr(0,1)=="-") {
                    vodka::syscalls::INVERT invertcall;
                    invertcall.uid=uidargs[2];
                    vodka::syscalls::syscall_container syscont;
                    syscont.invertele=invertcall;
                    syscont.thing="INVERT";
                    syscalls_output.push_back(syscont);
                }
                return true;
            } else {
                return true;
            }
        } else {
            raise(error_container("vodka.error.instruction.multiply.not_constance : Can't multiply variable without knowing their values.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
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