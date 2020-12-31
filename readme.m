%% Symbol Generator
% This code creates a symbol table, analyzes it,
% and write it to the screen.
%
% To execute, run the script
%   run_symbol_table
%
% The default configuration creates a 256x3 symbol table and prints it to
% the screen. It then calculates the peak side lobe level to see if it is a
% reasonable set of symbols.
%
%% Execution Notes
% M*N must be no more than 1024. Keep S and P at 32. Do not adjust PN
% without knowing what you are doing.
%
% When M is 256 and N is 3, it takes approximately 75 seconds to analyze
% the symbol table.
%
% To write the symbol table to a file, use a file ID as the first parameter
% to print_symbol_table.