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
using namespace vodka::errors;
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
bool vodka::library::kernel::treatement::print_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),3});
    auto eles=split(line," ");
    if (eles.size()>=2) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),3});
        vector<string> eltoprint(eles.begin()+1,eles.end());
        for (auto a:eltoprint) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),a)==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+a+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
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
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(error_container("vodka.error.kernel.print.invalid syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::treatement::add_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()>=4) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        vector<string> argsgive(eles.begin()+1,eles.end());
        for (auto a:argsgive) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),a)==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+a+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify a constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        for (auto a:argsgive) {
            if (find(supported_type.at("add").begin(),supported_type.at("add").end(),call.variablesdict_context[a].thing)==supported_type.at("add").end()) {
                raise(error_container("vodka.error.kernel.add.wrong_type : "+a+" isn't vodint or vodec type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        for (int i=1;i<argsgive.size();++i) {
            if (call.variablesdict_context[argsgive[i-1]].thing!=call.variablesdict_context[argsgive[i]].thing) {
                raise(error_container("vodka.error.kernel.add.not_same_type : All arguments msut be of the same type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        string addtype=call.variablesdict_context[argsgive[0]].thing;
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        vector<string> uidargs;
        for (auto a:argsgive) {
            if (addtype=="vodint") {
                uidargs.push_back(call.variablesdict_context[a].intele.varinfo.uuid);
            } else if (addtype=="vodec") {
                uidargs.push_back(call.variablesdict_context[a].decele.varinfo.uuid);
            }
        }
        vodka::syscalls::ADD syscal;
        syscal.argument_uid=vector<string>(uidargs.begin()+1,uidargs.end());
        syscal.output_uid=uidargs[0];
        vodka::syscalls::syscall_container syscont;
        syscont.thing="ADD";
        syscont.addele=syscal;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(error_container("vodka.error.kernel.add.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::treatement::invert_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==2) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        string arg=eles[1];
        if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg)==call.variableslist_context.end()) {
            raise(error_container("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify a constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        if (call.variablesdict_context[arg].thing!="vodint") {
            raise(error_container("vodka.error.kernel.invert.wrong_type : "+arg+" isn't vodint type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        string uidarg=call.variablesdict_context[arg].intele.varinfo.uuid;
        vodka::syscalls::INVERT syscal;
        syscal.uid=uidarg;
        vodka::syscalls::syscall_container syscont;
        syscont.thing="INVERT";
        syscont.invertele=syscal;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(error_container("vodka.error.kernel.invert.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::treatement::free_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),2});
    auto eles=split(line," ");
    if (eles.size()>=2) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),2});
        string arg;
        for (int y=1;y<eles.size();++y) {
            arg=eles[y];
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg)==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
            if (eles[y].substr(0,2)=="$$") {
                raise(error_container("vodka.error.variables.constant : Can't delete an kernel constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
            if (call.variablesdict_context[eles[1]].thing=="vodarg") {
                raise(error_container("vodka.error.variables.argument : Can't delete an argument.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
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
        syscall_output.push_back(syscont);
        var_flag=true;
        auto args_erase=vector<string>(eles.begin()+1,eles.end());
        for (auto a:args_erase) {
            auto it=remove(call.variableslist_context.begin(),call.variableslist_context.end(),a);
            call.variableslist_context.erase(it,call.variableslist_context.end());
            call.variablesdict_context.erase(a);
        }
        return true;
    } else {
        raise(error_container("vodka.error.kernel.free.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::treatement::abs_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==2) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        string arg=eles[1];
        if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg)==call.variableslist_context.end()) {
            raise(error_container("vodka.error.variables.not_declared : "+arg+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify a constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
    
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        if (find(supported_type.at("abs").begin(),supported_type.at("abs").end(),call.variablesdict_context[arg].thing)==supported_type.at("abs").end()) {
            raise(error_container("vodka.error.kernel.abs.wrong_type : "+arg+" isn't vodint or vodec type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        string uidarg;
        if (call.variablesdict_context[arg].thing=="vodint") {
            uidarg=call.variablesdict_context[arg].intele.varinfo.uuid;
        } else if (call.variablesdict_context[arg].thing=="vodec") {
            uidarg=call.variablesdict_context[arg].decele.varinfo.uuid;
        }
        vodka::syscalls::ABS abscall;
        abscall.uid=uidarg;
        vodka::syscalls::syscall_container syscont;
        syscont.thing="ABS";
        syscont.absele=abscall;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(error_container("vodka.error.kernel.abs.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::treatement::divmod_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==5) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        for (int y=0;y<arg.size();++y) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg[y])==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+arg[y]+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$" || eles[2].substr(0,2)=="$$" || eles[2].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify a constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        for (int y=0;y<arg.size();++y) {
            if (call.variablesdict_context[arg[y]].thing!="vodint") {
                raise(error_container("vodka.error.kernel.divmod.wrong_type : "+arg[y]+" isn't vodint type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (arg[0]==arg[1]) {
            raise(error_container("vodka.error.kernel.divmod.same_output : the two output variables can't be the same.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
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
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(error_container("vodka.error.kernel.divmod.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::treatement::toint_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==3) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        for (int y=0;y<arg.size();++y) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg[y])==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+arg[y]+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify a constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        if (call.variablesdict_context[arg[0]].thing!="vodint") {
            raise(error_container("vodka.error.kernel.toint.wrong_type : "+arg[0]+" isn't vodint type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        if (find(conversion_syscall.at("TOINT").begin(),conversion_syscall.at("TOINT").end(),call.variablesdict_context[arg[1]].thing)==conversion_syscall.at("TOINT").end()) {
            raise(error_container("vodka.error.kernel.toint.wrong_type : "+arg[1]+" isn't a acceptable type to convert into vodint.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        vodka::syscalls::TOINT tointcall;
        if (call.variablesdict_context[arg[1]].thing=="vodec") {
            tointcall.uid_source=call.variablesdict_context[arg[1]].decele.varinfo.uuid;
        } else if (call.variablesdict_context[arg[1]].thing=="vodarg") {
            tointcall.uid_source=call.variablesdict_context[arg[1]].argele.varinfo.uuid;
        }
        tointcall.uid_output=call.variablesdict_context[arg[0]].intele.varinfo.uuid;
        vodka::syscalls::syscall_container syscont;
        syscont.thing="TOINT";
        syscont.tointele=tointcall;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(error_container("vodka.error.kernel.toint.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::treatement::todec_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==3) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        for (int y=0;y<arg.size();++y) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg[y])==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+arg[y]+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify a constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        if (call.variablesdict_context[arg[0]].thing!="vodec") {
            raise(error_container("vodka.error.kernel.todec.wrong_type : "+arg[0]+" isn't vodec type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        if (find(conversion_syscall.at("TODEC").begin(),conversion_syscall.at("TODEC").end(),call.variablesdict_context[arg[1]].thing)==conversion_syscall.at("TODEC").end()) {
            raise(error_container("vodka.error.kernel.todec.wrong_type : "+arg[1]+" isn't a acceptable type to convert into vodec.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        vodka::syscalls::TODEC todeccall;
        if (call.variablesdict_context[arg[1]].thing=="vodint") {
            todeccall.uid_source=call.variablesdict_context[arg[1]].intele.varinfo.uuid;
        } else if (call.variablesdict_context[arg[1]].thing=="vodarg") {
            todeccall.uid_source=call.variablesdict_context[arg[1]].argele.varinfo.uuid;
        }
        todeccall.uid_output=call.variablesdict_context[arg[0]].decele.varinfo.uuid;
        vodka::syscalls::syscall_container syscont;
        syscont.thing="TODEC";
        syscont.todecele=todeccall;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(error_container("vodka.error.kernel.todec.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::treatement::divide_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==5) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        for (int y=0;y<arg.size();++y) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),arg[y])==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+arg[y]+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$" || eles[2].substr(0,2)=="$$" || eles[2].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify a constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        vector<string> typeschem={"vodec","vodec","vodec","vodint"};
        for (int i=0;i<arg.size();++i) {
            if (call.variablesdict_context[arg[i]].thing!=typeschem[i]) {
                raise(error_container("vodka.error.kernel.divide.wrong_type : "+arg[i]+" isn't "+typeschem[i]+" type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        vodka::syscalls::DIVIDE dividecall;
        dividecall.output_uid=call.variablesdict_context[arg[0]].decele.varinfo.uuid;
        dividecall.first_uid=call.variablesdict_context[arg[1]].decele.varinfo.uuid;
        dividecall.second_uid=call.variablesdict_context[arg[2]].decele.varinfo.uuid;
        dividecall.precision_uid=call.variablesdict_context[arg[3]].intele.varinfo.uuid;
        vodka::syscalls::syscall_container dividecont;
        dividecont.divideele=dividecall;
        dividecont.thing="DIVIDE";
        syscall_output.push_back(dividecont);
        return true;
    } else {
        raise(error_container("vodka.error.kernel.divide.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::treatement::mulint_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==4) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        for (auto a:arg) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),a)==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+a+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$" || eles[2].substr(0,2)=="$$" || eles[2].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify a constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        vector<string> typeschem={"vodint","vodint","vodint"};
        for (int i=0;i<arg.size();++i) {
            if (call.variablesdict_context[arg[i]].thing!=typeschem[i]) {
                raise(error_container("vodka.error.kernel.mulint.wrong_type : "+arg[i]+" isn't "+typeschem[i]+" type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        vodka::syscalls::MULINT mulintcall;
        mulintcall.output_uid=call.variablesdict_context[arg[0]].intele.varinfo.uuid;
        mulintcall.first_uid=call.variablesdict_context[arg[1]].intele.varinfo.uuid;
        mulintcall.second_uid=call.variablesdict_context[arg[2]].intele.varinfo.uuid;
        vodka::syscalls::syscall_container syscont;
        syscont.thing="MULINT";
        syscont.mulintele=mulintcall;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(error_container("vodka.error.kernel.mulint.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
bool vodka::library::kernel::treatement::muldec_int(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    log("Checking system call syntax.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,1},{call.cell_context.content.size(),4});
    auto eles=split(line," ");
    if (eles.size()==5) {
        log("Checking content existence.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,2},{call.cell_context.content.size(),4});
        auto arg=vector<string>(eles.begin()+1,eles.end());
        for (auto a:arg) {
            if (find(call.variableslist_context.begin(),call.variableslist_context.end(),a)==call.variableslist_context.end()) {
                raise(error_container("vodka.error.variables.not_declared : "+a+" wasn't declared before instruction.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        if (eles[1].substr(0,2)=="$$" || eles[1].substr(0,1)=="$" || eles[2].substr(0,2)=="$$" || eles[2].substr(0,1)=="$") {
            raise(error_container("vodka.error.variables.constant : Can't modify a constant.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
            return false;
        }
        log("Checking content datatype.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,3},{call.cell_context.content.size(),4});
        vector<string> typeschem={"vodec","vodec","vodec","vodint"};
        for (int i=0;i<arg.size();++i) {
            if (call.variablesdict_context[arg[i]].thing!=typeschem[i]) {
                raise(error_container("vodka.error.kernel.muldec.wrong_type : "+arg[i]+" isn't "+typeschem[i]+" type.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
                return false;
            }
        }
        log("Registering system call.",call.verbose_context,call.main_logstep_context,call.last_logstep_context,2,{(int)call.iteration_number_context+1,4},{call.cell_context.content.size(),4});
        vodka::syscalls::MULDEC muldeccall;
        muldeccall.output_uid=call.variablesdict_context[arg[0]].decele.varinfo.uuid;
        muldeccall.first_uid=call.variablesdict_context[arg[1]].decele.varinfo.uuid;
        muldeccall.second_uid=call.variablesdict_context[arg[2]].decele.varinfo.uuid;
        muldeccall.precision_uid=call.variablesdict_context[arg[3]].intele.varinfo.uuid;
        vodka::syscalls::syscall_container syscont;
        syscont.thing="MULDEC";
        syscont.muldecele=muldeccall;
        syscall_output.push_back(syscont);
        return true;
    } else {
        raise(error_container("vodka.error.kernel.muldec.invalid_syntax : Invalid syntax.",call.file_name_context,{line},{call.cell_context.start.line+(int)call.iteration_number_context+1},srclclstack));
        return false;
    }
}
//* Main function for parsing kernel internal library
bool vodka::library::kernel::treatement::kernel_treatement(sources_stack lclstack) {
    auto srclclstack=lclstack;
    srclclstack.add(__PRETTY_FUNCTION__,__FILE__);
    line=call.type_analyser.line_analyse.content;
    if (call.type_analyser.checked==false) {
        return false;
    }
    if (call.type_analyser.type!="internal_library") {
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