LIBRARY = Analyzers
OBJDIR = obj
DEPDIR = $(OBJDIR)/dep
SRCDIR = src
INCDIR = include
ADDITIONAL_ROOTMAPLIBRARY = -rml libAnalyzerTools.so -rml libGEScaleSyst.so -rml libDataFormats.so -rml libTree.so -rml libCondFormatsJetMETObjects.so

include $(SKFlat_WD)/makefile.common

INCLUDES += -I$(SKFlat_WD)/DataFormats/include/
INCLUDES += -I$(SKFlat_WD)/AnalyzerTools/include/
INCLUDES += -I$(SKFlat_WD)/external/GEScaleSyst/
INCLUDES += -I/cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw-patch/CMSSW_10_4_0_patch1/src/
#INCLUDES += -I/cvmfs/cms.cern.ch/slc6_amd64_gcc630/external/lhapdf/6.2.1-fmblme/include/
