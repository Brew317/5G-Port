"""
 GEN_SYMBOLS Generates symbols for sync words.
    X = GEN_SYMBOLS(N) generates an N length column vector of 32 bit
    sync words.
 
    1 <= N <= 1024  number of symbols
    SD is the 32-bit seed. It may not be zero.
 
    Note the first symbol generated is the seed. It is discarded. This is
    useful for continuing the sequence beyond 256 rows. Use the last
    generated symbol to continue the sequence.
 
    The generator polynomial defaults to
        [32 26 23 22 16 12 11 10 8 7 5 4 2 1 0]
 
  Uses comm.PNSequence from the Communications System Toolbox, vrsion 5.6.
 
    Copyright 2015 MIT Lincoln Laboratory
    Author: Michael J. Pitaro
 
    Ported from Matlab to Julia
    Author: Gary A. Garcia
"""

#  SD=hex2dec('00000001'); % seed
#      need to determine SD's datatype in Matlab


module G_symbols 
export gen_symbols

# defaults for gen_symbol function call
SD = 1   # seed value in UInt32
S = 32   # Number of bits per symbol
P = 32   # Order of polynomial

# Normal 0x04C11DB7
PN = [32 26 23 22 16 12 11 10 8 7 5 4 2 1 0]
  
# Account for variable number of arguments in function call


function gen_symbols end

function gen_symbols(N)
    gen_symbols(UInt32(N), UInt32(SD), UInt32(S), PN, UInt32(P))
end

function gen_symbols(N, SD)
    gen_symbols(UInt32(N), UInt32(SD), UInt32(S), PN, UInt32(P))
end

function gen_symbols(N, SD, S)
    gen_symbols(UInt32(N), UInt32(SD), UInt32(S), PN, UInt32(P))
end

# No error checking yet, Julia does not support Int32 well
# % Check inputs are positive integers.
# if (N~=uint32(N)) || (N > 1024)
#     error('N must be a positive integer <= 1024')
# end
# if SD~=uint32(SD) || (SD == 0)
#     error(sprintf('SD must be a positive integer <= %d',intmax('uint32')))
# end
# if (S~=uint32(S)) || (S > 32)
#     error('S must be a positive integer <= 32')
# end
# if (P~=uint32(P)) || (P > 32)
#     error('P must be a positive integer <= 32')
# end
# 
# % No error checking on PN yet. Add it later.


"""
### GEN_SYMBOLS Generates symbols for sync words.  
    X = GEN_SYMBOLS(N) generates an N length column vector of 32 bit  
    sync words.  
 
    1 <= N <= 1024  number of symbols  
    SD is the 32-bit seed. It may not be zero.  
 
    Note the first symbol generated is the seed. It is discarded. This is  
    useful for continuing the sequence beyond 256 rows. Use the last  
    generated symbol to continue the sequence.  
 
    The generator polynomial defaults to  
        [32 26 23 22 16 12 11 10 8 7 5 4 2 1 0]  
"""
function gen_symbols(N::UInt32, SD::UInt32, S::UInt32, PN, P::UInt32)
    return(Int64(N), Int64(SD), Int64(S), PN, Int64(P))
end

end # module