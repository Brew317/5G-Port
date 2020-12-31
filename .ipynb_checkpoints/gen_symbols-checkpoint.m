function [ X ] = gen_symbols( varargin )
%GEN_SYMBOLS Generates symbols for sync words.
%   X = GEN_SYMBOLS(N) generates an N length column vector of 32 bit
%   sync words.
%
%   1 <= N <= 1024  number of symbols
%   SD is the 32-bit seed. It may not be zero.
%
%   Note the first symbol generated is the seed. It is discarded. This is
%   useful for continuing the sequence beyond 256 rows. Use the last
%   generated symbol to continue the sequence.
%
%   The generator polynomial defaults to
%       [32 26 23 22 16 12 11 10 8 7 5 4 2 1 0]
%
% Uses comm.PNSequence from the Communications System Toolbox, vrsion 5.6.

%   Copyright 2015 MIT Lincoln Laboratory
%   Author: Michael J. Pitaro

%% process input arguments

SD=hex2dec('00000001'); % seed

S=32; % number of bits per symbol

% normal 0x04C11DB7
PN=[32 26 23 22 16 12 11 10 8 7 5 4 2 1 0];

P=32; % order of polynomial

switch nargin
    case 1,
        N=varargin{1};
    case 2,
        N=varargin{1};
        SD=varargin{2};
    case 3,
        N=varargin{1};
        SD=varargin{2};
        S=varargin{3};
    case 5,
        N=varargin{1};
        SD=varargin{2};
        S=varargin{3};
        PN=varargin{4};
        P=varargin{5};
    otherwise,
        error('unrecognized number of input arguments');
end

% Check inputs are positive integers.
if (N~=uint32(N)) || (N > 1024)
    error('N must be a positive integer <= 1024')
end
if SD~=uint32(SD) || (SD == 0)
    error(sprintf('SD must be a positive integer <= %d',intmax('uint32')))
end
if (S~=uint32(S)) || (S > 32)
    error('S must be a positive integer <= 32')
end
if (P~=uint32(P)) || (P > 32)
    error('P must be a positive integer <= 32')
end

% No error checking on PN yet. Add it later.

% If F is true, discard first generated symbol.
% It is just the bit reversal of the seed.
F='true';

% reverse the bit order of the seed
tmp=dec2bin(SD,P);
SD_rev=bin2dec(tmp(P:-1:1)); % reversed seed
clear tmp

% generate initial condition from seed
qq=dec2bin(SD_rev,P);
IC=zeros(P,1);
IC(find(qq=='1'))=1;

% create the pn generator object
hpn = comm.PNSequence('Polynomial',PN, ...
    'SamplesPerFrame', S, 'InitialConditions',IC);

if F
    % discard first symbol
    v=step(hpn);
end

% preallocate symbol table
X=zeros(N);

% weight of each bit
w=2.^((S-1):-1:0)';

% generate all the symbols
for n=1:N,
    % generate next symbol
    v=step(hpn);
    % convert to decimal
    X(n)=sum(v.*w);
end