#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "APItool.h"

int main(int argc, char * argv[]) {
 if(argc != 5) {
  printf("Error: invalid parameters\nUsage: %s {project name} {*.h.apitool} {*.h} {*.html}\n", argv[0]);
  exit(-1);
 }
 APItool_AST* as = APItool_parse_file(argv[2]);
 APItool_gen_pair(as, argv[3], argv[4], argv[1]);
 APItool_freeAST(as);
 return 0;
}
