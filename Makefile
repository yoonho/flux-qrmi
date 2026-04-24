hello_jobtap.so: hello_jobtap.c
	gcc -shared -fPIC -o hello_jobtap.so hello_jobtap.c $(pkg-config --cflags --libs flux-core)

reload:
	-flux jobtap remove hello_jobtap.so
	flux jobtap load $(PWD)/hello_jobtap.so

clean:
	rm hello_jobtap.so

# Shell plugin
#   flux run -o userrc=$PWD/hello_shell.lua hostname
#   flux submit -o userrc=$PWD/hello_shell.lua hostname
#   flux job attach $(flux job last)
#
# Install qrmi
#   python3 -m venv qrmi
#   source qrmi/bin/activate
#   pip install --upgrade pip
#   pip install qrmi
#
# Load the plugin - need full path
#   flux jobtap load $PWD/hello_jobtap.so
#
# Verify it's loaded
#   flux jobtap list
#   Output:
#   hello_jobtap.so
#
# Submit a test job
#   flux submit hostname
#
# Check the logs
#   flux dmesg | grep -i hello
#   Output:
#   Hello jobtap plugin loaded!
#   Hello! Job 123456 was submitted
#   Hello! Job 123456 is now running
#
# Unload the plugin
#   flux jobtap remove hello_jobtap.s
