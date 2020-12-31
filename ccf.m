function [ C ] = ccf( A, B )

N=length(A);

k=(-(N-1):N-1)';
C=zeros(size(k));
for v=1:length(k)
    C(v)=sum(A(1:N-abs(k(v))).*B(1+abs(k(v)):N));
end