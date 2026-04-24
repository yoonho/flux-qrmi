plugin.register {
    name = "hello",
    handlers = {
        {
            topic = "shell.init",
            fn = function()
                print("shell.init")
		print("Running Python script...")

                -- Execute and capture output
                local handle = io.popen("python3 $PWD/qrmi_acquire.py 2>&1")
                local output = handle:read("*a")
                local success, exit_type, exit_code = handle:close()

                print("Python output:")
                print(output)
                print("Exit code: " .. tostring(exit_code))

                -- Check if successful
                if exit_code ~= 0 then
                    print("Python script failed")
                end
            end
        },
        {
            topic = "shell.exit",
            fn = function()
                print("shell.exit")
            end
        }
    }
}
