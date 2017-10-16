#include "stdutil.hpp"

#ifndef NO_SYSTEM_STREAM_STDUTIL
fdbuf sib(STDIN_FILENO);
fdbuf sob(STDOUT_FILENO);
fdbuf seb(STDERR_FILENO);

istream<fdbuf> sis(sib);
ostream<fdbuf> sos(sob);
ostream<fdbuf> ses(seb);
#endif
