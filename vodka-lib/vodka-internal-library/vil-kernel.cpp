#include "../vodka-lib.h"
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
//* Some necessary functions
namespace inside_vilkernel {
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
using namespace inside_vilkernel;
//* Private functions for analysing each instructions
bool vodka::library::kernel::traitement::print_int() {
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()>=2) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),3});
        vector<string> eltoprint(eles.begin()+1,eles.end());
        for (auto a:eltoprint) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),a)==call.variableslist_context.end()) {
                error("vodka.error.variables.not_declared : "+a+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
                return false;
            }
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),3});
        vector<string> uidtoprint;
        for (auto a:eltoprint) {
            if (call.variablesdict_context[a].thing=="vodint") {
                uidtoprint.push_back(call.variablesdict_context[a].intele.varinfo.uuid);
            } else if (call.variablesdict_context[a].thing=="vodec") {
                uidtoprint.push_back(call.variablesdict_context[a].decele.varinfo.uuid);
            } else if (call.variablesdict_context[a].thing=="vodarg") {
                uidtoprint.push_back(call.variablesdict_context[a].argele.varinfo.uuid);
            }
        }
        vodka::syscalls::PRINT syscal;
        syscal.argument_uid=uidtoprint;
        vodka::syscalls::syscall_container syscont;
        syscont.thing="PRINT";
        syscont.printele=syscal;
        syscall_output=syscont;
        return true;
    } else {
        error("vodka.error.kernel.print.invalid syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
        return false;
    }
}
bool vodka::library::kernel::traitement::add_int() {
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()>=4) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        vector<string> argsgive(eles.begin()+1,eles.end());
        for (auto a:argsgive) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),a)==call.variableslist_context.end()) {
                error("vodka.error.variables.not_declared : "+a+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            error("vodka.error.variables.constant : Can't modify an constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        for (auto a:argsgive) {
            if (call.variablesdict_context[a].thing!="vodint" || call.variablesdict_context[a].intele.typeinfo.typenames!="vodint") {
                error("vodka.error.kernel.add.wrong_type : "+a+" isn't vodint type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
                return false;
            }
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        vector<string> uidargs;
        for (auto a:argsgive) {
            uidargs.push_back(call.variablesdict_context[a].intele.varinfo.uuid);
        }
        vodka::syscalls::ADD syscal;
        syscal.argument_uid=vector<string>(uidargs.begin()+1,uidargs.end());
        syscal.output_uid=uidargs[0];
        vodka::syscalls::syscall_container syscont;
        syscont.thing="ADD";
        syscont.addele=syscal;
        syscall_output=syscont;
        return true;
    } else {
        error("vodka.error.kernel.add.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
        return false;
    }
}
bool vodka::library::kernel::traitement::invert_int() {
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==2) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        string arg=eles[1];
        if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg)==call.variableslist_context.end()) {
            error("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            error("vodka.error.variables.constant : Can't modify an constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        if (call.variablesdict_context[arg].thing!="vodint" || call.variablesdict_context[arg].intele.typeinfo.typenames!="vodint") {
            error("vodka.error.kernel.invert.wrong_type : "+arg+" isn't vodint type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
            return false;
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        string uidarg=call.variablesdict_context[arg].intele.varinfo.uuid;
        vodka::syscalls::INVERT syscal;
        syscal.uid=uidarg;
        vodka::syscalls::syscall_container syscont;
        syscont.thing="INVERT";
        syscont.invertele=syscal;
        syscall_output=syscont;
        return true;
    } else {
        error("vodka.error.kernel.invert.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
        return false;
    }
}
bool vodka::library::kernel::traitement::free_int() {
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),2});
    auto eles=split(line," ");
    if (eles.size()>=2) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),2});
        string arg;
        for (int y=1;y<eles.size();++y) {
            arg=eles[y];
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg)==call.variableslist_context.end()) {
                error("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
                return false;
            }
            if (eles[y].substr(0,2)=="$$") {
                error("vodka.error.variables.constant : Can't delete an kernel constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
            }
            if (call.variablesdict_context[eles[1]].thing=="vodarg") {
                error("vodka.error.variables.argument : Can't delete an argument.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
            }
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),3});
        vector<string> argsuid;
        for (auto a:vector<string>(eles.begin()+1,eles.end())) {
            if (call.variablesdict_context[a].thing=="vodint") {
                argsuid.push_back(call.variablesdict_context[a].intele.varinfo.uuid);
            }
        }
        vodka::syscalls::FREE freecall;
        freecall.argument_uid=argsuid;
        vodka::syscalls::syscall_container syscont;
        syscont.freeele=freecall;
        syscont.thing="FREE";
        syscall_output=syscont;
        var_flag=true;
        auto args_erase=vector<string>(eles.begin()+1,eles.end());
        for (auto a:args_erase) {
            auto it=remove(call.variableslist_context.begin(),call.variableslist_context.end(),a);
            call.variableslist_context.erase(it,call.variableslist_context.end());
            call.variablesdict_context.erase(a);
        }
        return true;
    } else {
        error("vodka.error.kernel.free.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
        return false;
    }
}
bool vodka::library::kernel::traitement::abs_int() {
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==2) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        string arg=eles[1];
        if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg)==call.variableslist_context.end()) {
            error("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            error("vodka.error.variables.constant : Can't modify an constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
        }
    
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        if (call.variablesdict_context[arg].thing!="vodint" || call.variablesdict_context[arg].intele.typeinfo.typenames!="vodint") {
            error("vodka.error.kernel.abs.wrong_type : "+arg+" isn't vodint type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
            return false;
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        string uidarg=call.variablesdict_context[arg].intele.varinfo.uuid;
        vodka::syscalls::ABS abscall;
        abscall.uid=uidarg;
        vodka::syscalls::syscall_container syscont;
        syscont.thing="ABS";
        syscont.absele=abscall;
        syscall_output=syscont;
        return true;
    } else {
        error("vodka.error.kernel.abs.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
        return false;
    }
}
bool vodka::library::kernel::traitement::divmod_int() {
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==5) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        for (int y=0;y<arg.size();++y) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg[y])==call.variableslist_context.end()) {
                error("vodka.error.variables.not_declared : "+arg[y]+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$" || eles[2].substr(0,2)=="$$" || eles[2].substr(0,1)=="$") {
            error("vodka.error.variables.constant : Can't modify an constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        for (int y=0;y<arg.size();++y) {
            if (call.variablesdict_context[arg[y]].thing!="vodint" || call.variablesdict_context[arg[y]].intele.typeinfo.typenames!="vodint") {
                error("vodka.error.kernel.divmod.wrong_type : "+arg[y]+" isn't vodint type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
                return false;
            }
        }
        if (arg[0]==arg[1]) {
            error("vodka.error.kernel.divmod.same_output : the two output variables can't be the same.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
            return false;
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        string quouid=call.variablesdict_context[arg[0]].intele.varinfo.uuid;
        string resuid=call.variablesdict_context[arg[1]].intele.varinfo.uuid;
        string divid=call.variablesdict_context[arg[2]].intele.varinfo.uuid;
        string divis=call.variablesdict_context[arg[3]].intele.varinfo.uuid;
        vodka::syscalls::DIVMOD divmodcall;
        divmodcall.quotient_uid=quouid;
        divmodcall.rest_uid=resuid;
        divmodcall.dividend_uid=divid;
        divmodcall.divisor_uid=divis;
        vodka::syscalls::syscall_container syscont;
        syscont.thing="DIVMOD";
        syscont.divmodele=divmodcall;
        syscall_output=syscont;
        return true;
    } else {
        error("vodka.error.kernel.abs.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1});
        return false;
    }
}
//* Main function for parsing kernel internal library
bool vodka::library::kernel::traitement::kernel_traitement() {
    line=call.type_analyser.line_analyse.content;
    if (call.type_analyser.checked==false) {
        return false;
    }
    if (call.type_analyser.type!="internal_library") {
        return false;
    }
    if (line.substr(0,12)=="kernel.print") {
        return print_int();
    } else if (line.substr(0,10)=="kernel.add") {
        return add_int();
    } else if (line.substr(0,13)=="kernel.invert") {
        return invert_int();
    } else if (line.substr(0,11)=="kernel.free") {
        return free_int();
    } else if (line.substr(0,10)=="kernel.abs") {
        return abs_int();
    } else if (line.substr(0,13)=="kernel.divmod") {
        return divmod_int();
    }
    return false;
}