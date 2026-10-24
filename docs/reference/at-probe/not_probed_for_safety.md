# AT commands not probed for safety

These 190 commands exist on the SIM7600A-H (they appear in `AT+CLAC`, see [clac_commands.txt](clac_commands.txt)) but were never sent to the module, because they can reset it, write non-volatile memory, open a shell, send traffic or messages, or have unknown side effects. Each entry below is documented from the sources only: the SIMCom AT manual V3.00 and application notes in `docs/reference/`, other SIMCom manuals, and web searches.

How to read the entries:

- **Source** says where each fact came from. "Inferred from name" means nothing public documents the command and the Purpose is only a guess from its name. Do not rely on those.
- Syntax taken from a different SIMCom module's manual (SIM5360, SIM7070, A76XX, SIM900 and so on) is labelled as unverified on the SIM7600A-H.
- About 130 of the 190 have no public documentation at all. Most of those are SIMCom internal, factory or vendor-customisation commands (`+CNV*`, `+DM*`, `+CT*`, `+AYLA*`, `+CTFS*`).
- Entries are in the original probe-list order: power and reset, QoS, SIM/NV, SMTP, FTP, bootloader/test/OTA, Bluetooth, MQTT-style families.

Notable findings:

- `+CSHELL` is a root shell: SEC Consult advisory SA-20250611-0 (CVE-2025-26412) shows `AT+CSHELL="id"` returning `uid=0(root)`.
- Only `+CFUN`, `+CPOF`, `+CRESET`, `+CUSBPIDSWITCH`, `+CIPSEND`, the `+BT*` set and `+CFTPS*` are in the V3.00 manual. The plain `+CFTP*`, `+SMTP*`, `+SSL*`, `+CC*`/`+CM*` (non-`CMQTT`) names are not.
- For the phone, the only entries worth using are `+CFUN` (radio on/off, minimal functionality) and possibly `+CIPSEND` if raw TCP sockets are ever needed.

### `AT+CFUN`
- **Purpose:** Sets the phone functionality level (minimum, full, RF off, factory test, reset, offline).
- **Syntax:** `AT+CFUN=?` returns supported (<fun>),(<rst>); `AT+CFUN?` returns `+CFUN: <fun>`; `AT+CFUN=<fun>[,<rst>]`. <fun>: 0 minimum, 1 full/online, 4 RF transmit and receive off (flight mode), 5 factory test mode, 6 reset, 7 offline. <rst>: 0 no reset, 1 reset the ME first (only effective when <fun>=1). After CFUN=7, CFUN=6 or a restart is needed to go back online.
- **Risk:** Writes were not sent (read `AT+CFUN?` gives 1 on our unit). =0/4 drop the network and RF, =6 reboots the module, =5/7 leave it in a mode needing recovery; the link and data session are lost.
- **Source:** SIM7500_SIM7600 AT Command Manual V3.00 section 3.2.1, p.55
- **Phone use:** Maybe: CFUN=4/1 for flight mode or radio recovery, only deliberately from app code.

### `AT+CPOF`
- **Purpose:** Powers the module down; it stores user data, deregisters from the network, then shuts down.
- **Syntax:** Execute `AT+CPOF` returns OK; test `AT+CPOF=?` returns OK. No parameters.
- **Risk:** Module powers off and needs PWRKEY or power cycle to return; would drop the serial link mid-session.
- **Source:** SIM7500_SIM7600 AT Command Manual V3.00 section 3.2.12, p.69
- **Phone use:** Possibly for a clean shutdown feature; never casually.

### `AT+CRESET`
- **Purpose:** Resets (reboots) the module.
- **Syntax:** Execute `AT+CRESET` returns OK then the module restarts; test `AT+CRESET=?` returns OK. No parameters.
- **Risk:** Reboots the module, drops network/data and USB/UART enumeration, re-emits boot URCs (`SMS DONE`, `PB DONE`) and may change the baud/USB state.
- **Source:** SIM7500_SIM7600 AT Command Manual V3.00 section 3.2.13, p.70
- **Phone use:** Useful as a software recovery reset if the modem hangs; app code only, with reconnect handling.

### `AT+QCGEQMIN`
- **Purpose:** Undocumented on this module. Inference from name only: a Qualcomm-prefixed (QC) variant of the 3GPP PDP QoS command +CGEQMIN (TS 27.007); likely sets or reads a per-context QoS profile, but the SIMCom form and parameters are unknown.
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Writes PDP-context QoS settings that could change data-session behaviour or break attach; the standard +CGEQMIN is the documented equivalent if one is needed.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing. Standard counterpart +CGEQMIN is in 3GPP TS 27.007.
- **Phone use:** None

### `AT+QCGEQOS`
- **Purpose:** Undocumented on this module. Inference from name only: a Qualcomm-prefixed (QC) variant of the 3GPP PDP QoS command +CGEQOS (TS 27.007); likely sets or reads a per-context QoS profile, but the SIMCom form and parameters are unknown.
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Writes PDP-context QoS settings that could change data-session behaviour or break attach; the standard +CGEQOS is the documented equivalent if one is needed.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing. Standard counterpart +CGEQOS is in 3GPP TS 27.007.
- **Phone use:** None

### `AT+QCGEQREQ`
- **Purpose:** Undocumented on this module. Inference from name only: a Qualcomm-prefixed (QC) variant of the 3GPP PDP QoS command +CGEQREQ (TS 27.007); likely sets or reads a per-context QoS profile, but the SIMCom form and parameters are unknown.
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Writes PDP-context QoS settings that could change data-session behaviour or break attach; the standard +CGEQREQ is the documented equivalent if one is needed.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing. Standard counterpart +CGEQREQ is in 3GPP TS 27.007.
- **Phone use:** None

### `AT+QCGQMIN`
- **Purpose:** Undocumented on this module. Inference from name only: a Qualcomm-prefixed (QC) variant of the 3GPP PDP QoS command +CGQMIN (TS 27.007); likely sets or reads a per-context QoS profile, but the SIMCom form and parameters are unknown.
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Writes PDP-context QoS settings that could change data-session behaviour or break attach; the standard +CGQMIN is the documented equivalent if one is needed.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing. Standard counterpart +CGQMIN is in 3GPP TS 27.007.
- **Phone use:** None

### `AT+QCGQREQ`
- **Purpose:** Undocumented on this module. Inference from name only: a Qualcomm-prefixed (QC) variant of the 3GPP PDP QoS command +CGQREQ (TS 27.007); likely sets or reads a per-context QoS profile, but the SIMCom form and parameters are unknown.
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Writes PDP-context QoS settings that could change data-session behaviour or break attach; the standard +CGQREQ is the documented equivalent if one is needed.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing. Standard counterpart +CGQREQ is in 3GPP TS 27.007.
- **Phone use:** None

### `AT+QCGTFT`
- **Purpose:** Undocumented. Inference from name only: Qualcomm-prefixed variant of 3GPP +CGTFT (traffic flow template for a PDP context).
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Modifies PDP-context packet filters, which could break data routing.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+CRIRS`
- **Purpose:** Undocumented. Inference only: name resembles a 'reset'/'restore' style or IMS/radio related command; purpose unknown.
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Unknown side effects, possibly a reset or state change.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** Avoid

### `AT+SIMCOMATI`
- **Purpose:** Identification block command, observed on our module to return manufacturer, model, revision, build string, SVN, IMEI, '+GCAP: +CGSM' and a DeviceInfo line (reference doc section on identity).
- **Syntax:** Execute AT+SIMCOMATI (observed). No parameters known; otherwise undocumented.
- **Risk:** Probably harmless and read-only, but the response contains the IMEI and was not sent by the main probe as it is undocumented; our later behavior test did run it.
- **Source:** Not in the AT manual V3.00; observed on our module: docs/sim7600a-h-at-reference.md and docs/reference/at-probe/behavior_tests.sanitized.json
- **Phone use:** Maybe; a one-shot identity read (the documented ATI/+CGMR also work)

### `AT+BT`
- **Purpose:** No public documentation found; name suggests a bare Bluetooth prefix or root command (inference only).
- **Syntax:** Undocumented
- **Risk:** Listed in the repo's not-probed-for-safety list; unknown side effects.
- **Source:** none found (inferred from name)
- **Phone use:** None

### `AT+CQCNV`
- **Purpose:** No public documentation found. Inference from name only: Qualcomm NV related query/config command (name suggests 'QC NV').
- **Syntax:** Undocumented
- **Risk:** Not sent; unknown semantics in the NV family; could change persistent calibration. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CNVR`
- **Purpose:** No public documentation found. Inference from name only: reads a Qualcomm NV item by number.
- **Syntax:** Undocumented
- **Risk:** Not sent; read variant but sits with the NV family; not tested to avoid accidental exposure/side effects and unknown arguments. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CNVW`
- **Purpose:** No public documentation found. Inference from name only: writes a Qualcomm NV item by number.
- **Syntax:** Undocumented
- **Risk:** Not sent; NV write could corrupt IMEI/calibration/band data and permanently brick or alter the module identity. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CNVRWE`
- **Purpose:** No public documentation found. Inference from name only: read/write/erase of NV items (the 'RWE' suffix suggests read, write, erase).
- **Syntax:** Undocumented
- **Risk:** Not sent; NV modification could be irreversible. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CRUS`
- **Purpose:** Undocumented. Inference only: name resembles +CRUSET (observed to return OK/empty on our module); possibly a related 'run user settings' command.
- **Syntax:** Undocumented
- **Risk:** Unknown; skipped for safety. (CRUSET was probed: =? and ? returned empty OK.)
- **Source:** none found (appears only in the SIM5215/5216 command-set history without a definition: https://www.scribd.com/doc/252354254/At-Command-Set)
- **Phone use:** None

### `AT+SJDR`
- **Purpose:** Enables or disables the jamming detection function; with it on, the module reports URCs +SJDR: JAMMING DETECTION and +SJDR: NO JAMMING.
- **Syntax:** Test: AT+SJDR=? -> +SJDR: (0,1). Write: AT+SJDR=<value>, 0 disabled (default), 1 enabled. Companion +SJDCFG sets period (0-120), mnl (0-31), minch (0-254), detecstat (0-1); its parameters are auto-saved. Documented for the A76XX series, not verified on SIM7600A-H. Parameter saving mode NO_SAVE.
- **Risk:** Low: turns on a detector and extra URCs; skipped because it is not in the SIM7600 manual.
- **Source:** SIMCom A76XX Series JD Application Note V1.00 sections 2.2-2.4 (https://wiki.wirenboard.com/wiki/images/4/42/A76XX_Series_JD_Application_Note_V1.00.pdf)
- **Phone use:** None

### `AT+CNETSCAN`
- **Purpose:** Network scan that reports surrounding cells/operators with signal metrics (documented for SIM800/SIM900; Hologram guide also describes it as a scan with metrics). Not documented for SIM7600.
- **Syntax:** SIM800 manual (not verified on SIM7600): write form AT+CNETSCAN=<format> (page could not be fetched, 403). Syntax on SIM7600A-H: Undocumented.
- **Risk:** Network side effect: scanning can block for a long time and drop the registration; skipped for safety.
- **Source:** SIM800 Series AT Command Manual V1.09/V1.10 (via https://htmlapp.alldatasheet.com/html-pdf/1741386/SIMCOM/SIM800/50879/198/SIM800.html, snippet only); SIM900 manual revision history https://static.chipdip.ru/lib/164/DOC001164979.pdf. Not in SIM7500/7600 manual V3.00.
- **Phone use:** Avoid; use +COPS=? or +CPSI? instead

### `AT+COTACFG`
- **Purpose:** No public documentation found. Inference from name only: configures OTA update parameters (server/APN).
- **Syntax:** Undocumented
- **Risk:** Not sent; persistent setting changes. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CNSVS`
- **Purpose:** Undocumented. Inference from name only: a 'CNSV...' network-service/signal family command (the SIM5360 change log mentions the related-looking +CNSVUS and +CNSVUN, but gives no definition).
- **Syntax:** Undocumented
- **Risk:** Unknown; probe skipped it for safety. The set/query semantics are unknown, and a set form could change stored network-service state.
- **Source:** none found (SIM5360 ATC V0.16 https://www.elecrow.com/download/SIMCOM_SIM5360_ATC_EN_V0.16%20.pdf only names +CNSVUS/+CNSVUN in its change log)
- **Phone use:** None

### `AT+CNSVSQ`
- **Purpose:** Undocumented. Inference from name only: a 'CNSV...' network-service/signal family command (the SIM5360 change log mentions the related-looking +CNSVUS and +CNSVUN, but gives no definition).
- **Syntax:** Undocumented
- **Risk:** Unknown; probe skipped it for safety. The set/query semantics are unknown, and a set form could change stored network-service state.
- **Source:** none found (SIM5360 ATC V0.16 https://www.elecrow.com/download/SIMCOM_SIM5360_ATC_EN_V0.16%20.pdf only names +CNSVUS/+CNSVUN in its change log)
- **Phone use:** None

### `AT+CNSVSX`
- **Purpose:** Undocumented. Inference from name only: a 'CNSV...' network-service/signal family command (the SIM5360 change log mentions the related-looking +CNSVUS and +CNSVUN, but gives no definition).
- **Syntax:** Undocumented
- **Risk:** Unknown; probe skipped it for safety. The set/query semantics are unknown, and a set form could change stored network-service state.
- **Source:** none found (SIM5360 ATC V0.16 https://www.elecrow.com/download/SIMCOM_SIM5360_ATC_EN_V0.16%20.pdf only names +CNSVUS/+CNSVUN in its change log)
- **Phone use:** None

### `AT+CELLLOCK`
- **Purpose:** Undocumented. Inference from name only: locks the modem to a specific cell/frequency (cell lock).
- **Syntax:** Undocumented
- **Risk:** Network side effect: a lock to a wrong cell can leave the modem unable to register, and the setting may persist in NV; skipped for safety.
- **Source:** none found (inferred from name); web search for SIMCom CELLLOCK found only other vendors' commands (Quectel +QNWLOCK, Meig ^CELLLOCK)
- **Phone use:** Avoid

### `AT+CCGMDF`
- **Purpose:** Undocumented. Inference only: name suggests a modem configuration default or mode flag.
- **Syntax:** Undocumented
- **Risk:** Unknown; the SIM5360 change log notes a default value changed to 0 (no other detail) and a write could change stored configuration, so skipped.
- **Source:** none found (SIM5360 ATC V0.16 change log only: https://www.elecrow.com/download/SIMCOM_SIM5360_ATC_EN_V0.16%20.pdf)
- **Phone use:** None

### `AT+CPASSMGR`
- **Purpose:** Undocumented. Inference only: password manager (stored passwords or lock codes); purpose unclear.
- **Syntax:** Undocumented
- **Risk:** Unknown; may change or store lock passwords, so it was skipped for safety.
- **Source:** none found (name appears only in the SIM5215/5216 AT command set version history, without a definition: https://www.scribd.com/doc/252354254/At-Command-Set)
- **Phone use:** Avoid

### `AT+CPLMNPASS`
- **Purpose:** Undocumented. Inference only: PLMN password (carrier or network-lock related).
- **Syntax:** Undocumented. Observed on our module: AT+CPLMNPASS=? and AT+CPLMNPASS? both returned ERROR (probe_results.sanitized.jsonl).
- **Risk:** A write form might change a network lock; only the =? and ? forms were tried, both ERROR.
- **Source:** none found (only our own probe in docs/reference/at-probe/probe_results.sanitized.jsonl)
- **Phone use:** None

### `AT+CPLMNWLIST`
- **Purpose:** Undocumented. Inference from name only: PLMN white list (allowed-operator list), probably stored in NV.
- **Syntax:** Undocumented
- **Risk:** Likely writes an operator whitelist to NV, which could block registration on the carrier; skipped for safety.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** Avoid

### `AT+SMTPSRV`
- **Purpose:** Sets the SMTP server address and port for the module's built-in e-mail client (inferred from name; the SIM800/SIM900 e-mail family has the same names).
- **Syntax:** Undocumented for SIM7600. Forum examples on SIM900A/SIM800 show `AT+SMTPSRV="host",port` (e.g. port 465); not verified on SIM7600A-H.
- **Risk:** Writes SMTP session config; harmless alone but part of the SMTP send chain (needs a data bearer), so it was not probed.
- **Source:** Not in the SIM7500/SIM7600 manual V3.00 or the app notes; listed in AT+CLAC on our module. Forum only: https://forum.arduino.cc/t/sim900a-module-email-send/341962 (SIM900A).
- **Phone use:** None (e-mail client is out of scope).

### `AT+SMTPAUTH`
- **Purpose:** Enables SMTP authentication and sets username and password (inferred from name and SIM900 forum example).
- **Syntax:** Undocumented for SIM7600. SIM900A forum example: `AT+SMTPAUTH=1,"user","password"`; unverified here.
- **Risk:** Stores credentials in the e-mail session config; part of SMTP chain, not probed.
- **Source:** Not in the V3.00 manual. Forum only: https://forum.arduino.cc/t/sim900a-module-email-send/341962.
- **Phone use:** None.

### `AT+SMTPFROM`
- **Purpose:** Sets the sender address (and display name) of the outgoing e-mail (inferred).
- **Syntax:** Undocumented for SIM7600. SIM900A forum example uses an address plus a name string; exact form unverified.
- **Risk:** Session config only; part of the SMTP send chain, not probed.
- **Source:** Not in the V3.00 manual. Forum only: https://forum.arduino.cc/t/sim900a-module-email-send/341962.
- **Phone use:** None.

### `AT+SMTPRCPT`
- **Purpose:** Adds a recipient (to/cc/bcc) to the outgoing e-mail (inferred).
- **Syntax:** Undocumented for SIM7600. SIM900A forum example: `AT+SMTPRCPT=0,0,"address","name"` (two leading numeric fields, meaning not documented in sources found).
- **Risk:** Session config only; part of the SMTP chain, not probed.
- **Source:** Not in the V3.00 manual. Forum only: https://forum.arduino.cc/t/sim900a-module-email-send/341962.
- **Phone use:** None.

### `AT+SMTPSUB`
- **Purpose:** Sets the e-mail subject (inferred from name).
- **Syntax:** Undocumented. No public documentation found for SIM7600.
- **Risk:** Session config only; part of the SMTP chain, not probed.
- **Source:** None found (inferred from name); the SIM900 forum thread lists a subject step in the sequence.
- **Phone use:** None.

### `AT+SMTPBODY`
- **Purpose:** Enters the e-mail body text (inferred). In the SIM900A forum example the body is typed after a prompt and ended with Ctrl+Z.
- **Syntax:** Undocumented for SIM7600; exact form unverified.
- **Risk:** Enters a data-entry prompt that can swallow subsequent AT input until Ctrl+Z/ESC; not probed.
- **Source:** Not in the V3.00 manual. Forum only: https://forum.arduino.cc/t/sim900a-module-email-send/341962.
- **Phone use:** None.

### `AT+SMTPBCH`
- **Purpose:** Probably sends/uploads the body in chunks (batch); inference from name only.
- **Syntax:** Undocumented.
- **Risk:** Likely data-entry prompt; part of SMTP chain, not probed.
- **Source:** No public documentation found (inferred from name).
- **Phone use:** None.

### `AT+SMTPFILE`
- **Purpose:** Probably attaches a file from module storage to the e-mail (inferred from name).
- **Syntax:** Undocumented.
- **Risk:** Touches the module filesystem and e-mail session; not probed.
- **Source:** No public documentation found (inferred from name).
- **Phone use:** None.

### `AT+SMTPSEND`
- **Purpose:** Connects to the SMTP server and sends the composed e-mail (inferred; forum posts show a `+SMTPSEND: <code>` result, with codes like 61, 63, 65, 67 reported as failures).
- **Syntax:** Undocumented for SIM7600; the forum example is the execute form `AT+SMTPSEND`.
- **Risk:** Opens a data connection and transmits mail with credentials; billable data and network side effect, so not probed.
- **Source:** Not in the V3.00 manual. Forum only: https://forum.arduino.cc/t/sim900a-module-email-send/341962.
- **Phone use:** None.

### `AT+SMTPSTOP`
- **Purpose:** Presumably terminates the SMTP session and releases resources (inferred from name).
- **Syntax:** Undocumented.
- **Risk:** Harmless-but-needs-setup; whole SMTP family was left unprobed.
- **Source:** No public documentation found (inferred from name).
- **Phone use:** None.

### `AT+CFTPSINGLEIP`
- **Purpose:** Inference only: plain-FTP counterpart of AT+CFTPSSINGLEIP (data socket address type).
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPPORT`
- **Purpose:** Inference only: likely sets the FTP server port.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPMODE`
- **Purpose:** Inference only: likely selects active or passive FTP mode.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPTYPE`
- **Purpose:** Inference only: likely sets the transfer type (ASCII/binary), by analogy with AT+CFTPSTYPE.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPSERV`
- **Purpose:** Inference only: likely sets the FTP server address.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPUN`
- **Purpose:** Inference only: likely sets the FTP user name.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPPW`
- **Purpose:** Inference only: likely sets the FTP password.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPGETFILE`
- **Purpose:** Inference only: likely downloads a file from the FTP server into module storage, by analogy with AT+CFTPSGETFILE.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPPUTFILE`
- **Purpose:** Inference only: likely uploads a module-storage file to the FTP server, by analogy with AT+CFTPSPUTFILE.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPGET`
- **Purpose:** Inference only: likely downloads a file from the FTP server to the serial port, by analogy with AT+CFTPSGET.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPPUT`
- **Purpose:** Inference only: likely uploads data from the serial port to the FTP server, by analogy with AT+CFTPSPUT.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPLIST`
- **Purpose:** Inference only: likely lists a remote directory, by analogy with AT+CFTPSLIST.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPMKD`
- **Purpose:** Inference only: likely creates a remote directory (writes to the server).
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPRMD`
- **Purpose:** Inference only: likely removes a remote directory (destructive on the server).
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPDELE`
- **Purpose:** Inference only: likely deletes a remote file (destructive on the server).
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPGETSIZE`
- **Purpose:** Inference only: likely queries the size of a remote file, by analogy with AT+CFTPSSIZE.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPCACHERD`
- **Purpose:** Inference only: likely outputs cached download data, by analogy with AT+CFTPSCACHERD.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CGPSWARM`
- **Purpose:** Undocumented. Inference from name only: GNSS warm start (compare AT+CGPSCOLD and AT+CGPSHOT in the SIM7600 manual).
- **Syntax:** Undocumented
- **Risk:** Restarts the GNSS engine with a warm start and discards some satellite data; harmless but needs GNSS to be powered and set up, so it was skipped.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing. The related +CGPSCOLD/+CGPSHOT are in the SIM7500/7600 manual.
- **Phone use:** Maybe; only if GPS is used (prefer documented +CGPSHOT/+CGPSCOLD)

### `AT+CNV`
- **Purpose:** No public documentation found. Inference from name only: generic non-volatile (NV) item access, probably read/dump of NV items (name resembles Qualcomm NV).
- **Syntax:** Undocumented
- **Risk:** Not sent; NV access can change or expose IMEI, RF calibration or identity data; irreversible if written. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CPORTDELAY`
- **Purpose:** No public documentation found. Inference from name only: a delay setting for port (USB/UART) readiness.
- **Syntax:** Undocumented
- **Risk:** Not sent; persistent setting with unknown units/effect. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CIPSEND`
- **Purpose:** Sends data on an open TCP/UDP socket (after `+NETOPEN`/`+CIPOPEN`).
- **Syntax:** Test `AT+CIPSEND=?` -> `+CIPSEND: (0-9),(1-1500)`. TCP variable length: `AT+CIPSEND=<link_num>,` then `>` prompt, data, Ctrl+Z to send or ESC to cancel. TCP fixed length: `AT+CIPSEND=<link_num>,<length>`. UDP: `AT+CIPSEND=<link_num>,,<serverIP>,<serverPort>` or with `<length>`. Result `+CIPSEND: <link_num>,<reqSendLength>,<cnfSendLength>`; errors `+CIPERROR: <err>`. Max response time 120 s. In variable-length mode Ctrl+Z/ESC/ETX (0x03) in data must be escaped with ETX.
- **Risk:** Enters a data prompt and transmits data over the network; needs an open socket, so not probed.
- **Source:** AT Command Manual V3.00 section 11.2.5 (pp. 277-279); TCP/IP App Note V2.00.
- **Phone use:** Possibly, for a custom TCP/UDP data path; low priority.

### `AT+NETDORM`
- **Purpose:** On the SIM5320, sets or queries the network dormancy state (fast dormancy) for the data connection. SIM7600 doc not found; same behaviour likely but is inference.
- **Syntax:** SIM5320 form: `AT+NETDORM=?` returns `+NETDORM: (<op>s)`; `AT+NETDORM?` returns `+NETDORM: <dss_state>[,<dormancy_state>]`; `AT+NETDORM=<op>` with <op> 0 = leave dormancy, 1 = enter dormancy. Example `AT+NETDORM=1` returns OK.
- **Risk:** Not sent; network-state side effect (harmless but alters the data session). Read form untried.
- **Source:** SIM5320 AT Command Set V2.02 section 16.2.8, p.393 https://cdn-shop.adafruit.com/datasheets/SIMCOM_SIM5320_ATC_EN_V2.02.pdf
- **Phone use:** None.

### `AT+ENTERSLEEP`
- **Purpose:** No public documentation found. Inference from name only: puts the module into sleep mode.
- **Syntax:** Undocumented
- **Risk:** Not sent; may stop the serial link until a wake line/event; hard to recover without DTR/wake handling. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Maybe: power saving later, but prefer documented +CSCLK/+CFUN.

### `AT+PUSH`
- **Purpose:** Undocumented. Inference only: possibly WAP push or push notification handling.
- **Syntax:** Undocumented
- **Risk:** Unknown; skipped for safety.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+HTTPCMD`
- **Purpose:** Unknown; sits next to `+HTTPOPEN`, `+HTTPRECV`, `+HTTPSTATUS`, `+HTTPCLOSE` in an undocumented alternate HTTP stack on our module (inference: sends a raw HTTP request/command).
- **Syntax:** Undocumented.
- **Risk:** Probably sends data over an opened HTTP connection (network side effect); not probed.
- **Source:** No public documentation found; web searches for it returned nothing. The documented HTTP set is `+HTTPINIT/PARA/ACTION/READ` (manual V3.00 and SIM800 HTTP compatibility note).
- **Phone use:** Avoid; use the documented `+HTTPACTION` or `+CCH*` path.

### `AT+CFTPSSINGLEIP`
- **Purpose:** Sets how the FTP(S) data socket IP address is chosen; some servers need 1. Set before AT+CFTPSLOGIN.
- **Syntax:** Test `AT+CFTPSSINGLEIP=?` -> `(0,1)`; read `AT+CFTPSSINGLEIP?` -> `+CFTPSSINGLEIP: <singleip>`; write `AT+CFTPSSINGLEIP=<singleip>`: 0 = per server's PORT response, 1 = same as control socket.
- **Risk:** Harmless setting (no network traffic) but listed in the unprobed FTP family.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.16, p.320; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSSTART`
- **Purpose:** Starts the FTP(S) service by activating the PDP context; must precede other FTP(S) commands.
- **Syntax:** Execute `AT+CFTPSSTART` -> `OK` then `+CFTPSSTART: <errcode>` (0 = success; codes in 12.3.1).
- **Risk:** Activates a data connection (network side effect, possibly billable); not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.1, p.303; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSSTOP`
- **Purpose:** Stops the FTP(S) service by deactivating the PDP context.
- **Syntax:** Execute `AT+CFTPSSTOP` -> `OK` and `+CFTPSSTOP: <errcode>`.
- **Risk:** Tears down the data context, which may disturb other IP sessions; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.2, p.304; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSLOGIN`
- **Purpose:** Logs in to an FTP or FTPS server.
- **Syntax:** Test `AT+CFTPSLOGIN=?` -> `"ADDRESS",(1-65535)[,"USERNAME","PASSWORD"[,(0-3)]]`; write `AT+CFTPSLOGIN="<host>",<port>,"<username>","<password>"[,<server_type>]` (0 FTP, 1 explicit FTPS AUTH SSL, 2 explicit AUTH TLS, 3 implicit, default 3) -> `OK` then `+CFTPSLOGIN: <errcode>`.
- **Risk:** Connects to a remote server and sends credentials in the clear for server_type 0; needs a server; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.3, p.305; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSLOGOUT`
- **Purpose:** Logs out of the FTP(S) server.
- **Syntax:** Test `AT+CFTPSLOGOUT=?` -> `OK`; execute `AT+CFTPSLOGOUT` -> `OK`, `+CFTPSLOGOUT: <errcode>`.
- **Risk:** Needs an active login; harmless otherwise but not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.4, p.306; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSGETFILE`
- **Purpose:** Downloads a file from the FTP(S) server into module storage.
- **Syntax:** Test `AT+CFTPSGETFILE=?` -> `[{non-ascii}]"FILEPATH"[,(1-4)]`; write `AT+CFTPSGETFILE="<filepath>"[,<dir>[,<offset>]]` -> `OK` then `+CFTPSGETFILE: <errcode>`. dir 1=F:/ (/cache, default), 2=D:/ SD, 3=E:/, 4=/mssl_cert/ (CA files); offset = resume position.
- **Risk:** Writes to module flash/SD (can fill limited space or overwrite files) and uses billable data; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.12, p.314; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSGET`
- **Purpose:** Downloads a file from the FTP(S) server directly to the serial port, optionally via a cache.
- **Syntax:** Test `AT+CFTPSGET=?`; write `AT+CFTPSGET="<filepath>"[,<rest_size>[,<using_cache>]]` -> `OK`, `+CFTPSGET: DATA,<len>` blocks, `+CFTPSGET: 0`; using_cache=1 gives `+CFTPS: RECV EVENT` and data is read with AT+CFTPSCACHERD.
- **Risk:** Streams file data into the UART (could swamp it) over billable data; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.14, p.317; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSPUTFILE`
- **Purpose:** Uploads a file from module storage to the FTP(S) server.
- **Syntax:** Test `AT+CFTPSPUTFILE=?` -> `[{non-ascii}]"FILEPATH"[,(1-3),(0-2147483647)]`; write `AT+CFTPSPUTFILE="<filepath>"[,<dir>[,<rest_size>]]` -> `OK`, `+CFTPSPUTFILE: <errcode>`. dir 1=F:/ (default), 2=D:/, 3=E:/; rest_size = FTP REST offset.
- **Risk:** Sends module data off-device and may overwrite a remote file; billable data; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.13, p.316; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSPUT`
- **Purpose:** Uploads data sent through the serial port to a file on the FTP(S) server.
- **Syntax:** Test `AT+CFTPSPUT=?`; write `AT+CFTPSPUT="<filepath>"[,<data_len>[,<rest_size>]]`, then enter data after `>` (data_len max 2048; if omitted, end with Ctrl+Z with ETX escaping) -> `OK`, `+CFTPSPUT: <errcode>`.
- **Risk:** Writes to a remote server, may overwrite files; leaves the modem in data-entry mode if sent bare; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.15, p.319; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSLIST`
- **Purpose:** Lists the items in a remote directory (current directory if none given), output to serial port.
- **Syntax:** Write `AT+CFTPSLIST[="<dir>"]` -> `OK`, `+CFTPSLIST: DATA,<len>` + listing, `+CFTPSLIST: 0`.
- **Risk:** Needs login; data traffic; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.11, p.313; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSMKD`
- **Purpose:** Creates a directory on the FTP(S) server.
- **Syntax:** Test `AT+CFTPSMKD=?` -> `"DIR"`; write `AT+CFTPSMKD="<dir>"` -> `OK`, `+CFTPSMKD: <errcode>`.
- **Risk:** Modifies remote server state; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.5, p.307; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSRMD`
- **Purpose:** Deletes a directory on the FTP(S) server.
- **Syntax:** Test `AT+CFTPSRMD=?`; write `AT+CFTPSRMD="<dir>"` -> `OK`, `+CFTPSRMD: <errcode>`.
- **Risk:** Destructive on the remote server; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.6, p.308; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSDELE`
- **Purpose:** Deletes a file on the FTP(S) server.
- **Syntax:** Test `AT+CFTPSDELE=?`; write `AT+CFTPSDELE="<filename>"` -> `OK`, `+CFTPSDELE: <errcode>`.
- **Risk:** Destructive on the remote server; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.7, p.309; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSCWD`
- **Purpose:** Changes the current directory on the FTP(S) server.
- **Syntax:** Test `AT+CFTPSCWD=?`; write `AT+CFTPSCWD="<dir>"` -> `OK`, `+CFTPSCWD: <errcode>`.
- **Risk:** Needs login; low risk but not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.8, p.310; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSPWD`
- **Purpose:** Gets the current directory on the FTP(S) server.
- **Syntax:** Execute `AT+CFTPSPWD` -> `OK`, `+CFTPSPWD: "<dir>"`.
- **Risk:** Needs login; harmless but not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.9, p.311; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSTYPE`
- **Purpose:** Sets the transfer type, ASCII or binary.
- **Syntax:** Test `AT+CFTPSTYPE=?` -> `(A,I)`; read `AT+CFTPSTYPE?` -> `+CFTPSTYPE: <type>`; write `AT+CFTPSTYPE=<type>` (A ASCII, I binary) -> `OK`, `+CFTPSTYPE: <errcode>`.
- **Risk:** Manual says to log in first; low risk but not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.10, p.312; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSCACHERD`
- **Purpose:** Outputs data cached by AT+CFTPSGET (using_cache=1) to the serial port.
- **Syntax:** Read `AT+CFTPSCACHERD?` -> `+CFTPSCACHERD: <len>`; execute `AT+CFTPSCACHERD` -> `+CFTPSGET: DATA,<out_len>` ... `OK` (max 1024 bytes per call).
- **Risk:** Only meaningful after a cached download; harmless otherwise but not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.17, p.321; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSABORT`
- **Purpose:** Aborts any FTP(S) operation; logs out of the server and deactivates the PDP context.
- **Syntax:** Execute `AT+CFTPSABORT` -> `OK`, `+CFTPSABORT: <errcode>`.
- **Risk:** Drops the data context (affects other IP sessions); not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.18, p.322; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSSIZE`
- **Purpose:** Gets the size of a file on the FTP(S) server.
- **Syntax:** Test `AT+CFTPSSIZE=?`; write `AT+CFTPSSIZE="<filepath>"` -> `OK`, `+CFTPSSIZE: <filesize>` (or errcode).
- **Risk:** Needs login; low risk but not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 section 12.2.19, p.323; also FTPS Application Note V2.00.
- **Phone use:** None

### `AT+CFTPSSLCFG`
- **Purpose:** Sets which SSL context (from AT+CSSLCFG) the FTPS control or data session uses.
- **Syntax:** Not in the SIM7500/SIM7600 manual V3.00 or FTPS note V2.00. Per the SIM7672X/SIM7652X FTP(S) note (a different module family, so only indicative): test `AT+CFTPSSLCFG=?` gives `+CFTPSSLCFG: (0,1),(0-9)`; write `AT+CFTPSSLCFG=<session_id>,<ssl_id>` with session_id 0 = control session, 1 = data session, ssl_id 0-9 = SSL context ID.
- **Risk:** Only changes an in-RAM FTPS setting (NO_SAVE per the 7672X note) but can break later FTPS logins; not probed because FTPS is unused. Behaviour on SIM7600A-H unconfirmed.
- **Source:** SIM7672X & SIM7652X Series FTP(S) Application Note V1.00 section 2.2.18, p.30 (https://files.waveshare.com/wiki/ESP32-S3-SIM7670G-4G/SIM7672X_SIM7652X_Series_FTP(S)_Application_Note_V1.00.pdf); not in SIM7600 docs.
- **Phone use:** None

### `AT+CHTTPSSEND`
- **Purpose:** Sends a request over the module's HTTPS client opened by `+CHTTPSSTART`/`+CHTTPSOPSE` (a third-party blog shows the sequence START, OPSE, SEND, RECV, CLSE, STOP).
- **Syntax:** Undocumented in official sources; our module's `+CHTTPSOPSE=?` shows `"ADDRESS",(1-65535)[,(1-2)]`, but nothing was observed for SEND.
- **Risk:** Transmits data over an open TLS connection (network, possibly billable); not probed.
- **Source:** Not in V3.00 manual or app notes; sequence only mentioned at https://zbotic.in/sim7600-4g-lte-module-http-gps-data-with-arduino/ (unverified).
- **Phone use:** Avoid; prefer documented `+CCHOPEN`/`+CCHSEND` or `+HTTPACTION`.

### `AT+VZWAPNE`
- **Purpose:** Undocumented. Inference from name only: Verizon APN entry/enable (carrier-specific APN table editing).
- **Syntax:** Undocumented
- **Risk:** A write could alter carrier APN configuration in NV; skipped for safety.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** Avoid

### `AT+AYLAFCFG`
- **Purpose:** Undocumented. Inference from name only: part of an Ayla Networks IoT agent command family (file/factory configuration).
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Part of the +AYLA family (Ayla Networks IoT cloud agent, inference from the prefix); using it would need an Ayla account and could contact the cloud.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+CTEST`
- **Purpose:** No public documentation found. Inference from name only: a factory/engineering test command.
- **Syntax:** Undocumented
- **Risk:** Not sent; engineering tests may transmit RF or alter state. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CREDTEA`
- **Purpose:** No public documentation found. Inference from name only: unclear; the name resembles a TEA (Tiny Encryption Algorithm) related command used in factory authentication.
- **Syntax:** Undocumented
- **Risk:** Not sent; possible factory/security function. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CAPFS`
- **Purpose:** No public documentation found. Inference from name only: operations on the modem's APPS file system (name suggests 'AP FS'), possibly a format/erase.
- **Syntax:** Undocumented
- **Risk:** Not sent; may erase or alter the module file system or stored settings. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CBACKUPEFS2`
- **Purpose:** No public documentation found. Inference from name only: backs up the modem's EFS (Qualcomm embedded file system, includes NV/calibration) to a second area.
- **Syntax:** Undocumented
- **Risk:** Not sent; writes flash and may hold or restore calibration; unknown side effects. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CBACKUPRESULT`
- **Purpose:** No public documentation found. Inference from name only: reports the result of an EFS backup (pairs with +CBACKUPEFS2).
- **Syntax:** Undocumented
- **Risk:** Not sent; probably a harmless query but unverified and unknown form. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CRESTORECOUNT`
- **Purpose:** No public documentation found. Inference from name only: a counter of EFS restore events (pairs with the backup commands).
- **Syntax:** Undocumented
- **Risk:** Not sent; probably a read-only counter but unverified; may be a trigger. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CLEDITST`
- **Purpose:** On the older SIM5360, adjusts LED driver current (the LCD backlight/LED intensity, 0 mA disables) and saves it automatically. Whether the SIM7600 behaves the same is not documented (inference).
- **Syntax:** SIM5360 form: `AT+CLEDITST=?` returns `+CLEDITST: (<led_type>s),(<val>s)`; `AT+CLEDITST?` returns `+CLEDITST: <val>`; `AT+CLEDITST=<led_type>,<val>` with <led_type> 0 = LCD, <val> 0-9 = 0 mA to 90 mA in 10 mA steps (default 10, i.e. 100 mA). SIM7600 syntax not published.
- **Risk:** Not sent. On SIM5360 the setting is saved automatically (persistent); low risk but unverified on SIM7600.
- **Source:** SIM5360 AT Command Set V0.16 section 9.35, p.265 https://www.elecrow.com/download/SIMCOM_SIM5360_ATC_EN_V0.16%20.pdf
- **Phone use:** None (T-Deck has its own display).

### `AT+CTAT`
- **Purpose:** No public documentation found. Inference from name only: a test/trigger AT command (name unclear; possibly 'test AT').
- **Syntax:** Undocumented
- **Risk:** Not sent; unknown. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CDEVPORT`
- **Purpose:** No public documentation found. Inference from name only: selects which device port (UART/USB) is used for a function (name suggests 'device port').
- **Syntax:** Undocumented
- **Risk:** Not sent; could redirect the AT port and lock us out. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CLOOPBACK`
- **Purpose:** No public documentation found. Inference from name only: an audio or data loopback test.
- **Syntax:** Undocumented
- **Risk:** Not sent; test mode that may ring audio or transmit. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CUSBAUDIO`
- **Purpose:** No public documentation found. Inference from name only: enables the USB audio interface (compare SIM7100_SIM7500_SIM7600 USB Audio Application Note, which does not name this command in our extract).
- **Syntax:** Undocumented
- **Risk:** Not sent; USB re-enumeration, may change audio routing. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CEDUMP`
- **Purpose:** Documented only for the SIM7070/SIM7080/SIM7090 family: sets whether the module resets or enters download mode when it crashes. Not documented for SIM7600; same meaning is plausible but is inference.
- **Syntax:** SIM7070 form: `AT+CEDUMP?` returns `+CEDUMP: <mode>`; `AT+CEDUMP=<mode>`; <mode> 0 = reset on crash (default), 1 = go into download mode on crash. SIM7600 syntax not published.
- **Risk:** Not sent; a setting of 1 would leave the module in download mode after a crash.
- **Source:** SIM7070_SIM7080_SIM7090 AT Command Manual V1.03 section 5.2.27, p.137 https://www.waveshare.com/w/upload/0/02/SIM7070_SIM7080_SIM7090_Series_AT_Command_Manual_V1.03.pdf
- **Phone use:** None.

### `AT+CUSBADB`
- **Purpose:** Enables the USB ADB (Android Debug Bridge) interface of the module, which is closed by default; a restart is needed for it to take effect, after which Windows shows an Android Composite ADB Interface.
- **Syntax:** `AT+CUSBADB=1` enables ADB (documented form); other values and test/read forms Undocumented (inference: `=0` disables).
- **Risk:** Not sent. After OK the module must be restarted; ADB gives a debug shell on the module's Linux and re-enumerates USB; security-sensitive.
- **Source:** SIM7600 Series Open Linux Development Guide V2.00 (p.22) https://files.waveshare.com/upload/b/b2/SIM7600_Series_Open_Linux_Development_Guide_V2.00.pdf
- **Phone use:** Avoid.

### `AT+CUSBPIDSWITCH`
- **Purpose:** Changes the module's USB PID (which interface set it exposes) permanently.
- **Syntax:** `AT+CUSBPIDSWITCH=?` returns `(9000,9001,9002,9003,9004,9005,9006,9007,9011,9016,9018,9019,901A,901B,9020-902B),(0-1),(0-1)`; `AT+CUSBPIDSWITCH?` returns `+CUSBPIDSWITCH: <pid>`; `AT+CUSBPIDSWITCH=<pid>,<reservel>,<reservel2>`. <pid> default 9001; the two reserved flags are 0 or 1 and unexplained.
- **Risk:** Not sent. Changes the USB composition (port set) and may need a restart; a wrong PID can hide the AT port or break drivers.
- **Source:** SIM7500_SIM7600 AT Command Manual V3.00 section 19.2.14, p.411
- **Phone use:** None; avoid.

### `AT+CUSBMSS`
- **Purpose:** No public documentation found. Inference from name only: USB mass storage mode switch for the module's file system.
- **Syntax:** Undocumented
- **Risk:** Not sent; would re-enumerate USB and expose the file system. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+BOOTLDR`
- **Purpose:** No public documentation found. Inference from name only: enters the bootloader or download mode for firmware flashing.
- **Syntax:** Undocumented
- **Risk:** Not sent; could leave the module in download mode needing a flash tool/power cycle; possible bricking. Another reference doc rates it as bricking-capable. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+DMRESET`
- **Purpose:** No public documentation found. Inference from name only: resets the device-management configuration (or the module via DM).
- **Syntax:** Undocumented
- **Risk:** Not sent; reset side effect, may wipe provisioning or reboot. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CAPNET`
- **Purpose:** No public documentation found. Inference from name only: unclear; possibly APN/network related setup ('AP NET').
- **Syntax:** Undocumented
- **Risk:** Not sent; may start a data connection. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+DMSESSION`
- **Purpose:** No public documentation found. Inference from name only: starts or queries an OMA-DM session with the carrier server.
- **Syntax:** Undocumented
- **Risk:** Not sent; network side effect and possible carrier-initiated config/firmware changes. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+FACTORYSESSION`
- **Purpose:** No public documentation found. Inference from name only: starts a factory/production session mode.
- **Syntax:** Undocumented
- **Risk:** Not sent; could put the module in factory mode that changes behaviour or unlocks other commands. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+FOTAEN`
- **Purpose:** No public documentation found. Inference from name only: enables or disables FOTA (firmware over the air) updates.
- **Syntax:** Undocumented
- **Risk:** Not sent; firmware update setting. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+COTA`
- **Purpose:** Downloads a FOTA delta package over FTP into the module's cache; used before AT+CDELTA applies the update.
- **Syntax:** `AT+COTA="filename","ip",port,"user","password"` and the same with a trailing `,cont` to resume an unfinished download. Prints download progress then OK. Apply with `AT+CDELTA="/cache/<file>"`. Requires an active network (`AT+NETACT=1`).
- **Risk:** Not sent. Billable data download and the next step (CDELTA) rewrites firmware; needs an FTP server and a matching delta file.
- **Source:** SIM7500_SIM7600 Series FOTA Application Note V2.00 section 3.1.2 https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/module/sim7600x/SIM7500_SIM7600%20Series_FOTA_Application%20Note_V2.00.pdf
- **Phone use:** Avoid.

### `AT+FOTADL`
- **Purpose:** No public documentation found. Inference from name only: starts a FOTA firmware download.
- **Syntax:** Undocumented
- **Risk:** Not sent; downloads data (billable) and may lead to firmware change. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CFTPDL`
- **Purpose:** Inference only: a 'download' variant of the plain FTP client; function unknown.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPSDL`
- **Purpose:** Inference only: a download variant, possibly the FTPS counterpart of AT+CFTPDL; function unknown.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPDLINFO`
- **Purpose:** Inference only: likely reports download status or info; function unknown.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPDLEXIT`
- **Purpose:** Inference only: likely exits/aborts a download session; function unknown.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+CFTPDLTO`
- **Purpose:** Inference only: likely sets a download timeout; function unknown.
- **Syntax:** Undocumented
- **Risk:** Would open/alter an FTP session or transfer over the cellular data connection (billable data, network side effect) and needs a server and PDP context; not probed for safety. Exact behaviour unknown.
- **Source:** none found (inferred from name). No public documentation found. Name appears in AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) but was not probed. Not in the SIM7500/SIM7600 AT manual V3.00 or FTPS application note V2.00; web search found only the unrelated SIM800/SIM900-style AT+FTP* family (AT+FTPSERV, AT+FTPUN etc.), which is a different command set.
- **Phone use:** None (FTPS/FTP is not needed for a phone).

### `AT+POC`
- **Purpose:** Undocumented. Inference only: Push-to-talk over Cellular (PoC) service control.
- **Syntax:** Undocumented
- **Risk:** Unknown; might register a PoC service or start network traffic.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+CSHELL`
- **Purpose:** Undocumented SIMCom command that runs a shell command on the module's embedded Linux and returns its output. SEC Consult found that on the SIM7600G it executes as root (CVE-2025-26412); SIMCom told them the command is not properly documented and the plan was to remove it. The SIMCom FTPS Application Note also uses `AT+CSHELL="sync"` to flush buffered data to the module's disk before a reset.
- **Syntax:** `AT+CSHELL="<command>"`: shell command string; response `+CSHELL: <output>`. SEC Consult PoC: `AT+CSHELL="id"` returned `+CSHELL: uid=0(root) gid=0(root)`. `AT+CSHELL="sync"` returns OK (FTPS note). No formal syntax definition published by SIMCom.
- **Risk:** Not sent. Arbitrary root command execution on the modem; can damage the file system or firmware. Do not expose raw AT passthrough on anything networked.
- **Source:** SEC Consult advisory SA-20250611-0 https://sec-consult.com/vulnerability-lab/advisory/undocumented-root-shell-access-on-simcom-modem/ ; SIM7500_SIM7600 FTPS Application Note V2.00 (the sync example)
- **Phone use:** Avoid.

### `AT+CAUTONET`
- **Purpose:** No public documentation found. Inference from name only: unclear; presumably automatic network/data connection configuration.
- **Syntax:** Undocumented
- **Risk:** Not sent; may change data/auto-connect behaviour persistently. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CNETCNCT`
- **Purpose:** No public documentation found. Inference from name only: unclear; presumably network connect (data call).
- **Syntax:** Undocumented
- **Risk:** Not sent; would open a billable data session. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CAUTOAPN`
- **Purpose:** No public documentation found. Inference from name only: unclear; presumably automatic APN selection setting.
- **Syntax:** Undocumented
- **Risk:** Not sent; may change APN configuration persistently. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CFOTASWITCH`
- **Purpose:** No public documentation found. Inference from name only: switches FOTA mode on or off.
- **Syntax:** Undocumented
- **Risk:** Not sent; firmware update behaviour change. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+SCHO`
- **Purpose:** Undocumented in the SIM7600 manual. Inference from name only: open a logical channel to a SIM applet by AID (similar to the 3GPP +CCHO).
- **Syntax:** Undocumented
- **Risk:** Opens a UICC logical channel; harmless-ish but can leave a channel open and needs AID knowledge, so skipped.
- **Source:** none found (standard +CCHO is in 3GPP TS 27.007)
- **Phone use:** None

### `AT+SCHC`
- **Purpose:** Undocumented in the SIM7600 manual. Inference from name only: close a logical channel (similar to the 3GPP +CCHC).
- **Syntax:** Undocumented
- **Risk:** Closes a UICC logical channel; only meaningful after SCHO.
- **Source:** none found (standard +CCHC is in 3GPP TS 27.007)
- **Phone use:** None

### `AT+SSAPDU`
- **Purpose:** Undocumented. Inference from name only: send an APDU to the SIM, perhaps with a logical channel (similar to +CGLA / +CSIM).
- **Syntax:** Undocumented
- **Risk:** Raw APDUs to the UICC can lock the SIM (PIN/PUK attempts) or alter SIM files; skipped for safety.
- **Source:** none found (inferred from name)
- **Phone use:** Avoid

### `AT+SIMAUTH`
- **Purpose:** Undocumented. Inference from name only: run a SIM authentication (GSM/UMTS challenge-response) on the SIM.
- **Syntax:** Undocumented
- **Risk:** Authentication runs on the SIM, which can desynchronize sequence numbers; skipped for safety.
- **Source:** none found (inferred from name)
- **Phone use:** None

### `AT+ISIMAUTH`
- **Purpose:** Undocumented. Inference from name only: IMS (ISIM) authentication challenge.
- **Syntax:** Undocumented
- **Risk:** Same concerns as +SIMAUTH; also needs an ISIM, which the SIM may lack.
- **Source:** none found (inferred from name)
- **Phone use:** None

### `AT+SHCMD`
- **Purpose:** No public documentation found. Inference from name only: runs a shell command on the module's embedded Linux. A 2026 vulnerability entry (BDU:2026-00054), cited in our reference doc, mentions an undocumented AT command on SIM7600G executing system commands as root; it names no command.
- **Syntax:** Undocumented
- **Risk:** Not sent; arbitrary root command execution; security-sensitive and could damage the file system. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CSDCHANGE`
- **Purpose:** No public documentation found. Inference from name only: unclear; possibly changes the SD card / storage device, or a 'CSD' mode.
- **Syntax:** Undocumented
- **Risk:** Not sent; unknown. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CSDACTV`
- **Purpose:** No public documentation found. Inference from name only: unclear; possibly activates an SD or CSD (circuit-switched data) function.
- **Syntax:** Undocumented
- **Risk:** Not sent; unknown. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+BTPOWER`
- **Purpose:** Starts or stops the module's Bluetooth (CSR) application.
- **Syntax:** Test `AT+BTPOWER=?` -> `+BTPOWER: (0-1)`; read `AT+BTPOWER?` -> `+BTPOWER: <flag>`; write `AT+BTPOWER=<flag>[,<debug_switch>]` (flag 0 stop, 1 start; debug_switch only 1 = save BT log file after start, not settable when flag is 0).
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all). Starting it also may write a log file.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.1 (p.494)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTHOST`
- **Purpose:** Gets or sets the module's Bluetooth name and shows its MAC.
- **Syntax:** Read `AT+BTHOST?` -> `+BTHOST: <host_name>,<host mac addr>`; write `AT+BTHOST=<btname>` (max 64 chars; default SIM7600_BT_xxxxxx from MAC low bytes).
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all). Write changes a stored name.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.2 (p.495)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTPIN`
- **Purpose:** No public documentation found for the SIM7500/SIM7600 series; name suggests setting or answering a pairing PIN code (inference only). The manual's pairing flow instead uses `AT+BTPAIR` with a pin code in mode 6.
- **Syntax:** Undocumented
- **Risk:** Unknown; BT-related so not probed.
- **Source:** none found (inferred from name); web search of SIM800 and SIM7600 BT documents did not surface it
- **Phone use:** None

### `AT+BTSETSCAN`
- **Purpose:** No public documentation found; name suggests setting discoverable/scannable mode of the module (inference only).
- **Syntax:** Undocumented
- **Risk:** Unknown; could change BT visibility; not probed.
- **Source:** none found (inferred from name)
- **Phone use:** None

### `AT+BTSCAN`
- **Purpose:** Scans for nearby Bluetooth devices and lists name, MAC and RSSI.
- **Syntax:** Test `AT+BTSCAN=?` -> `+BTSCAN: (0-1),(0-1),(6-60)`; write `AT+BTSCAN=<doscan>[,<mode>[,<timeout>]]` (doscan 0 stop/1 scan; mode 0 show paired/1 hide paired; timeout seconds, default 10). Results as URCs `+BTSCAN: <status>,<index>,<name>,<mac>,<rssi>`, end flag `+BTSCAN: 1`.
- **Risk:** Radio activity only, but needs BT powered on (`AT+BTPOWER=1`); skipped with the BT family.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.3 (p.495)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTIOCAP`
- **Purpose:** Sets the Bluetooth pairing I/O capability.
- **Syntax:** Test `AT+BTIOCAP=?` -> `+BTIOCAP: (0-3)`; write `AT+BTIOCAP=<mode>` (0 display only, 1 display+yes/no, 2 keyboard only, 3 no display/input); replies `+BTIOCAP: 1`.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.4 (p.496)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTPAIR`
- **Purpose:** Pairs with a scanned remote device, or accepts/rejects an incoming pairing request.
- **Syntax:** Test `AT+BTPAIR=?` -> `+BTPAIR: (index)`; initiate `AT+BTPAIR=0,<scan index>` (index from BTSCAN) then URC `+BTPAIRING: <mode>,<name>,<mac>[,<passkey>]`; answer with `AT+BTPAIR=<mode>,<accept>[,<passkey>]` (mode 1 compare, 2 passkey, 3 rebond, 4 notify, 5 just work, 6 pin code; accept 0/1); result `+BTPAIR: <result>[,<name>,<mac>]`. Pairing times out after about 30 s; accept must follow the +BTPAIRING URC.
- **Risk:** Stores a bond in module storage; Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.5 (pp.497-498)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTUNPAIR`
- **Purpose:** Removes a paired device.
- **Syntax:** Test `AT+BTUNPAIR=?` -> `+BTUNPAIR: (index)`; write `AT+BTUNPAIR=<paired index>` (index from AT+BTPAIRED) -> `+BTUNPAIR: <status>` (0 fail, 1 success).
- **Risk:** Deletes a stored bond; Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.6 (p.499)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTPAIRED`
- **Purpose:** Lists the bonded Bluetooth devices.
- **Syntax:** Read `AT+BTPAIRED?` -> `+BTPAIRED: <num>,<index>,<name>,<mac>` per device (manual spells the URC `+BTPAERED` in one place, evident typo).
- **Risk:** Read-only but part of the BT family and gave ERROR to read on this module in the repo's probe notes for similar names; not probed.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.7 (p.499)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTSPPSRV`
- **Purpose:** Activates or deactivates the Bluetooth SPP (serial profile) server.
- **Syntax:** Test `AT+BTSPPSRV=?` -> `+BTSPPSRV: (0-1)`; read `AT+BTSPPSRV?` -> `+BTSPPSRV: <status>`; write `AT+BTSPPSRV=<flag>` (0 deactive, 1 active) -> `+BTSPPSRV: <status>`.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all). Makes the module connectable.
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.8 (p.500)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTSPPPROF`
- **Purpose:** Queries whether a paired remote device has SPP service active.
- **Syntax:** `AT+BTSPPPROF=<index>` (index from AT+BTPAIRED) -> `+BTSPPPROF: <status>` (0 not active, 1 active).
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.9 (p.501)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTSPPCONN`
- **Purpose:** Connects or disconnects an SPP link to a paired device.
- **Syntax:** Test `AT+BTSPPCONN=?` -> `(0-1)`; read `AT+BTSPPCONN?` -> `+BTSPPCONN: <status>`; write `AT+BTSPPCONN=<action>[,<paired index>]` (0 disconnect, 1 connect; index max 64) -> `+BTSPPCONN: <status>[,<max frame size>][,<mac>]`. The URC can also arrive when a remote connects.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.10 (p.501)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTSPPSEND`
- **Purpose:** Sends data over the SPP link.
- **Syntax:** Write `AT+BTSPPSEND=<data>` where data is UCS2 hex (e.g. 003100320033006100620063 = "123abc") -> `OK` then `+BTSPPSEND: <result>` (0 fail, 1 success). Incoming data arrives as URC `+BTSPPRECV: <len>,<data>`.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.11 (p.502)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTREG`
- **Purpose:** Registers or unregisters the BLE GATT server application.
- **Syntax:** Write `AT+BTGATTREG=<status>` (0 unregister, 1 register) -> `+BTGATTREG: <status>`.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.12 (p.503)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTACT`
- **Purpose:** Activates the GATT server (advertising/connectable).
- **Syntax:** Execute `AT+BTGATTACT` -> `+BTGATTACT: <status>` (1 active); write `AT+BTGATTACT=<auto_broadcast>[,<preferredMTU>]` (auto re-activate after disconnect 0/1; MTU 24-512, default 23).
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.13 (p.504)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTCREDB`
- **Purpose:** Creates the GATT database.
- **Syntax:** Execute `AT+BTGATTCREDB` -> `+BTGATTCREDB: <status>` (1 success).
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.14 (p.504)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTCRESRV`
- **Purpose:** Creates a GATT service in the database.
- **Syntax:** Write `AT+BTGATTCRESRV=<uuid>` (4 or 32 hex chars) -> `+BTGATTCRESRV: <status>`.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.15 (p.505)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTCRECHAR`
- **Purpose:** Creates a characteristic in the current service.
- **Syntax:** Write `AT+BTGATTCRECHAR=<uuid>,<property>,<permission>` (uuid 4 or 32 hex) -> `+BTGATTCRECHAR: <status>,0X<uuid>,<handle>`. The manual does not enumerate the property/permission values (example uses 2,16).
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.16 (p.505)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTCRECHARDES`
- **Purpose:** Creates the characteristic description (descriptor).
- **Syntax:** Execute `AT+BTGATTCRECHARDES` -> `+BTGATTCRECHARDES: <status>` (1 success).
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.17 (p.506)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTSRVADD`
- **Purpose:** Adds the built database to the GATT server.
- **Syntax:** Execute `AT+BTGATTSRVADD` -> `+BTGATTSRVADD: <status>`.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.18 (p.507)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTREADCFM`
- **Purpose:** Responds to a client read request (URC `+BTGATTREADIND: <handle>,<maxlen>`).
- **Syntax:** Write `AT+BTGATTREADCFM=<respCode>,<data>` (0 success) -> `+BTGATTREADCFM: 1`; if data length equals maxlen the READIND repeats.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.19 (p.507)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTWRCFM`
- **Purpose:** Responds to a client write request (URC `+BTGATTWRIND: <handle>,<data>`).
- **Syntax:** Write `AT+BTGATTWRCFM=<result>` (0 success) -> `+BTGATTWRCFM: <status>`.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.20 (p.508)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTNOTIFY`
- **Purpose:** Sends a GATT notification to the connected client.
- **Syntax:** Write `AT+BTGATTNOTIFY=<handle>,<data>` (data max 20 chars) -> `+BTGATTNOTIFY: <status>`.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.21 (p.509)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+BTGATTSENDIND`
- **Purpose:** Sends a GATT indication to the connected client.
- **Syntax:** Write `AT+BTGATTSENDIND=<handle>,<data>` (data max 20 chars) -> `+BTGATTSENDIND: <status>`.
- **Risk:** Changes Bluetooth state on a module with no probed baseline; BT commands were deliberately skipped (all give ERROR on read/test if absent on this variant; the SIM7600A-H may not expose BT at all).
- **Source:** SIM7500_SIM7600 Series AT Command Manual V3.00 s28.2.22 (p.509)
- **Phone use:** None (the T-Deck has its own ESP32-S3 Bluetooth)

### `AT+CHSICDEVSLEEP`
- **Purpose:** No public documentation found. Inference from name only: sleep control for the HSIC device interface.
- **Syntax:** Undocumented
- **Risk:** Not sent; could suspend the HSIC link; unused on our UART/USB. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CCINIT`
- **Purpose:** No public documentation found. By name only (inference), likely to initialise a cloud-client session.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None

### `AT+CCAUTH`
- **Purpose:** No public documentation found. By name only (inference), likely to set cloud authentication (e.g. product/device credentials).
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None

### `AT+CCSEND`
- **Purpose:** No public documentation found. By name only (inference), likely to send data to the cloud.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None

### `AT+CCYIELD`
- **Purpose:** No public documentation found. By name only (inference), likely to let the client poll/receive incoming messages.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None

### `AT+CCDEINIT`
- **Purpose:** No public documentation found. By name only (inference), likely to tear down the cloud-client session.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None

### `AT+CMAUTH`
- **Purpose:** No public documentation found. By name only (inference), likely to set MQTT authentication (credentials).
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None (use the documented AT+CMQTT* family if MQTT is ever needed)

### `AT+CMPARA`
- **Purpose:** No public documentation found. By name only (inference), likely to set MQTT parameters.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None (use the documented AT+CMQTT* family if MQTT is ever needed)

### `AT+CMCONN`
- **Purpose:** No public documentation found. By name only (inference), likely to connect to the MQTT broker.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None (use the documented AT+CMQTT* family if MQTT is ever needed)

### `AT+CMPUB`
- **Purpose:** No public documentation found. By name only (inference), likely to publish a message.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None (use the documented AT+CMQTT* family if MQTT is ever needed)

### `AT+CMSUB`
- **Purpose:** No public documentation found. By name only (inference), likely to subscribe to a topic.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None (use the documented AT+CMQTT* family if MQTT is ever needed)

### `AT+CMUNSUB`
- **Purpose:** No public documentation found. By name only (inference), likely to unsubscribe from a topic.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None (use the documented AT+CMQTT* family if MQTT is ever needed)

### `AT+CMYIELD`
- **Purpose:** No public documentation found. By name only (inference), likely to process/poll incoming MQTT messages.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None (use the documented AT+CMQTT* family if MQTT is ever needed)

### `AT+CMAUTOYIELD`
- **Purpose:** No public documentation found. By name only (inference), likely to enable automatic polling of incoming MQTT messages.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None (use the documented AT+CMQTT* family if MQTT is ever needed)

### `AT+CMDISCONN`
- **Purpose:** No public documentation found. By name only (inference), likely to disconnect from the broker.
- **Syntax:** Undocumented
- **Risk:** Probably a vendor/cloud-specific MQTT client (inference from the CC/CM prefix and chunk context only). Not probed: might open a network connection or alter stored cloud/MQTT config.
- **Source:** none found (inferred from name)
- **Phone use:** None (use the documented AT+CMQTT* family if MQTT is ever needed)

### `AT+COINIT`
- **Purpose:** No public documentation found. Inference from name only: initialises the OTA update client.
- **Syntax:** Undocumented
- **Risk:** Not sent; may start update machinery. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CODOWNLOAD`
- **Purpose:** No public documentation found. Inference from name only: starts an OTA download.
- **Syntax:** Undocumented
- **Risk:** Not sent; billable data and possible update. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CODEINIT`
- **Purpose:** No public documentation found. Inference from name only: de-initialises the OTA client (pairs with +COINIT).
- **Syntax:** Undocumented
- **Risk:** Not sent; unknown. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+SSLOPEN`
- **Purpose:** Name suggests opening an SSL connection; appears in the SIM7600A-H's AT+CLAC with `+SSLPARA/READ/WRITE/CLOSE`, a possible vendor-customised SSL socket set (inference).
- **Syntax:** Undocumented.
- **Risk:** Opens a network connection; not probed.
- **Source:** No public documentation found; not in the SSL App Note V2.00, which documents `+CCHSET/CCHSTART/CCHOPEN/CCHSEND/CCHRECV/CCHCLOSE` instead.
- **Phone use:** Avoid; use documented `+CCH*`.

### `AT+SSLPARA`
- **Purpose:** Presumably sets parameters for the `+SSL*` socket set (inferred from name).
- **Syntax:** Undocumented.
- **Risk:** Unknown config write; not probed.
- **Source:** No public documentation found (inferred from name).
- **Phone use:** Avoid.

### `AT+SSLREAD`
- **Purpose:** Presumably reads received data from the `+SSL*` connection (inferred).
- **Syntax:** Undocumented.
- **Risk:** Needs an open connection; not probed with the family.
- **Source:** No public documentation found (inferred from name).
- **Phone use:** Avoid.

### `AT+SSLWRITE`
- **Purpose:** Presumably sends data over the `+SSL*` connection (inferred).
- **Syntax:** Undocumented.
- **Risk:** Network transmission; not probed.
- **Source:** No public documentation found (inferred from name).
- **Phone use:** Avoid.

### `AT+SSLCLOSE`
- **Purpose:** Presumably closes the `+SSL*` connection (inferred).
- **Syntax:** Undocumented.
- **Risk:** Low, but part of the unprobed family.
- **Source:** No public documentation found (inferred from name).
- **Phone use:** Avoid.

### `AT+AYLACFG`
- **Purpose:** Undocumented. Inference from name only: part of an Ayla Networks IoT agent command family (configuration).
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Part of the +AYLA family (Ayla Networks IoT cloud agent, inference from the prefix); using it would need an Ayla account and could contact the cloud.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+AYLASERVICE`
- **Purpose:** Undocumented. Inference from name only: part of an Ayla Networks IoT agent command family (service).
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Part of the +AYLA family (Ayla Networks IoT cloud agent, inference from the prefix); using it would need an Ayla account and could contact the cloud.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+AYLATEMPLATE`
- **Purpose:** Undocumented. Inference from name only: part of an Ayla Networks IoT agent command family (template).
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Part of the +AYLA family (Ayla Networks IoT cloud agent, inference from the prefix); using it would need an Ayla account and could contact the cloud.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+AYLASTATUS`
- **Purpose:** Undocumented. Inference from name only: part of an Ayla Networks IoT agent command family (status).
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Part of the +AYLA family (Ayla Networks IoT cloud agent, inference from the prefix); using it would need an Ayla account and could contact the cloud.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+AYLASEND`
- **Purpose:** Undocumented. Inference from name only: part of an Ayla Networks IoT agent command family (send).
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Part of the +AYLA family (Ayla Networks IoT cloud agent, inference from the prefix); using it would need an Ayla account and could contact the cloud.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+AYLARECV`
- **Purpose:** Undocumented. Inference from name only: part of an Ayla Networks IoT agent command family (receive).
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Part of the +AYLA family (Ayla Networks IoT cloud agent, inference from the prefix); using it would need an Ayla account and could contact the cloud.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+AYLAFRESET`
- **Purpose:** Undocumented. Inference from name only: part of an Ayla Networks IoT agent command family (factory reset).
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Name suggests a factory reset of the Ayla agent configuration, possibly irreversible; do not send.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+AYLADEBUG`
- **Purpose:** Undocumented. Inference from name only: part of an Ayla Networks IoT agent command family (debug).
- **Syntax:** Undocumented
- **Risk:** Not sent to the module (probe skipped it for safety; see docs/reference/at-probe/not_probed_for_safety.md). Part of the +AYLA family (Ayla Networks IoT cloud agent, inference from the prefix); using it would need an Ayla account and could contact the cloud.
- **Source:** Listed in AT+CLAC output (docs/reference/at-probe/clac_commands.txt); not in the AT manual V3.00 or any other supplied PDF; web search found nothing.
- **Phone use:** None

### `AT+CTFSGETID`
- **Purpose:** No public documentation found. Inference from name only: a TFS (trusted/true file system, Qualcomm) command that gets an ID.
- **Syntax:** Undocumented
- **Risk:** Not sent; likely a read, but part of the TFS auth set. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CTFSDECRYPT`
- **Purpose:** No public documentation found. Inference from name only: decrypts TFS data.
- **Syntax:** Undocumented
- **Risk:** Not sent; part of a factory auth set; unknown effects. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CTFSAUTH`
- **Purpose:** No public documentation found. Inference from name only: authenticates a TFS (factory) session, likely unlocking restricted functions.
- **Syntax:** Undocumented
- **Risk:** Not sent; may unlock privileged mode. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** Avoid

### `AT+CDMPOWER`
- **Purpose:** No public documentation found. Inference from name only: a device-management (DM) power related setting; 'DM' may mean OMA-DM/FOTA device management.
- **Syntax:** Undocumented
- **Risk:** Not sent; unknown; might power down or alter DM state. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+DMSET`
- **Purpose:** No public documentation found. Inference from name only: device-management (OMA-DM, carrier provisioning) settings.
- **Syntax:** Undocumented
- **Risk:** Not sent; could change carrier/provisioning state or trigger a DM session. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+DMCONFIGEXT`
- **Purpose:** No public documentation found. Inference from name only: extended device-management configuration (carrier DM profile).
- **Syntax:** Undocumented
- **Risk:** Not sent; could change persistent carrier provisioning. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None

### `AT+CMDLOG`
- **Purpose:** No public documentation found. Inference from name only: enables logging of commands/diagnostic logs.
- **Syntax:** Undocumented
- **Risk:** Not sent; may start diagnostic output on a port. Listed by AT+CLAC on our module (docs/reference/at-probe/clac_commands.txt) and deliberately not sent (not_probed_for_safety.md).
- **Source:** none found (inferred from name); web searches for SIMCom/Waveshare docs returned nothing
- **Phone use:** None
