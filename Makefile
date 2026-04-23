hello_jobtap.so:
	gcc -shared -fPIC -o hello_jobtap.so hello_jobtap.c $(pkg-config --cflags --libs flux-core)

clean:
	rm hello_jobtap.so

# Load the plugin
#   flux jobtap load ./hello_jobtap.so
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
