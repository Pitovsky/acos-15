#include "hash.h"
#include <stdio.h>


int main(){
node** T = create(2);
insert1("kello", T, 2);
insert1 ("hey", T, 2);
show_node("hey", T, 2);
delete("hey",T,2);
show_node("hey", T,2);
return 0;}
