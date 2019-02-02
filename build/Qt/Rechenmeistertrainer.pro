TEMPLATE=subdirs

# SUBDIRS lists all subprojects
SUBDIRS += Rechenmeistertrainer

# where to find the sub projects
Rechenmeistertrainer.file = ../../Rechenmeistertrainer/projects/Qt/Rechenmeistertrainer.pro

# on Windows we build our own Qwt
#win32-msvc* {
	SUBDIRS += qwt
	qwt.file = ../../externals/qwt/projects/Qt/qwt.pro
	Rechenmeistertrainer.depends = qwt
#



