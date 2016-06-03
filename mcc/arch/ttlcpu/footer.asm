
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
    mov(to_acc, progdata)    data(222)             // 0
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(72)              // 1
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(155)             // 2
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(91)              // 3
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(77)              // 4
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(87)              // 5
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(215)             // 6
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(88)              // 7
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(223)             // 8
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(95)              // 9
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(221)             // A
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(199)             // B
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(150)             // C
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(203)             // D
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(151)             // E
    mov(to_pc, progdata)    defl("hex2sym_out")
    mov(to_acc, progdata)    data(149)             // F
    mov(to_pc, progdata)    defl("hex2sym_out")
lbl("hex2sym_out")
    ret()



// _fitpage(213)
// lbl("end")





