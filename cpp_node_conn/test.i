%module scheduler

%{
//#include "../scheduler/Model/Observation.h"
#include "../scheduler/Model/Target.h"
//#include "../scheduler/Model/Request.h"
//#include "../scheduler/Model/Obsconditions.h"
//#include "/home/pfe/dev/Cpp/Libraries/libnova-0.15.0/src/libnova/libnova.h"
//#include "../scheduler/Model/targettest.h"
%}

/* Let's just grab the original header file here */
//#include "../scheduler/Model/Observation.h"
%include "../scheduler/Model/Target.h"
//#include "../scheduler/Model/Request.h"
//%include "../scheduler/Model/Obsconditions.h"
//%include "/home/pfe/dev/Cpp/Libraries/libnova-0.15.0/src/libnova/libnova.h"
//%include "../scheduler/Model/targettest.h"