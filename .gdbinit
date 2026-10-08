set filename-display basename

python
import os
port = os.getenv("GDB_PORT", "1337")
gdb.execute(f"target extended-remote :{port}")
end

define reload
    load
    monitor reset halt
    hbreak main
    continue
end

reload
