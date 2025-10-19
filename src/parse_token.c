static APItool_token* S_parse_token(void) {
  APItool_token* returnv = malloc(sizeof(APItool_token));
  APItool_token_value val;
  fgets(line, sizeof(line), fr);
  if(strcmp(line, "? function\n") == 0) {
    lnum++;
    val.func = S_parse_function();
    returnv->tt = APItool_FUNCTION;
    returnv->token = val;
    APItool_Function_docs* doc = malloc(sizeof(APItool_Function_docs));
    val.func->doc = doc;
    doc->description = NULL;
    doc->returns = NULL;
    doc->argsarray_docs = calloc(val.func->argsarray_len, sizeof(char*)); // all NULL
    char* errmsg = "invalid function documentation";
    while(fpeek(fr) != '\n') {
     _asrt(getc(fr) == '?', errmsg);
     _asrt(getc(fr) == ' ', errmsg);
     switch(getc(fr)) {
      case 'd':
       S_nom_nom_nom("escription: ", errmsg);
       char* des = malloc(1000);
       doc->description = S_read_till_nl(des);
       break;
      case 'r':
       S_nom_nom_nom("eturns: ", errmsg);
       char* ret = malloc(500);
       doc->returns = S_read_till_nl(ret);
       break;
      case 'a':
       S_nom_nom_nom("rg ", errmsg);
       char* argname = S_parse_symbol();
       _asrt(getc(fr) == ':', errmsg);
       _asrt(getc(fr) == ' ', errmsg);
       char* arg = malloc(500);
       doc->argsarray_docs[S_search_argsarray(val.func, argname, errmsg)] = S_read_till_nl(arg);
       break;
      default: _asrt(false, errmsg);
     }
     lnum++;
    }
  } else if(strcmp(line, "? variable\n") == 0) {
   lnum++;
   val.var = S_parse_variable();
   returnv->tt = APItool_VARIABLE;
   returnv->token = val;
   val.var->doc_description = NULL;
   char* errmsg = "invalid variable documentation";
   while(fpeek(fr) != '\n') {
    _asrt(getc(fr) == '?', errmsg);
    _asrt(getc(fr) == ' ', errmsg);
    if(getc(fr) == 'd') {
     S_nom_nom_nom("escription: ", errmsg);
     char* des = malloc(1000);
     val.var->doc_description = S_read_till_nl(des);
    } else { _asrt(false, errmsg); }
    lnum++;
   }
  } else {
   bool isstruct = strcmp(line, "? struct\n") == 0;
   bool isunion = (isstruct) ? false : (strcmp(line, "? union\n") == 0);
   if(isstruct || isunion) {
    lnum++;
    val.sun = S_parse_struct_union(isunion);
    returnv->tt = APItool_STRUCT_UNION;
    returnv->token = val;
    char* errmsg = "inalid struct/union documentation";
    APItool_Struct_Union_Enum_docs* dc = malloc(sizeof(APItool_Struct_Union_Enum_docs));
    val.sun->doc = dc;
    dc->description = NULL;
    dc->typesarray_docs = calloc(val.sun->typesarray_len, sizeof(char*));
    while(fpeek(fr) != '\n') {
     _asrt(getc(fr) == '?', errmsg);
     _asrt(getc(fr) == ' ', errmsg);
     switch(getc(fr)) {
      case 'd':
       S_nom_nom_nom("escription: ", errmsg);
       char* des = malloc(1000);
       dc->description = S_read_till_nl(des);
       break;
      case 'i':
       S_nom_nom_nom("tem ", errmsg);
       char* itm = S_parse_symbol();
       _asrt(getc(fr) == ':', errmsg);
       _asrt(getc(fr) == ' ', errmsg);
       char* idoc = malloc(500);
       dc->typesarray_docs[S_search_typesarray(val.sun, itm, errmsg)] = S_read_till_nl(idoc);
       break;
      default: _asrt(false, errmsg);
     }
     lnum++;
    }
   } else if(strcmp(line, "? enum\n") == 0) {
    lnum++;
    val.enm = S_parse_enum();
    returnv->tt = APItool_ENUM;
    returnv->token = val;
    char* errmsg = "invalid enum documentation";
    APItool_Struct_Union_Enum_docs* dc = malloc(sizeof(APItool_Struct_Union_Enum_docs));
    val.enm->doc = dc;
    dc->description = NULL;
    dc->typesarray_docs = calloc(val.enm->array_len, sizeof(char*));
    while(fpeek(fr) != '\n') {
     _asrt(getc(fr) == '?', errmsg);
     _asrt(getc(fr) == ' ', errmsg);
     switch(getc(fr)) {
      case 'd':
       S_nom_nom_nom("escription: ", errmsg);
       char* des = malloc(1000);
       dc->description = S_read_till_nl(des);
       break;
      case 'f':
       S_nom_nom_nom("ield ", errmsg);
       char* itm = S_parse_symbol();
       _asrt(getc(fr) == ':', errmsg);
       _asrt(getc(fr) == ' ', errmsg);
       char* idoc = malloc(500);
       dc->typesarray_docs[S_search_array(val.enm, itm, errmsg)] = S_read_till_nl(idoc);
       break;
      default: _asrt(false, errmsg);
     }
     lnum++;
    }
   } else {
     _asrt(false, "invalid top-level token");
   }
  }
  return returnv;
}
