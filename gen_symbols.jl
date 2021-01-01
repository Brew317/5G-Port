# GEN_SYMBOLS Generates symbols for sync words.
#    X = GEN_SYMBOLS(N) generates an N length column vector of 32 bit
#    sync words.
# 
#    1 <= N <= 1024  number of symbols
#    SD is the 32-bit seed. It may not be zero.
# 
#    Note the first symbol generated is the seed. It is discarded. This is
#    useful for continuing the sequence beyond 256 rows. Use the last
#    generated symbol to continue the sequence.
# 
#    The generator polynomial defaults to
#        [32 26 23 22 16 12 11 10 8 7 5 4 2 1 0]
# 
#  Uses comm.PNSequence from the Communications System Toolbox, vrsion 5.6.
# 
#    Copyright 2015 MIT Lincoln Laboratory
#    Author: Michael J. Pitaro
# 
#    Ported from Matlab to Julia
#    Author: Gary A. Garcia


#  SD=hex2dec('00000001'); % seed
#      need to determine SD's datatype in Matlab

# defaults for gen_symbol function call
SD = 1   # seed value
S = 32   # Number of bits per symbol
P = 32   # Order of polynomial

# Normal 0x04C11DB7
PN = [32 26 23 22 16 12 11 10 8 7 5 4 2 1 0]
  
# Account for variable number of arguments in function call

function gen_symbols(N)
    gen_symbols(N, SD, S, PN, P)
end

function gen_symbols(N, SD)
    gen_symbols(N, SD, S, PN, P)
end

function gen_symbols(N, SD, S)
    gen_symbols(N, SD, S, PN, P)
end

function gen_symbols(N, SD, S, PN, P)
    return(N, SD, S, PN, P)
end

