# Ting and voice calls

US carriers have shut down 2G/3G, so voice on an LTE module generally needs VoLTE (IMS), and carriers often approve devices by IMEI. The SIM is from **Ting**, an MVNO.

## What is known

- The same SIM placed voice calls on the **SIM7600A-H** (old HAT, over UART). So Ting accepted that module and provided voice to it.
- Ting's help center says a device must be unlocked, support its bands and technologies, and pass its IMEI Compatibility Checker: https://internet.ting.com/help-center-articles/can-i-bring-my-current-phone-to-ting
- It lists VoLTE support among the reasons devices fail. Anecdotal and undated: Ting runs on T-Mobile's network and now requires VoLTE. Treat as a lead, not fact.
- A 2023 PinePhone thread reported one IMEI passing the checker while two others failed, so results can differ per IMEI: https://forum.pine64.org/showthread.php?pid=116227

## What it means for the project

- Use the **A-H board**. It is proven on this SIM and carrier.
- The G-H module on the (B) V2 is a different IMEI and variant. Ting may refuse it for voice even though the SIM works. Do not invest in the dead-bug route until it passes the checks below.

## Checks

1. Read the A-H's IMEI with `AT+CGSN` and note `AT+COPS?`, `AT+CEREG?` while a call works.
2. If the G-H route is ever revived, read its IMEI and run it through Ting's checker (or ask Ting support whether a SIM7600G-H is accepted for voice) before building anything.

## Update: confirmed working

On the A-H with the Ting line fixed (see [board-test-findings.md](board-test-findings.md)), voice calls ring through with audio on the HAT's headset jack, and SMS sends and arrives. VoLTE was already enabled on the module. An earlier failure was most likely the Ting line being registered against a wrong IMEI, not the module.
