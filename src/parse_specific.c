static APItool_Function* S_parse_function(void) {
  char* errmsg = "invalid function structure";
  APItool_Function* returnv = malloc(sizeof(APItool_Function));
  
  returnv->returntype = S_parse_type();
  _asrt(getc(fr) == ' ', errmsg);
  returnv->funcname = S_parse_symbol();
  _asrt(getc(fr) == '(', errmsg);
  returnv->argsarray_names = malloc(sizeof(char*)*20);
  returnv->argsarray_types = malloc(sizeof(char*)*20);
  returnv->argsarray_len = 0;
  if(fpeek(fr) != ')') {
    for(int i = 0 ;; i++) {
      returnv->argsarray_types[i] = S_parse_type();
      _asrt(getc(fr) == ' ', errmsg);
      returnv->argsarray_names[i] = S_parse_symbol();
      returnv->argsarray_len++;
      if(fpeek(fr) == ')') { break; }
      _asrt(getc(fr) == ',', errmsg);
      _asrt(getc(fr) == ' ', errmsg);
    }
  }
  getc(fr);
  _asrt(getc(fr) == ';', errmsg);
  _asrt(getc(fr) == '\n', errmsg);
  lnum++;

  return returnv;
}

static APItool_Variable* S_parse_variable(void) {
 char* errmsg = "invalid variable structure";
 APItool_Variable* returnv = malloc(sizeof(APItool_Variable));

 returnv->type = S_parse_type();
 _asrt(getc(fr) == ' ', errmsg);
 returnv->varname = S_parse_symbol();
 _asrt(getc(fr) == ';', errmsg);
 _asrt(getc(fr) == '\n', errmsg);
 lnum++;

 return returnv;
}

static APItool_Struct_Union* S_parse_struct_union(bool isunion) {
 char errmsg[25]; // allocate on a stack, because it's not persistently needed
 snprintf(errmsg, 25, "invalid %s structure", isunion ? "union" : "struct");
 APItool_Struct_Union* returnv = malloc(sizeof(APItool_Struct_Union));

 char* namee;
 returnv->isunion = isunion;
 bool typedefed = false;
 if(fpeek(fr) == 't') {
  S_nom_nom_nom("typedef ", errmsg);
  typedefed = true;
 }
 if(fpeek(fr) == 's' && !isunion) {
  S_nom_nom_nom("struct ", errmsg);
 } else if(fpeek(fr) == 'u' && isunion) {
  S_nom_nom_nom("union ", errmsg);
 } else {
  _asrt(false, errmsg);
 }
 if(fpeek(fr) != '{') {
  returnv->marked = typedefed ? APItool_TYPEDEF_IMMEDIATE : APItool_IMMEDIATE;
  namee = S_parse_symbol();
  _asrt(getc(fr) == ' ', errmsg);
  _asrt(getc(fr) == '{', errmsg);
 } else {
  getc(fr);
  _asrt(typedefed, errmsg);
  returnv->marked = APItool_TYPEDEF;
 }
 _asrt(getc(fr) == '\n', errmsg);
 lnum++;

 char** nams = malloc(sizeof(char*) * 20);
 char** typs = malloc(sizeof(char*) * 20);
 int siz = 20;
 int i;
 for(i = 0; fpeek(fr) != '}'; i++) {
  _asrt(getc(fr) == ' ', errmsg); // sorry about forced indent, it's like fucking python
  if(i >= siz) {
   siz += 20;
   nams = realloc(nams, sizeof(char*) * siz);
   typs = realloc(typs, sizeof(char*) * siz);
  }
  typs[i] = S_parse_type();
  _asrt(getc(fr) == ' ', errmsg);
  nams[i] = S_parse_symbol();
  _asrt(getc(fr) == ';', errmsg);
  _asrt(getc(fr) == '\n', errmsg);
  lnum++;
 }
 getc(fr);
 returnv->typesarray_names = nams; // it's the final value, all reallocs have been realloc'd
 returnv->typesarray_types = typs; // same here
 returnv->typesarray_len = i;
 if(typedefed) {
  _asrt(getc(fr) == ' ', errmsg);
  namee = S_parse_symbol();
 }
 _asrt(getc(fr) == ';', errmsg);
 _asrt(getc(fr) == '\n', errmsg);
 returnv->sunname = namee;

 return returnv;
}

static APItool_Enum* S_parse_enum(void) {
 char* errmsg = "invalid enum structure";
 APItool_Enum* returnv = malloc(sizeof(APItool_Enum));

 char* namee;
 bool typedefed = false;
 if(fpeek(fr) == 't') {
  S_nom_nom_nom("typedef ", errmsg);
  typedefed = true;
 }
 S_nom_nom_nom("enum ", errmsg);
 if(fpeek(fr) != '{') {
  returnv->marked = typedefed ? APItool_TYPEDEF_IMMEDIATE : APItool_IMMEDIATE;
  namee = S_parse_symbol();
  _asrt(getc(fr) == ' ', errmsg);
  _asrt(getc(fr) == '{', errmsg);
 } else {
  getc(fr);
  _asrt(typedefed, errmsg);
  returnv->marked = APItool_TYPEDEF;
 }
 _asrt(getc(fr) == '\n', errmsg);
 lnum++;

 char** nams = malloc(sizeof(char*) * 20);
 int siz = 20;
 int i;
 for(i = 0 ;; i++) {
  _asrt(getc(fr) == ' ', errmsg);
  if(i >= siz) {
   siz += 20;
   nams = realloc(nams, sizeof(char*) * siz);
  }
  nams[i] = S_parse_symbol();
  if(fpeek(fr) == '\n') {
   i++;
   lnum++;
   getc(fr);
   _asrt(getc(fr) == '}', errmsg);
   break;
  }
  _asrt(getc(fr) == ',', errmsg);
  _asrt(getc(fr) == '\n', errmsg);
  lnum++;
 }
 returnv->array_names = nams;
 returnv->array_len = i;
 if(typedefed) {
  _asrt(getc(fr) == ' ', errmsg);
  namee = S_parse_symbol();
 }
 _asrt(getc(fr) == ';', errmsg);
 _asrt(getc(fr) == '\n', errmsg);
 returnv->enumname = namee;

 return returnv;
}
