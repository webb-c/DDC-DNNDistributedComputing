#
# OMNeT++/OMNEST Makefile for dnn_distributed_computing
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -KINET4_5_PROJ=../../inet4.5 -DINET_IMPORT -I. -I. -I$$\(INET4_5_PROJ\)/src -L$$\(INET4_5_PROJ\)/src -lINET$$\(D\)
#

# Name of target to be created (-o option)
TARGET_DIR = .
TARGET_NAME = dnn_distributed_computing$(D)
TARGET = $(TARGET_NAME)$(EXE_SUFFIX)
TARGET_IMPLIB = $(TARGET_NAME)$(IMPLIB_SUFFIX)
TARGET_IMPDEF = $(TARGET_NAME)$(IMPDEF_SUFFIX)
TARGET_FILES = $(TARGET_DIR)/$(TARGET)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I. -I. -I$(INET4_5_PROJ)/src

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = $(LDFLAG_LIBPATH)$(INET4_5_PROJ)/src  -lINET$(D)

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/ddc/applications/ComputeDNN.o \
    $O/ddc/applications/NodeInformation.o \
    $O/ddc/backlog/PhysicalBacklog.o \
    $O/ddc/backlog/VirtualBacklog.o \
    $O/ddc/communicator/Communicator.o \
    $O/ddc/dnn/DNNConfig.o \
    $O/ddc/dnn/DNNConfigFactory.o \
    $O/ddc/dnn/DNNID.o \
    $O/ddc/dnn/DNNInformation.o \
    $O/ddc/dnn/DNNOutput.o \
    $O/ddc/dnn/DNNRouteID.o \
    $O/ddc/dnn/DNNSublayer.o \
    $O/ddc/dnn/DNNSublayerConfig.o \
    $O/ddc/dnn/DNNSublayerConfigFactory.o \
    $O/ddc/dnn/DNNSublayerFactory.o \
    $O/ddc/dnn/DNNSublayerTable.o \
    $O/ddc/layeredgraph/LayeredGraph.o \
    $O/ddc/layeredgraph/algorithm/AlgorithmBase.o \
    $O/ddc/layeredgraph/algorithm/Dijkstra.o \
    $O/ddc/layernode/LayerNode.o \
    $O/ddc/layernode/LayerNodePair.o \
    $O/ddc/layernode/LayerNodePairPath.o \
    $O/ddc/libs/utils/utils.o \
    $O/ddc/nodes/agent/Agent.o \
    $O/ddc/nodes/controlplane/Controlplane.o \
    $O/ddc/processingunit/ProcessingUnit.o \
    $O/ddc/message/Compute_m.o \
    $O/ddc/message/ComputeFinish_m.o \
    $O/ddc/message/DummyData_m.o \
    $O/ddc/message/RequestArrivalRate_m.o \
    $O/ddc/message/RequestNodeInformation_m.o \
    $O/ddc/message/Reset_m.o \
    $O/ddc/message/ResponseArrivalRate_m.o \
    $O/ddc/message/ResponseNodeInformation_m.o \
    $O/ddc/message/Route_m.o \
    $O/ddc/message/Schedule_m.o

# Message files
MSGFILES = \
    ddc/message/Compute.msg \
    ddc/message/ComputeFinish.msg \
    ddc/message/DummyData.msg \
    ddc/message/RequestArrivalRate.msg \
    ddc/message/RequestNodeInformation.msg \
    ddc/message/Reset.msg \
    ddc/message/ResponseArrivalRate.msg \
    ddc/message/ResponseNodeInformation.msg \
    ddc/message/Route.msg \
    ddc/message/Schedule.msg

# SM files
SMFILES =

# Other makefile variables (-K)
INET4_5_PROJ=../../inet4.5

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
CONFIGFILE = $(shell opp_configfilepath)
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)
ifneq ($(PLATFORM),win32)
LIBS += -Wl,-rpath,$(abspath $(INET4_5_PROJ)/src)
endif

COPTS = $(CFLAGS) $(IMPORT_DEFINES) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE (if COPTS has changed since last build)
# and make the object files depend on it
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
  $(shell $(MKPATH) "$O")
  $(file >$(COPTS_FILE),$(COPTS))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
-include makefrag

#------------------------------------------------------------------------------

# Main target
all: $(TARGET_FILES)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clang-msabi)
	-$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib) 2>/dev/null

$O/$(TARGET_NAME).pdb: $O/$(TARGET)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

# disabling all implicit rules
.SUFFIXES :

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_FILES)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(CLEANALL_COMMAND)
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

help:
	@echo "$$HELP_SYNOPSYS"
	@echo "$$HELP_TARGETS"
	@echo "$$HELP_VARIABLES"
	@echo "$$HELP_EXAMPLES"

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)
