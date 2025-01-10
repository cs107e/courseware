fibonacci:
    addi    a0, zero, 10 # number of terms we want
    addi    a1, zero, 0  # fibonacci number (1st term)
    addi    a2, zero, 1  # fibonacci number (2nd term)
# now, a1 holds fibonacci_0 and
#      a2 holds fibonacci_1
# Now calculate the rest of the terms, which will
# go into a1 and a2 as the algorithm continues
# After 10 iterations, a1 should hold 55 and a2 should hold 89
fib_loop:    
    add     a3, a1, a2 # nth term
    addi    a1, a2, 0  # update first
    addi    a2, a3, 0  # update second
    addi    a0, a0, -1  # decrement number of terms left
    bgt     a0, zero, fib_loop # branch if not finished

