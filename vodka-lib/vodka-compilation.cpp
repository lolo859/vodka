#include "vodka-lib.h"
#include <string>
#include <iostream>
#include <vector>
using namespace vodka::utilities;
using namespace vodka::errors;
//* Remove the inline comments
bool vodka::compilation::VodkaFile::_remove_comments() {
    for (int i=0;i<file_content.size();++i) {
        if (file_content[i]!="") {
            if (file_content[i].find("§",0)==0) {
                file_content[i]="";
            } else {
                file_content[i]=string_utilities::split(file_content[i],"§")[0];
            }
        }
    }
    return true;
}
//* Detect symbols and process symbols non-related to cells
bool vodka::compilation::PreCompilation::_parse_symbols(int& log_main_step,SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (file.get_remove_comments()==false) {
        return false;
    }
    for (size_t i=0;i<file.file_content.size();++i) {
        vodka::utilities::structs::symbol temp;
        string line=file.file_content[i];
        output::log("Detecting if line "+to_string(i+1)+" contain symbol :",log_main_step,1,{(int)i+1},{file.file_content.size()});
        if (line.rfind("£",0)==0) {
            output::log("Allocating memory.",log_main_step,2,{(int)i+1,1},{file.file_content.size(),3});
            temp.line=i+1;
            temp.content=line;
            auto ele=string_utilities::split(line," ");
            temp.type=ele[0].substr(2,ele[0].size());
            output::log("Checking symbol.",log_main_step,2,{(int)i+1,2},{file.file_content.size(),3});
            if (find(InternalSymbol.begin(),InternalSymbol.end(),temp.type)==InternalSymbol.end()) {
                int linenum=i+1;
                raise(ErrorContainer("vodka.error.symbol.unknow_symbol : Unknow symbol found : "+temp.type,file.file_source,{line},{linenum},lclstack));
                return false;
            } else {
                output::log("Saving symbol.",log_main_step,2,{(int)i+1,3},{file.file_content.size(),3});
                symbols_list.push_back(temp);
            }
        } else {
            output::log("No symbol detected.",log_main_step,2,{(int)i+1,1},{file.file_content.size(),1});
        }
    }
    log_main_step=log_main_step+1;
    output::log("Searching define replacement instruction :",log_main_step);
    for (size_t i=0;i<symbols_list.size();++i) {
        output::log("Checking symbol type :",log_main_step,1,{(int)i+1},{symbols_list.size()});
        if (symbols_list[i].type=="VODEFINE") {
            auto eles=string_utilities::split(symbols_list[i].content," ");
            if (eles.size()==3) {
                replacement[eles[1]]=eles[2];
                output::log("Replacement found. "+eles[1]+" will be replaced with "+eles[2]+".",log_main_step,2,{(int)i+1,1},{symbols_list.size(),1});
            } else {
                raise(ErrorContainer("vodka.error.vodefine.invalid_syntax : Invalid syntax for define replacement declaration.",file.file_source,{symbols_list[i].content},{symbols_list[i].line},lclstack));
                return false;
            }
        } else {
            output::log("No VODEFINE symbol detected.",log_main_step,2,{(int)i+1,1},{symbols_list.size(),1});
        }
    }
    log_main_step=log_main_step+1;
    bool typefound=false;
    output::log("Looking for type symbol :",log_main_step);
    for (size_t i=0;i<symbols_list.size();++i) {
        output::log("Checking symbol type.",log_main_step,1,{(int)i+1},{symbols_list.size()});
        if (symbols_list[i].type=="VODTYPE") {
            if (typefound==false) {
                typefound=true;
            } else {
                raise(ErrorContainer("vodka.error.vodtype.confusion : A vodka program can only contain one VODTYPE symbol.",file.file_source,{symbols_list[i].content},{symbols_list[i].line},lclstack));
                return -1;
            }
        }
    }
    if (typefound==false) {
        raise(ErrorContainer("vodka.error.vodtype.not_found : Can't find VODTYPE symbol.",file.file_source,{"<no line affected>"},{0},lclstack));
        return false;
    }
    return true;
}
//* Processing VODTYPE symbol
bool vodka::compilation::PreCompilation::_detect_program_type(int& log_main_step,SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (get_parse_symbol()==false) {
        return false;
    }
    output::log("Detecting program type :",log_main_step);
    for (size_t i=0;i<symbols_list.size();++i) {
        output::log("Checking if line contain type symbol :",log_main_step,1,{(int)i+1},{symbols_list.size()});
        if (symbols_list[i].type=="VODTYPE") {
            output::log("Checking syntax.",log_main_step,2,{(int)i+1,1},{symbols_list.size(),3});
            auto ele=string_utilities::split(symbols_list[i].content," ");
            if (ele.size()!=2) {
                raise(ErrorContainer("vodka.error.vodtype.invalid_syntax : Invalid syntax for program type declaration.",file.file_source,{symbols_list[i].content},{symbols_list[i].line},lclstack));
                return false;
            }
            output::log("Checking program type.",log_main_step,2,{(int)i+1,2},{symbols_list.size(),3});
            if (find(InternalTypes.begin(),InternalTypes.end(),ele[1])==InternalTypes.end()) {
                raise(ErrorContainer("vodka.error.vodtype.unknow_type : Unknow type : "+ele[1],file.file_source,{symbols_list[i].content},{symbols_list[i].line},lclstack));
                return false;
            }
            output::log("Saving program type.",log_main_step,2,{(int)i+1,3},{symbols_list.size(),3});
            program_type=ele[1];
        } else {
            output::log("The symbol isn't the type symbol.",log_main_step,2,{(int)i+1,1},{symbols_list.size(),1});
        }
    }
    return true;
}
//* Process symbols related to cells
bool vodka::compilation::PreCompilation::_detect_cells(int& log_main_step,SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (get_detect_program_type()==false) {
        return false;
    }
    output::log("Detecting cells :",log_main_step);
    for (size_t i=0;i<symbols_list.size();++i) {
        if (symbols_list[i].type=="VODSTART") {
            output::log("Checking start symbol syntax.",log_main_step,1,{1},{10});
            if (symbols_list.size()>=i+2 && symbols_list[i+1].type=="VODEND") {
                output::log("Allocating memory.",log_main_step,1,{2},{10});
                structs::cell temp;
                output::log("Detecting cell start and end symbol.",log_main_step,1,{3},{10});
                temp.start=symbols_list[i];
                temp.end=symbols_list[i+1];
                output::log("Detecting cell name.",log_main_step,1,{4},{10});
                auto args=string_utilities::split(symbols_list[i].content," ");
                if (args.size()>1) {
                    temp.name=args[1];
                } else {
                    raise(ErrorContainer("vodka.error.vodstart.name_not_found : Can't find cell's name.",file.file_source,{symbols_list[i].content},{symbols_list[i].line},lclstack));
                    return false;
                }
                output::log("Checking if an cell with the same name already exist in the program.",log_main_step,1,{5},{10});
                auto it=find(cells_names.begin(),cells_names.end(),temp.name);
                vector<string> contenterror;
                vector<int> lineerror;
                if (it!=cells_names.end()) {
                    for (size_t y=0;y<cells_list.size();++y) {
                        if (cells_list[y].name==temp.name) {
                            contenterror.push_back(cells_list[y].start.content);
                            lineerror.push_back(cells_list[y].start.line);
                        }
                    } 
                    contenterror.push_back(temp.start.content);
                    lineerror.push_back(temp.start.line);
                    raise(ErrorContainer("vodka.error.cell.confusion : An existing cell with the same name already exists.",file.file_source,contenterror,lineerror,lclstack));
                    return false;
                }
                output::log("Detecting cell argument.",log_main_step,1,{6},{10});
                if (args.size()>2) {
                    temp.args=vector<string>(args.begin()+2,args.end());
                }
                output::log("Detecting cell output.",log_main_step,1,{7},{10});
                auto outs=string_utilities::split(temp.end.content," ");
                if (outs.size()>1) {
                    temp.outs=vector<string>(outs.begin()+1,outs.end());
                }
                output::log("Saving cell content.",log_main_step,1,{8},{10});
                int startline=temp.start.line;
                int endline=temp.end.line-2;
                for (int i=startline;i<endline+1;++i) {
                    temp.content.push_back(file.file_content[i]);
                }
                output::log("Saving cell.",log_main_step,1,{9},{10});
                cells_list.push_back(temp);
                cells_names.push_back(temp.name);
                output::log("Detecting if cell is the main cell.",log_main_step,1,{10},{10});
                if (temp.name=="main") {
                    maincell=temp;
                    for (auto a:maincell.args) {
                        auto uid=genvyid();
                        vodka::variables::VariableMetadata argsinfo;
                        argsinfo.algo_dependant=true;
                        argsinfo.is_vodka_constant=false;
                        argsinfo.is_kernel_constant=false;
                        argsinfo.in_data_section=false;
                        argsinfo.uuid=genvyid();
                        argsinfo.name=a;
                        vodka::variables::VodargVariable argscont;
                        vodka::variables::VariableContainer varcont;
                        varcont.thing=vodka::variables::VariableDatatype::vodarg;
                        varcont.variable_metadata=argsinfo;
                        varcont.vodarg_element=argscont;
                        maincell_args_dict[a]=varcont;
                        maincell_args_list.push_back(a);
                        variables_list.push_back(a);
                        variables_dict[a]=varcont;
                    }
                }
            } else {
                raise(ErrorContainer("vodka.error.cell.vodend_not_found : Can't find corresponding VODEND symbol to the previous VODSTART.",file.file_source,{symbols_list[i].content},{symbols_list[i].line},lclstack));
                return false;
            }
        }
    }
    log_main_step=log_main_step+1;
    output::log("Verifying if program contain a main cell.",log_main_step);
    if (maincell.name!="main") {
        raise(ErrorContainer("vodka.error.cell.main_not_found : Can't find the main cell.",file.file_source,{"<no line affected>"},{0},lclstack));
        return false;
    }
    return true;
}
//* Apply pre-treatement on the code
bool vodka::compilation::PreCompilation::_code_pretreatement(vector<string>& compiled_output,bool replace,int& log_main_step,SourcesStack srclclstack) {
    auto lclstack=srclclstack;
    lclstack.add(__PRETTY_FUNCTION__,__FILE__);
    if (get_detect_cells()==false) {
        return false;
    }
    output::log("Writing args section.",log_main_step);
    for (auto argm:maincell_args_dict) {
        compiled_output.push_back(argm.second.variable_metadata.uuid);
    }
    if (replace==false) {
        log_main_step=log_main_step+1;
        output::log("Skipping replacement step because of -r.",log_main_step);
    } else {
        log_main_step=log_main_step+1;
        output::log("Started define replacing process.",log_main_step);
        for (int i=0;i<maincell.content.size();++i) {
            for (auto r:replacement) {
                auto temp=string_utilities::split(maincell.content[i],r.first);
                string newline;
                for (int y=0;y<temp.size();++y) {
                    if (y==0) {
                        newline=temp[0];
                    } else {
                        newline=newline+r.second+temp[y];
                    }
                }
                if (maincell.content[i].substr(0,r.first.size())==r.first) {
                    newline=r.second+newline;
                }
                maincell.content[i]=newline;
            }
        }
        for (auto u:cells_list) {
            for (int i=0;i<u.content.size();++i) {
                for (auto r:replacement) {
                    auto temp=string_utilities::split(maincell.content[i],r.first);
                    string newline;
                    for (int y=0;y<temp.size();++y) {
                        if (y==0) {
                            newline=temp[0];
                        } else {
                            newline=newline+r.second+temp[y];
                        }
                    }
                    if (maincell.content[i].substr(0,r.first.size())==r.first) {
                        newline=r.second+newline;
                    }
                    maincell.content[i]=newline;
                }
            }
        }
    }
    log_main_step=log_main_step+1;
    output::log("Started datatypes replacement process.",log_main_step);
    vector<string> directs_data;
    for (int i=0;i<maincell.content.size();++i) {
        vodka::analyser::LineSyntaxChecker localline;
        localline.line_number=maincell.start.line+i+1;
        localline.file=file.file_source;
        localline.content=maincell.content[i];
        auto direct_declared_data=string_utilities::split(maincell.content[i]," ");
        if (direct_declared_data.size()>=2) {
            if (direct_declared_data[0]!="vodka") {
                bool skip=false;
                if (vodka::IndexStartDatatypeReplacement.at(direct_declared_data[0])!=-1) {
                    direct_declared_data.erase(direct_declared_data.begin(),direct_declared_data.begin()+vodka::IndexStartDatatypeReplacement.at(direct_declared_data[0]));
                } else {
                    skip=true;
                }
                for (auto arg:direct_declared_data) {
                    if (arg.substr(0,1)=="#" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                        if (skip==true) {
                            raise(ErrorContainer("vodka.error.line.datatype_replacement.instruction_ineligible : This instruction can't be pre-processed for datatypes replacement process",file.file_source,{maincell.content[i]},{maincell.start.line+i+1},lclstack));
                            return false;
                        }
                        if (vodka::type::vodint::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                            vodka::variables::VodintVariable intele;
                            intele.value=vodka::type::vodint::remove_zero(arg.substr(1,arg.size()-1));
                            if (intele.value=="null") {
                                raise(ErrorContainer("vodka.error.line.datatype_replacement.forbidden_value : Values declared inside function can't be null",file.file_source,{maincell.content[i]},{maincell.start.line+i+1},lclstack));
                                return false;
                            }
                            vodka::variables::VariableContainer contele;
                            vodka::variables::VariableMetadata varinfo;
                            varinfo.is_vodka_constant=true;
                            varinfo.name=arg;
                            varinfo.in_data_section=true;
                            varinfo.algo_dependant=false;
                            varinfo.is_kernel_constant=true;
                            varinfo.uuid=vodka::utilities::genvyid();
                            contele.variable_metadata=varinfo;
                            contele.vodint_element=intele;
                            contele.thing=vodka::variables::VariableDatatype::vodint;
                            variables_dict[arg]=contele;
                            variables_list.push_back(arg);
                            directs_data.push_back(arg);
                        } else {
                            return false;
                        }
                    } else if (arg.substr(0,1)=="%" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                        if (skip==true) {
                            raise(ErrorContainer("vodka.error.line.datatype_replacement.instruction_ineligible : This instruction can't be pre-processed for datatypes replacement process",file.file_source,{maincell.content[i]},{maincell.start.line+i+1},lclstack));
                            return false;
                        }
                        if (vodka::type::vodec::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                            vodka::variables::VodecVariable decele;
                            decele.value=vodka::type::vodec::remove_zero(arg.substr(1,arg.size()-1));
                            if (decele.value=="null") {
                                raise(ErrorContainer("vodka.error.line.datatype_replacement.forbidden_value : Values declared inside function can't be null",file.file_source,{maincell.content[i]},{maincell.start.line+i+1},lclstack));
                                return false;
                            }
                            vodka::variables::VariableContainer contele;
                            vodka::variables::VariableMetadata varinfo;
                            varinfo.algo_dependant=false;
                            varinfo.is_vodka_constant=true;
                            varinfo.in_data_section=true;
                            varinfo.is_kernel_constant=true;
                            varinfo.uuid=vodka::utilities::genvyid();
                            varinfo.name=arg;
                            contele.variable_metadata=varinfo;
                            contele.vodec_element=decele;
                            contele.thing=vodka::variables::VariableDatatype::vodec;
                            variables_dict[arg]=contele;
                            variables_list.push_back(arg);
                            directs_data.push_back(arg);
                        } else {
                            return false;
                        }
                    }
                }
            }
        }
    }
    for (auto cell:cells_list) {
        for (int i=0;i<cell.content.size();++i) {
            vodka::analyser::LineSyntaxChecker localline;
            localline.line_number=cell.start.line+i+1;
            localline.file=file.file_source;
            localline.content=cell.content[i];
            auto direct_declared_data=string_utilities::split(cell.content[i]," ");
            if (direct_declared_data.size()>2) {
                if (direct_declared_data[0]!="vodka") {
                    bool skip=false;
                    if (vodka::IndexStartDatatypeReplacement.at(direct_declared_data[0])!=-1) {
                        direct_declared_data.erase(direct_declared_data.begin(),direct_declared_data.begin()+vodka::IndexStartDatatypeReplacement.at(direct_declared_data[0]));
                    } else {
                        skip=true;
                    }
                    for (auto arg:direct_declared_data) {
                        if (arg.substr(0,1)=="#" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                            if (skip==true) {
                                raise(ErrorContainer("vodka.error.line.datatype_replacement.instruction_ineligible : This instruction can't be pre-processed for datatypes replacement process",file.file_source,{maincell.content[i]},{maincell.start.line+i+1},lclstack));
                                return false;
                            }
                            if (vodka::type::vodint::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                                vodka::variables::VodintVariable intele;
                                intele.value=vodka::type::vodint::remove_zero(arg.substr(1,arg.size()-1));
                                if (intele.value=="null") {
                                    raise(ErrorContainer("vodka.error.line.datatype_replacement.forbidden_value : Values declared inside function can't be null",file.file_source,{maincell.content[i]},{maincell.start.line+i+1},lclstack));
                                    return false;
                                }
                                vodka::variables::VariableContainer contele;
                                vodka::variables::VariableMetadata varinfo;
                                varinfo.algo_dependant=false;
                                varinfo.is_vodka_constant=true;
                                varinfo.in_data_section=true;
                                varinfo.is_kernel_constant=true;
                                varinfo.uuid=vodka::utilities::genvyid();
                                varinfo.name=arg;
                                contele.variable_metadata=varinfo;
                                contele.vodint_element=intele;
                                contele.thing=vodka::variables::VariableDatatype::vodint;
                                variables_dict[arg]=contele;
                                variables_list.push_back(arg);
                                directs_data.push_back(arg);
                            } else {
                                return false;
                            }
                        } else if (arg.substr(0,1)=="%" && find(directs_data.begin(),directs_data.end(),arg)==directs_data.end()) {
                            if (skip==true) {
                                raise(ErrorContainer("vodka.error.line.datatype_replacement.instruction_ineligible : This instruction can't be pre-processed for datatypes replacement process",file.file_source,{maincell.content[i]},{maincell.start.line+i+1},lclstack));
                                return false;
                            }
                            if (vodka::type::vodec::check_value(arg.substr(1,arg.size()-1),localline,lclstack)) {
                                vodka::variables::VodecVariable decele;
                                decele.value=vodka::type::vodec::remove_zero(arg.substr(1,arg.size()-1));
                                if (decele.value=="null") {
                                    raise(ErrorContainer("vodka.error.line.datatype_replacement.forbidden_value : Values declared inside function can't be null",file.file_source,{maincell.content[i]},{maincell.start.line+i+1},lclstack));
                                    return false;
                                }
                                vodka::variables::VariableContainer contele;
                                vodka::variables::VariableMetadata varinfo;
                                varinfo.algo_dependant=false;
                                varinfo.is_vodka_constant=true;
                                varinfo.in_data_section=true;
                                varinfo.is_kernel_constant=true;
                                varinfo.uuid=vodka::utilities::genvyid();
                                varinfo.name=arg;
                                contele.variable_metadata=varinfo;
                                contele.vodec_element=decele;
                                contele.thing=vodka::variables::VariableDatatype::vodec;
                                variables_dict[arg]=contele;
                                variables_list.push_back(arg);
                                directs_data.push_back(arg);
                            } else {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}