This directory contain the file to setup jenkins project (tested on Jenkins 1.487)

You can create a new directory (CartesianMeshRuntime) in JENKINS_HOME/jobs, and copy the config.xml file in it.
The reload your jenkins, edit the project to setup URLS of repos and redmine and you are ready.

Please ensure that you installed the jenkins plugins :
 - Jenkins Cppcheck Plug-in
 - Jenkins GIT plugin
 - Jenkins GIT client plugin
 - Jenkins Valgrind Plug-in
 - Warnings Plug-in
 - xUnit plugin

