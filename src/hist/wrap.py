import types
from xml.etree import ElementTree as ET


ti = 0
types
filename = ""
classname = ""

def iterate_type(name, f) :
    global types
    l = f.findall(name)
    for t in l:
        types[t.get("id")] = t.get("name")

def iterate_derived_type(name, pre, post, f):
    global types
    l = f.findall(name)
    for t in l:
        id_ = t.get("id")
        type_ = t.get("type")
        type_str = types.get(type_)
        if(type(type_str) == str):
            types[id_] = pre + type_str + post
    

def type_map(code) :
    global ti
    global types
    global filename
    if(ti == 0) :
        ti = 1
        types = {}
        f = ET.parse(filename)
        iterate_type("FundamentalType", f)
        iterate_type("Typedef", f)
        iterate_type("Class", f)
        iterate_type("Struct", f)

        iterate_derived_type("CvQualifiedType", "const ", "", f)
        iterate_derived_type("ReferenceType", "", "&", f)
        iterate_derived_type("PointerType", "", "*", f)

    out = types.get(code)
    if(type(out) == str) :
        return out
    else:
        return code
        

def class_methods() :
    global filename
    global classname
    f = ET.parse(filename)
    methods_ = f.findall("Method")
    methods = []
    class_scope = classname + "::"
    for m in methods_:
        clf = type_map(m.get("context"))
        if(type(clf) == str):
            if(clf == classname):
                methods.append(m)
    return methods

def arguments(method) :
    return method.findall("Argument")

def get_type(arg) :
    global filename
    return type_map(arg.attrib["type"])

def get_return(meth) :
    global filename
    return type_map(meth.attrib["returns"])

def is_public(method) :
    return (method.attrib.get("access") == "public")

ngeneric = 0
def format_arg(argument) :
    global ngeneric
    argType = get_type(argument)
    argName = argument.attrib.get("name")
    argDefault = argument.attrib.get("default")
    if(type(argName) != str):
        argName = "arg" + str(ngeneric)
        ngeneric += 1

    out = argType + " " + argName
    if type(argDefault) == str:
        out += " = " + argDefault
    return out

'''
def print_method(method) :
    global classname
    to_print = get_return(method) + " " + classname + "::" + method.attrib["name"] + "("
    args = arguments(method)
    if (len(args) == 0) :
        to_print += ")"
    elif (len(args) == 1) :
        to_print += format_arg(args[0]) + ")"
    else :
        for i in range(0, len(args)-1) :
            to_print += format_arg(args[i]) + ", "
        to_print += format_arg(args[len(args)-1]) + ")"
    print to_print
'''

def get_name(arg):
    global ngeneric
    out = arg.get("name")
    if(type(out) == str) :
        return out
    else :
        ngeneric += 1
        return "arg" + str(ngeneric-1)
    

def print_method(method) :
    global classname
    to_print = ""
    static = method.get("static")
    if(type(static) == str):
        if(static == "1"):
            return
    to_print += "/// <a href = \"http://root.cern.ch/root/html/TH1.html#TH1:" + method.get("name") + "\">"
    to_print += "*** TH1 Member Function ***</a>\n"
    to_print += get_return(method) + " " + method.get("name")
    args = arguments(method)
    global ngeneric
    ngeneric = 0
    if (len(args) == 0) :
        to_print += "()"
    elif (len(args) == 1) :
        to_print += "(" + format_arg(args[0]) + ")"
    else :
        to_print += "("
        for i in range(0, len(args)-1) :
            to_print += format_arg(args[i]) + ", "
        to_print += format_arg(args[len(args)-1]) + ")"

    ngeneric = 0
    const = method.get("const")
    if(type(const) == str):
        if(const == "1"):
            to_print += " const"
            const = 1
        else:
            const = 0
    to_print += "\n{\n"
    to_print += "  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);\n"
    to_print += "  return AS_TH1->" + get_name(method) + "("
    if (len(args) == 0) :
        to_print += ");\n}"
    elif (len(args) == 1) :
        to_print += get_name(args[0]) + ");\n}"
    else :
        for i in range(0, len(args)-1) :
            to_print += get_name(args[i]) + ", "
        to_print += get_name(args[len(args)-1]) + ");\n}"

    print to_print


if __name__ == '__main__' :
    global filename
    global classname
    filename = "/Users/gchristian/soft/develop/rootbeer/src/hist/TH1.xml"
    classname = "TH1"
    methods = class_methods()
    print "//! \\file WrapTH1.hxx"
    print "//! \\brief Wraps TH1 member functions."
    print "//! \\details This file provides simple wrappers around the majority of TH1 member functions, "
    print "//! for use in rb::hist::Base derived classes. Each wrapper simply delegates the appropriate "
    print "//! member function to fHistVariant, casted to TH1* using the Cast or ConstCast visitors in"
    print "//! Visitor.hxx"
    print "//!"
    print "//! The file was generated using wrap.py, operating on the XML file TH1.xml, which was"
    print "//! produced by running the program gccxml on the root v5.32/01 version of TH1.h"
    print "//! Subsequently, member functions that we did not want transferred to rb::hist::Base"
    print "//! (or which would not compile) were commented out by hand."
    print "#define AS_TH1 visit::hist::Cast::Do(fHistVariant)\n"
    for method in methods:
        if(is_public(method) and method.get("file") == "f35"):
            print_method(method)
    print "\n#undef AS_TH1"
