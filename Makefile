#############################################################################
# Makefile for building: abonent2
# Generated by qmake (2.01a) (Qt 4.7.4) on: Wed Apr 18 12:42:34 2012
# Project:  abonent2.pro
# Template: app
# Command: /usr/bin/qmake-qt4 -o Makefile abonent2.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_SQL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fstack-protector --param=ssp-buffer-size=4 -m32 -march=i686 -mtune=atom -fasynchronous-unwind-tables -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -g -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fstack-protector --param=ssp-buffer-size=4 -m32 -march=i686 -mtune=atom -fasynchronous-unwind-tables -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/lib/qt4/mkspecs/linux-g++ -I. -I/usr/include/QtCore -I/usr/include/QtGui -I/usr/include/QtSql -I/usr/include -I. -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib -lQtSql -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake-qt4
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = 
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = AboCommentsDialog.cpp \
		AboHistoryDialog.cpp \
		AbonentDialog.cpp \
		AbonentsWindow.cpp \
		AbonentTypesDialog.cpp \
		AccrualsPanel.cpp \
		AccrualsRPanel.cpp \
		ArcsDialog.cpp \
		ClientsDialog.cpp \
		CorrectionDialog.cpp \
		CorrectionsDialog.cpp \
		DateInputDialog.cpp \
		FinallyDialog.cpp \
		main.cpp \
		mainwindow.cpp \
		OperatorsDialog.cpp \
		PayDialog.cpp \
		PaysDialog.cpp \
		ServicesDialog.cpp \
		ServicesPanel.cpp \
		SqlManager.cpp \
		SummaryCDialog.cpp \
		SummaryFixCDialog.cpp \
		TPlansDialog.cpp \
		UsersDialog.cpp moc_AboCommentsDialog.cpp \
		moc_AboHistoryDialog.cpp \
		moc_AbonentDialog.cpp \
		moc_AbonentsWindow.cpp \
		moc_AbonentTypesDialog.cpp \
		moc_AccrualsPanel.cpp \
		moc_AccrualsRPanel.cpp \
		moc_ArcsDialog.cpp \
		moc_ClientsDialog.cpp \
		moc_CorrectionDialog.cpp \
		moc_CorrectionsDialog.cpp \
		moc_DateInputDialog.cpp \
		moc_FinallyDialog.cpp \
		moc_mainwindow.cpp \
		moc_OperatorsDialog.cpp \
		moc_PayDialog.cpp \
		moc_PaysDialog.cpp \
		moc_ServicesDialog.cpp \
		moc_ServicesPanel.cpp \
		moc_SqlManager.cpp \
		moc_SummaryCDialog.cpp \
		moc_SummaryFixCDialog.cpp \
		moc_TPlansDialog.cpp \
		moc_UsersDialog.cpp
OBJECTS       = AboCommentsDialog.o \
		AboHistoryDialog.o \
		AbonentDialog.o \
		AbonentsWindow.o \
		AbonentTypesDialog.o \
		AccrualsPanel.o \
		AccrualsRPanel.o \
		ArcsDialog.o \
		ClientsDialog.o \
		CorrectionDialog.o \
		CorrectionsDialog.o \
		DateInputDialog.o \
		FinallyDialog.o \
		main.o \
		mainwindow.o \
		OperatorsDialog.o \
		PayDialog.o \
		PaysDialog.o \
		ServicesDialog.o \
		ServicesPanel.o \
		SqlManager.o \
		SummaryCDialog.o \
		SummaryFixCDialog.o \
		TPlansDialog.o \
		UsersDialog.o \
		moc_AboCommentsDialog.o \
		moc_AboHistoryDialog.o \
		moc_AbonentDialog.o \
		moc_AbonentsWindow.o \
		moc_AbonentTypesDialog.o \
		moc_AccrualsPanel.o \
		moc_AccrualsRPanel.o \
		moc_ArcsDialog.o \
		moc_ClientsDialog.o \
		moc_CorrectionDialog.o \
		moc_CorrectionsDialog.o \
		moc_DateInputDialog.o \
		moc_FinallyDialog.o \
		moc_mainwindow.o \
		moc_OperatorsDialog.o \
		moc_PayDialog.o \
		moc_PaysDialog.o \
		moc_ServicesDialog.o \
		moc_ServicesPanel.o \
		moc_SqlManager.o \
		moc_SummaryCDialog.o \
		moc_SummaryFixCDialog.o \
		moc_TPlansDialog.o \
		moc_UsersDialog.o
DIST          = /usr/lib/qt4/mkspecs/common/g++-multilib.conf \
		/usr/lib/qt4/mkspecs/common/unix.conf \
		/usr/lib/qt4/mkspecs/common/linux.conf \
		/usr/lib/qt4/mkspecs/qconfig.pri \
		/usr/lib/qt4/mkspecs/features/qt_functions.prf \
		/usr/lib/qt4/mkspecs/features/qt_config.prf \
		/usr/lib/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/lib/qt4/mkspecs/features/default_pre.prf \
		/usr/lib/qt4/mkspecs/features/release.prf \
		/usr/lib/qt4/mkspecs/features/default_post.prf \
		/usr/lib/qt4/mkspecs/features/warn_on.prf \
		/usr/lib/qt4/mkspecs/features/qt.prf \
		/usr/lib/qt4/mkspecs/features/unix/thread.prf \
		/usr/lib/qt4/mkspecs/features/moc.prf \
		/usr/lib/qt4/mkspecs/features/resources.prf \
		/usr/lib/qt4/mkspecs/features/uic.prf \
		/usr/lib/qt4/mkspecs/features/yacc.prf \
		/usr/lib/qt4/mkspecs/features/lex.prf \
		/usr/lib/qt4/mkspecs/features/include_source_dir.prf \
		abonent2.pro
QMAKE_TARGET  = abonent2
DESTDIR       = 
TARGET        = abonent2

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: abonent2.pro  /usr/lib/qt4/mkspecs/linux-g++/qmake.conf /usr/lib/qt4/mkspecs/common/g++-multilib.conf \
		/usr/lib/qt4/mkspecs/common/unix.conf \
		/usr/lib/qt4/mkspecs/common/linux.conf \
		/usr/lib/qt4/mkspecs/qconfig.pri \
		/usr/lib/qt4/mkspecs/features/qt_functions.prf \
		/usr/lib/qt4/mkspecs/features/qt_config.prf \
		/usr/lib/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/lib/qt4/mkspecs/features/default_pre.prf \
		/usr/lib/qt4/mkspecs/features/release.prf \
		/usr/lib/qt4/mkspecs/features/default_post.prf \
		/usr/lib/qt4/mkspecs/features/warn_on.prf \
		/usr/lib/qt4/mkspecs/features/qt.prf \
		/usr/lib/qt4/mkspecs/features/unix/thread.prf \
		/usr/lib/qt4/mkspecs/features/moc.prf \
		/usr/lib/qt4/mkspecs/features/resources.prf \
		/usr/lib/qt4/mkspecs/features/uic.prf \
		/usr/lib/qt4/mkspecs/features/yacc.prf \
		/usr/lib/qt4/mkspecs/features/lex.prf \
		/usr/lib/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtSql.prl \
		/usr/lib/libQtCore.prl \
		/usr/lib/libQtGui.prl
	$(QMAKE) -o Makefile abonent2.pro
/usr/lib/qt4/mkspecs/common/g++-multilib.conf:
/usr/lib/qt4/mkspecs/common/unix.conf:
/usr/lib/qt4/mkspecs/common/linux.conf:
/usr/lib/qt4/mkspecs/qconfig.pri:
/usr/lib/qt4/mkspecs/features/qt_functions.prf:
/usr/lib/qt4/mkspecs/features/qt_config.prf:
/usr/lib/qt4/mkspecs/features/exclusive_builds.prf:
/usr/lib/qt4/mkspecs/features/default_pre.prf:
/usr/lib/qt4/mkspecs/features/release.prf:
/usr/lib/qt4/mkspecs/features/default_post.prf:
/usr/lib/qt4/mkspecs/features/warn_on.prf:
/usr/lib/qt4/mkspecs/features/qt.prf:
/usr/lib/qt4/mkspecs/features/unix/thread.prf:
/usr/lib/qt4/mkspecs/features/moc.prf:
/usr/lib/qt4/mkspecs/features/resources.prf:
/usr/lib/qt4/mkspecs/features/uic.prf:
/usr/lib/qt4/mkspecs/features/yacc.prf:
/usr/lib/qt4/mkspecs/features/lex.prf:
/usr/lib/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtSql.prl:
/usr/lib/libQtCore.prl:
/usr/lib/libQtGui.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile abonent2.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/abonent21.0.0 || $(MKDIR) .tmp/abonent21.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/abonent21.0.0/ && $(COPY_FILE) --parents AboCommentsDialog.h AboHistoryDialog.h AbonentDialog.h AbonentsWindow.h AbonentTypesDialog.h AccrualsPanel.h AccrualsRPanel.h ArcsDialog.h ClientsDialog.h CorrectionDialog.h CorrectionsDialog.h DateInputDialog.h FinallyDialog.h mainwindow.h OperatorsDialog.h PayDialog.h PaysDialog.h ServicesDialog.h ServicesPanel.h SqlManager.h SummaryCDialog.h SummaryFixCDialog.h TPlansDialog.h UsersDialog.h .tmp/abonent21.0.0/ && $(COPY_FILE) --parents AboCommentsDialog.cpp AboHistoryDialog.cpp AbonentDialog.cpp AbonentsWindow.cpp AbonentTypesDialog.cpp AccrualsPanel.cpp AccrualsRPanel.cpp ArcsDialog.cpp ClientsDialog.cpp CorrectionDialog.cpp CorrectionsDialog.cpp DateInputDialog.cpp FinallyDialog.cpp main.cpp mainwindow.cpp OperatorsDialog.cpp PayDialog.cpp PaysDialog.cpp ServicesDialog.cpp ServicesPanel.cpp SqlManager.cpp SummaryCDialog.cpp SummaryFixCDialog.cpp TPlansDialog.cpp UsersDialog.cpp .tmp/abonent21.0.0/ && (cd `dirname .tmp/abonent21.0.0` && $(TAR) abonent21.0.0.tar abonent21.0.0 && $(COMPRESS) abonent21.0.0.tar) && $(MOVE) `dirname .tmp/abonent21.0.0`/abonent21.0.0.tar.gz . && $(DEL_FILE) -r .tmp/abonent21.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_AboCommentsDialog.cpp moc_AboHistoryDialog.cpp moc_AbonentDialog.cpp moc_AbonentsWindow.cpp moc_AbonentTypesDialog.cpp moc_AccrualsPanel.cpp moc_AccrualsRPanel.cpp moc_ArcsDialog.cpp moc_ClientsDialog.cpp moc_CorrectionDialog.cpp moc_CorrectionsDialog.cpp moc_DateInputDialog.cpp moc_FinallyDialog.cpp moc_mainwindow.cpp moc_OperatorsDialog.cpp moc_PayDialog.cpp moc_PaysDialog.cpp moc_ServicesDialog.cpp moc_ServicesPanel.cpp moc_SqlManager.cpp moc_SummaryCDialog.cpp moc_SummaryFixCDialog.cpp moc_TPlansDialog.cpp moc_UsersDialog.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_AboCommentsDialog.cpp moc_AboHistoryDialog.cpp moc_AbonentDialog.cpp moc_AbonentsWindow.cpp moc_AbonentTypesDialog.cpp moc_AccrualsPanel.cpp moc_AccrualsRPanel.cpp moc_ArcsDialog.cpp moc_ClientsDialog.cpp moc_CorrectionDialog.cpp moc_CorrectionsDialog.cpp moc_DateInputDialog.cpp moc_FinallyDialog.cpp moc_mainwindow.cpp moc_OperatorsDialog.cpp moc_PayDialog.cpp moc_PaysDialog.cpp moc_ServicesDialog.cpp moc_ServicesPanel.cpp moc_SqlManager.cpp moc_SummaryCDialog.cpp moc_SummaryFixCDialog.cpp moc_TPlansDialog.cpp moc_UsersDialog.cpp
moc_AboCommentsDialog.cpp: AboCommentsDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) AboCommentsDialog.h -o moc_AboCommentsDialog.cpp

moc_AboHistoryDialog.cpp: AboHistoryDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) AboHistoryDialog.h -o moc_AboHistoryDialog.cpp

moc_AbonentDialog.cpp: AbonentDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) AbonentDialog.h -o moc_AbonentDialog.cpp

moc_AbonentsWindow.cpp: AbonentsWindow.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) AbonentsWindow.h -o moc_AbonentsWindow.cpp

moc_AbonentTypesDialog.cpp: AbonentTypesDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) AbonentTypesDialog.h -o moc_AbonentTypesDialog.cpp

moc_AccrualsPanel.cpp: AccrualsPanel.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) AccrualsPanel.h -o moc_AccrualsPanel.cpp

moc_AccrualsRPanel.cpp: AccrualsRPanel.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) AccrualsRPanel.h -o moc_AccrualsRPanel.cpp

moc_ArcsDialog.cpp: ArcsDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) ArcsDialog.h -o moc_ArcsDialog.cpp

moc_ClientsDialog.cpp: ClientsDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) ClientsDialog.h -o moc_ClientsDialog.cpp

moc_CorrectionDialog.cpp: CorrectionDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) CorrectionDialog.h -o moc_CorrectionDialog.cpp

moc_CorrectionsDialog.cpp: CorrectionsDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) CorrectionsDialog.h -o moc_CorrectionsDialog.cpp

moc_DateInputDialog.cpp: DateInputDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) DateInputDialog.h -o moc_DateInputDialog.cpp

moc_FinallyDialog.cpp: FinallyDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) FinallyDialog.h -o moc_FinallyDialog.cpp

moc_mainwindow.cpp: mainwindow.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp

moc_OperatorsDialog.cpp: OperatorsDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) OperatorsDialog.h -o moc_OperatorsDialog.cpp

moc_PayDialog.cpp: PayDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) PayDialog.h -o moc_PayDialog.cpp

moc_PaysDialog.cpp: PaysDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) PaysDialog.h -o moc_PaysDialog.cpp

moc_ServicesDialog.cpp: ServicesDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) ServicesDialog.h -o moc_ServicesDialog.cpp

moc_ServicesPanel.cpp: ServicesPanel.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) ServicesPanel.h -o moc_ServicesPanel.cpp

moc_SqlManager.cpp: SqlManager.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) SqlManager.h -o moc_SqlManager.cpp

moc_SummaryCDialog.cpp: SummaryCDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) SummaryCDialog.h -o moc_SummaryCDialog.cpp

moc_SummaryFixCDialog.cpp: SummaryFixCDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) SummaryFixCDialog.h -o moc_SummaryFixCDialog.cpp

moc_TPlansDialog.cpp: TPlansDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) TPlansDialog.h -o moc_TPlansDialog.cpp

moc_UsersDialog.cpp: UsersDialog.h
	/usr/lib/qt4/bin/moc $(DEFINES) $(INCPATH) UsersDialog.h -o moc_UsersDialog.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

AboCommentsDialog.o: AboCommentsDialog.cpp AboCommentsDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AboCommentsDialog.o AboCommentsDialog.cpp

AboHistoryDialog.o: AboHistoryDialog.cpp AboHistoryDialog.h \
		PayDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AboHistoryDialog.o AboHistoryDialog.cpp

AbonentDialog.o: AbonentDialog.cpp AbonentDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AbonentDialog.o AbonentDialog.cpp

AbonentsWindow.o: AbonentsWindow.cpp AbonentsWindow.h \
		AbonentDialog.h \
		ServicesPanel.h \
		AccrualsPanel.h \
		AccrualsRPanel.h \
		AboHistoryDialog.h \
		SummaryCDialog.h \
		SummaryFixCDialog.h \
		AboCommentsDialog.h \
		SqlManager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AbonentsWindow.o AbonentsWindow.cpp

AbonentTypesDialog.o: AbonentTypesDialog.cpp AbonentTypesDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AbonentTypesDialog.o AbonentTypesDialog.cpp

AccrualsPanel.o: AccrualsPanel.cpp AccrualsPanel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AccrualsPanel.o AccrualsPanel.cpp

AccrualsRPanel.o: AccrualsRPanel.cpp AccrualsRPanel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o AccrualsRPanel.o AccrualsRPanel.cpp

ArcsDialog.o: ArcsDialog.cpp ArcsDialog.h \
		SqlManager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ArcsDialog.o ArcsDialog.cpp

ClientsDialog.o: ClientsDialog.cpp ClientsDialog.h \
		FinallyDialog.h \
		CorrectionsDialog.h \
		SqlManager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ClientsDialog.o ClientsDialog.cpp

CorrectionDialog.o: CorrectionDialog.cpp CorrectionDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CorrectionDialog.o CorrectionDialog.cpp

CorrectionsDialog.o: CorrectionsDialog.cpp CorrectionsDialog.h \
		CorrectionDialog.h \
		SqlManager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o CorrectionsDialog.o CorrectionsDialog.cpp

DateInputDialog.o: DateInputDialog.cpp DateInputDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o DateInputDialog.o DateInputDialog.cpp

FinallyDialog.o: FinallyDialog.cpp FinallyDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o FinallyDialog.o FinallyDialog.cpp

main.o: main.cpp mainwindow.h \
		UsersDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

mainwindow.o: mainwindow.cpp mainwindow.h \
		AbonentsWindow.h \
		ClientsDialog.h \
		AbonentTypesDialog.h \
		TPlansDialog.h \
		ServicesDialog.h \
		PaysDialog.h \
		CorrectionsDialog.h \
		DateInputDialog.h \
		OperatorsDialog.h \
		ArcsDialog.h \
		SqlManager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp

OperatorsDialog.o: OperatorsDialog.cpp OperatorsDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o OperatorsDialog.o OperatorsDialog.cpp

PayDialog.o: PayDialog.cpp PayDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PayDialog.o PayDialog.cpp

PaysDialog.o: PaysDialog.cpp PaysDialog.h \
		PayDialog.h \
		SqlManager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o PaysDialog.o PaysDialog.cpp

ServicesDialog.o: ServicesDialog.cpp ServicesDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ServicesDialog.o ServicesDialog.cpp

ServicesPanel.o: ServicesPanel.cpp ServicesPanel.h \
		SqlManager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ServicesPanel.o ServicesPanel.cpp

SqlManager.o: SqlManager.cpp SqlManager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SqlManager.o SqlManager.cpp

SummaryCDialog.o: SummaryCDialog.cpp SummaryCDialog.h \
		SqlManager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SummaryCDialog.o SummaryCDialog.cpp

SummaryFixCDialog.o: SummaryFixCDialog.cpp SummaryFixCDialog.h \
		SqlManager.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o SummaryFixCDialog.o SummaryFixCDialog.cpp

TPlansDialog.o: TPlansDialog.cpp TPlansDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o TPlansDialog.o TPlansDialog.cpp

UsersDialog.o: UsersDialog.cpp UsersDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o UsersDialog.o UsersDialog.cpp

moc_AboCommentsDialog.o: moc_AboCommentsDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_AboCommentsDialog.o moc_AboCommentsDialog.cpp

moc_AboHistoryDialog.o: moc_AboHistoryDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_AboHistoryDialog.o moc_AboHistoryDialog.cpp

moc_AbonentDialog.o: moc_AbonentDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_AbonentDialog.o moc_AbonentDialog.cpp

moc_AbonentsWindow.o: moc_AbonentsWindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_AbonentsWindow.o moc_AbonentsWindow.cpp

moc_AbonentTypesDialog.o: moc_AbonentTypesDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_AbonentTypesDialog.o moc_AbonentTypesDialog.cpp

moc_AccrualsPanel.o: moc_AccrualsPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_AccrualsPanel.o moc_AccrualsPanel.cpp

moc_AccrualsRPanel.o: moc_AccrualsRPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_AccrualsRPanel.o moc_AccrualsRPanel.cpp

moc_ArcsDialog.o: moc_ArcsDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ArcsDialog.o moc_ArcsDialog.cpp

moc_ClientsDialog.o: moc_ClientsDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ClientsDialog.o moc_ClientsDialog.cpp

moc_CorrectionDialog.o: moc_CorrectionDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_CorrectionDialog.o moc_CorrectionDialog.cpp

moc_CorrectionsDialog.o: moc_CorrectionsDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_CorrectionsDialog.o moc_CorrectionsDialog.cpp

moc_DateInputDialog.o: moc_DateInputDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_DateInputDialog.o moc_DateInputDialog.cpp

moc_FinallyDialog.o: moc_FinallyDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_FinallyDialog.o moc_FinallyDialog.cpp

moc_mainwindow.o: moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp

moc_OperatorsDialog.o: moc_OperatorsDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_OperatorsDialog.o moc_OperatorsDialog.cpp

moc_PayDialog.o: moc_PayDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_PayDialog.o moc_PayDialog.cpp

moc_PaysDialog.o: moc_PaysDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_PaysDialog.o moc_PaysDialog.cpp

moc_ServicesDialog.o: moc_ServicesDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ServicesDialog.o moc_ServicesDialog.cpp

moc_ServicesPanel.o: moc_ServicesPanel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_ServicesPanel.o moc_ServicesPanel.cpp

moc_SqlManager.o: moc_SqlManager.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_SqlManager.o moc_SqlManager.cpp

moc_SummaryCDialog.o: moc_SummaryCDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_SummaryCDialog.o moc_SummaryCDialog.cpp

moc_SummaryFixCDialog.o: moc_SummaryFixCDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_SummaryFixCDialog.o moc_SummaryFixCDialog.cpp

moc_TPlansDialog.o: moc_TPlansDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_TPlansDialog.o moc_TPlansDialog.cpp

moc_UsersDialog.o: moc_UsersDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_UsersDialog.o moc_UsersDialog.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

