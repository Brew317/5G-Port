function print_symbol_table( FID, X, M, N, SD )
%

%   Copyright 2015 MIT Lincoln Laboratory
%   Author: Michael J. Pitaro

fprintf(FID,'# of symbol patterns = %d\n',M);
fprintf(FID,'# of symbols per pattern = %d\n',N);
fprintf(FID,'pseudo-number generator seed = %08X\n',SD);
fprintf(FID,'\n');
fprintf(FID,'Index     ');
for n=0:N-1
    fprintf(FID,'S%d        ',n);
end
fprintf(FID,'\n');
for m=1:M
    if m-1 < 10
        fprintf(FID,'%d         ',m-1);
    elseif m-1 < 100
        fprintf(FID,'%d        ',m-1);
    else
        fprintf(FID,'%d       ',m-1);
    end
    for n=1:N
        ii=(m-1)*N+n;
        fprintf(FID,'%08X  ',X(ii));
    end
    fprintf(FID,'\n');
end