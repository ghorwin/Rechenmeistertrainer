TEMPLATE=subdirs

# SUBDIRS lists all subprojects
SUBDIRS += Rechenmeistertrainer \
    qwt

# where to find the sub projects
Rechenmeistertrainer.file = ../../Rechenmeistertrainer/projects/Qt/Rechenmeistertrainer.pro
qwt.file = ../../externals/qwt/projects/Qt/qwt.pro

Rechenmeistertrainer.depends = qwt

