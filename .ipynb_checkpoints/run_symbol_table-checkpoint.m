% run gen_symbols

%   Copyright 2015 MIT Lincoln Laboratory
%   Author: Michael J. Pitaro

M=256; % number of patterns
N=3; % number of symbols per pattern

S=32; % number of bits per symbol
P=32; % order of PN polynomial

% select seed
% SD=hex2dec('00000001'); % seed
SD=randi(2^P-1);

% generate M*N values
X=gen_symbols(M*N,SD);

% create and print a table
% could open a file and use file ID as first parameter
print_symbol_table(1,X,M,N,SD);

% calculate peak side lobe ratio
% Takes approximately 75 seconds for 256x3
PSLR=check_symbols(X,S);
if PSLR == 0
    fprintf(1,'bad seed!\n');
    fprintf(1,'seed=%08X\n',SD);
else
    fprintf(1,'seed=%08X, pslr=%g dB\n',SD,PSLR);
end