import os

print(__file__ + ": Hello, World!")
print(__file__ + ": " + os.getenv('QRMI_QPU_RESOURCES', 'error'))
