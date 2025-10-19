static char* S_strip_backtick(char* src) {
 char* returnv = malloc(strlen(src)+1); // we won't use the whole buffer
 char* mut = returnv;
 int i;
 for(i = 0; i < (int)strlen(src); i++) {
  if(src[i] == '`') {
   i++;
   if(src[i] != '`') {
    *mut++ = '`';
    *mut++ = src[i];
   }
  } else {
   *mut++ = src[i];
  }
 }
 *mut++ = 0;
 return returnv;
}

int S_where_in_ast(APItool_AST* ctx, char* name) {
 for(int i = 0; i < ctx->numtokens; i++) {
  APItool_token* t = ctx->val[i];
  switch(t->tt) {
   case APItool_FUNCTION:;
    if(strcmp((t->token.func)->funcname,name) == 0) { return i; }
    break;
   case APItool_VARIABLE:;
    if(strcmp((t->token.var)->varname,name) == 0) { return i; }
    break;
   case APItool_STRUCT_UNION:;
    if(strcmp((t->token.sun)->sunname,name) == 0) { return i; }
    break;
   case APItool_ENUM:;
    if(strcmp((t->token.enm)->enumname,name) == 0) { return i; }
    break;
   default: break;
  }
 }
 return -1;
}

static char* S_type_linker(char* src, APItool_AST* context) {
 char* returnv = malloc(strlen(src)+20);
 char* mut = returnv;
 int siz = strlen(src)+20;
 (void)context;
 int i;
 for(i = 0; i < (int)strlen(src); i++) {
  if(src[i] == '`') {
   i++;
   if(src[i] != '`') {
    *mut++ = '`';
    *mut++ = src[i];
   } else {
    int j = i;
    for(;; i++) {
     if(src[i] == '`') {
      i++;
      if(src[i] == '`') {
       break;
      }
     }
    }
    int diff = (i-2)-j;

    siz += diff;
    int diff_alloc = mut - returnv;
    returnv = realloc(returnv, siz);
    mut = returnv+diff_alloc;

    char* inside = malloc(diff);
    strncpy(inside, src+j+1, diff);
    inside[diff] = 0;
    mut += sprintf(mut, "[%s]<u>(Section %d)</u>", inside, S_where_in_ast(context, inside));
    free(inside);
   }
  } else {
   *mut++ = src[i];
  }
 }
 *mut++ = 0;
 return returnv;
}
