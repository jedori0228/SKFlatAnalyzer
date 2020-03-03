LIBRARY = Analyzers
OBJDIR = obj
DEPDIR = $(OBJDIR)/dep
SRCDIR = src
INCDIR = include
ADDITIONAL_ROOTMAPLIBRARY = -rml libAnalyzerTools.so -rml libGEScaleSyst.so -rml libDataFormats.so -rml libTree.so -rml libjecsys.so

include $(SKFlat_WD)/makefile.common

INCLUDES += -I$(SKFlat_WD)/DataFormats/include/
INCLUDES += -I$(SKFlat_WD)/AnalyzerTools/include/
INCLUDES += -I$(SKFlat_WD)/external/GEScaleSyst/
INCLUDES += -I$(SKFlat_WD)/external/jecsys/

CXXFLAGS += -Wall -Wextra -Wno-overloaded-virtual -Wno-ignored-qualifiers -Wno-unused-parameter -Wno-unused-function
