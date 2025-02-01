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
//* Some necessary functions
namespace inside_analyser {
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
}
using namespace inside_analyser;
//* Checking if the line is conform to vodka syntax (doesn't check the line argument)
bool vodka::analyser::line::check() {
    if (content=="") {
        skip=true;
        return true;
    }
    if (content.substr(0,1)==">" || content.substr()==">>") {
        return true;
    } else if (content.substr(0,6)=="vodka ") {
        return true;
    }
    for (auto a:vodka::internal_library) {
        if (content.substr(0,a.size())==a && split(content,".").size()>=2) {
            auto functions=vodka::internal_library_functions.at(a);
            for (auto b:functions) {
                if (split(content,".")[1].substr(0,b.size())==b) {
                    return true;
                }
            }
        }
    }
    for (auto a:vodka::vodka_instruction) {
        if (content.substr(0,a.size())==a) {
            return true;
        }
    }
    return false;
}
//* Analyse the type of line (variable declaration, vodka instruction, library instruction, debug line)
bool vodka::analyser::type_analyser::line_type_analyse() {
    if (line_analyse.checked==false) {
        return false;
    } else {
        if (line_analyse.content.substr(0,2)==">>") {
            type="debug_two";
            return true;
        } else if (line_analyse.content.substr(0,1)==">" && line_analyse.content.substr(0,2)!=">>") {
            type="debug_one";
            return true;
        } else if (line_analyse.content.substr(0,6)=="vodka ") {
            type="var";
            return true;
        }
        for (auto a:vodka::internal_library) {
            if (line_analyse.content.substr(0,a.size())==a) {
                type="internal_library";
                library_name=a;
                return true;
            }
        }
        for (auto a:vodka::vodka_instruction) {
            if (line_analyse.content.substr(0,a.size())==a) {
                type="vodka_instruction";
                instruction_name=a;
                return true;
            }
        }
    }
    return false;
}
//* Parse the variable declaration (name, datatype, value, constant)
bool vodka::analyser::var_dec_analyser::var_dec_analyse() {
    if (line_analyse.checked==false || line_analyse.line_analyse.checked==false || line_analyse.type!="var") {
        return false;
    }
    if (line_analyse.line_analyse.content.substr(0,5)=="vodka") {
        auto eles=split(line_analyse.line_analyse.content,"=");
        if (eles.size()>=2) {
            auto namepart=split(eles[0]," ");
            string valuestr;
            for (int i=1;i<eles.size();++i) {
                if (i==1) {
                    valuestr=eles[i];
                } else {
                    valuestr=valuestr+" "+eles[i];
                }
            }
            auto valuepart=split(valuestr," ");
            if (namepart.size()==2) {
                name=namepart[1];
                is_kernel_const=name.substr(0,2)=="$$";
                is_vodka_const=name.substr(0,1)=="$";
            } else {
                return false;
            }
            if (valuepart.size()>=2) {
                datatype=valuepart[0];
                value=valuestr.substr(datatype.size()+1);
                if (datatype=="vodka" && name.substr(0,2)=="$$") {
                    return false;
                } else {
                    return true;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}
//* Check the type and value of the variable (use vodka::type::<concerned type>::check_value(), if datatype is vodka, please include a list of already declared variables inside the context argument)
bool vodka::analyser::var_dec_analyser::check_type_value(vector<string> context) {
    if (checked) {
        bool f=false;
        for (auto a:vodka::internal_type) {
            if (datatype==a) {
                f=true;
                break;
            } else {
                f=false;
            }
        }
        if (!f) {
            return false;
        }
        if (datatype=="vodint") {
            return vodka::type::vodint::check_value(value);
        } else if (datatype=="vodec") {
            return vodka::type::vodec::check_value(value);
        } else if (datatype=="vodka") {
            if (name.substr(0,2)=="$$") {
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
                return false;
            }
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
//* Make the corresponding vodka::variables::variable
bool vodka::analyser::var_dec_analyser::make_info() {
    if (checked_type_value) {
        if (datatype!="vodka") {
            var.algo_dependant=false;
            var.consts=is_vodka_const;
            var.define=is_kernel_const;
            var.uuid=to_string(vodka::utilities::genuid());
            var.varname=name;
            return true;
        } else {
            if (source_duplication.thing!="") {
                if (source_duplication.thing=="vodint") {
                    var.algo_dependant=source_duplication.intele.varinfo.algo_dependant;
                    var.consts=is_vodka_const;
                    var.write=false;
                    var.uuid=to_string(vodka::utilities::genuid());
                    var.varname=name;
                    return true;
                } else if (source_duplication.thing=="vodec") {
                    var.algo_dependant=source_duplication.decele.varinfo.algo_dependant;
                    var.consts=is_vodka_const;
                    var.write=false;
                    var.uuid=to_string(vodka::utilities::genuid());
                    var.varname=name;
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }
    } else {
        return false;
    }
}
//* Make a pre-treatement of the value to store
bool vodka::analyser::var_dec_analyser::pre_treatement() {
    if (checked_type_value) {
        if (datatype=="vodint") {
            value=vodka::type::vodint::remove_zero(value);
            return true;
        } else if (datatype=="vodec") {
            value=vodka::type::vodec::remove_zero(value);
            return true;
        } else if (datatype=="vodka") {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}
//* Output the variable under a vodka::variable::variable_container object
bool vodka::analyser::var_dec_analyser::output() {
    if (pre_treated) {
        if (datatype=="vodint") {
            vodka::variables::vodint varr;
            varr.varinfo=var;
            varr.value=value;
            var_object.intele=varr;
            var_object.thing="vodint";
            vodka::syscalls::ASSIGN asscall;
            asscall.output_uid=var.uuid;
            asscall.value=value;
            vodka::syscalls::syscall_container asscont;
            asscont.thing="ASSIGN";
            asscont.assignele=asscall;
            vodka_object=asscont;
            return true;
        } else if (datatype=="vodec") {
            vodka::variables::vodec varr;
            varr.varinfo=var;
            varr.value=value;
            var_object.decele=varr;
            var_object.thing="vodec";
            vodka::syscalls::ASSIGN asscall;
            asscall.output_uid=var.uuid;
            asscall.value=value;
            vodka::syscalls::syscall_container asscont;
            asscont.thing="ASSIGN";
            asscont.assignele=asscall;
            vodka_object=asscont;
            return true;
        } else if (datatype=="vodka") {
            if (source_duplication.thing!="") {
                if (source_duplication.thing=="vodint") {
                    vodka::variables::vodint varr;
                    varr.varinfo=var;
                    varr.value=source_duplication.intele.value;
                    var_object.intele=varr;
                    var_object.thing="vodint";
                    vodka::syscalls::DUPLICATE dupcall;
                    dupcall.output_uid=var.uuid;
                    dupcall.source_uid=source_duplication.intele.varinfo.uuid;
                    vodka::syscalls::syscall_container dupcont;
                    dupcont.thing="DUPLICATE";
                    dupcont.duplicateele=dupcall;
                    vodka_object=dupcont;
                    return true;
                } else if (source_duplication.thing=="vodec") {
                    vodka::variables::vodec varr;
                    varr.varinfo=var;
                    varr.value=source_duplication.decele.value;
                    var_object.decele=varr;
                    var_object.thing="vodec";
                    vodka::syscalls::DUPLICATE dupcall;
                    dupcall.output_uid=var.uuid;
                    dupcall.source_uid=source_duplication.intele.varinfo.uuid;
                    vodka::syscalls::syscall_container dupcont;
                    dupcont.thing="DUPLICATE";
                    dupcont.duplicateele=dupcall;
                    vodka_object=dupcont;
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}