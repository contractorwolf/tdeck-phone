# SIM7600A-H AT command index (generated from the module)

Every command name seen from three sources: the module's own `AT+CLAC` list, the SIMCom AT manual V3.00 table of contents, and extra names I tried from other SIMCom module families. For each name: whether the manual documents it (section and page), whether `AT+CLAC` lists it, and what the module answered to the two read-only forms `AT<cmd>=?` (test) and `AT<cmd>?` (read). Context and findings are in [sim7600a-h-at-reference.md](sim7600a-h-at-reference.md).

How to read it:
- A bare `ERROR` to both forms does **not** prove the command is missing: many commands only have an execute form (`AT+X`) or need parameters. The module answers an unknown name the same way (bare `ERROR`), so "ERROR / ERROR" means "unknown or execute-only".
- A real answer to either form (a range, a value, `+CME ERROR`, `OK`) proves the command exists.
- `not probed (safety)` marks names deliberately never sent (resets, power-off, NV and firmware writes, shells, sending, network connection starters). Most exist on the module because `AT+CLAC` lists them; see the reference for the reasons.
- `not probed` means the name is in the manual or the CLAC list but fell outside the probe lists: the basic `AT&` and `ATx` commands, and the vendor-prefixed `$` names (`$QC…`, `$MY…`), which I left alone because their purpose is unknown.
- Result text is shortened to 70 characters; identifiers, cell IDs and IP addresses are removed. Full sanitized results: [reference/at-probe/](reference/at-probe/).

Totals: 896 names in this table; 374 are in the manual's table of contents; the module's `AT+CLAC` lists 723.

| Command | Manual | In CLAC | `=?` (test) | `?` (read) |
|---|---|---|---|---|
| `AT$CCLK` | — | yes | not probed | not probed |
| `AT$CREG` | — | yes | not probed | not probed |
| `AT$CSQ` | — | yes | not probed | not probed |
| `AT$DLOGCFG` | — | yes | not probed | not probed |
| `AT$MYBAND` | — | yes | not probed | not probed |
| `AT$MYBANDSET` | — | yes | not probed | not probed |
| `AT$MYBCCH` | — | yes | not probed | not probed |
| `AT$MYCCID` | — | yes | not probed | not probed |
| `AT$MYCONFIG` | — | yes | not probed | not probed |
| `AT$MYFTPCLOSE` | — | yes | not probed | not probed |
| `AT$MYFTPEXTPARA` | — | yes | not probed | not probed |
| `AT$MYFTPGET` | — | yes | not probed | not probed |
| `AT$MYFTPOPEN` | — | yes | not probed | not probed |
| `AT$MYFTPPUT` | — | yes | not probed | not probed |
| `AT$MYFTPSIZE` | — | yes | not probed | not probed |
| `AT$MYFTPZMODE` | — | yes | not probed | not probed |
| `AT$MYGMR` | — | yes | not probed | not probed |
| `AT$MYGPSPOS` | — | yes | not probed | not probed |
| `AT$MYIPFILTER` | — | yes | not probed | not probed |
| `AT$MYMODEM` | — | yes | not probed | not probed |
| `AT$MYNETACCEPT` | — | yes | not probed | not probed |
| `AT$MYNETACK` | — | yes | not probed | not probed |
| `AT$MYNETACT` | — | yes | not probed | not probed |
| `AT$MYNETCLOSE` | — | yes | not probed | not probed |
| `AT$MYNETCON` | — | yes | not probed | not probed |
| `AT$MYNETCREATE` | — | yes | not probed | not probed |
| `AT$MYNETINFO` | — | yes | not probed | not probed |
| `AT$MYNETLOCALPORT` | — | yes | not probed | not probed |
| `AT$MYNETOPEN` | — | yes | not probed | not probed |
| `AT$MYNETREAD` | — | yes | not probed | not probed |
| `AT$MYNETSRV` | — | yes | not probed | not probed |
| `AT$MYNETURC` | — | yes | not probed | not probed |
| `AT$MYNETWRITE` | — | yes | not probed | not probed |
| `AT$MYPING` | — | yes | not probed | not probed |
| `AT$MYPOWEROFF` | — | yes | not probed | not probed |
| `AT$MYSOCKETLED` | — | yes | not probed | not probed |
| `AT$MYSYSINFO` | — | yes | not probed | not probed |
| `AT$MYSYSMODE` | — | yes | not probed | not probed |
| `AT$MYTYPE` | — | yes | not probed | not probed |
| `AT$MYURCSYSINFO` | — | yes | not probed | not probed |
| `AT$QCACQDBC` | — | yes | not probed | not probed |
| `AT$QCANTE` | — | yes | not probed | not probed |
| `AT$QCAPNE` | — | yes | not probed | not probed |
| `AT$QCATMOD` | — | yes | not probed | not probed |
| `AT$QCBANDPREF` | — | yes | not probed | not probed |
| `AT$QCBOOTVER` | — | yes | not probed | not probed |
| `AT$QCCLAC` | — | yes | not probed | not probed |
| `AT$QCCLR` | — | yes | not probed | not probed |
| `AT$QCCNMI` | — | yes | not probed | not probed |
| `AT$QCCOPS` | — | yes | not probed | not probed |
| `AT$QCCSGCOPS` | — | yes | not probed | not probed |
| `AT$QCCTM` | — | yes | not probed | not probed |
| `AT$QCDEFPROF` | — | yes | not probed | not probed |
| `AT$QCDGEN` | — | yes | not probed | not probed |
| `AT$QCDMR` | — | yes | not probed | not probed |
| `AT$QCDNSP` | — | yes | not probed | not probed |
| `AT$QCDNSS` | — | yes | not probed | not probed |
| `AT$QCDRX` | — | yes | not probed | not probed |
| `AT$QCHWREV` | — | yes | not probed | not probed |
| `AT$QCMRUC` | — | yes | not probed | not probed |
| `AT$QCMRUE` | — | yes | not probed | not probed |
| `AT$QCNSP` | — | yes | not probed | not probed |
| `AT$QCPBMPREF` | — | yes | not probed | not probed |
| `AT$QCPDPCFGE` | — | yes | not probed | not probed |
| `AT$QCPDPCFGEXT` | — | yes | not probed | not probed |
| `AT$QCPDPIMSCFGE` | — | yes | not probed | not probed |
| `AT$QCPDPLT` | — | yes | not probed | not probed |
| `AT$QCPDPP` | — | yes | not probed | not probed |
| `AT$QCPINSTAT` | — | yes | not probed | not probed |
| `AT$QCPRFCRT` | — | yes | not probed | not probed |
| `AT$QCPRFMOD` | — | yes | not probed | not probed |
| `AT$QCPWRDN` | — | yes | not probed | not probed |
| `AT$QCRCIND` | — | yes | not probed | not probed |
| `AT$QCRMCALL` | — | yes | not probed | not probed |
| `AT$QCRPW` | — | yes | not probed | not probed |
| `AT$QCRSRP` | — | yes | not probed | not probed |
| `AT$QCRSRQ` | — | yes | not probed | not probed |
| `AT$QCSIMAPP` | — | yes | not probed | not probed |
| `AT$QCSIMSTAT` | — | yes | not probed | not probed |
| `AT$QCSIMT` | — | yes | not probed | not probed |
| `AT$QCSQ` | — | yes | not probed | not probed |
| `AT$QCSYSMODE` | — | yes | not probed | not probed |
| `AT$QCTER` | — | yes | not probed | not probed |
| `AT$QCVOLT` | — | yes | not probed | not probed |
| `AT$SLTETA` | — | yes | not probed | not probed |
| `AT$SRFINFO` | — | yes | not probed | not probed |
| `AT*CNTI` | — | yes | *CNTI: (0-2) | *CNTI: 0,NONE |
| `AT+AUTOCSQ` | §3.2.9 p.66 | yes | +AUTOCSQ: (0-1),(0-1) | +AUTOCSQ: 0,0 |
| `AT+AYLACFG` | — | yes | not probed (safety) | not probed (safety) |
| `AT+AYLADEBUG` | — | yes | not probed (safety) | not probed (safety) |
| `AT+AYLAFCFG` | — | yes | not probed (safety) | not probed (safety) |
| `AT+AYLAFRESET` | — | yes | not probed (safety) | not probed (safety) |
| `AT+AYLARECV` | — | yes | not probed (safety) | not probed (safety) |
| `AT+AYLASEND` | — | yes | not probed (safety) | not probed (safety) |
| `AT+AYLASERVICE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+AYLASTATUS` | — | yes | not probed (safety) | not probed (safety) |
| `AT+AYLATEMPLATE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+BOOTLDR` | — | yes | not probed (safety) | not probed (safety) |
| `AT+BT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTACT` | §28.2.13 p.504 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTCONN` | §28.2.24 p.510 | no | ERROR | ERROR |
| `AT+BTGATTCRECHAR` | §28.2.16 p.505 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTCRECHARDES` | §28.2.17 p.506 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTCREDB` | §28.2.14 p.504 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTCRESRV` | §28.2.15 p.505 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTNOTIFY` | §28.2.21 p.509 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTREADCFM` | §28.2.19 p.507 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTREADIND` | §28.2.25 p.511 | no | ERROR | ERROR |
| `AT+BTGATTREG` | §28.2.12 p.503 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTSENDIND` | §28.2.22 p.509 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTSRVADD` | §28.2.18 p.507 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTWRCFM` | §28.2.20 p.508 | yes | not probed (safety) | not probed (safety) |
| `AT+BTGATTWRIND` | §28.2.26 p.511 | no | ERROR | ERROR |
| `AT+BTHOST` | §28.2.2 p.495 | yes | not probed (safety) | not probed (safety) |
| `AT+BTIOCAP` | §28.2.4 p.496 | yes | not probed (safety) | not probed (safety) |
| `AT+BTPAIR` | §28.2.5 p.497 | yes | not probed (safety) | not probed (safety) |
| `AT+BTPAIRED` | §28.2.7 p.499 | yes | not probed (safety) | not probed (safety) |
| `AT+BTPIN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+BTPOWER` | §28.2.1 p.494 | yes | not probed (safety) | not probed (safety) |
| `AT+BTSCAN` | §28.2.3 p.495 | yes | not probed (safety) | not probed (safety) |
| `AT+BTSETSCAN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+BTSPPCONN` | §28.2.10 p.501 | yes | not probed (safety) | not probed (safety) |
| `AT+BTSPPPROF` | §28.2.9 p.501 | yes | not probed (safety) | not probed (safety) |
| `AT+BTSPPRECV` | §28.2.23 p.510 | no | ERROR | ERROR |
| `AT+BTSPPSEND` | §28.2.11 p.502 | yes | not probed (safety) | not probed (safety) |
| `AT+BTSPPSRV` | §28.2.8 p.500 | yes | not probed (safety) | not probed (safety) |
| `AT+BTUNPAIR` | §28.2.6 p.499 | yes | not probed (safety) | not probed (safety) |
| `AT+CACDBFN` | §5.2.24 p.144 | yes | +CACDBFN: (Handset_cal.acdb,Handset_dingxin.acdb,Handset_tianmai.acdb) | +CACDBFN: Handset_cal.acdb |
| `AT+CACM` | §3.2.14 p.70 | no | (empty) | +CME ERROR: SIM busy |
| `AT+CADC` | §19.2.4 p.403 | yes | +CADC: (0,2) | ERROR |
| `AT+CADC0` | — | yes | +CADC0: (0,2) | ERROR |
| `AT+CADC2` | §19.2.5 p.404 | yes | +CADC2: (0,2) | ERROR |
| `AT+CAGPS` | — | no | ERROR | ERROR |
| `AT+CALA` | — | no | ERROR | ERROR |
| `AT+CALC1` | — | yes | +CALC1: (0x00-0x1FF) | +CALC1: |
| `AT+CALC2` | — | yes | +CALC2: (0x00-0x1FF) | +CALC2: |
| `AT+CALC3` | — | yes | +CALC3: (0x00-0x1FF) | +CALC3: |
| `AT+CALCN` | — | yes | +CALCN: (0x00-0x1FF) | +CALCN: |
| `AT+CALD` | — | no | ERROR | ERROR |
| `AT+CALM` | — | yes | ERROR | ERROR |
| `AT+CAMM` | §3.2.15 p.71 | no | (empty) | +CME ERROR: SIM busy |
| `AT+CAOC` | §4.2.7 p.94 | no | +CAOC: (0-2) | +CAOC: 1 |
| `AT+CAPFOTA` | §23.2.1 p.451 | yes | +CAPFOTA: (0-1) | +CAPFOTA: 0 |
| `AT+CAPFS` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CAPNET` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CARECV` | — | no | ERROR | ERROR |
| `AT+CASSISTLOC` | §18.2.3 p.395 | yes | ERROR | ERROR |
| `AT+CASSLCFG` | — | no | ERROR | ERROR |
| `AT+CATR` | §3.2.11 p.68 | yes | +CATR: (0-7) | +CATR: 0 |
| `AT+CAUDIOMODE` | — | no | ERROR | ERROR |
| `AT+CAUDPATH` | — | no | ERROR | ERROR |
| `AT+CAUTH` | — | no | ERROR | ERROR |
| `AT+CAUTOAPN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CAUTONET` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CBACKUPEFS2` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CBACKUPRESULT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CBAND` | — | no | ERROR | ERROR |
| `AT+CBANDCFG` | — | no | ERROR | ERROR |
| `AT+CBATCHK` | — | no | ERROR | ERROR |
| `AT+CBC` | §19.2.11 p.409 | yes | ERROR | ERROR |
| `AT+CBCINFO` | — | no | ERROR | ERROR |
| `AT+CBDS` | — | yes | +CBDS: (0,1) | +CBDS: 0 |
| `AT+CBST` | §5.2.3 p.118 | yes | +CBST: (0,7,12,14,16,17,39,43,48,51,71,75,80,81,83,84,116,134),(0,1,4)… | +CBST: 0,0,1 |
| `AT+CCAUTH` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CCDEINIT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CCED` | — | yes | ERROR | <redacted: identifier, location or private data> |
| `AT+CCERTDELE` | §10.2.15 p.268 | yes | (empty) | ERROR |
| `AT+CCERTDOWN` | §10.2.13 p.266 | yes | +CCERTDOWN: (5-128),(1-10240) | ERROR |
| `AT+CCERTLIST` | §10.2.14 p.267 | yes | (empty) | ERROR |
| `AT+CCERTVERIFY` | — | yes | +CCERTVERIFY: (0-3) | +CCERTVERIFY: 0 |
| `AT+CCFC` | §5.2.11 p.128 | no | +CCFC: (0,1,2,3,4,5) | ERROR |
| `AT+CCGMDF` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CCHADDR` | §10.2.7 p.256 | yes | ERROR | ERROR |
| `AT+CCHCFG` | §10.2.8 p.257 | yes | +CCHCFG: "sendtimeout",(0-1),(60-150) \| +CCHCFG: "sslctx",(0-1),(0-9) | +CCHCFG: 0,, \| +CCHCFG: 1,, |
| `AT+CCHCLOSE` | §10.2.4 p.253 | yes | ERROR | ERROR |
| `AT+CCHMODE` | §10.2.10 p.259 | yes | +CCHMODE: (0,1) | +CCHMODE: 0 |
| `AT+CCHOPEN` | §10.2.3 p.251 | yes | +CCHOPEN: (0,1),"ADDRESS",(1-65535)[,(1-2)[,(1-65535)]] | +CCHOPEN: 0,"",,, \| +CCHOPEN: 1,"",,, |
| `AT+CCHRECV` | §10.2.6 p.255 | yes | ERROR | +CCHRECV: LEN,0,0 |
| `AT+CCHSEND` | §10.2.5 p.253 | yes | +CCHSEND: (0,1),(1-2048) | +CCHSEND: 0,0,1,0 |
| `AT+CCHSET` | §10.2.11 p.260 | yes | +CCHSET: (0,1),(0,1) | +CCHSET: 0,0 |
| `AT+CCHSSLCFG` | §10.2.9 p.258 | yes | +CCHSSLCFG: (0,1),(0-9) | +CCHSSLCFG: 0, \| +CCHSSLCFG: 1, |
| `AT+CCHSTART` | §10.2.1 p.249 | yes | ERROR | ERROR |
| `AT+CCHSTOP` | §10.2.2 p.250 | yes | ERROR | ERROR |
| `AT+CCINFO` | — | yes | (empty) | ERROR |
| `AT+CCINIT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CCLASS` | — | yes | ERROR | +CCLASS: 33,33 |
| `AT+CCLK` | §3.2.17 p.73 | no | (empty) | +CCLK: "26/10/18,14:27:21-24" |
| `AT+CCMXPAUSE` | — | yes | (empty) | ERROR |
| `AT+CCMXPLAY` | §21.2.5 p.443 | yes | +CCMXPLAY: (0-1),(0-255) | ERROR |
| `AT+CCMXPLAYWAV` | §21.2.3 p.441 | yes | +CCMXPLAYWAV: (1-2),(0-255) | ERROR |
| `AT+CCMXRESUME` | — | yes | (empty) | ERROR |
| `AT+CCMXSTOP` | §21.2.6 p.444 | yes | (empty) | ERROR |
| `AT+CCMXSTOPWAV` | §21.2.4 p.442 | yes | (empty) | ERROR |
| `AT+CCODECINGAIN` | — | yes | +CCODECINGAIN: (0x00-0xFF) | +CCODECINGAIN: |
| `AT+CCODECOUTGAIN` | — | yes | +CCODECOUTGAIN: (0-1),(0x00-0xFF) | +CCODECOUTGAIN: , |
| `AT+CCPUTEMP` | — | yes | +CCPUTEMP: (0-5) | ERROR |
| `AT+CCSEND` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CCUART` | — | yes | +CCUART: (0-1) | +CCUART: 0 |
| `AT+CCUG` | §4.2.5 p.91 | no | (empty) | +CCUG: 0,0,0 |
| `AT+CCUSAGE` | — | no | ERROR | ERROR |
| `AT+CCWA` | §5.2.9 p.126 | no | +CCWA: (0-1) | +CCWA: 0 |
| `AT+CCYIELD` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CDATAMSG` | — | yes | ERROR | ERROR |
| `AT+CDELTA` | §19.2.8 p.407 | yes | ERROR | ERROR |
| `AT+CDEVPORT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CDIP` | — | no | +CDIP: (0-1) | ERROR |
| `AT+CDMPOWER` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CDNSGHNAME` | §11.3.2 p.296 | yes | (empty) | ERROR |
| `AT+CDNSGIP` | §11.3.1 p.295 | yes | (empty) | ERROR |
| `AT+CDNSSRV` | — | yes | ERROR | (empty) |
| `AT+CDTAM` | §22.2.1 p.446 | yes | +CDTAM: (0-1) | +CDTAM: 0 |
| `AT+CECALLCFG` | §26.2.3 p.463 | no | ERROR | ERROR |
| `AT+CECALLE` | §26.2.2 p.462 | no | ERROR | ERROR |
| `AT+CECALLPOS` | §26.2.4 p.464 | no | ERROR | ERROR |
| `AT+CECALLS` | §26.2.1 p.461 | no | ERROR | ERROR |
| `AT+CECALLTIME` | §26.2.5 p.465 | no | ERROR | ERROR |
| `AT+CECALLTOUT` | §26.2.7 p.467 | no | ERROR | ERROR |
| `AT+CECALLVERSION` | §26.2.6 p.466 | no | ERROR | ERROR |
| `AT+CECDT` | §5.2.32 p.151 | yes | +CECDT: (0x0000-0x7FFF) | ERROR |
| `AT+CECH` | §5.2.31 p.150 | yes | +CECH: (0x0000-0x7FFF) | ERROR |
| `AT+CECM` | §5.2.42 p.160 | yes | +CECM: (0-9) | +CECM: 0 |
| `AT+CECRX` | §5.2.39 p.157 | yes | +CECRX: (0,1) | ERROR |
| `AT+CECWB` | §5.2.33 p.151 | yes | +CECWB: (0x0000-0x7FFF) | ERROR |
| `AT+CEDUMP` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CEER` | §5.2.8 p.125 | yes | (empty) | ERROR |
| `AT+CELLLOCK` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CEMNL` | — | no | ERROR | ERROR |
| `AT+CEMODE` | — | yes | +CEMODE: (0-3) | +CEMODE: 1 |
| `AT+CENABLELAN` | §25.2.1 p.456 | yes | +CENABLELAN: (0-1) | +CENABLELAN: 0 |
| `AT+CEQ2` | — | yes | +CEQ2: (0x100-0x17F,0x00-0xFF) | +CEQ2: |
| `AT+CEQ3` | — | yes | +CEQ3: (0x100-0x17F,0x00-0xFF) | +CEQ3: |
| `AT+CEQ4` | — | yes | +CEQ4: (0x100-0x17F,0x00-0xFF) | +CEQ4: |
| `AT+CEQ5` | — | yes | +CEQ5: (0x00-0xFF) | +CEQ5: |
| `AT+CEQM` | — | yes | +CEQM: (0x100-0x17F,0x00-0xFF) | +CEQM: |
| `AT+CEREG` | §4.2.16 p.110 | yes | +CEREG: (0-2) | +CEREG: 0,1 |
| `AT+CESQ` | — | yes | +CESQ: (0-63,99),(0-7,99),(0-96,255),(0-49,255),(0-34,255),(0-97,255) | ERROR |
| `AT+CEXTEPLMN` | — | yes | +CEXTEPLMN: (0-1) | +CEXTEPLMN: 1 |
| `AT+CEXTERNTONE` | — | no | ERROR | ERROR |
| `AT+CEXTPLMNBAND` | — | yes | ERROR | +CEXTPLMNBAND: 0 |
| `AT+CFDISK` | §19.2.13 p.410 | yes | +CFDISK: (1-4)[...] | (empty) |
| `AT+CFGRI` | §19.2.16 p.413 | yes | +CFGRI: (0-1),(10-6000),(20-6000) | +CFGRI: 0,60,120 |
| `AT+CFGRIDTM` | — | yes | +CFGRIDTM: (10-6000),(20-6000) | +CFGRIDTM: 60,120 |
| `AT+CFLAUR` | — | yes | ERROR | ERROR |
| `AT+CFNSIN` | §5.2.37 p.155 | yes | +CFNSIN: (0x2000-0x7FFF) | ERROR |
| `AT+CFNSLVL` | §5.2.38 p.156 | yes | +CFNSLVL: (0x0000-0x7FFF) | ERROR |
| `AT+CFNSMOD` | §5.2.36 p.154 | yes | +CFNSMOD: (0x00FF,0x0073,0x00F3,0x01FF) | ERROR |
| `AT+CFOTASWITCH` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFRAUR` | — | yes | ERROR | ERROR |
| `AT+CFTPCACHERD` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPDELE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPDL` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPDLEXIT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPDLINFO` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPDLTO` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPGET` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPGETFILE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPGETSIZE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPLIST` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPMKD` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPMODE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPPORT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPPUT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPPUTFILE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPPW` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPRMD` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSABORT` | §12.2.18 p.322 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSCACHERD` | §12.2.17 p.321 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSCWD` | §12.2.8 p.310 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSDELE` | §12.2.7 p.309 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSDL` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSERV` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSGET` | §12.2.14 p.317 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSGETFILE` | §12.2.12 p.314 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSINGLEIP` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSLIST` | §12.2.11 p.313 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSLOGIN` | §12.2.3 p.305 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSLOGOUT` | §12.2.4 p.306 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSMKD` | §12.2.5 p.307 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSPUT` | §12.2.15 p.319 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSPUTFILE` | §12.2.13 p.316 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSPWD` | §12.2.9 p.311 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSRMD` | §12.2.6 p.308 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSSINGLEIP` | §12.2.16 p.320 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSSIZE` | §12.2.19 p.323 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSSLCFG` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSSTART` | §12.2.1 p.303 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSSTOP` | §12.2.2 p.304 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPSTYPE` | §12.2.10 p.312 | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPTYPE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTPUN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CFTRANRX` | §20.2.11 p.435 | yes | +CFTRANRX: [{non-ascii}]"FILEPATH" | ERROR |
| `AT+CFTRANTX` | §20.2.12 p.436 | yes | +CFTRANTX: [{non-ascii}]"FILEPATH" | ERROR |
| `AT+CFUN` | §3.2.1 p.55 | yes | not probed (safety) | not probed (safety) |
| `AT+CGACT` | §8.2.3 p.187 | no | +CGACT: (0,1) | +CGACT: 1,1 \| +CGACT: 2,1 \| +CGACT: 3,0 |
| `AT+CGATT` | §8.2.2 p.186 | no | +CGATT: (0,1) | +CGATT: 1 |
| `AT+CGAUTH` | §8.2.15 p.214 | yes | +CGAUTH: ,,127,127(for CDMA1x-EvDo only) \| +CGAUTH: (1-24,100-179),(0… | +CGAUTH: 1,0 \| +CGAUTH: 2,0 \| +CGAUTH: 3,0 |
| `AT+CGCLASS` | §8.2.13 p.211 | yes | +CGCLASS: ("A") | +CGCLASS: "A" |
| `AT+CGCONTRDP` | — | yes | +CGCONTRDP: ( 1,2 ) | <redacted: identifier, location or private data> |
| `AT+CGDATA` | §8.2.11 p.208 | yes | +CGDATA: ("PPP") | ERROR |
| `AT+CGDCONT` | §8.2.4 p.188 | yes | +CGDCONT: (1-24,100-179),"IP",,,(0-2),(0-4),(0-1),(0-1) \| +CGDCONT: (… | +CGDCONT: 1,"IP","hologram","<ip>",0,0,0,0 \| +CGDCONT: 2,"IPV4V6","im… |
| `AT+CGDRT` | §19.2.20 p.418 | yes | +CGDRT: (3,6,40,41,43,44,77),(0,1) | ERROR |
| `AT+CGDSCONT` | §8.2.5 p.190 | yes | +CGDSCONT: (1-24,100-179),(2,3),"IP",(0-2),(0-4) \| +CGDSCONT: (1-24,1… | +CGDSCONT: |
| `AT+CGEQMIN` | §8.2.10 p.204 | yes | +CGEQMIN: "IP",(0-4),(0-11520),(0-42200),(0-11520),(0-42200),(0-2),(0-… | +CGEQMIN: |
| `AT+CGEQOS` | — | yes | +CGEQOS: (1-24,100-179),(0-9),(0-150000),(0-50000),(0-150000),(0-50000… | +CGEQOS: |
| `AT+CGEQOSRDP` | — | yes | (empty) | ERROR |
| `AT+CGEQREQ` | §8.2.8 p.198 | yes | +CGEQREQ: "IP",(0-4),(0-11520),(0-42200),(0-11520),(0-42200),(0-2),(0-… | +CGEQREQ: 2,2,0,0,0,0,2,0,"0E0","0E0",3,0,1,0,1 |
| `AT+CGEREP` | §8.2.14 p.212 | yes | +CGEREP: (0-2),(0-1) | +CGEREP: 0,0 |
| `AT+CGFUNC` | §19.2.19 p.417 | yes | +CGFUNC: (40,44),(0-1) | ERROR |
| `AT+CGGETV` | §19.2.22 p.420 | yes | +CGGETV: (3,6,40,41,43,44,77) | ERROR |
| `AT+CGISR` | §19.2.23 p.421 | yes | +CGISR: (40,41,43,44,77),(0-2),(0-1),(URC char [size(45)]) | ERROR |
| `AT+CGMI` | §2.2.28 p.47 | no | (empty) | ERROR |
| `AT+CGMM` | §2.2.29 p.48 | no | (empty) | ERROR |
| `AT+CGMR` | §2.2.30 p.49 | no | (empty) | ERROR |
| `AT+CGNSCMD` | — | no | ERROR | ERROR |
| `AT+CGNSINF` | — | no | ERROR | ERROR |
| `AT+CGNSSEQ` | — | no | ERROR | ERROR |
| `AT+CGNSSINFO` | §17.2.21 p.387 | yes | +CGNSSINFO: (0-255) | +CGNSSINFO: 0 |
| `AT+CGNSSMODE` | §17.2.22 p.389 | yes | +CGNSSMODE: (0-15),(0-1) | +CGNSSMODE: 1,2 |
| `AT+CGNSTST` | — | no | ERROR | ERROR |
| `AT+CGNSURC` | — | no | ERROR | ERROR |
| `AT+CGPADDR` | §8.2.12 p.209 | yes | +CGPADDR: (1,2,3) | <redacted: identifier, location or private data> |
| `AT+CGPIAF` | — | yes | +CGPIAF: (0-1),(0-1),(0-1),(0-1) | +CGPIAF: 0,0,0,0 |
| `AT+CGPRS` | — | no | ERROR | ERROR |
| `AT+CGPS` | §17.2.1 p.369 | yes | +CGPS: (0-1),(1-3) | +CGPS: 1,1 |
| `AT+CGPSAUTO` | §17.2.7 p.374 | yes | +CGPSAUTO: (0,1) | +CGPSAUTO: 1 |
| `AT+CGPSCOLD` | §17.2.3 p.371 | yes | (empty) | ERROR |
| `AT+CGPSDEL` | §17.2.12 p.379 | yes | (empty) | ERROR |
| `AT+CGPSFTM` | §17.2.11 p.378 | yes | (empty) | +CGPSFTM: 0 |
| `AT+CGPSFTPEPH` | — | no | ERROR | ERROR |
| `AT+CGPSHOR` | §17.2.19 p.386 | yes | +CGPSHOR: (0-1800000) | +CGPSHOR: 50 |
| `AT+CGPSHOT` | §17.2.4 p.372 | yes | (empty) | ERROR |
| `AT+CGPSINFO` | §17.2.2 p.370 | yes | +CGPSINFO: (0-255) | +CGPSINFO: 0 |
| `AT+CGPSINFOCFG` | §17.2.16 p.382 | yes | +CGPSINFOCFG: (0-255),(0-262143) | +CGPSINFOCFG: 0,0 |
| `AT+CGPSIPV6` | §17.2.23 p.390 | no | ERROR | ERROR |
| `AT+CGPSMD` | §17.2.10 p.377 | yes | +CGPSMD: (0,1) | +CGPSMD: 1 |
| `AT+CGPSMODE` | — | yes | +CGPSMODE: (0-2) | +CGPSMODE: 2 |
| `AT+CGPSMSB` | §17.2.18 p.385 | yes | ERROR | ERROR |
| `AT+CGPSNMEA` | §17.2.8 p.375 | yes | +CGPSNMEA: (0-262143) | +CGPSNMEA: 63 |
| `AT+CGPSNMEARATE` | §17.2.9 p.376 | yes | +CGPSNMEARATE: (0,1) | +CGPSNMEARATE: 0 |
| `AT+CGPSNOTIFY` | §17.2.20 p.386 | yes | +CGPSNOTIFY: (0-2) | ERROR |
| `AT+CGPSPMD` | §17.2.17 p.384 | yes | +CGPSPMD: (1-65407) | +CGPSPMD: 65407 |
| `AT+CGPSSSL` | §17.2.6 p.373 | yes | +CGPSSSL: (0,1) | +CGPSSSL: 1 |
| `AT+CGPSSTATUS` | — | no | ERROR | ERROR |
| `AT+CGPSURL` | §17.2.5 p.373 | yes | (empty) | +CGPSURL: "0" |
| `AT+CGPSWARM` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CGPSXD` | §17.2.14 p.380 | yes | +CGPSXD: (0-2) | +CGPSXD: 0 |
| `AT+CGPSXDAUTO` | §17.2.15 p.381 | yes | +CGPSXDAUTO: (0,1) | +CGPSXDAUTO: 0 |
| `AT+CGPSXE` | §17.2.13 p.379 | yes | +CGPSXE: (0,1) | +CGPSXE: 0 |
| `AT+CGPSXTRADATA` | §17.2.24 p.390 | no | not probed | not probed |
| `AT+CGQMIN` | §8.2.9 p.202 | yes | +CGQMIN: "IP",(0-3),(0-4),(0-5),(0-9),(0-18,31) \| +CGQMIN: "PPP",(0-3… | +CGQMIN: |
| `AT+CGQREQ` | §8.2.7 p.195 | yes | +CGQREQ: "IP",(0-3),(0-4),(0-5),(0-9),(0-18,31) \| +CGQREQ: "PPP",(0-3… | +CGQREQ: |
| `AT+CGREG` | §8.2.1 p.184 | yes | +CGREG: (0-2) | +CGREG: 0,1 |
| `AT+CGSCONTRDP` | — | yes | (empty) | ERROR |
| `AT+CGSETI` | — | yes | ERROR | (empty) |
| `AT+CGSETV` | §19.2.21 p.419 | yes | +CGSETV: (3,6,40,41,43,44,77),(0-1) | ERROR |
| `AT+CGSI` | — | yes | ERROR | (empty) |
| `AT+CGSMS` | §9.2.10 p.228 | yes | +CGSMS: (0-3) | +CGSMS: 1 |
| `AT+CGSN` | §2.2.31 p.49 | no | (empty) | <redacted: identifier, location or private data> |
| `AT+CGSOCKCONT` | — | yes | +CGSOCKCONT: (1-24,100-179),"IP",,,(0-2),(0-4),(0-1),(0-1) \| +CGSOCKC… | <redacted: identifier, location or private data> |
| `AT+CGTFT` | §8.2.6 p.192 | yes | +CGTFT: "IP",(1-16),(0-255),,(0-255),(0-65535.0-65535),(0-65535.0-6553… | +CGTFT: |
| `AT+CGTFTRDP` | — | yes | (empty) | ERROR |
| `AT+CGTP` | — | yes | (empty) | +CGTP: 1,1,,443,,443 |
| `AT+CHLD` | §5.2.10 p.127 | no | +CHLD: (0,1,1x,2,2x,3,4) | ERROR |
| `AT+CHOLD` | — | no | ERROR | ERROR |
| `AT+CHSET` | — | no | ERROR | ERROR |
| `AT+CHSICDEVSLEEP` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CHSICSLEEP` | §25.2.4 p.459 | yes | +CHSICSLEEP: (0-1) | +CHSICSLEEP: 1 |
| `AT+CHSN` | — | yes | +CHSN: (0-2,4,7),(0),(0),(0) | +CHSN: 0,0,0,0 |
| `AT+CHTPSERV` | §14.2.1 p.340 | yes | +CHTPSERV: "ADD","HOST",(1-65535),(0-1)[,"PROXY",(1-65535)] \| +CHTPSE… | (empty) |
| `AT+CHTPUPDATE` | §14.2.2 p.341 | yes | (empty) | +CHTPUPDATE: NULL |
| `AT+CHTTPACT` | — | yes | +CHTTPACT: "ADDRESS",(1-65535) | ERROR |
| `AT+CHTTPSCLSE` | — | yes | ERROR | ERROR |
| `AT+CHTTPSDL` | — | yes | ERROR | +CHTTPSDL: 0 |
| `AT+CHTTPSHEAD` | — | yes | ERROR | +CHTTPSHEAD: 0 |
| `AT+CHTTPSOPSE` | — | yes | +CHTTPSOPSE:"ADDRESS",(1-65535)[,(1-2)] | ERROR |
| `AT+CHTTPSRECV` | — | yes | ERROR | +CHTTPSRECV: LEN,0 |
| `AT+CHTTPSSEND` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CHTTPSSLCFG` | — | yes | +CHTTPSSLCFG: (0-9) | +CHTTPSSLCFG: |
| `AT+CHTTPSSTART` | — | yes | ERROR | ERROR |
| `AT+CHTTPSSTATE` | — | yes | ERROR | ERROR |
| `AT+CHTTPSSTOP` | — | yes | ERROR | ERROR |
| `AT+CHUP` | §5.2.2 p.117 | no | not probed | not probed |
| `AT+CICCID` | §3.2.3 p.57 | no | (empty) | <redacted: identifier, location or private data> |
| `AT+CIFSR` | — | no | ERROR | ERROR |
| `AT+CIMI` | §2.2.33 p.51 | no | (empty) | <redacted: identifier, location or private data> |
| `AT+CIMIM` | §2.2.34 p.52 | no | (empty) | ERROR |
| `AT+CIND` | — | no | +CIND: ("battchg",(0-5)),("signal",(0-5)),("service",(0-1)),("call",(0… | +CIND: 0,3,1,0,0,0,1,0 |
| `AT+CINICMPALLOW` | — | yes | +CINICMPALLOW: (0-0xFFFFFFFFFFFFFFFF) | +CINICMPALLOW: 0x00000000FFFFFFFF |
| `AT+CINQSN` | — | yes | ERROR | <redacted: identifier, location or private data> |
| `AT+CIPACK` | §11.2.16 p.293 | yes | +CIPACK: (0-9) | ERROR |
| `AT+CIPACT` | — | yes | ERROR | <redacted: identifier, location or private data> |
| `AT+CIPCCFG` | §11.2.13 p.290 | yes | +CIPCCFG: (0-10),(0-1000),(0),(0-1),(0-1),(0-1),(500-120000) | +CIPCCFG: 10,0,0,1,0,0,500 |
| `AT+CIPCLOSE` | §11.2.4 p.276 | yes | +CIPCLOSE: (0-9) | +CIPCLOSE: 0,0,0,0,0,0,0,0,0,0 |
| `AT+CIPCSGP` | — | no | ERROR | ERROR |
| `AT+CIPDNS` | — | no | ERROR | ERROR |
| `AT+CIPDNSSET` | §11.3.3 p.297 | yes | (empty) | +CIPDNSSET: 3,30000,7 |
| `AT+CIPDPDP` | — | no | ERROR | ERROR |
| `AT+CIPENFSDORM` | — | yes | +CIPENFSDORM: (0-1) | +CIPENFSDORM: 0 |
| `AT+CIPENSRXGET` | — | yes | +CIPENSRXGET: (0-1) | +CIPENSRXGET: 0 |
| `AT+CIPFILTERSET` | — | yes | +CIPFILTERSET: (0-9),(0,1) | +CIPFILTERSET: 1 \| +CIPFILTERSET: 0,1 \| +CIPFILTERSET: 1,1 \| +CIPFI… |
| `AT+CIPHEAD` | §11.2.8 p.285 | yes | +CIPHEAD: (0-1) | +CIPHEAD: 1 |
| `AT+CIPMODE` | §11.2.10 p.287 | yes | +CIPMODE: (0-1) | +CIPMODE: 0 |
| `AT+CIPMUX` | — | no | ERROR | ERROR |
| `AT+CIPOPEN` | §11.2.3 p.273 | yes | +CIPOPEN: (0-9),("TCP","UDP") | +CIPOPEN: 0 \| +CIPOPEN: 1 \| +CIPOPEN: 2 \| +CIPOPEN: 3 \| +CIPOPEN: … |
| `AT+CIPPING` | — | no | ERROR | ERROR |
| `AT+CIPQSEND` | — | yes | +CIPQSEND: (0-9),(1-1500) | (empty) |
| `AT+CIPRXGET` | §11.2.6 p.281 | yes | +CIPRXGET: (0-4),(0-9),(1-1500) | +CIPRXGET: 0 |
| `AT+CIPSCONT` | — | no | ERROR | ERROR |
| `AT+CIPSEND` | §11.2.5 p.277 | yes | not probed (safety) | not probed (safety) |
| `AT+CIPSENDMODE` | §11.2.11 p.288 | yes | +CIPSENDMODE: (0,1) | +CIPSENDMODE: 0 |
| `AT+CIPSPRT` | — | no | ERROR | ERROR |
| `AT+CIPSRIP` | §11.2.9 p.286 | yes | +CIPSRIP: (0-1) | +CIPSRIP: 1 |
| `AT+CIPSTAT` | — | yes | +CIPSTAT: (0-9) | ERROR |
| `AT+CIPSWTMD` | — | yes | +CIPSWTMD: (0,1) | +CIPSWTMD: 0 |
| `AT+CIPTIMEOUT` | §11.2.12 p.289 | yes | (empty) | +CIPTIMEOUT:120000,120000,120000 |
| `AT+CIPTIMER` | — | no | ERROR | ERROR |
| `AT+CLANCTRL` | §25.2.3 p.458 | yes | +CLANCTRL: (0-5) | ERROR |
| `AT+CLANMODE` | §25.2.2 p.457 | yes | +CLANMODE: (0-2) | +CLANMODE: 0 |
| `AT+CLARFCN` | — | yes | ERROR | ERROR |
| `AT+CLBS` | §18.2.1 p.392 | yes | +CLBS: (1,2,3,4,9),(1-24,100-179),(-180.000000-180.000000),(-90.000000… | <redacted: identifier, location or private data> |
| `AT+CLBSCFG` | §18.2.2 p.394 | yes | +CLBSCFG: (0-1),3,"Param Value" | ERROR |
| `AT+CLCC` | §5.2.7 p.123 | yes | +CLCC: (0-1) | +CLCC: 0 |
| `AT+CLCCEX` | — | yes | ERROR | ERROR |
| `AT+CLCK` | §4.2.3 p.88 | no | not probed | not probed |
| `AT+CLDCH` | — | yes | +CLDCH: (0-1) | +CLDCH: 0 |
| `AT+CLEARFCN` | — | yes | ERROR | +CLEARFCN: 0, 0 |
| `AT+CLECELL` | — | yes | ERROR | +CLECELL: 0, 0 |
| `AT+CLEDITST` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CLEDITT` | — | no | ERROR | ERROR |
| `AT+CLGCELL` | — | yes | ERROR | ERROR |
| `AT+CLIP` | §5.2.12 p.130 | no | +CLIP: (0-1) | ERROR |
| `AT+CLIR` | §5.2.13 p.132 | no | +CLIR: (0-2) | ERROR |
| `AT+CLOOPBACK` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CLTEREJTIMER` | — | yes | ERROR | +CLTEREJTIMER: 60000 |
| `AT+CLUARFCN` | — | yes | ERROR | ERROR |
| `AT+CLUCELL` | — | yes | ERROR | ERROR |
| `AT+CLVL` | §5.2.22 p.142 | yes | +CLVL: (0-5) | +CLVL: 4 |
| `AT+CMAUTH` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CMAUTOYIELD` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CMCFG` | — | yes | ERROR | ERROR |
| `AT+CMCONN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CMCRXD` | — | yes | ERROR | ERROR |
| `AT+CMDISCONN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CMDLOG` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CMEDIA` | — | no | ERROR | ERROR |
| `AT+CMEDIAVOL` | — | no | ERROR | ERROR |
| `AT+CMEE` | §3.2.18 p.75 | yes | +CMEE: (0,1,2) | +CMEE: 2 |
| `AT+CMER` | — | no | +CMER: (0-3),(0),(0),(0-1),(0-1) | +CMER: 0,0,0,0,0 |
| `AT+CMGD` | §9.2.16 p.241 | no | not probed | not probed |
| `AT+CMGEF` | — | no | ERROR | ERROR |
| `AT+CMGF` | §9.2.3 p.219 | yes | +CMGF: (0-1) | +CMGF: 1 |
| `AT+CMGL` | §9.2.11 p.229 | no | +CMGL: ("REC UNREAD","REC READ","STO UNSENT","STO SENT","ALL") | <redacted: identifier, location or private data> |
| `AT+CMGMT` | §9.2.17 p.242 | no | (empty) | ERROR |
| `AT+CMGP` | §9.2.22 p.247 | no | ERROR | ERROR |
| `AT+CMGR` | §9.2.12 p.233 | no | (empty) | <redacted: identifier, location or private data> |
| `AT+CMGRD` | §9.2.19 p.244 | no | (empty) | ERROR |
| `AT+CMGRMI` | — | yes | +CMGRMI: (4) | ERROR |
| `AT+CMGS` | §9.2.13 p.237 | no | not probed | not probed |
| `AT+CMGSEX` | §9.2.20 p.245 | no | not probed | not probed |
| `AT+CMGSI` | — | yes | +CMGSI: (2-5) | ERROR |
| `AT+CMGW` | §9.2.15 p.239 | no | not probed | not probed |
| `AT+CMIC` | — | yes | +CMIC: (1) | +CMIC: 1 |
| `AT+CMICGAIN` | §5.2.26 p.145 | yes | +CMICGAIN: (0-8) | +CMICGAIN: 3 |
| `AT+CMMS` | — | no | +CMMS: (0-2) | +CMMS: 0 |
| `AT+CMNB` | — | no | ERROR | ERROR |
| `AT+CMOD` | §5.2.18 p.138 | yes | +CMOD: (0) | +CMOD: 0 |
| `AT+CMOUTMT` | — | yes | +CMOUTMT: (0x00-0x1FF) | +CMOUTMT: |
| `AT+CMPARA` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CMPUB` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CMQTTACCQ` | §16.2.3 p.347 | yes | +CMQTTACCQ: (0-1),(1-128),(0-1) | +CMQTTACCQ: 0,"", \| +CMQTTACCQ: 1,"", |
| `AT+CMQTTCFG` | §16.2.17 p.363 | yes | +CMQTTCFG: "checkUTF8",(0-1),(0-1) \| +CMQTTCFG: "optimeout",(0-1),(20… | +CMQTTCFG: 0,1,120 \| +CMQTTCFG: 1,1,120 |
| `AT+CMQTTCNCTTIMEOUT` | — | yes | +CMQTTCNCTTIMEOUT: (0-1),(0,10-180) | +CMQTTCNCTTIMEOUT: 0 \| +CMQTTCNCTTIMEOUT: 1 |
| `AT+CMQTTCONNECT` | §16.2.9 p.354 | yes | +CMQTTCONNECT: (0-1),(9-256),(1-64800),(0-1) | +CMQTTCONNECT: 0 \| +CMQTTCONNECT: 1 |
| `AT+CMQTTDISC` | §16.2.8 p.353 | yes | +CMQTTDISC: (0-1),(0,60-180) | +CMQTTDISC: 0,1 \| +CMQTTDISC: 1,1 |
| `AT+CMQTTPAYLOAD` | §16.2.11 p.356 | yes | +CMQTTPAYLOAD: (0-1),(1-10240) | (empty) |
| `AT+CMQTTPING` | — | yes | +CMQTTPING: (0-1),(0-2),(60-180),(0-1),(0-1) | (empty) |
| `AT+CMQTTPUB` | §16.2.12 p.357 | yes | +CMQTTPUB: (0-1),(0-2),(60-180),(0-1),(0-1) | (empty) |
| `AT+CMQTTREL` | §16.2.4 p.349 | yes | +CMQTTREL: (0-1) | (empty) |
| `AT+CMQTTSSLCFG` | §16.2.5 p.350 | yes | +CMQTTSSLCFG: (0,1),(0-9) | +CMQTTSSLCFG: 0, \| +CMQTTSSLCFG: 1, |
| `AT+CMQTTSTART` | §16.2.1 p.345 | yes | ERROR | ERROR |
| `AT+CMQTTSTOP` | §16.2.2 p.346 | yes | ERROR | ERROR |
| `AT+CMQTTSUB` | §16.2.14 p.360 | yes | +CMQTTSUB: (0-1),(1-1024),(0-2),(0-1) | (empty) |
| `AT+CMQTTSUBTOPIC` | §16.2.13 p.359 | yes | +CMQTTSUBTOPIC: (0-1),(1-1024),(0-2) | (empty) |
| `AT+CMQTTTOPIC` | §16.2.10 p.355 | yes | +CMQTTTOPIC: (0-1),(1-1024) | (empty) |
| `AT+CMQTTUNSUB` | §16.2.16 p.362 | yes | +CMQTTUNSUB: (0-1),(1-1024),(0-1) | (empty) |
| `AT+CMQTTUNSUBTOPIC` | §16.2.15 p.361 | yes | +CMQTTUNSUBTOPIC: (0-1),(1-1024) | (empty) |
| `AT+CMQTTWILLMSG` | §16.2.7 p.352 | yes | +CMQTTWILLMSG: (0-1),(1-1024),(0-2) | (empty) |
| `AT+CMQTTWILLTOPIC` | §16.2.6 p.351 | yes | +CMQTTWILLTOPIC: (0-1),(1-1024) | (empty) |
| `AT+CMSCR` | — | yes | ERROR | +CMSCR: 2 |
| `AT+CMSD` | §26.2.10 p.469 | no | ERROR | ERROR |
| `AT+CMSDCONTROL` | §26.2.11 p.470 | no | ERROR | ERROR |
| `AT+CMSDOIDDATA` | §26.2.9 p.468 | no | ERROR | ERROR |
| `AT+CMSS` | §9.2.14 p.238 | no | not probed | not probed |
| `AT+CMSSEX` | §9.2.21 p.246 | no | not probed | not probed |
| `AT+CMSSN` | — | yes | ERROR | +CMSSN: 0 |
| `AT+CMSUB` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CMTE` | — | yes | +CMTE: (0,1) | +CMTE: 0 |
| `AT+CMUNSUB` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CMUT` | §5.2.20 p.140 | yes | +CMUT: (0-1) | +CMUT: 0 |
| `AT+CMUX` | §19.2.18 p.416 | no | +CMUX: (0),(0),(1-8),(1-1500),(0),(0),(2-1000) | +CMUX: 0,0,5,118,0,0,600 |
| `AT+CMVP` | §9.2.18 p.243 | no | +CMVP: (0-255) | +CMVP: 255 |
| `AT+CMYIELD` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CNAOP` | §4.2.13 p.104 | yes | ERROR | +CNAOP: 7,9,5,3,2,4 |
| `AT+CNBP` | §4.2.12 p.101 | yes | ERROR | +CNBP: 0x<redacted>,0x<redacted>A,0x<redacted>F |
| `AT+CNCFG` | — | no | ERROR | ERROR |
| `AT+CNETCNCT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CNETIPADDR` | — | yes | ERROR | ERROR |
| `AT+CNETLIGHT` | — | no | ERROR | ERROR |
| `AT+CNETSCAN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CNETSTART` | — | yes | ERROR | +CNETSTART: 0 |
| `AT+CNETSTOP` | — | yes | ERROR | ERROR |
| `AT+CNLPPG` | §5.2.40 p.158 | yes | +CNLPPG: (0x0000-0x7FFF) | ERROR |
| `AT+CNLPPL` | §5.2.41 p.159 | yes | +CNLPPL: (0x0000-0x7FFF) | ERROR |
| `AT+CNLSA` | — | yes | +CNLSA: (0-1) | +CNLSA: 0 |
| `AT+CNMA` | §9.2.8 p.224 | no | (empty) | ERROR |
| `AT+CNMI` | §9.2.9 p.226 | no | +CNMI: (0,1,2),(0,1,2,3),(0,2),(0,1,2),(0,1) | +CNMI: 2,1,0,0,0 |
| `AT+CNMP` | §4.2.11 p.99 | yes | +CNMP: (2,9,10,13,14,19,22,38,39,48,54,59,60,63,67) | +CNMP: 2 |
| `AT+CNRP` | — | yes | +CNRP: (1,3,FF) | +CNRP: FF |
| `AT+CNSDP` | — | yes | +CNSDP: (0-2) | +CNSDP: 2 |
| `AT+CNSLIM` | §5.2.35 p.153 | yes | +CNSLIM: (0x0000-0x7FFF) | ERROR |
| `AT+CNSMOD` | §4.2.15 p.108 | yes | +CNSMOD: (0-1) | +CNSMOD: 0,8 |
| `AT+CNSN` | §5.2.34 p.152 | yes | +CNSN: (0x0000-0x7FFF) | ERROR |
| `AT+CNSVN` | — | yes | ERROR | ERROR |
| `AT+CNSVS` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CNSVSQ` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CNSVSX` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CNSVUN` | — | yes | ERROR | ERROR |
| `AT+CNSVUS` | — | yes | ERROR | ERROR |
| `AT+CNTP` | §15.2.1 p.343 | yes | +CNTP: 255,(-47~48) | <redacted: identifier, location or private data> |
| `AT+CNTPCID` | — | no | ERROR | ERROR |
| `AT+CNUM` | §6.2.5 p.172 | no | (empty) | <redacted: identifier, location or private data> |
| `AT+CNV` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CNVR` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CNVRWE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CNVW` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CODEC` | — | yes | +CODEC: (1-63),(1-7) | +CODEC: 63,7 |
| `AT+CODECCTL` | §5.2.45 p.162 | yes | +CCODECCTL: (0-1) | +CCODECCTL: 0 |
| `AT+CODEINIT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CODOWNLOAD` | — | yes | not probed (safety) | not probed (safety) |
| `AT+COINIT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+COLP` | §5.2.14 p.133 | no | +COLP: (0-1) | ERROR |
| `AT+COPN` | §4.2.10 p.98 | no | (empty) | ERROR |
| `AT+COPS` | §4.2.2 p.86 | no | not probed | not probed |
| `AT+COTA` | — | yes | not probed (safety) | not probed (safety) |
| `AT+COTACFG` | — | yes | not probed (safety) | not probed (safety) |
| `AT+COUTGAIN` | §5.2.27 p.146 | yes | +COUTGAIN: (0-8) | +COUTGAIN: 8 |
| `AT+CPAS` | §3.2.19 p.76 | no | +CPAS: (0,3,4) | ERROR |
| `AT+CPASSMGR` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CPATEMP` | — | yes | ERROR | ERROR |
| `AT+CPBF` | §6.2.3 p.169 | no | not probed | not probed |
| `AT+CPBR` | §6.2.2 p.168 | no | +CPBR: (1-100),40,14 | ERROR |
| `AT+CPBS` | §6.2.1 p.166 | no | +CPBS: ("SM","DC","FD","LD","MC","ME","RC","EN","ON") | +CPBS: "SM",3,100 |
| `AT+CPBW` | §6.2.4 p.171 | no | not probed | not probed |
| `AT+CPCMBANDWIDTH` | §5.2.46 p.163 | yes | +CPCMBANDWIDTH: (0-1),(0-1) | +CPCMBANDWIDTH: 0,1 |
| `AT+CPCMFREQ` | — | no | ERROR | ERROR |
| `AT+CPCMFRM` | §5.2.43 p.161 | yes | +CPCMFRM: (0-1) | +CPCMFRM: 0 |
| `AT+CPCMMODE` | — | yes | +CPCMMODE: (0-1),(0-1) | +CPCMMODE: 0,1 |
| `AT+CPCMREG` | §5.2.25 p.145 | yes | +CPCMREG: (0-1) | +CPCMREG: 0 |
| `AT+CPIN` | §3.2.2 p.56 | no | not probed | not probed |
| `AT+CPING` | §11.3.4 p.298 | yes | +CPING: IP address,(1,2),(1-100),(4-188),(1000-10000),(10000-100000),(… | ERROR |
| `AT+CPINGSTOP` | §11.3.5 p.299 | yes | (empty) | ERROR |
| `AT+CPLMNPASS` | — | yes | ERROR | ERROR |
| `AT+CPLMNWLIST` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CPLS` | — | no | +CPLS: 0,1,2 | +CPLS: 0 |
| `AT+CPMS` | §9.2.2 p.218 | no | +CPMS: ("ME","MT","SM","SR"),("ME","MT","SM","SR"),("ME","SM") | +CPMS: "SM",1,30,"SM",1,30,"SM",1,30 |
| `AT+CPMUTEMP` | §19.2.12 p.409 | yes | ERROR | ERROR |
| `AT+CPMVT` | §19.2.7 p.406 | yes | +CPMVT: (0,1),(3200-4000),(4000-4300) | +CPMVT: 0,3200,4300 |
| `AT+CPOF` | §3.2.12 p.69 | yes | not probed (safety) | not probed (safety) |
| `AT+CPOL` | §4.2.9 p.97 | no | not probed | not probed |
| `AT+CPORTDELAY` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CPPPACT` | — | yes | ERROR | +CPPPACT: 0 |
| `AT+CPSI` | §4.2.14 p.105 | yes | +CPSI: (0-255) | <redacted: identifier, location or private data> |
| `AT+CPTONE` | §5.2.44 p.162 | yes | +CPTONE: (0-16) | ERROR |
| `AT+CPTONETEST` | — | yes | +CPTONETEST	: (1-100)(0-9,A-F,*,#)(50-100) | ERROR |
| `AT+CPUC` | §3.2.16 p.72 | no | not probed | not probed |
| `AT+CPWD` | §4.2.4 p.90 | no | not probed | not probed |
| `AT+CQCNV` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CR` | §5.2.5 p.121 | yes | +CR: (0,1) | +CR: 0 |
| `AT+CRC` | §5.2.6 p.122 | yes | +CRC: (0,1) | +CRC: 0 |
| `AT+CREC` | §21.2.1 p.438 | yes | (empty) | +CREC: 0 |
| `AT+CRECAMR` | §21.2.2 p.439 | yes | (empty) | +CRECAMR: 0 |
| `AT+CREDTEA` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CREG` | §4.2.1 p.84 | yes | +CREG: (0-2) | +CREG: 0,1 |
| `AT+CRES` | — | yes | +CRES: 0 | ERROR |
| `AT+CRESET` | §3.2.13 p.70 | yes | not probed (safety) | not probed (safety) |
| `AT+CRESTORECOUNT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CRIIC` | §19.2.9 p.407 | yes | (empty) | ERROR |
| `AT+CRIRS` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CRIS` | — | no | ERROR | ERROR |
| `AT+CRLP` | §5.2.4 p.119 | yes | +CRLP: (0-61),(0-61),(38-255),(1-255),0 \| +CRLP: (0-61),(0-61),(38-25… | +CRLP: 61,61,48,6,0 \| +CRLP: 61,61,48,6,1 \| +CRLP: 240,240,52,6,2 |
| `AT+CRPAAO` | — | yes | +CRPAAO: (0,1) | +CRPAAO: 0 |
| `AT+CRSL` | — | yes | ERROR | ERROR |
| `AT+CRSM` | §3.2.5 p.59 | no | not probed | not probed |
| `AT+CRSVD` | — | no | ERROR | ERROR |
| `AT+CRUPSI` | — | yes | (empty) | (empty) |
| `AT+CRUS` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CRUSET` | — | yes | (empty) | (empty) |
| `AT+CRXGAIN` | — | no | ERROR | ERROR |
| `AT+CRXVOL` | §5.2.30 p.149 | yes | +CRXVOL: (0x0000-0xFFFF) | ERROR |
| `AT+CSAS` | — | yes | +CSAS: 0 | ERROR |
| `AT+CSCA` | §9.2.4 p.220 | yes | (empty) | <redacted: identifier, location or private data> |
| `AT+CSCB` | §9.2.5 p.221 | yes | +CSCB: (0-1) | +CSCB: 0,"","" |
| `AT+CSCFOTA` | §23.2.2 p.452 | yes | ERROR | ERROR |
| `AT+CSCHN` | — | yes | (empty) | ERROR |
| `AT+CSCLK` | §19.2.17 p.415 | yes | +CSCLK: (0-1) | +CSCLK: 0 |
| `AT+CSCON` | — | no | ERROR | ERROR |
| `AT+CSCS` | §2.2.32 p.50 | yes | +CSCS: ("IRA","GSM","UCS2") | +CSCS: "IRA" |
| `AT+CSDACTV` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CSDCARDISOK` | — | yes | +CSDCARDISOK: (0-1) | +CSDCARDISOK: 0 |
| `AT+CSDCHANGE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CSDF` | — | no | +CSDF: (1-7),(1-2) | ERROR |
| `AT+CSDH` | §9.2.7 p.223 | yes | +CSDH: (0-1) | +CSDH: 0 |
| `AT+CSDVC` | §5.2.47 p.164 | yes | +CSDVC: (0,1,3) | +CSDVC: 1 |
| `AT+CSGS` | — | no | ERROR | ERROR |
| `AT+CSGSNR` | — | yes | ERROR | +CSGSNR: 2 |
| `AT+CSHELL` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CSIM` | §3.2.4 p.58 | no | not probed | not probed |
| `AT+CSMP` | §9.2.6 p.222 | yes | (empty) | +CSMP: 17,,0,0 |
| `AT+CSMS` | §9.2.1 p.217 | yes | +CSMS: (0-1) | +CSMS: 0,1,1,1 |
| `AT+CSNINFO` | — | yes | ERROR | <redacted: identifier, location or private data> |
| `AT+CSNS` | — | no | ERROR | ERROR |
| `AT+CSOCKAUTH` | — | yes | +CSOCKAUTH: ,,127,127(for CDMA1x-EvDo only) \| +CSOCKAUTH: (1-24,100-1… | <redacted: identifier, location or private data> |
| `AT+CSOCKSETPN` | — | yes | +CSOCKSETPN: (1-24,100-179) | +CSOCKSETPN: 1 |
| `AT+CSPKGAIN` | — | no | ERROR | ERROR |
| `AT+CSPN` | §3.2.7 p.64 | no | (empty) | +CSPN: "",0 |
| `AT+CSQ` | §3.2.8 p.65 | no | not probed | not probed |
| `AT+CSQDELTA` | §3.2.10 p.67 | yes | +CSQDELTA: (0-5) | +CSQDELTA: 5 |
| `AT+CSQFMT` | — | yes | +CSQFMT: (0-1) | +CSQFMT: 1 |
| `AT+CSRP` | — | yes | (empty) | ERROR |
| `AT+CSSLCA` | — | yes | +CSSLCA: (0-9),(5-128) | +CSSLCA: 0,"" \| +CSSLCA: 1,"" \| +CSSLCA: 2,"" \| +CSSLCA: 3,"" \| +C… |
| `AT+CSSLCERT` | — | yes | +CSSLCERT: (0-9),(5-128) | +CSSLCERT: 0,"" \| +CSSLCERT: 1,"" \| +CSSLCERT: 2,"" \| +CSSLCERT: 3,… |
| `AT+CSSLCFG` | §10.2.12 p.261 | yes | +CSSLCFG: "sslversion",(0-9),(0-4) \| +CSSLCFG: "authmode",(0-9),(0-3)… | +CSSLCFG: 0,4,0,1,300,"","","",0,"",0x0 \| +CSSLCFG: 1,4,0,1,300,"",""… |
| `AT+CSSLKEY` | — | yes | +CSSLKEY: (0-9),(5-128) | +CSSLKEY: 0,"" \| +CSSLKEY: 1,"" \| +CSSLKEY: 2,"" \| +CSSLKEY: 3,"" \… |
| `AT+CSSN` | §4.2.8 p.95 | yes | +CSSN: (0-1),(0-1) | +CSSN: 0,0 |
| `AT+CSTA` | §5.2.17 p.137 | yes | +CSTA: (129,145,161,177) | +CSTA: 129 |
| `AT+CSTT` | — | no | ERROR | ERROR |
| `AT+CSUB` | — | yes | (empty) | ERROR |
| `AT+CSURCH` | — | yes | ERROR | ERROR |
| `AT+CSURCHQ` | — | yes | ERROR | ERROR |
| `AT+CSURV` | — | yes | ERROR | +CSURV: 0 |
| `AT+CSURVC` | — | yes | ERROR | ERROR |
| `AT+CSURVU` | — | yes | ERROR | ERROR |
| `AT+CSURVUC` | — | yes | ERROR | ERROR |
| `AT+CSVM` | §3.2.22 p.78 | no | not probed | not probed |
| `AT+CTAT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CTBURST` | — | yes | ERROR | ERROR |
| `AT+CTCPFIN` | — | yes | +CTCPFIN: (0,1) | +CTCPFIN: 0 |
| `AT+CTCPKA` | — | yes | +CTCPKA: (0,1),(1-120),(1-10) | +CTCPKA: 0,120,10 |
| `AT+CTCPSET` | — | no | ERROR | ERROR |
| `AT+CTEST` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CTEUTP` | — | yes | +CTEUTP: (0,1) | +CTEUTP: 1 |
| `AT+CTFSAUTH` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CTFSDECRYPT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CTFSGETID` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CTISCLK` | — | yes | +CTISCLK: (0,1) | ERROR |
| `AT+CTTS` | §22.2.2 p.447 | yes | (empty) | +CTTS: 0 |
| `AT+CTTSPARAM` | §22.2.3 p.449 | yes | +CTTSPARAM: (0-2),(0-3),(0-3),(0-2),(0-2) | +CTTSPARAM: 2,3,0,1,1 |
| `AT+CTXFREQ` | — | no | ERROR | ERROR |
| `AT+CTXMICGAIN` | §5.2.29 p.148 | yes | +CTXMICGAIN: (0,1),(0x0000-0xFFFF) | ERROR |
| `AT+CTXVOL` | §5.2.28 p.147 | yes | +CTXVOL: (0x0000-0xFFFF) | ERROR |
| `AT+CTZR` | §4.2.18 p.113 | no | +CTZR: (0-1) | +CTZR: 0 |
| `AT+CTZU` | §4.2.17 p.112 | no | +CTZU: (0-1) | +CTZU: 1 |
| `AT+CTZUA` | — | no | ERROR | ERROR |
| `AT+CTZVL` | — | yes | ERROR | +CTZVL: 1 |
| `AT+CUARTLOG` | — | yes | +CUARTLOG: (0-10000) | +CUARTLOG: 0 |
| `AT+CUARTRM` | — | yes | +CUARTRM: (0-1) | +CUARTRM: 0 |
| `AT+CUARTSD` | — | yes | +CUARTSD: (500-10000) | +CUARTSD: 500 |
| `AT+CUPURE` | — | yes | +CUPURE: (0,1) | +CUPURE: 1 |
| `AT+CURCD` | — | yes | +CURCD: (0-10000) | +CURCD: 0 |
| `AT+CUSBADB` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CUSBAUDIO` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CUSBMSS` | — | yes | not probed (safety) | not probed (safety) |
| `AT+CUSBPIDSWITCH` | §19.2.14 p.411 | yes | not probed (safety) | not probed (safety) |
| `AT+CUSD` | §4.2.6 p.93 | no | +CUSD: (0-2) | +CUSD: 0 |
| `AT+CUSETI` | — | yes | ERROR | (empty) |
| `AT+CUSI` | — | yes | ERROR | (empty) |
| `AT+CV120` | — | yes | +CV120: (1),(0-1),(1),(0),(0),(0) | +CV120: 1,1,1,0,0,0 |
| `AT+CVALARM` | §19.2.1 p.401 | yes | +CVALARM: (0,1),(3300-4000),(4000-4300) | +CVALARM: 0,3300,4300 |
| `AT+CVAUXS` | §19.2.2 p.402 | yes | +CVAUXS: (0-1) | +CVAUXS: 1 |
| `AT+CVAUXV` | §19.2.3 p.402 | yes | +CVAUXV: (1700-3050) | +CVAUXV: 2850 |
| `AT+CVHU` | §5.2.1 p.116 | yes | +CVHU: (0-1) | +CVHU: 1 |
| `AT+CVMOD` | — | no | +CVMOD: (0-3) | +CVMOD: 3 |
| `AT+CWAUTH` | §27.3.4 p.475 | no | ERROR | ERROR |
| `AT+CWBCAST` | §27.3.3 p.474 | no | ERROR | ERROR |
| `AT+CWCLICNT` | §27.3.9 p.479 | no | ERROR | ERROR |
| `AT+CWDHCP` | §27.3.7 p.478 | no | ERROR | ERROR |
| `AT+CWIIC` | §19.2.10 p.408 | yes | (empty) | ERROR |
| `AT+CWISO` | §27.3.6 p.477 | no | ERROR | ERROR |
| `AT+CWLANMGET` | §27.3.14 p.484 | no | ERROR | ERROR |
| `AT+CWLANMSG` | §27.3.13 p.484 | no | ERROR | ERROR |
| `AT+CWLANSRV` | §27.3.12 p.1 | no | ERROR | ERROR |
| `AT+CWMACADDR` | §27.3.15 p.485 | no | ERROR | ERROR |
| `AT+CWMAP` | §27.3.1 p.472 | no | ERROR | ERROR |
| `AT+CWMAPCFG` | §27.3.11 p.480 | no | ERROR | ERROR |
| `AT+CWMOCH` | §27.3.5 p.476 | no | ERROR | ERROR |
| `AT+CWNAT` | §27.3.8 p.478 | no | ERROR | ERROR |
| `AT+CWNETCNCT` | §27.3.16 p.486 | no | ERROR | ERROR |
| `AT+CWNETPREF` | — | yes | +CWNETPREF: <FirstPreferred>,<SecondPreferred>,<ThirdPreferred> | +CWNETPREF: ""eth"",""wlan"",""wwan"" |
| `AT+CWRRCR` | — | yes | ERROR | +CWRRCR: WCDMA RRC NOT ACTIVATION |
| `AT+CWRSTD` | §27.3.10 p.480 | no | ERROR | ERROR |
| `AT+CWSSID` | §27.3.2 p.473 | no | ERROR | ERROR |
| `AT+CWSTACFG` | §27.3.19 p.489 | no | ERROR | ERROR |
| `AT+CWSTAINIT` | §27.3.20 p.490 | no | ERROR | ERROR |
| `AT+CWSTAIP` | §27.3.17 p.487 | no | ERROR | ERROR |
| `AT+CWSTASCAN` | §27.3.18 p.487 | no | ERROR | ERROR |
| `AT+CWUSRINFO` | §27.3.21 p.491 | no | ERROR | ERROR |
| `AT+DMCONFIGEXT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+DMRESET` | — | yes | not probed (safety) | not probed (safety) |
| `AT+DMSESSION` | — | yes | not probed (safety) | not probed (safety) |
| `AT+DMSET` | — | yes | not probed (safety) | not probed (safety) |
| `AT+DR` | — | yes | +DR: (0-1) | +DR: 0 |
| `AT+DS` | — | yes | +DS: (0-3),(0),(512-2048),(6) | +DS: 0,0,2048,6 |
| `AT+DTMFCTL` | — | yes | +DTMFCTL: (0-1),(0-1) | +DTMFCTL: 1,1 |
| `AT+ENTERSLEEP` | — | yes | not probed (safety) | not probed (safety) |
| `AT+ES` | — | yes | +ES: (6),,(8) | +ES: ,, |
| `AT+ESA` | — | yes | +ESA: (0),,,,(0),(0),(0-255), | +ESA: 0,,,,0,0,255, |
| `AT+FACTORYSESSION` | — | yes | not probed (safety) | not probed (safety) |
| `AT+FCLASS` | — | yes | 0,2.0 | 0 |
| `AT+FNSMOD` | — | yes | +FNSMOD: (0,1) | ERROR |
| `AT+FOTADL` | — | yes | not probed (safety) | not probed (safety) |
| `AT+FOTAEN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+FSATTRI` | §20.2.7 p.430 | yes | (empty) | ERROR |
| `AT+FSCD` | §20.2.1 p.425 | yes | (empty) | +FSCD: C:/ |
| `AT+FSCOPY` | §20.2.10 p.433 | yes | (empty) | ERROR |
| `AT+FSDEL` | §20.2.5 p.429 | yes | (empty) | ERROR |
| `AT+FSLOCA` | §20.2.9 p.432 | yes | +FSLOCA: (0) | +FSLOCA: 0 |
| `AT+FSLS` | §20.2.4 p.428 | yes | +FSLS: (0-2) | +FSLS: SUBDIRECTORIES:0,FILES:0 |
| `AT+FSLST` | — | yes | (empty) | ERROR |
| `AT+FSMEM` | §20.2.8 p.431 | yes | (empty) | ERROR |
| `AT+FSMKDIR` | §20.2.2 p.426 | yes | (empty) | ERROR |
| `AT+FSRENAME` | §20.2.6 p.430 | yes | (empty) | ERROR |
| `AT+FSRMDIR` | §20.2.3 p.427 | yes | (empty) | ERROR |
| `AT+GCAP` | §2.2.35 p.52 | yes | (empty) | ERROR |
| `AT+GMI` | — | yes | (empty) | ERROR |
| `AT+GMM` | — | yes | (empty) | ERROR |
| `AT+GMR` | — | yes | (empty) | ERROR |
| `AT+GOI` | — | no | ERROR | ERROR |
| `AT+GSN` | — | yes | (empty) | <redacted: identifier, location or private data> |
| `AT+GSV` | — | no | ERROR | ERROR |
| `AT+HDRCSQ` | — | yes | ERROR | ERROR |
| `AT+HTTPACTION` | §13.2.4 p.330 | yes | +HTTPACTION: (0-3) | ERROR |
| `AT+HTTPAUTHKEY` | — | yes | (empty) | ERROR |
| `AT+HTTPCLOSE` | — | yes | (empty) | ERROR |
| `AT+HTTPCMD` | — | yes | not probed (safety) | not probed (safety) |
| `AT+HTTPDATA` | §13.2.7 p.333 | yes | (empty) | ERROR |
| `AT+HTTPHEAD` | §13.2.5 p.331 | yes | (empty) | ERROR |
| `AT+HTTPINIT` | §13.2.1 p.326 | yes | ERROR | ERROR |
| `AT+HTTPOPEN` | — | yes | (empty) | (empty) |
| `AT+HTTPPARA` | §13.2.3 p.327 | yes | (empty) | (empty) |
| `AT+HTTPPOSTFILE` | §13.2.8 p.334 | yes | +HTTPPOSTFILE: <filename>[,(1-3)[,(0-3)[,(0-1)]]] | ERROR |
| `AT+HTTPREAD` | §13.2.6 p.332 | yes | ERROR | +HTTPREAD: LEN,0 |
| `AT+HTTPREADFILE` | §13.2.9 p.335 | yes | +HTTPREADFILE: <filename>[,(1-4)] | (empty) |
| `AT+HTTPRECV` | — | yes | +HTTPRECV: offset,length | ERROR |
| `AT+HTTPSTATUS` | — | yes | (empty) | +HTTPSTATUS: GET,0,0,0 |
| `AT+HTTPTERM` | §13.2.2 p.327 | yes | ERROR | ERROR |
| `AT+HWINFO` | — | yes | ERROR | ERROR |
| `AT+ICF` | §2.2.13 p.36 | yes | +ICF: (1-6),(0-3) | +ICF: 3,3 |
| `AT+IFC` | §2.2.14 p.37 | yes | +IFC: (0,2),(0,2) | +IFC: 0,0 |
| `AT+IMSREGDB` | — | yes | ERROR | +IMSREGDB: 272,17,1283,136,64,103,17,1280,136,64,512,17,0,0,0 |
| `AT+IMSSIPCFG` | — | yes | ERROR | +IMSSIPCFG: 2000,16000,32000 |
| `AT+IMSSMSCFG` | — | yes | ERROR | +IMSSMSCFG: 1,0 |
| `AT+IPADDR` | §11.2.7 p.284 | yes | (empty) | ERROR |
| `AT+IPR` | §2.2.12 p.35 | yes | +IPR: (0,300,600,1200,2400,4800,9600,19200,38400,57600,115200,230400,4… | +IPR: 9600 |
| `AT+IPREX` | §19.2.15 p.412 | yes | +IPREX: (0,300,600,1200,2400,4800,9600,19200,38400,57600,115200,230400… | +IPREX: 9600 |
| `AT+ISIMAUTH` | — | yes | not probed (safety) | not probed (safety) |
| `AT+MCELL` | — | yes | ERROR | ERROR |
| `AT+MONI` | — | yes | +MONI: 0,0 | <redacted: identifier, location or private data> |
| `AT+MORING` | §5.2.21 p.141 | no | +MORING: (0-1) | +MORING: 0 |
| `AT+NETACT` | — | yes | +NETACT: (0-1) | +NETACT: 0 |
| `AT+NETCLOSE` | §11.2.2 p.272 | yes | (empty) | ERROR |
| `AT+NETDORM` | — | yes | not probed (safety) | not probed (safety) |
| `AT+NETMODE` | — | yes | +NETMODE: (1,2) | +NETMODE: 2 |
| `AT+NETOPEN` | §11.2.1 p.270 | yes | ERROR | +NETOPEN: 0 |
| `AT+PACSP` | — | yes | ERROR | ERROR |
| `AT+PLAYRINGTONE` | — | yes | +PLAYRINGTONE: (0,1),(ringtone_path) | +PLAYRINGTONE: 0, /system/ring/telephone_Ring.mp3 |
| `AT+POC` | — | yes | not probed (safety) | not probed (safety) |
| `AT+PSRAT` | — | yes | ERROR | +PSRAT: LTE FDD |
| `AT+PUSH` | — | yes | not probed (safety) | not probed (safety) |
| `AT+QCGEQMIN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+QCGEQOS` | — | yes | not probed (safety) | not probed (safety) |
| `AT+QCGEQREQ` | — | yes | not probed (safety) | not probed (safety) |
| `AT+QCGQMIN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+QCGQREQ` | — | yes | not probed (safety) | not probed (safety) |
| `AT+QCGTFT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+QCRMCALL` | — | no | ERROR | ERROR |
| `AT+RFMODE` | — | yes | ERROR | ERROR |
| `AT+SAPBR` | — | no | ERROR | ERROR |
| `AT+SBARCELL` | — | yes | ERROR | +SBARCELL: 0 |
| `AT+SBASECFG` | — | yes | ERROR | (empty) |
| `AT+SCHC` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SCHO` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SDBGINFO` | — | yes | ERROR | ERROR |
| `AT+SERVERSTART` | §11.2.14 p.291 | yes | +SERVERSTART: (0-65535),(0-3) | +CIPERROR: 2 \|  \| ERROR |
| `AT+SERVERSTOP` | §11.2.15 p.292 | yes | (empty) | ERROR |
| `AT+SHCMD` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SIDET` | §5.2.23 p.143 | yes | +SIDET: (0-1) | ERROR |
| `AT+SIMAUTH` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SIMCOMATI` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SIMEI` | §3.2.20 p.77 | yes | (empty) | +SIMEI: <redacted> |
| `AT+SIMSTATUS` | — | yes | ERROR | <redacted: identifier, location or private data> |
| `AT+SJDR` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SLEEPINFO` | — | yes | ERROR | ERROR |
| `AT+SLTEREJINFO` | — | yes | ERROR | +SLTEREJINFO: 0,0,000000,0,0,000000 |
| `AT+SMECNSMOD` | — | yes | +SMECNSMOD: (0,1) | ERROR |
| `AT+SMEID` | §3.2.21 p.78 | yes | ERROR | ERROR |
| `AT+SMSTATUS` | — | no | ERROR | ERROR |
| `AT+SMTPAUTH` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SMTPBCH` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SMTPBODY` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SMTPFILE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SMTPFROM` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SMTPRCPT` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SMTPSEND` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SMTPSRV` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SMTPSTOP` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SMTPSUB` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SPIC` | §3.2.6 p.63 | no | (empty) | ERROR |
| `AT+SSAPDU` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SSLCLOSE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SSLOPEN` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SSLPARA` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SSLREAD` | — | yes | not probed (safety) | not probed (safety) |
| `AT+SSLWRITE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+STENV` | §7.2.6 p.181 | no | (empty) | ERROR |
| `AT+STGI` | §7.2.2 p.175 | no | (empty) | ERROR |
| `AT+STGR` | §7.2.3 p.178 | no | (empty) | ERROR |
| `AT+STIN` | §7.2.1 p.174 | no | (empty) | ERROR |
| `AT+STK` | §7.2.4 p.180 | no | +STK: (0-1) | +STK: 0 |
| `AT+STKFMT` | §7.2.5 p.180 | no | ERROR | +STKFMT: 0 |
| `AT+STSM` | §7.2.7 p.182 | no | (empty) | ERROR |
| `AT+UCUSAGE` | — | no | ERROR | ERROR |
| `AT+UIMHOTSWAPLEVEL` | §24.2.2 p.455 | no | ERROR | +UIMHOTSWAPLEVEL: 1 |
| `AT+UIMHOTSWAPON` | §24.2.1 p.454 | no | ERROR | +UIMHOTSWAPON: 0 |
| `AT+VMUTE` | §5.2.19 p.139 | yes | +VMUTE: (0-1) | +VMUTE: 0 |
| `AT+VOLTEDTMFCFG` | — | yes | +VOLTEDTMFCFG: (1-400),(1-9999) | +VOLTEDTMFCFG: 200,4000 |
| `AT+VOLTESETTING` | — | yes | +VOLTESETTING: (0-1) | +VOLTESETTING: 1 |
| `AT+VTD` | §5.2.16 p.136 | no | +VTD: (0-255) | +VTD: 0 |
| `AT+VTS` | §5.2.15 p.134 | no | +VTS: (0-9,*,#,A,B,C,D) | ERROR |
| `AT+VZWAPNE` | — | yes | not probed (safety) | not probed (safety) |
| `AT+VZWRSRP` | — | yes | ERROR | <redacted: identifier, location or private data> |
| `AT+VZWRSRQ` | — | yes | ERROR | <redacted: identifier, location or private data> |
| `AT+WS46` | — | yes | +WS46: (12,22,25,28,29) | 25 |
| `AT^BODYSARGSM` | — | yes | ^BODYSARGSM: (15-33),00000000 | ^BODYSARGSM: |
| `AT^BODYSARLTE` | — | yes | ^BODYSARLTE: (12-23),<redacted>a | ^BODYSARLTE: |
| `AT^BODYSARON` | — | yes | ^BODYSARON: (0,1) | ^BODYSARON: 0 |
| `AT^BODYSARWCDMA` | — | yes | ^BODYSARWCDMA: (12-23),00000012 | ^BODYSARWCDMA: (23,00000012) |
| `AT^CARDMODE` | — | yes | ERROR | ^CARDMODE: 2 |
| `AT^CLPORT` | — | yes | (empty) | (empty) |
| `AT^CSCLK` | — | yes | ERROR | ^CSCLK: 1 |
| `AT^CTCPKA` | — | yes | (empty) | (empty) |
| `AT^CURCREAD` | — | yes | (empty) | ^CURCREAD: 0,0 |
| `AT^DSCI` | — | yes | ^DSCI: (0-1) | ^DSCI: 0 |
| `AT^MODE` | — | yes | ^MODE: (0-1) | ^MODE: 0 |
| `AT^PWRCTL` | — | yes | ERROR | ERROR |
| `AT^SPN` | — | yes | ERROR | ERROR |
| `AT^SYSCONFIG` | — | yes | ERROR | ^SYSCONFIG: 2,0,1,2 |
| `AT^SYSINFO` | — | yes | ERROR | ERROR |
| `AT%V` | — | yes | not probed | not probed |
| `AT&C` | §2.2.15 p.38 | yes | not probed | not probed |
| `AT&D` | §2.2.18 p.40 | yes | not probed | not probed |
| `AT&E` | §2.2.25 p.46 | yes | not probed | not probed |
| `AT&F` | §2.2.21 p.42 | yes | not probed | not probed |
| `AT&S` | §2.2.19 p.40 | yes | not probed | not probed |
| `AT&V` | §2.2.17 p.39 | yes | not probed | not probed |
| `AT&W` | §2.2.26 p.46 | yes | not probed | not probed |
| `ATA` | — | yes | not probed | not probed |
| `ATD` | — | yes | not probed | not probed |
| `ATE` | — | yes | not probed | not probed |
| `ATH` | — | yes | not probed | not probed |
| `ATI` | — | yes | not probed | not probed |
| `ATL` | — | yes | not probed | not probed |
| `ATM` | — | yes | not probed | not probed |
| `ATO` | — | yes | not probed | not probed |
| `ATP` | — | yes | not probed | not probed |
| `ATQ` | — | yes | not probed | not probed |
| `ATS0` | — | yes | not probed | not probed |
| `ATS10` | — | yes | not probed | not probed |
| `ATS103` | — | yes | not probed | not probed |
| `ATS104` | — | yes | not probed | not probed |
| `ATS11` | — | yes | not probed | not probed |
| `ATS2` | — | yes | not probed | not probed |
| `ATS3` | — | yes | not probed | not probed |
| `ATS30` | — | yes | not probed | not probed |
| `ATS4` | — | yes | not probed | not probed |
| `ATS5` | — | yes | not probed | not probed |
| `ATS6` | — | yes | not probed | not probed |
| `ATS7` | — | yes | not probed | not probed |
| `ATS8` | — | yes | not probed | not probed |
| `ATS9` | — | yes | not probed | not probed |
| `ATT` | — | yes | not probed | not probed |
| `ATV` | — | yes | not probed | not probed |
| `ATX` | — | yes | not probed | not probed |
| `ATZ` | — | yes | not probed | not probed |
| `AT\Q` | — | yes | not probed | not probed |
| `AT\S` | — | yes | not probed | not probed |
| `AT\V` | §2.2.24 p.45 | yes | not probed | not probed |
