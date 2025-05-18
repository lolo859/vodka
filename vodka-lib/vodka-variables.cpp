#include "vodka-lib.h"
#include <string>
using namespace std;
//* Convert VariableDatatype object to string
string vodka::variables::datatype_to_string(VariableDatatype datatype) {
    switch (datatype) {
        case VariableDatatype::vodint:return "vodint";
        case VariableDatatype::vodec:return "vodec";
        case VariableDatatype::vodarg:return "vodarg";
        default:return "Unknown";
    }
}