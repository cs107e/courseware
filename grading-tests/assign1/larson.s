# larson.s
# --------
# 4-LED larson scanner written in asm
# connect leds to gpios PB0, PB1, PB2, PB3
#
# Author: Julie Zelenski <zelenski@cs.stanford.edu>


# B bank config reg @ 0x2000030, 4 bits per pin, output = 0b0001
# B bank data reg   @ 0x2000040, 1 bit per pin

    lui     a0,0x2000       # a0 holds const base addr PB peripheral = 0x2000000
    lui     a1, 0x1         # a1 = 0x1000
    addi    a1,a1,0x111     # a1 = 0x1111
    sw      a1,0x30(a0)     # config all of PB0-PB3 as output
    addi    a3,zero,1       # a3 = PB set pin, start at 1, will left/right shift to increment

larson:
forward:
    sw      a3,0x40(a0)         # set (a3 single bit, will clear others)

    lui     a2,0x1f00
wait_fw:
    addi    a2,a2,-1
    bne     a2,zero,wait_fw

    slli    a3,a3,1             # left shift a3 move from PBN to PBN+1
    addi    a4,zero,8
    blt     a3,a4,forward       # forward while a3 < 8 (PB0 up to PB2)

backward:
    sw      a3,0x40(a0) # set

    lui     a2,0x1f00
wait_bw:
    addi    a2,a2,-1
    bne     a2,zero,wait_bw

    srli    a3,a3,1             # right shift a3 move from PBN to PBN-1
    addi    a4,zero,1
    bgt     a3,a4,backward      # backward while a3 > 0 (PB3 down to PB1)

    j larson
