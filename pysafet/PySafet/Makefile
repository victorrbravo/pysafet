TARGET = Safet.so
OFILES = sipSafetcmodule.o sipSafetQList0101SafetDocument.o sipSafetQMap0100QString0100QString.o sipSafetSafetWorkflow.o sipSafetParsedSqlToData.o sipSafetSafetYAWL.o sipSafetSafetVariable.o sipSafetSafetDocument.o sipSafetSafetXmlObject.o sipSafetMainWindow.o
HFILES = sipAPISafet.h 

CC = gcc
CXX = g++
LINK = g++
CPPFLAGS = -DNDEBUG -DQT_NO_DEBUG -DQT_CORE_LIB -I. -I/usr/local/include/libdigidoc -I/usr/include/python2.7 -I/usr/share/qt4/mkspecs/default -I/usr/include/qt4/QtCore -I/usr/include/qt4 -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4/QtNetwork -I/usr/include/qt4/QtSql -I/usr/include/qt4/QtWebKit -I/usr/include/qt4/QtSvg -I/home/vbravo/websafet/inflow -I/home/vbravo/websafet/src
CFLAGS = -fPIC  -g -O2 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security  -D_FORTIFY_SOURCE=2 -Wall -W -D_REENTRANT
CXXFLAGS = -pipe -fPIC  -g -O2 -fstack-protector --param=ssp-buffer-size=4 -Wformat -Werror=format-security  -D_FORTIFY_SOURCE=2 -Wall -W -D_REENTRANT
LFLAGS = -L/home/vbravo/websafet/lib -shared  -Wl,-z,relro -Wl,-O1 -Wl,--version-script=Safet.exp
LIBS = -L/usr/lib -lsafet -lQtNetwork -lQtSvg -lQtSql -ltar -lQtCore
MOC = moc-qt4
.SUFFIXES: .c .o .cpp .cc .cxx .C


.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

all: $(TARGET)

$(OFILES): $(HFILES)

$(TARGET): $(OFILES)
	@echo '{ global: initSafet; local: *; };' > Safet.exp
	$(LINK) $(LFLAGS) -o $(TARGET) $(OFILES) $(LIBS)

install: $(TARGET)
	@test -d $(DESTDIR)/usr/lib/python2.7/dist-packages || mkdir -p $(DESTDIR)/usr/lib/python2.7/dist-packages
	cp -f $(TARGET) $(DESTDIR)/usr/lib/python2.7/dist-packages/$(TARGET)
	@test -d $(DESTDIR)/usr/share/sip/SafetXmlObject || mkdir -p $(DESTDIR)/usr/share/sip/SafetXmlObject
	cp -f Safet.sip $(DESTDIR)/usr/share/sip/SafetXmlObject/Safet.sip

clean:
	-rm -f $(TARGET)
	-rm -f sipSafetcmodule.o
	-rm -f sipSafetQList0101SafetDocument.o
	-rm -f sipSafetQMap0100QString0100QString.o
	-rm -f sipSafetSafetWorkflow.o
	-rm -f sipSafetParsedSqlToData.o
	-rm -f sipSafetSafetYAWL.o
	-rm -f sipSafetSafetVariable.o
	-rm -f sipSafetSafetDocument.o
	-rm -f sipSafetSafetXmlObject.o
	-rm -f sipSafetMainWindow.o
	-rm -f Safet.exp
