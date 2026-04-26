CFLAGS = -fPIC -Wall $(shell pkg-config --cflags flux-core)
LDFLAGS = -shared $(shell pkg-config --libs flux-core)

qrmi_shell.so: qrmi_shell.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

install:
	install -D qrmi_shell.so $(shell flux config builtin shell_pluginpath)/qrmi_shell.so

clean:
	rm -f qrmi_shell.so

.PHONY: install clean

# C shell plugin (above)
#   make; make install
#   flux run --env=QRMI_QPU_RESOURCES="ibm_marrakash" --env=QRMI_IBM_QRS_ENDPOINT="https://quantum.cloud.ibm.com/api/v1" ./run_sampler.sh
#
# Lua shell plugin - insufficient functionality
#   flux run -o userrc=$PWD/hello_shell.lua hostname
#   flux submit -o userrc=$PWD/hello_shell.lua hostname
#   flux job attach $(flux job last)
