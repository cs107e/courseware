.attribute arch, "rv64im_zicsr"

.globl _sample
_sample:
    add a3,a4,a5
    xori a1,a1,1
    bnez a2,_sample
    sd ra,40(sp)
    .word 0x0

.globl _rtype
_rtype:
    add    x27,x28,x29
    sub    x3,x4,x5
    xor    x6,x7,x7
    or     x9,x10,x11
    and    x12,x13,x14
    srl    x15,x16,x17
    sra    x18,x19,x20
    sll    x21,x22,x23
    sltu   x24,x25,x26
    .word 0x0

.globl _itype
_itype:
    addi   a4,a5,1
    slli   zero,a2,63
    slti   a5,a6,7
    sltiu  a5,a6,9
    xori   a2,a3,531
    srai   a2,a3,12
    srli   a2,a3,17
    ori    a2,a3,1193
    andi   a2,a4,8
    addi   a4,a5,-1
    .word 0x0

.globl _stype
_stype:
    lh     a0,-1(a1)
    lw     a3,4(a4)
    ld     zero,12(a3)
    sb     a1,1(a0)
    sh     a2,2(a1)
    sw     a3,16(a2)
    sb     a4,0(sp)
    sd     a5,8(s1)
    beq    a3,a2,_stype
    bne    a3,a2,.
    blt    a3,a2,.+10
    bge    a3,a2,.+4
    .word 0x0

.globl _utype
_utype:
    lui    a2,0x70000
    auipc  a3,0x80000
    j      .
    jalr   a1,512
    jalr   zero,512
    jal    _cstart
    jal    a0,.+20
    .word 0x0

.globl _rv64
_rv64:
    ld     zero,2(a3)
    lwu    a4,4(a5)
    sd     a4,6(a6)
    sllw   a0,a1,a2
    slliw  a0,a1,2
    srlw   a2,a3,a4
    srliw  a5,a6,7
    sraw   a0,a1,a2
    sraiw  zero,a3,2
    addw   a4,a5,a4
    addiw  a6,a3,7
    subw   a0,a1,a2
    .word 0x0

.globl _pseudo
_pseudo:
    addi   zero,zero,0
    nop
    ret
    jal     _pseudo

#    li      a0,107
    addi    a0,zero,107

#    mv      a0,a1
    addi    a0,a1,0

#    xori    a0,a1,-1
    not     a0,a1

#    sub     a0,zero,a1
    neg     a0,a1

    beq    a3,zero,.
    bne    a3,zero,.
    blt    a3,zero,.
    bge    a3,zero,.+4
    bltu   a3,zero,.
    bgeu   a3,zero,.-12
    ble    zero,a2,.     # converts to bge a2, zero
    bgt    zero,a2,.+8   # converts to blt a2, zero
    bleu   zero,a2,.-4   # converts to bgeu a2, zero
    bgtu   zero,a2,.-8   # converts to bltu a2, zero
    .word 0x0
