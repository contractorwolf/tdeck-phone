# AT probe tools

Scripts used to explore the SIM7600A-H on the old Waveshare HAT. They produced the data behind [docs/sim7600a-h-at-reference.md](../../docs/sim7600a-h-at-reference.md). Output files are in [docs/reference/at-probe/](../../docs/reference/at-probe/) (sanitized: identifiers, cell IDs and IP addresses removed).

**Read-only by design, but read the code before running it on your own module.** `probe.py` and `discover.py` send only `AT<cmd>=?` and `AT<cmd>?`, and skip a deny list of commands that reset, power off, write NV memory or firmware, open shells, send messages or start network connections. `behave.py` runs a fixed set of safe queries and a few volatile settings changes (`ATE0/ATE1`, `AT+CMEE`, `AT+CPMS`, `AT+CSDH`) and restores them.

They assume the module on `/dev/cu.usbserial-0001` at 9600 baud and need `pyserial`. Each takes a scratch directory as its argument and expects `clac_cmds.txt` (the output of `AT+CLAC`, one name per line) and, for `discover.py`, `doc_absent.json`. They were written for a one-off investigation, not as a supported tool.

| Script | What it did |
|---|---|
| `probe.py` | Probed every non-denied `+`, `^` and `*` command from `AT+CLAC` with the two read-only forms (406 commands) |
| `discover.py` | Probed 206 more names: manual commands that `AT+CLAC` omits, plus well-known names from other SIMCom modules |
| `behave.py` | Information commands, error formats with `AT+CMEE` 0/1/2, echo, case, SMS storage capacity, timing |
