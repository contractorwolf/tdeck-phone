import serial,time,json,sys
S=sys.argv[1]
s=serial.Serial('/dev/cu.usbserial-0001',9600,timeout=0.2)
log=[]
def send(c,maxw=8,raw=False):
    s.reset_input_buffer(); data=(c if raw else c+'\r\n').encode()
    t0=time.time(); s.write(data); b=b''; q=time.time(); first=None
    while time.time()-t0<maxw:
        d=s.read(500)
        if d:
            if first is None: first=round(time.time()-t0,3)
            b+=d; q=time.time()
        t=b.rstrip()
        if (t.endswith(b'OK') or t.endswith(b'ERROR') or b'ERROR:' in t) and time.time()-q>0.2: break
    r=b.decode(errors='replace')
    log.append({'cmd':c,'raw':r,'first_byte_s':first,'total_s':round(time.time()-t0,2)})
    return r
G1=['ATI','AT+GMI','AT+GMM','AT+GMR','AT+GSN','AT+CGMI','AT+CGMM','AT+CGMR','AT+CSUB','AT+SIMCOMATI','AT+CCPUTEMP','AT+CPATEMP','AT+CADC','AT+CBC','AT+CCLK?','AT+CTZU?','AT+CNSMOD?','AT+CFUN?','AT+CNMP?','AT+CNBP?','AT+CGMR','AT+CSCLK?','AT+CMEE?','AT+COPS?','AT+CGREG?','AT+CREG?','AT+CESQ','AT+CSQ','AT+CCINFO','AT+CPMS?','AT+CSDH?','AT+CPBS?','AT+CPBS=?','AT+CPBR=?','AT+CLCC','AT+CHUP','AT+CGPS?','AT+CGPSINFO','AT+CGNSSINFO','AT+CGPSINFOCFG?','AT+CGPSNMEA?','AT+CLBS=?','AT+CFSINIT?','AT+FSCD?','AT+FSLS?','AT+CNETSTART?','AT+CSOCKSETPN?','AT+CIPMODE?','AT+CGATT?','AT+CGACT?','AT+CGPADDR','AT+CGDCONT?','AT+CGEQOS?','AT+CGCONTRDP=1','AT+CGCONTRDP=2','AT+CNTP?','AT+CCLK?','AT+CRSL?','AT+CMICGAIN?','AT+COUTGAIN?','AT+CMICGAIN=?','AT+COUTGAIN=?','AT+CSIDET?','AT+CSDVC?','AT+CPCMREG?','AT+CODECCTL?','AT+CECM?','AT+CECH?','AT+CACDBFN=?','AT+CREC=?','AT+CCMXPLAYWAV=?','AT+CTTS=?','AT+CATR?','AT+CSCS?','AT+CMGF?','AT+CNMI?','AT+CSMP?','AT+CSCA?','AT+CSMS?','AT+CRC?','AT+CVHU?','AT+CLIP=?','AT+CLIR?','AT+CCWA?','AT+CCFC=?','AT+CLCK=?','AT+CHLD=?','AT+CUSD?','AT+CSSN?','AT+CPWD=?','AT+VMUTE?','AT+CMUT?','AT+CLVL?','AT+IFC?','AT+ICF?','AT+IPREX?','AT+CMUX?','AT+CSCLK=?','AT+CTZR?','AT+CLTS?','AT+CNSMOD=?','AT+CPSMS?','AT+CEDRXS?','AT+CEDRXRDP','AT+CRSM=?','AT+CIMI','AT+CICCID','AT+CCID','AT+CNUM']
for c in G1: send(c)
# behavior tests
send('at'); send('At'); send('AT   '); send('AT+FOOBAR'); send('AT+FOOBAR=?'); send('AT+FOOBAR?')
send('AT\r',raw=True)
send('ATE0'); send('AT'); send('ATE1')
send('AT+CMEE=0'); send('AT+CMGR=999'); send('AT+CMGD=999'); send('AT+CPIN=1111'); 
send('AT+CMEE=1'); send('AT+CMGR=999'); send('AT+CMGD=999'); send('AT+CMGR=-1'); send('AT+CMGR=abc'); send('AT+CNMI=9')
send('AT+CMEE=2'); send('AT+CMGR=999'); send('AT+CMGD=999'); send('AT+CMGR=-1'); send('AT+CMGR=abc'); send('AT+CNMI=9'); send('AT+CSCS="FOO"'); send('AT+CSQ=5')
# SMS storage capacity
send('AT+CPMS="ME","ME","ME"'); send('AT+CPMS?'); send('AT+CPMS="MT","MT","MT"'); send('AT+CPMS?'); send('AT+CPMS="SM","SM","SM"'); send('AT+CPMS?')
# listing format toggle
send('AT+CMGL="ALL"'); send('AT+CSDH=1'); send('AT+CMGL="ALL"'); send('AT+CMGR=0'); send('AT+CSDH=0'); send('AT+CMGR=0'); send('AT+CSDH?')
# timing: 10x AT and CSQ
for _ in range(10): send('AT')
for _ in range(5): send('AT+CSQ')
json.dump(log,open(S+'/behave.json','w'),indent=0)
print('done',len(log))
