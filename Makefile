all:
	g++ -O0 -g3 -Wall -o dmmlogger main.cc port.cc dmm.cc serial_dmm.cc ut60e.cc trigger.cc continuous_trigger.cc settle_trigger.cc
