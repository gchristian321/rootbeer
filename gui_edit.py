import sys

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print "usage: python gui_edit.py filename\n"
        exit(1)

    fnamein = sys.argv[1]
    fbase = fnamein[0:fnamein.rfind(".")]
    fnameheader = ""
    fnamesource = ""
    if fbase == "src/GuiLayout":
        fnameheader = "Gui.hxx"
        fnamesource = "Gui.cxx"
    else:
        fnameheader = fbase + "_edit.hxx"
        fnamesource = fbase + "_edit.cxx"

    fin = open(fnamein, 'r')
    if fnameheader == "Gui.hxx":
        fheader = open("src/"+fnameheader, 'w')
        fsource = open("src/"+fnamesource, 'w')
    else:
        fheader = open(fnameheader, 'w')
        fsource = open(fnamesource, 'w')

    fincguard = fnameheader[0:fnameheader.rfind(".")].upper() + "_HXX"
    fheader.write("#ifndef " + fincguard + "\n#define " + fincguard + "\n")
    fheader.write("#include \"Rint.hxx\"\n\n\n")

    decl = []
    for line in fin.readlines():
        if(line.find(" = new") != -1):
            tok = line.split()
            srcout = "   /* " + tok[0] + "* */ " + tok[1][1:]
            for i in range(2, len(tok)):
                srcout += " " + tok[i]
            if(tok[0] == "TGTextButton"):
                srcout += "\n" + tok[1][1:] + "->SetFont(ufont->GetFontStruct());\n"
            headerout = "extern " + tok[0] + " " +tok[1] + ";"
            if(line.find("\"") != -1):
                headerout += " // " + line[line.find("\""):(line.find("\"", line.find("\"")+1)+1)]
            headerout += "\n"
            fheader.write(headerout)
            fsource.write(srcout)
            decl.append(tok[0] + " " +tok[1] + " = 0;")

        elif (line == "}  \n"):
            fsource.write("\n   // Call MakeConnections function, defined in " + fnameheader + "\n")
            fsource.write("   MakeConnections(); \n" + line)

        elif (line[0] == "#"):
            fheader.write(line)
            if(line == "#include \"Riostream.h\"\n"):
                inc = "#include \"" + fnameheader + "\"\n"
                fsource.write(inc)
                fheader.write("\n")
          
        else:
            fsource.write(line)

    # fheader.write("\n\n// ====== Functions imported from gui_wrap.hxx ====== //\n")
    # fwrap = open("gui_wrappers.hxx", 'r')
    # for line in fwrap.readlines():
    #     fheader.write(line)
    # fwrap.close()
    fsource.write("\n")
    for d in decl:
        fsource.write(d)
    fsource.write("\n// ================================================== //\n\n")
    fconnect = open("src/MakeConnections.hxx", 'r')
    for line in fconnect:
        fsource.write(line)
    fconnect.close()
    fheader.write("\nextern void GuiLayout();\n")
    fheader.write("\nextern void MakeConnections();\n")
    fheader.write("\n\n\n\n#endif\n")
