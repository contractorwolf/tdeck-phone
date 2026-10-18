"""Read-only AT probe: sends only 'AT<cmd>=?' and 'AT<cmd>?' for names the module lists in AT+CLAC.
Skips a denylist of anything that could reset, power off, write NV/firmware, send, or open shells."""
import serial,time,json,sys,re,os
S=sys.argv[1]
cmds=[l.strip() for l in open(S+'/clac_cmds.txt') if l.strip()]
DENY=re.compile(r'^[+^*$](CRESET|CPOF|CPOWD|BOOTLDR|DMRESET|DMSESSION|FACTORYSESSION|DMSET|DMCONFIGEXT|FOTA\w*|COTA\w*|CODOWNLOAD|CODEINIT|COINIT|CNV\w*|CQCNV|CAPFS|CBACKUPEFS2|CBACKUPRESULT|CRESTORECOUNT|CTEST|CEDUMP|SHCMD|CSHELL|CUSBADB|CUSBMSS|CUSBAUDIO|CUSBPIDSWITCH|CDEVPORT|CPORTDELAY|CLOOPBACK|CLEDITST|CTAT|CREDTEA|ENTERSLEEP|NETDORM|SIMAUTH|ISIMAUTH|SSAPDU|SCHO|SCHC|CSIM|CRSM|CGLA|CRLA|CMGS|CMGSEX|CMSS|CMGW\w*|CIPSEND|CHTTPSSEND|SMTP\w*|CFTP\w*|HTTPCMD|CELLLOCK|CNETSCAN|COPS|CFUN|CGMS|CSDCHANGE|CSDACTV|CTFS\w*|CDMPOWER|CMDLOG|SJDR|CRIRS|CGPSWARM|QCPWRDN|MYPOWEROFF|QC\w+|MY\w+|DLOGCFG|SLTETA|SRFINFO|SIMCOMATI|CCGMDF|CRUS|CNSVS\w*|PUSH|POC|CHSICDEVSLEEP|AYLA\w*|VZWAPNE|BT\w*|CPASSMGR|CPLMNPASS|CPLMNWLIST|CAPNET|CAUTONET|CNETCNCT|CAUTOAPN|CFOTASWITCH|CCINIT|CCAUTH|CCSEND|CCYIELD|CCDEINIT|CMAUTH|CMPARA|CMCONN|CMPUB|CMSUB|CMUNSUB|CMYIELD|CMAUTOYIELD|CMDISCONN|SSLOPEN|SSLPARA|SSLREAD|SSLWRITE|SSLCLOSE)$',re.I)
todo=[c for c in cmds if c[0] in '+^*' and not DENY.match(c)]
skipped=[c for c in cmds if c[0] in '+^*' and DENY.match(c)]
out=S+'/probe_results.jsonl'
done=set()
if os.path.exists(out):
    for l in open(out): done.add(json.loads(l)['cmd'])
open(S+'/probe_skipped.txt','w').write('\n'.join(skipped))
print('probe',len(todo),'skip',len(skipped),'already',len(done),flush=True)
s=serial.Serial('/dev/cu.usbserial-0001',9600,timeout=0.2)
def send(c,maxw=8):
    s.reset_input_buffer(); s.write((c+'\r\n').encode())
    t0=time.time(); b=b''; q=time.time()
    while time.time()-t0<maxw:
        d=s.read(500)
        if d: b+=d; q=time.time()
        tail=b.rstrip()
        if (tail.endswith(b'OK') or tail.endswith(b'ERROR') or b'ERROR:' in tail) and time.time()-q>0.15: break
    return b.decode(errors='replace').replace('\r','').strip(), round(time.time()-t0,2)
def alive():
    for _ in range(6):
        r,_t=send('AT',3)
        if 'OK' in r: return True
        time.sleep(3)
    return False
f=open(out,'a')
for c in todo:
    if c in done: continue
    rec={'cmd':c}
    for form,suffix in (('test','=?'),('read','?')):
        r,t=send('AT'+c+suffix)
        rec[form]=r; rec[form+'_s']=t
    f.write(json.dumps(rec)+'\n'); f.flush()
    if rec['test_s']>=7.9 or rec['read_s']>=7.9:
        print('slow/timeout at',c,flush=True)
        if not alive():
            print('MODULE NOT RESPONDING after',c,flush=True); sys.exit(2)
print('DONE',flush=True)
