CC := clpru
LD := lnkpru
HEXBIN := hexpru
 
SWDIR = /usr/share/pru_am335x_starterware
CGTDIR = /usr/share/ti/cgt-pru
 
INCLUDEDIR = -I$(SWDIR)/include -I$(SWDIR)/include/hw -I$(SWDIR)/include/pru -I$(CGTDIR)/include
LIBDIR = -i$(SWDIR)/lib -i$(CGTDIR)/lib
 
CMD = lnk.cmd
OBJECTS = pot_adc.o
TARGET = pot_adc.out
 
CFLAGS := -O3 -Dam335x $(INCLUDEDIR)
 
$(TARGET): $(OBJECTS)
	$(LD) $(CMD) -o $@ $(OBJECTS) $(LIBDIR) -llibstarterware.a
 
%.o: %.c
	$(CC) -c --output_file=$@ $< $(CFLAGS)
 
.PHONY: clean
 
clean:
	rm -f $(OBJECTS) $(TARGET)
	
