
/*----------------------------------------------------------*/

lbl("hex2sym")
    ld(to_acc, SP)
    add(literal) lit(2+1)
    mov(to_ramaddr, frm_acc)
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
    add(literal)    defl("hex2sym_in")
    mov(to_pch, literal)    defh("hex2sym_in")
    mov(to_pc, frm_acc)
_fitpage(66)
lbl("hex2sym_in")
    mov(to_acc, literal)    lit(222)             // 0
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(72)              // 1
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(155)             // 2
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(91)              // 3
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(77)              // 4
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(87)              // 5
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(215)             // 6
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(88)              // 7
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(223)             // 8
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(95)              // 9
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(221)             // A
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(199)             // B
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(150)             // C
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(203)             // D
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(151)             // E
    mov(to_pc, literal)    defl("hex2sym_out")
    mov(to_acc, literal)    lit(149)             // F
    mov(to_pc, literal)    defl("hex2sym_out")
lbl("hex2sym_out")
    ret()



// _fitpage(213)
// lbl("end")





