#ifdef RB_EXTERN
#define RB_IMPORT_DATA(CLASS, SYMBOL, NAME, VISIBLE, ARGS)	\
  extern rb::TData<CLASS> * SYMBOL;
#undef RB_EXTERN

#elif defined RB_INIT
#define RB_IMPORT_DATA(CLASS, SYMBOL, NAME, VISIBLE, ARGS)	\
  rb::TData<CLASS> * SYMBOL = 0;
#undef RB_INIT

#elif defined RB_ALLOCATE
#define RB_IMPORT_DATA(CLASS, SYMBOL, NAME, VISIBLE, ARGS)	\
  SYMBOL = new rb::TData<CLASS> (NAME, VISIBLE, ARGS);
#undef RB_ALLOCATE

#elif defined RB_DEALLOCATE
#define RB_IMPORT_DATA(CLASS, SYMBOL, NAME, VISIBLE, ARGS)	\
  delete SYMBOL;
#undef RB_DEALLOCATE

#else
#error "Improper inclusion of ImportData.h"

#endif



#include "../user/ImportData.h"












#undef RB_IMPORT_DATA
