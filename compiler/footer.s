
/*----------------------------------------------------------*/

lbl("hex2sym")
    ld(to_acc, SP)
    add(progdata) data(2+1)
    mov(to_mar, frm_acc)
    mov(to_acc, frm_ram)
    shl()
    shl()
    shl()
    shl()
    rol()
    rol()
    rol()
    rol()
    shl()  // mul * 2
    shl()  // mul * 2
    add(progdata)    defl("hex2sym_in")
    mov(to_pch, progdata)    defh("hex2sym_in")
    mov(to_pc, frm_acc)
_fitpage(66)
lbl("hex2sym_in")
    mov(to_acc, progdata)    data(0xDE)            // 0
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0x48)            // 1
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0x9B)            // 2
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0x5B)            // 3
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0x4D)            // 4
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0x57)            // 5
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0xD7)            // 6
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0x58)            // 7
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0xDF)            // 8
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0x5F)            // 9
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0xDD)            // A
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0xC7)            // B
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0x96)            // C
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0xCB)            // D
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0x97)            // E
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(0x95)            // F
    mov(to_pc, progdata)    defl("hex2sym_out")
lbl("hex2sym_out")
    ret()

