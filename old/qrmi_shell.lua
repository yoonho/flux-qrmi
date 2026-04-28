plugin.register {
    name = "hello",
    handlers = {
        {
            topic = "shell.init",
            fn = function()
		print("[shell.init] Running Python script...")
                local posix = require("posix.stdlib")
                print("[shell.init] Value of QRMI_QPU_RESOURCES: " .. (posix.getenv("QRMI_QPU_RESOURCES") or "not set"))

                -- Execute and capture output
                local handle = io.popen(". /home/fluxuser/.venv/bin/activate; python $PWD/qrmi_acquire.py 2>&1")
                local output = handle:read("*all")
                local success = handle:close()

                if success then
                    print("[shell.init] Python script output: " .. output)
                else
                    print("[shell.init] Error executing Python script")
                end

                local posix = require("posix.stdlib")
                posix.setenv("MY_VARIABLE", "my_value", 1)
                print("[shell.init] Value of MY_VARIABLE: " .. (posix.getenv("MY_VARIABLE") or "not set"))
            end
        },
        {
            topic = "task.init",
            fn = function()
                print("[task.init] ...")
            end
        },
        {
            topic = "task.exec",
            fn = function()
                local posix = require("posix.stdlib")
                print("[task_exec] Value of MY_VARIABLE: " .. (posix.getenv("MY_VARIABLE") or "not set"))
            end
        },
        {
            topic = "shell.exit",
            fn = function()
                print("[shell.exit] ...")
            end
        }
    }
}
