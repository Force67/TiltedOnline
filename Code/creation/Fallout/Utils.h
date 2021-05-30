#pragma once

#if defined(TP_SKYRIM)
#define POINTER_SKYRIMSE(className, variableName, ...) static AutoPtr<className> variableName(__VA_ARGS__)
#else
#define POINTER_SKYRIMSE(className, variableName, ...) ;
#endif

#if defined(TP_FALLOUT)
#define POINTER_FALLOUT4(className, variableName, ...) static AutoPtr<className> variableName(__VA_ARGS__)
#else
#define POINTER_FALLOUT4(className, variableName, ...) ;
#endif

