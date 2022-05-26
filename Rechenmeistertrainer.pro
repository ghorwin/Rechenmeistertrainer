TEMPLATE=subdirs

# SUBDIRS lists all subprojects
SUBDIRS += Rechenmeistertrainer

# where to find the sub projects
Rechenmeistertrainer.file = Rechenmeistertrainer/Rechenmeistertrainer.pro

# on Windows we build our own Qwt
windows {
	SUBDIRS += qwt
	qwt.file = externals/qwt/projects/Qt/qwt.pro
	Rechenmeistertrainer.depends = qwt
}

