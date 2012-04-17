"""@file wrap.py
@brief Generates source files for wrapping TH1 member functions in rb::hist::Base
"""
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
    m_virtual = method.get("virtual")
    virtual = 0
    if(type(m_virtual) == str):
        if(m_virtual == "1"):
            virtual = 1 
    to_print += "/// <a href = \"http://root.cern.ch/root/html/" + classname + ".html#" + classname +":" + method.get("name") + "\">"
    to_print += "*** " + classname + " Member Function ***</a>\n"
    if(virtual):
        to_print += "virtual "
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


'''
/// <a href = "http://root.cern.ch/root/html/TAttLine.html#TAttLine:ResetAttLine">*** TAttLine Member Function ***</a>
virtual void ResetAttLine(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ResetAttLine(option);
}
/// <a href = "http://root.cern.ch/root/html/TAttLine.html#TAttLine:SaveLineAttributes">*** TAttLine Member Function ***</a>
virtual void SaveLineAttributes(ostream& out, const char* name, Int_t coldef = 1, Int_t stydef = 1, Int_t widdef = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SaveLineAttributes(out, name, coldef, stydef, widdef);
}
/// <a href = "http://root.cern.ch/root/html/TAttLine.html#TAttLine:SetLineColor">*** TAttLine Member Function ***</a>
virtual void SetLineColor(Color_t lcolor)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLineColor(lcolor);
} 
/// <a href = "http://root.cern.ch/root/html/TAttLine.html#TAttLine:SetLineStyle">*** TAttLine Member Function ***</a>
virtual void SetLineStyle(Style_t lstyle)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLineStyle(lstyle);
}
/// <a href = "http://root.cern.ch/root/html/TAttLine.html#TAttLine:SetLineWidth">*** TAttLine Member Function ***</a>
virtual void SetLineWidth(Width_t lwidth)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetLineWidth(lwidth);
}
/// <a href = "http://root.cern.ch/root/html/TAttFill.html#TAttFill:ResetAttFill">*** TAttFill Member Function ***</a>
virtual void ResetAttFill(Option_t* option = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ResetAttFill(option);
}
/// <a href = "http://root.cern.ch/root/html/TAttFill.html#TAttFill:SaveFillAttributes">*** TAttFill Member Function ***</a>
virtual void SaveFillAttributes(ostream& out, const char* name, Int_t coldef = 1, Int_t stydef = 1001)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SaveFillAttributes(out, name, coldef, stydef, stydef);
}
/// <a href = "http://root.cern.ch/root/html/TAttFill.html#TAttFill:SetFillColor">*** TAttFill Member Function ***</a>
virtual void SetFillColor(Color_t fcolor)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetFillColor(fcolor);
}
/// <a href = "http://root.cern.ch/root/html/TAttFill.html#TAttFill:SetFillStyle">*** TAttFill Member Function ***</a>
virtual void SetFillStyle(Style_t fstyle)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetFillStyle(fstyle);
}
/// <a href = "http://root.cern.ch/root/html/TAttMarker.html#TAttMarker:ResetAttMarker">*** TAttMarker Member Function ***</a>
virtual void ResetAttMarker(Option_t* toption = "")
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->ResetAttMarker(toption);
}
/// <a href = "http://root.cern.ch/root/html/TAttMarker.html#TAttMarker:SaveMarkerAttributes">*** TAttMarker Member Function ***</a>
virtual void SaveMarkerAttributes(ostream& out, const char* name, Int_t coldef = 1, Int_t stydef = 1, Int_t sizdef = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SaveMarkerttributes(out, name, coldef, stydef, sizdef);
/// <a href = "http://root.cern.ch/root/html/TAttMarker.html#TAttMarker:SetMarkerColor">*** TAttMarker Member Function ***</a>
virtual void SetMarkerColor(Color_t tcolor = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetMarkerColor(tcolor);
}
/// <a href = "http://root.cern.ch/root/html/TAttMarker.html#TAttMarker:SetMarkerSize">*** TAttMarker Member Function ***</a>
virtual void SetMarkerSize(Size_t msize = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetMarkerSize(msize);
}
/// <a href = "http://root.cern.ch/root/html/TAttMarker.html#TAttMarker:SetMarkerStyle">*** TAttMarker Member Function ***</a>
virtual void SetMarkerStyle(Style_t mstyle = 1)
{
  rb::ScopedLock<rb::Mutex> LOCK (TTHREAD_GLOBAL_MUTEX);
  return AS_TH1->SetMarkerStyle(mstyle);
}
'''
