This prolog webservice is currently running on http://uvm061.dei.isep.ipp.pt
(you need to be logged in DEINET VPN in order to use the service)


The following steps were performed in order to setup the Prolog webserver:
1) installed swi-prolog
$ sudo apt-get install swi-prolog

2) created a directory for all prolog file
$ sudo mkdir /var/www/PROLOG_WEBSERVER

3) added a test file (test.pl)

4) ran swi-prolog
$ swipl

5) loaded test.pl and started the server at port 8888
?- ['test.pl']
?- server(8888).

6) opened the following address in browser
http://uvm061.dei.isep.ipp.pt:8888/hello_lapr5

Currently available services:
http://uvm061.dei.isep.ipp.pt:8888/depth_search
http://uvm061.dei.isep.ipp.pt:8888/breadth_search
http://uvm061.dei.isep.ipp.pt:8888/branch_and_bound
http://uvm061.dei.isep.ipp.pt:8888/branch_and_bound?personA=Joao&personB=JoseCid

To start the Prolog server just execute the following command:
/etc/init.d/start-prolog-server.sh 