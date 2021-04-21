#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-PIC18F26J53_PRO.mk)" "nbproject/Makefile-local-PIC18F26J53_PRO.mk"
include nbproject/Makefile-local-PIC18F26J53_PRO.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PIC18F26J53_PRO
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PIC18FX6J53.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PIC18FX6J53.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../../../USB/USB.c ../../../../USB/usb_msd.c ../Shared_Files/flash.c ../Shared_Files/usb_app.c ../Shared_Files/usb_descriptors.c ../Shared_Files/usb_scsi_inq.c ../Shared_Files/main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/671494620/USB.p1 ${OBJECTDIR}/_ext/671494620/usb_msd.p1 ${OBJECTDIR}/_ext/1662128652/flash.p1 ${OBJECTDIR}/_ext/1662128652/usb_app.p1 ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1 ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1 ${OBJECTDIR}/_ext/1662128652/main.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/671494620/USB.p1.d ${OBJECTDIR}/_ext/671494620/usb_msd.p1.d ${OBJECTDIR}/_ext/1662128652/flash.p1.d ${OBJECTDIR}/_ext/1662128652/usb_app.p1.d ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1.d ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1.d ${OBJECTDIR}/_ext/1662128652/main.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/671494620/USB.p1 ${OBJECTDIR}/_ext/671494620/usb_msd.p1 ${OBJECTDIR}/_ext/1662128652/flash.p1 ${OBJECTDIR}/_ext/1662128652/usb_app.p1 ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1 ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1 ${OBJECTDIR}/_ext/1662128652/main.p1

# Source Files
SOURCEFILES=../../../../USB/USB.c ../../../../USB/usb_msd.c ../Shared_Files/flash.c ../Shared_Files/usb_app.c ../Shared_Files/usb_descriptors.c ../Shared_Files/usb_scsi_inq.c ../Shared_Files/main.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-PIC18F26J53_PRO.mk dist/${CND_CONF}/${IMAGE_TYPE}/PIC18FX6J53.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F26J53
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/671494620/USB.p1: ../../../../USB/USB.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/671494620" 
	@${RM} ${OBJECTDIR}/_ext/671494620/USB.p1.d 
	@${RM} ${OBJECTDIR}/_ext/671494620/USB.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/671494620/USB.p1 ../../../../USB/USB.c 
	@-${MV} ${OBJECTDIR}/_ext/671494620/USB.d ${OBJECTDIR}/_ext/671494620/USB.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/671494620/USB.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/671494620/usb_msd.p1: ../../../../USB/usb_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/671494620" 
	@${RM} ${OBJECTDIR}/_ext/671494620/usb_msd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/671494620/usb_msd.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/671494620/usb_msd.p1 ../../../../USB/usb_msd.c 
	@-${MV} ${OBJECTDIR}/_ext/671494620/usb_msd.d ${OBJECTDIR}/_ext/671494620/usb_msd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/671494620/usb_msd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1662128652/flash.p1: ../Shared_Files/flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1662128652" 
	@${RM} ${OBJECTDIR}/_ext/1662128652/flash.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1662128652/flash.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1662128652/flash.p1 ../Shared_Files/flash.c 
	@-${MV} ${OBJECTDIR}/_ext/1662128652/flash.d ${OBJECTDIR}/_ext/1662128652/flash.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1662128652/flash.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1662128652/usb_app.p1: ../Shared_Files/usb_app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1662128652" 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_app.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_app.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1662128652/usb_app.p1 ../Shared_Files/usb_app.c 
	@-${MV} ${OBJECTDIR}/_ext/1662128652/usb_app.d ${OBJECTDIR}/_ext/1662128652/usb_app.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1662128652/usb_app.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1: ../Shared_Files/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1662128652" 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1 ../Shared_Files/usb_descriptors.c 
	@-${MV} ${OBJECTDIR}/_ext/1662128652/usb_descriptors.d ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1: ../Shared_Files/usb_scsi_inq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1662128652" 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1 ../Shared_Files/usb_scsi_inq.c 
	@-${MV} ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.d ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1662128652/main.p1: ../Shared_Files/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1662128652" 
	@${RM} ${OBJECTDIR}/_ext/1662128652/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1662128652/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1662128652/main.p1 ../Shared_Files/main.c 
	@-${MV} ${OBJECTDIR}/_ext/1662128652/main.d ${OBJECTDIR}/_ext/1662128652/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1662128652/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/671494620/USB.p1: ../../../../USB/USB.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/671494620" 
	@${RM} ${OBJECTDIR}/_ext/671494620/USB.p1.d 
	@${RM} ${OBJECTDIR}/_ext/671494620/USB.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/671494620/USB.p1 ../../../../USB/USB.c 
	@-${MV} ${OBJECTDIR}/_ext/671494620/USB.d ${OBJECTDIR}/_ext/671494620/USB.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/671494620/USB.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/671494620/usb_msd.p1: ../../../../USB/usb_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/671494620" 
	@${RM} ${OBJECTDIR}/_ext/671494620/usb_msd.p1.d 
	@${RM} ${OBJECTDIR}/_ext/671494620/usb_msd.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/671494620/usb_msd.p1 ../../../../USB/usb_msd.c 
	@-${MV} ${OBJECTDIR}/_ext/671494620/usb_msd.d ${OBJECTDIR}/_ext/671494620/usb_msd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/671494620/usb_msd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1662128652/flash.p1: ../Shared_Files/flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1662128652" 
	@${RM} ${OBJECTDIR}/_ext/1662128652/flash.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1662128652/flash.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1662128652/flash.p1 ../Shared_Files/flash.c 
	@-${MV} ${OBJECTDIR}/_ext/1662128652/flash.d ${OBJECTDIR}/_ext/1662128652/flash.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1662128652/flash.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1662128652/usb_app.p1: ../Shared_Files/usb_app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1662128652" 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_app.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_app.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1662128652/usb_app.p1 ../Shared_Files/usb_app.c 
	@-${MV} ${OBJECTDIR}/_ext/1662128652/usb_app.d ${OBJECTDIR}/_ext/1662128652/usb_app.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1662128652/usb_app.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1: ../Shared_Files/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1662128652" 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1 ../Shared_Files/usb_descriptors.c 
	@-${MV} ${OBJECTDIR}/_ext/1662128652/usb_descriptors.d ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1662128652/usb_descriptors.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1: ../Shared_Files/usb_scsi_inq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1662128652" 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1 ../Shared_Files/usb_scsi_inq.c 
	@-${MV} ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.d ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1662128652/usb_scsi_inq.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/_ext/1662128652/main.p1: ../Shared_Files/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1662128652" 
	@${RM} ${OBJECTDIR}/_ext/1662128652/main.p1.d 
	@${RM} ${OBJECTDIR}/_ext/1662128652/main.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     -o ${OBJECTDIR}/_ext/1662128652/main.p1 ../Shared_Files/main.c 
	@-${MV} ${OBJECTDIR}/_ext/1662128652/main.d ${OBJECTDIR}/_ext/1662128652/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/_ext/1662128652/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PIC18FX6J53.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/PIC18FX6J53.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/PIC18FX6J53.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/PIC18FX6J53.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/PIC18FX6J53.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/PIC18FX6J53.X.${IMAGE_TYPE}.map  -DXPRJ_PIC18F26J53_PRO=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=0-1FFF -Os -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file --fill=0@0x2000:0xFBFF  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-download -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=compiled:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/PIC18FX6J53.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/PIC18F26J53_PRO
	${RM} -r dist/PIC18F26J53_PRO

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
