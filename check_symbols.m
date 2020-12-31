function [ R ] = check_symbols( varargin )
%CHECK_SYMBOLS Summary of this function goes here
%   Detailed explanation goes here

%% process input arguments

S=32; % number of bits per symbol

switch nargin
    case 1,
        X=varargin{1};
    case 2,
        X=varargin{1};
        SD=varargin{2};
    otherwise,
        error('unrecognized number of input arguments');
end

% check inputs
if (S~=uint32(S)) || (S > 32)
    error('S must be a positive integer <= 32')
end

%% Initial conditions
R=0;

N=length(X);

T=zeros(S,N);
psl=zeros(N,N);

for n=1:N
    qq=dec2bin(X(n),S);
    a=zeros(S,1);
    a(find(qq=='1'))=1;
    a(find(a==0))=-1;
%     fprintf(1,'%d: %08X\n',n,X(n));
    T(:,n)=a;
end

for n1=1:N-1
    for n2=(n1+1):N
        c_ab=ccf(T(:,n1),T(:,n2));
        psl(n1,n2)=max(abs(c_ab));
        if psl(n1,n2)==S
            warning('peak side lobe is too high')
            return
        end
        psl(n2,n1)=psl(n1,n2);
%         fprintf(1,'(%d,%d): %08X %08X %d\n', ...
%             n1,n2,X(n1),X(n2),psl(n1,n2));
    end
end

psl_max=max(max(psl));
R=-20*log10(S-psl_max);
% fprintf(1,'psl_max=%d, pslr=%g dB\n',psl_max,R);

%%
end