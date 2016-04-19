PLUGIN_NAME = event_counter

HEADERS = event-counter.h

SOURCES = event-counter.cpp\
          moc_event-counter.cpp\

LIBS = 

### Do not edit below this line ###

include $(shell rtxi_plugin_config --pkgdata-dir)/Makefile.plugin_compile
