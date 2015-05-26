modes:
"w" - possible write
"r" - possible read, as usual
"a" - set pointer on the end of file, as usual
"t" - clear file (truncate(0) if exist)
"Sn" - set buffer size for size file as n
S.e.:
"wt" - as usual fopen(..., "w")
"waS4096" - appending file and set buffer size as 4096

