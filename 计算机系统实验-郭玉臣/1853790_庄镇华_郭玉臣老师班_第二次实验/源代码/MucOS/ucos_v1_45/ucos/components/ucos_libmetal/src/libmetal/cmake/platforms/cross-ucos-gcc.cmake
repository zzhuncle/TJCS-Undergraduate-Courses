# cmake 3.3.2 does not know CMAKE_SYSTEM_NAME=uCOS, we set it to Generic
include (cross-generic-gcc)
string (TOLOWER "uCOS"                PROJECT_SYSTEM)
string (TOUPPER "uCOS"                PROJECT_SYSTEM_UPPER)

# vim: expandtab:ts=2:sw=2:smartindent
