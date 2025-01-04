#pragma once

namespace esphome {
namespace ade7880 {

// Page 87-88 Table 30. Registers Located in DSP Data Memory RAM
enum DspRegisters {
  // Register Name                    Addres  R/W  Bt  CommBln  Ty  Default     Description
  // ----------------------------     ------  ---  --  -------  --  ----------  --------------------------------------------------------------------
  ADE7880_AIGAIN = 0x4380,         // 0x4380  R/W  24  32 ZPSE  S   0x000000    Phase A current gain adjust.
  ADE7880_AVGAIN,                  // 0x4381  R/W  24  32 ZPSE  S   0x000000    Phase A voltage gain adjust.
  ADE7880_BIGAIN,                  // 0x4382  R/W  24  32 ZPSE  S   0x000000    Phase B current gain adjust.
  ADE7880_BVGAIN,                  // 0x4383  R/W  24  32 ZPSE  S   0x000000    Phase B voltage gain adjust.
  ADE7880_CIGAIN,                  // 0x4384  R/W  24  32 ZPSE  S   0x000000    Phase C current gain adjust.
  ADE7880_CVGAIN,                  // 0x4385  R/W  24  32 ZPSE  S   0x000000    Phase C voltage gain adjust.
  ADE7880_NIGAIN,                  // 0x4386  R/W  24  32 ZPSE  S   0x000000    Neutral current gain adjust.
  ADE7880_DICOEFF = 0x4388,        // 0x4388  R/W  24  32 ZPSE  S   0x0000000   Register used in the digital integrator algorithm. If the integrator is turned on, it must be set at 0xFF8000. In practice, it is transmitted as 0xFFF8000.
  ADE7880_APGAIN,                  // 0x4389  R/W  24  32 ZPSE  S   0x000000    Phase A power gain adjust.
  ADE7880_AWATTOS,                 // 0x438A  R/W  24  32 ZPSE  S   0x000000    Phase A total active power offset adjust.
  ADE7880_BPGAIN,                  // 0x438B  R/W  24  32 ZPSE  S   0x000000    Phase B power gain adjust.
  ADE7880_BWATTOS,                 // 0x438C  R/W  24  32 ZPSE  S   0x000000    Phase B total active power offset adjust.
  ADE7880_CPGAIN,                  // 0x438D  R/W  24  32 ZPSE  S   0x000000    Phase C power gain adjust.
  ADE7880_CWATTOS,                 // 0x438E  R/W  24  32 ZPSE  S   0x000000    Phase C total active power offset adjust.
  ADE7880_AIRMSOS,                 // 0x438F  R/W  24  32 ZPSE  S   0x000000    Phase A current rms offset.
  ADE7880_AVRMSOS,                 // 0x4390  R/W  24  32 ZPSE  S   0x000000    Phase A voltage rms offset.
  ADE7880_BIRMSOS,                 // 0x4391  R/W  24  32 ZPSE  S   0x000000    Phase B current rms offset.
  ADE7880_BVRMSOS,                 // 0x4392  R/W  24  32 ZPSE  S   0x000000    Phase B voltage rms offset.
  ADE7880_CIRMSOS,                 // 0x4393  R/W  24  32 ZPSE  S   0x000000    Phase C current rms offset.
  ADE7880_CVRMSOS,                 // 0x4394  R/W  24  32 ZPSE  S   0x000000    Phase C voltage rms offset.
  ADE7880_NIRMSOS,                 // 0x4395  R/W  24  32 ZPSE  S   0x000000    Neutral current rms offset.
  ADE7880_HPGAIN = 0x4398,         // 0x4398  R/W  24  32 ZPSE  S   0x000000    Harmonic powers gain adjust.
  ADE7880_ISUMLVL,                 // 0x4399  R/W  24  32 ZPSE  S   0x000000    Threshold used in comparison between the sum of phase currents and the neutral current.
  ADE7880_VLEVEL = 0x439F,         // 0x439F  R/W  28  32 ZP    S   0x0000000   Register used in the algorithm that computes the fundamental active and reactive powers. Set this register according to Equation 22 for proper functioning of fundamental powers and harmonic computations.
  ADE7880_AFWATTOS = 0x43A2,       // 0x43A2  R/W  24  32 ZPSE  S   0x000000    Phase A fundamental active power offset adjust.
  ADE7880_BFWATTOS,                // 0x43A3  R/W  24  32 ZPSE  S   0x000000    Phase B fundamental active power offset adjust.
  ADE7880_CFWATTOS,                // 0x43A4  R/W  24  32 ZPSE  S   0x000000    Phase C fundamental active power offset adjust.
  ADE7880_AFVAROS,                 // 0x43A5  R/W  24  32 ZPSE  S   0x000000    Phase A fundamental reactive power offset adjust.
  ADE7880_BFVAROS,                 // 0x43A6  R/W  24  32 ZPSE  S   0x000000    Phase B fundamental reactive power offset adjust.
  ADE7880_CFVAROS,                 // 0x43A7  R/W  24  32 ZPSE  S   0x000000    Phase C fundamental reactive power offset adjust.
  ADE7880_AFIRMSOS,                // 0x43A8  R/W  24  32 ZPSE  S   0x000000    Phase A fundamental current rms offset.
  ADE7880_BFIRMSOS,                // 0x43A9  R/W  24  32 ZPSE  S   0x000000    Phase B fundamental current rms offset.
  ADE7880_CFIRMSOS,                // 0x43AA  R/W  24  32 ZPSE  S   0x000000    Phase C fundamental current rms offset.
  ADE7880_AFVRMSOS,                // 0x43AB  R/W  24  32 ZPSE  S   0x000000    Phase A fundamental voltage rms offset.
  ADE7880_BFVRMSOS,                // 0x43AC  R/W  24  32 ZPSE  S   0x000000    Phase B fundamental voltage rms offset.
  ADE7880_CFVRMSOS,                // 0x43AD  R/W  24  32 ZPSE  S   0x000000    Phase C fundamental voltage rms offset.
  ADE7880_HXWATTOS,                // 0x43AE  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic X (see Harmonics Calculations section for details).
  ADE7880_HYWATTOS,                // 0x43AF  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic Y (see Harmonics Calculations section for details).
  ADE7880_HZWATTOS,                // 0x43B0  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic Z (see Harmonics Calculations section for details).
  ADE7880_HXVAROS,                 // 0x43B1  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic X (see Harmonics Calculations section for details).
  ADE7880_HYVAROS,                 // 0x43B2  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic Y (see Harmonics Calculations section for details).
  ADE7880_HZVAROS,                 // 0x43B3  R/W  24  32 ZPSE  S   0x000000    Active power offset adjust on harmonic Z (see Harmonics Calculations section for details).
  ADE7880_HXIRMSOS,                // 0x43B4  R/W  24  32 ZPSE  S   0x000000    Current rms offset on harmonic X (see Harmonics Calculations section for details).
  ADE7880_HYIRMSOS,                // 0x43B5  R/W  24  32 ZPSE  S   0x000000    Current rms offset on harmonic Y (see Harmonics Calculations section for details).
  ADE7880_HZIRMSOS,                // 0x43B6  R/W  24  32 ZPSE  S   0x000000    Current rms offset on harmonic Z (see Harmonics Calculations section for details).
  ADE7880_HXVRMSOS,                // 0x43B7  R/W  24  32 ZPSE  S   0x000000    Voltage rms offset on harmonic X (see Harmonics Calculations section for details).
  ADE7880_HYVRMSOS,                // 0x43B8  R/W  24  32 ZPSE  S   0x000000    Voltage rms offset on harmonic Y (see Harmonics Calculations section for details).
  ADE7880_HZVRMSOS,                // 0x43B9  R/W  24  32 ZPSE  S   0x000000    Voltage rms offset on harmonic Z (see Harmonics Calculations section for details).
  ADE7880_AIRMS = 0x43C0,          // 0x43C0  R    24  32 ZP    S   N/A         Phase A current rms value.
  ADE7880_AVRMS,                   // 0x43C1  R    24  32 ZP    S   N/A         Phase A voltage rms value.
  ADE7880_BIRMS,                   // 0x43C2  R    24  32 ZP    S   N/A         Phase B current rms value.
  ADE7880_BVRMS,                   // 0x43C3  R    24  32 ZP    S   N/A         Phase B voltage rms value.
  ADE7880_CIRMS,                   // 0x43C4  R    24  32 ZP    S   N/A         Phase C current rms value.
  ADE7880_CVRMS,                   // 0x43C5  R    24  32 ZP    S   N/A         Phase C voltage rms value.
  ADE7880_NIRMS,                   // 0x43C6  R    24  32 ZP    S   N/A         Neutral current rms value.
  ADE7880_ISUM                     // 0x43C7  R    28  32 ZP    S   N/A         Sum of IAWV, IBWV and ICWV registers.
};

// Page 88 Table 31. Internal DSP Memory RAM Registers
enum InternalDspRegisters {
  ADE7880_Run = 0xE228             // 0xE228  R/W  16  16       U   0x0000      Run register starts and stops the DSP. See the Digital Signal Processor section for more details.
};

// Page 89 Table 32. Billable Registers
enum BillableRegisters {
  ADE7880_AWATTHR = 0xE400,        // 0xE400  R    32  32       S   0x00000000  Phase A total active energy accumulation.
  ADE7880_BWATTHR,                 // 0xE401  R    32  32       S   0x00000000  Phase B total active energy accumulation.
  ADE7880_CWATTHR,                 // 0xE402  R    32  32       S   0x00000000  Phase C total active energy accumulation.
  ADE7880_AFWATTHR,                // 0xE403  R    32  32       S   0x00000000  Phase A fundamental active energy accumulation.
  ADE7880_BFWATTHR,                // 0xE404  R    32  32       S   0x00000000  Phase B fundamental active energy accumulation.
  ADE7880_CFWATTHR,                // 0xE405  R    32  32       S   0x00000000  Phase C fundamental active energy accumulation.
  ADE7880_AFVARHR = 0xE409,        // 0xE409  R    32  32       S   0x00000000  Phase A fundamental reactive energy accumulation.
  ADE7880_BFVARHR,                 // 0xE40A  R    32  32       S   0x00000000  Phase B fundamental reactive energy accumulation.
  ADE7880_CFVARHR,                 // 0xE40B  R    32  32       S   0x00000000  Phase C fundamental reactive energy accumulation.
  ADE7880_AVAHR,                   // 0xE40C  R    32  32       S   0x00000000  Phase A apparent energy accumulation.
  ADE7880_BVAHR,                   // 0xE40D  R    32  32       S   0x00000000  Phase B apparent energy accumulation.
  ADE7880_CVAHR                    // 0xE40E  R    32  32       S   0x00000000  Phase C apparent energy accumulation.
};

// Page 89-93 Table 33. Configuration and Power Quality Registers
enum ConfigurationAndPowerQualityRegisters {
  ADE7880_IPEAK = 0xE500,          // 0xE500  R    32  32       U   N/A         Current peak register. See Figure 60 and Table 34 for details about its composition.
  ADE7880_VPEAK,                   // 0xE501  R    32  32       U   N/A         Voltage peak register. See Figure 60 and Table 35 for details about its composition.
  ADE7880_STATUS0,                 // 0xE502  R/W  32  32       U   N/A         Interrupt Status Register 0. See Table 36.
  ADE7880_STATUS1,                 // 0xE503  R/W  32  32       U   N/A         Interrupt Status Register 1. See Table 37.
  ADE7880_AIMAV,                   // 0xE504  R    20  32 ZP    U   N/A         Phase A current mean absolute value computed during PSM0 and PSM1 modes.
  ADE7880_BIMAV,                   // 0xE505  R    20  32 ZP    U   N/A         Phase B current mean absolute value computed during PSM0 and PSM1 modes.
  ADE7880_CIMAV,                   // 0xE506  R    20  32 ZP    U   N/A         Phase C current mean absolute value computed during PSM0 and PSM1 modes.
  ADE7880_OILVL,                   // 0xE507  R/W  24  32 ZP    U   0xFFFFFF    Overcurrent threshold.
  ADE7880_OVLVL,                   // 0xE508  R/W  24  32 ZP    U   0xFFFFFF    Overvoltage threshold.
  ADE7880_SAGLVL,                  // 0xE509  R/W  24  32 ZP    U   0x000000    Voltage SAG level threshold.
  ADE7880_MASK0,                   // 0xE50A  R/W  32  32       U   0x00000000  Interrupt Enable Register 0. See Table 38.
  ADE7880_MASK1,                   // 0xE50B  R/W  32  32       U   0x00000000  Interrupt Enable Register 1. See Table 39.
  ADE7880_IAWV,                    // 0xE50C  R    24  32 SE    S   N/A         Instantaneous value of Phase A current.
  ADE7880_IBWV,                    // 0xE50D  R    24  32 SE    S   N/A         Instantaneous value of Phase B current.
  ADE7880_ICWV,                    // 0xE50E  R    24  32 SE    S   N/A         Instantaneous value of Phase C current.
  ADE7880_INWV,                    // 0xE50F  R    24  32 SE    S   N/A         Instantaneous value of neutral current.
  ADE7880_VAWV,                    // 0xE510  R    24  32 SE    S   N/A         Instantaneous value of Phase A voltage.
  ADE7880_VBWV,                    // 0xE511  R    24  32 SE    S   N/A         Instantaneous value of Phase B voltage.
  ADE7880_VCWV,                    // 0xE512  R    24  32 SE    S   N/A         Instantaneous value of Phase C voltage.
  ADE7880_AWATT,                   // 0xE513  R    24  32 SE    S   N/A         Instantaneous value of Phase A total active power.
  ADE7880_BWATT,                   // 0xE514  R    24  32 SE    S   N/A         Instantaneous value of Phase B total active power.
  ADE7880_CWATT,                   // 0xE515  R    24  32 SE    S   N/A         Instantaneous value of Phase C total active power.
  ADE7880_AVA = 0xE519,            // 0xE519  R    24  32 SE    S   N/A         Instantaneous value of Phase A apparent power.
  ADE7880_BVA,                     // 0xE51A  R    24  32 SE    S   N/A         Instantaneous value of Phase B apparent power.
  ADE7880_CVA,                     // 0xE51B  R    24  32 SE    S   N/A         Instantaneous value of Phase C apparent power.
  ADE7880_CHECKSUM = 0xE51F,       // 0xE51F  R    32  32       U   0xAFFA63B9  Checksum verification. See the Checksum Register section for details.
  ADE7880_VNOM,                    // 0xE520  R/W  24  32 ZP    S   0x000000    Nominal phase voltage rms used in the alternative computation of the apparent power. When the VNOMxEN bit is set, the
                                   //                                           applied voltage input in the corresponding phase is ignored and all corresponding rms voltage instances are replaced by the value in the VNOM register.
  ADE7880_LAST_RWDATA32 = 0xE5FF,  // 0xE5FF  R    32  32       U   N/A         Contains the data from the last successful 32-bit register communication.
  ADE7880_PHSTATUS,                // 0xE600  R    16  16       U   N/A         Phase peak register. See Table 40.
  ADE7880_ANGLE0,                  // 0xE601  R    16  16       U   N/A         Time Delay 0. See the Time Interval Between Phases section for details.
  ADE7880_ANGLE1,                  // 0xE602  R    16  16       U   N/A         Time Delay 1. See the Time Interval Between Phases section for details.
  ADE7880_ANGLE2,                  // 0xE603  R    16  16       U   N/A         Time Delay 2. See the Time Interval Between Phases section for details.
  ADE7880_PHNOLOAD = 0xE608,       // 0xE608  R    16  16       U   N/A         Phase no load register. See Table 41.
  ADE7880_LINECYC = 0xE60C,        // 0xE60C  R/W  16  16       U   0xFFFF      Line cycle accumulation mode count.
  ADE7880_ZXTOUT,                  // 0xE60D  R/W  16  16       U   0xFFFF      Zero-crossing timeout count.
  ADE7880_COMPMODE,                // 0xE60E  R/W  16  16       U   0x01FF      Computation-mode register. See Table 42.
  ADE7880_Gain,                    // 0xE60F  R/W  16  16       U   0x0000      PGA gains at ADC inputs. See Table 43.
  ADE7880_CFMODE,                  // 0xE610  R/W  16  16       U   0x0EA0      CFx configuration register. See Table 44.
  ADE7880_CF1DEN,                  // 0xE611  R/W  16  16       U   0x0000      CF1 denominator.
  ADE7880_CF2DEN,                  // 0xE612  R/W  16  16       U   0x0000      CF2 denominator.
  ADE7880_CF3DEN,                  // 0xE613  R/W  16  16       U   0x0000      CF3 denominator.
  ADE7880_APHCAL,                  // 0xE614  R/W  10  16 ZP    S   0x0000      Phase calibration of Phase A. See Table 45.
  ADE7880_BPHCAL,                  // 0xE615  R/W  10  16 ZP    S   0x0000      Phase calibration of Phase B. See Table 45.
  ADE7880_CPHCAL,                  // 0xE616  R/W  10  16 ZP    S   0x0000      Phase calibration Phase of C. See Table 45.
  ADE7880_PHSIGN,                  // 0xE617  R    16  16       U   N/A         Power sign register. See Table 46.
  ADE7880_CONFIG,                  // 0xE618  R/W  16  16       U   0x0002      ADE7880 configuration register. See Table 47.
  ADE7880_MMODE = 0xE700,          // 0xE700  R/W  8   8        U   0x1C        Measurement mode register. See Table 48.
  ADE7880_ACCMODE,                 // 0xE701  R/W  8   8        U   0x80        Accumulation mode register. See Table 49.
  ADE7880_LCYCMODE,                // 0xE702  R/W  8   8        U   0x78        Line accumulation mode behavior. See Table 51.
  ADE7880_PEAKCYC,                 // 0xE703  R/W  8   8        U   0x00        Peak detection half line cycles.
  ADE7880_SAGCYC,                  // 0xE704  R/W  8   8        U   0x00        SAG detection half line cycles.
  ADE7880_CFCYC,                   // 0xE705  R/W  8   8        U   0x01        Number of CF pulses between two consecutive energy latches. See the Synchronizing Energy Registers with CFx Outputs section.
  ADE7880_HSDC_CFG,                // 0xE706  R/W  8   8        U   0x00        HSDC configuration register. See Table 52.
  ADE7880_Version,                 // 0xE707  R    8   8        U               Version of die.
  ADE7880_DSPWP_SET = 0xE7E3,      // 0xE7E3  W    8   8        U   0x00        Write protect DSP (0x80) or enable write (0x00). See page 40.
  ADE7880_Reserved,                // 0xE7E4  R    8   8        U   0x08        This register must remain at this value for checksum functionality to work. If this register shows a different value while being read, reset the chip before working with the checksum feature.
  ADE7880_LAST_RWDATA8 = 0xE7FD,   // 0xE7FD  R    8   8        U   N/A         Contains the data from the last successful 8-bit register communication.
  ADE7880_DSPWP_SEL,               // 0xE7FE  W    8   8        U   0xAD        Select DSP writeprotect. See page 40.
  ADE7880_FVRMS = 0xE880,          // 0xE880  R    24  32       S   N/A         The rms value of the fundamental component of the phase voltage.
  ADE7880_FIRMS,                   // 0xE881  R    24  32       S   N/A         The rms value of the fundamental component of the phase current
  ADE7880_FWATT,                   // 0xE882  R    24  32       S   N/A         The active power of the fundamental component.
  ADE7880_FVAR,                    // 0xE883  R    24  32       S   N/A         The reactive power of the fundamental component.
  ADE7880_FVA,                     // 0xE884  R    24  32       S   N/A         The apparent power of the fundamental component.
  ADE7880_FPF,                     // 0xE885  R    24  32       S   N/A         The power factor of the fundamental component.
  ADE7880_VTHD,                    // 0xE886  R    24  32       S   N/A         Total harmonic distortion of the phase voltage.
  ADE7880_ITHD,                    // 0xE887  R    24  32       S   N/A         Total harmonic distortion of the phase current.
  ADE7880_HXVRMS,                  // 0xE888  R    24  32       S   N/A         The rms value of the phase voltage harmonic X.
  ADE7880_HXIRMS,                  // 0xE889  R    24  32       S   N/A         The rms value of the phase current harmonic X.
  ADE7880_HXWATT,                  // 0xE88A  R    24  32       S   N/A         The active power of the harmonic X.
  ADE7880_HXVAR,                   // 0xE88B  R    24  32       S   N/A         The reactive power of the harmonic X.
  ADE7880_HXVA,                    // 0xE88C  R    24  32       S   N/A         The apparent power of the harmonic X.
  ADE7880_HXPF,                    // 0xE88D  R    24  32       S   N/A         The power factor of the harmonic X.
  ADE7880_HXVHD,                   // 0xE88E  R    24  32       S   N/A         Harmonic distortion of the phase voltage harmonic X relative to the fundamental.
  ADE7880_HXIHD,                   // 0xE88F  R    24  32       S   N/A         Harmonic distortion of the phase current harmonic X relative to the fundamental.
  ADE7880_HYVRMS,                  // 0xE890  R    24  32       S   N/A         The rms value of the phase voltage harmonic Y.
  ADE7880_HYIRMS,                  // 0xE891  R    24  32       S   N/A         The rms value of the phase current harmonic Y.
  ADE7880_HYWATT,                  // 0xE892  R    24  32       S   N/A         The active power of the harmonic Y.
  ADE7880_HYVAR,                   // 0xE893  R    24  32       S   N/A         The reactive power of the harmonic Y.
  ADE7880_HYVA,                    // 0xE894  R    24  32       S   N/A         The apparent power of the harmonic Y.
  ADE7880_HYPF,                    // 0xE895  R    24  32       S   N/A         The power factor of the harmonic Y.
  ADE7880_HYVHD,                   // 0xE896  R    24  32       S   N/A         Harmonic distortion of the phase voltage harmonic Y relative to the fundamental.
  ADE7880_HYIHD,                   // 0xE897  R    24  32       S   N/A         Harmonic distortion of the phase current harmonic Y relative to the fundamental.
  ADE7880_HZVRMS,                  // 0xE898  R    24  32       S   N/A         The rms value of the phase voltage harmonic Z.
  ADE7880_HZIRMS,                  // 0xE899  R    24  32       S   N/A         The rms value of the phase current harmonic Z.
  ADE7880_HZWATT,                  // 0xE89A  R    24  32       S   N/A         The active power of the harmonic Z.
  ADE7880_HZVAR,                   // 0xE89B  R    24  32       S   N/A         The reactive power of the harmonic Z.
  ADE7880_HZVA,                    // 0xE89C  R    24  32       S   N/A         The apparent power of the harmonic Z.
  ADE7880_HZPF,                    // 0xE89D  R    24  32       S   N/A         The power factor of the harmonic Z.
  ADE7880_HZVHD,                   // 0xE89E  R    24  32       S   N/A         Harmonic distortion of the phase voltage harmonic Z relative to the fundamental.
  ADE7880_HZIHD,                   // 0xE89F  R    24  32       S   N/A         Harmonic distortion of the phase current harmonic Z relative to the fundamental.
  ADE7880_HCONFIG = 0xE900,        // 0xE900  R/W  16  16       U   0x08        Harmonic Calculations Configuration register. See Table 54.
  ADE7880_APF = 0xE902,            // 0xE902  R    16  16       S   N/A         Phase A power factor.
  ADE7880_BPF,                     // 0xE903  R    16  16       S   N/A         Phase B power factor.
  ADE7880_CPF,                     // 0xE904  R    16  16       S   N/A         Phase C power factor.
  ADE7880_APERIOD,                 // 0xE905  R    16  16       U   N/A         Line period on Phase A voltage.
  ADE7880_BPERIOD,                 // 0xE906  R    16  16       U   N/A         Line period on Phase B voltage.
  ADE7880_CPERIOD,                 // 0xE907  R    16  16       U   N/A         Line period on Phase C voltage.
  ADE7880_APNOLOAD,                // 0xE908  R/W  16  16       U   0x0000      No load threshold in the total/ fundamental active power data paths. Do not write 0xFFFF to this register.
  ADE7880_VARNOLOAD,               // 0xE909  R/W  16  16       U   0x0000      No load threshold in the total/ fundamental reactive power data path. Do not write 0xFFFF to this register.
  ADE7880_VANOLOAD,                // 0xE90A  R/W  16  16       U   0x0000      No load threshold in the apparent power data path. Do not write 0xFFFF to this register.
  ADE7880_LAST_ADD = 0xE9FE,       // 0xE9FE  R    16  16       U   N/A         The address of the register successfully accessed during the last read/write operation.
  ADE7880_LAST_RWDATA16,           // 0xE9FF  R    16  16       U   N/A         Contains the data from the last successful 16-bit register communication.
  ADE7880_CONFIG3,                 // 0xEA00  R/W  8   8        U   0x01        Configuration register. See Table 53.
  ADE7880_LAST_OP,                 // 0xEA01  R    8   8        U   N/A         Indicates the type, read or write, of the last successful read/write operation.
  ADE7880_WTHR,                    // 0xEA02  R/W  8   8        U   0x03        Threshold used in phase total/fundamental active power data path.
  ADE7880_VARTHR,                  // 0xEA03  R/W  8   8        U   0x03        Threshold used in phase total/fundamental reactive power data path.
  ADE7880_VATHR,                   // 0xEA04  R/W  8   8        U   0x03        Threshold used in phase apparent power data path.
  ADE7880_HX = 0xEA08,             // 0xEA08  R/W  8   8        U   3           Selects an index of the harmonic monitored by the harmonic computations.
  ADE7880_HY,                      // 0xEA09  R/W  8   8        U   5           Selects an index of the harmonic monitored by the harmonic computations.
  ADE7880_HZ,                      // 0xEA0A  R/W  8   8        U   7           Selects an index of the harmonic monitored by the harmonic computations.
  ADE7880_LPOILVL = 0xEC00,        // 0xEC00  R/W  8   8        U   0x07        Overcurrent threshold used during PSM2 mode. See Table 55 in which the register is detailed.
  ADE7880_CONFIG2                  // 0xEC01  R/W  8   8        U   0x00        Configuration register used during PSM1 mode. See Table 56.
};

// Page 93-94 Table 36. STATUS0 Register (Address 0xE502)
enum Status0Register {
  STATUS0_AEHF = 1 << 0,           // Bit 0  When this bit is set to 1, it indicates that Bit 30 of any one of the total active energy registers (AWATTHR, BWATTHR, or CWATTHR) has changed.
  STATUS0_FAEHF = 1 << 1,          // Bit 1  When this bit is set to 1, it indicates that Bit 30 of any one of the fundamental active energy registers, FWATTHR, BFWATTHR, or CFWATTHR, has changed.
  STATUS0_FREHF = 1 << 3,          // Bit 3  When this bit is set to 1, it indicates that Bit 30 of any one of the fundamental reactive energy registers, AFVARHR, BFVARHR, or CFVARHR, has changed.
  STATUS0_VAEHF = 1 << 4,          // Bit 4  When this bit is set to 1, it indicates that Bit 30 of any one of the apparent energy registers (AVAHR, BVAHR, or CVAHR) has changed.
  STATUS0_LENERGY = 1 << 5,        // Bit 5  When this bit is set to 1, in line energy accumulation mode, it indicates the end of an integration over an integer number of half line cycles set in the LINECYC register.
  STATUS0_REVAPA = 1 << 6,         // Bit 6  When this bit is set to 1, it indicates that the Phase A active power identified by Bit 6 (REVAPSEL) in the ACCMODE register (total or fundamental) has changed sign. The sign itself is indicated in Bit 0 (AWSIGN) of the PHSIGN register (see Table 46).
  STATUS0_REVAPB = 1 << 7,         // Bit 7  When this bit is set to 1, it indicates that the Phase B active power identified by Bit 6 (REVAPSEL) in the ACCMODE register (total or fundamental) has changed sign. The sign itself is indicated in Bit 1 (BWSIGN) of the PHSIGN register (see Table 46).
  STATUS0_REVAPC = 1 << 8,         // Bit 8  When this bit is set to 1, it indicates that the Phase C active power identified by Bit 6 (REVAPSEL) in the ACCMODE register (total or fundamental) has changed sign. The sign itself is indicated in Bit 2 (CWSIGN) of the PHSIGN register (see Table 46).
  STATUS0_REVPSUM1 = 1 << 9,       // Bit 9  When this bit is set to 1, it indicates that the sum of all phase powers in the CF1 data path has changed sign. The sign itself is indicated in Bit 3 (SUM1SIGN) of the PHSIGN register (see Table 46).
  STATUS0_REVFRPA = 1 << 10,       // Bit 10 When this bit is set to 1, it indicates that the Phase A fundamental reactive power has changed sign. The sign itself is indicated in Bit 4 (AFVARSIGN) of the PHSIGN register (see Table 46).
  STATUS0_REVFRPB = 1 << 11,       // Bit 11 When this bit is set to 1, it indicates that the Phase B fundamental reactive power has changed sign. The sign itself is indicated in Bit 5 (BFVARSIGN) of the PHSIGN register (see Table 46).
  STATUS0_REVFRPC = 1 << 12,       // Bit 12 When this bit is set to 1, it indicates that the Phase C fundamental reactive power has changed sign. The sign itself is indicated in Bit 6 (CFVARSIGN) of the PHSIGN register (see Table 46).
  STATUS0_REVPSUM2 = 1 << 13,      // Bit 13 When this bit is set to 1, it indicates that the sum of all phase powers in the CF2 data path has changed sign. The sign itself is indicated in Bit 7 (SUM2SIGN) of the PHSIGN register (see Table 46).
  STATUS0_CF1 = 1 << 14,           // Bit 14 When this bit is set to 1, it indicates a high-to-low transition has occurred at CF1 pin; that is, an active low pulse has been generated. The bit is set even if the CF1 output is disabled by setting Bit 9 (CF1DIS) to 1 in the CFMODE register. The type of power used at the CF1 pin is determined by Bits[2:0] (CF1SEL[2:0]) in the CFMODE register (see Table 44).
  STATUS0_CF2 = 1 << 15,           // Bit 15 When this bit is set to 1, it indicates a high-to-low transition has occurred at the CF2 pin; that is, an active low pulse has been generated. The bit is set even if the CF2 output is disabled by setting Bit 10 (CF2DIS) to 1 in the CFMODE register. The type of power used at the CF2 pin is determined by Bits[5:3] (CF2SEL[2:0]) in the CFMODE register (see Table 44).
  STATUS0_CF3 = 1 << 16,           // Bit 16 When this bit is set to 1, it indicates a high-to-low transition has occurred at CF3 pin; that is, an active low pulse has been generated. The bit is set even if the CF3 output is disabled by setting Bit 11 (CF3DIS) to 1 in the CFMODE register. The type of power used at the CF3 pin is determined by Bits[8:6] (CF3SEL[2:0]) in the CFMODE register (see Table 44).
  STATUS0_DREADY = 1 << 17,        // Bit 17 When this bit is set to 1, it indicates that all periodical (at 8 kHz rate) DSP computations have finished.
  STATUS0_REVPSUM3 = 1 << 18,      // Bit 18 When this bit is set to 1, it indicates that the sum of all phase powers in the CF3 data path has changed sign. The sign itself is indicated in Bit 8 (SUM3SIGN) of the PHSIGN register (see Table 46).
  STATUS0_HREADY = 1 << 19         // Bit 19 When this bit is set to 1, it indicates the harmonic block output registers are updated. If Bit 0 (HRCFG) in the HCONFIG register is cleared to 0, this flag is set to 1 every time the harmonic block output registers are updated at a rate identified by Bits [7:5] (HRATE) in the HCONFIG register starting HSTIME (Bits [4:3] in the HCONFIG register) after the harmonic block setup. If Bit HRCFG is set to 1, the HREADY flag is set to 1 every time the harmonic block output registers are updated at a rate identified by Bits [7:5] (HRATE) in the HCONFIG register, starting immediately after the harmonic block setup.
};

// Page 95-96 Table 37. STATUS1 Register (Address 0xE503)
enum Status1Register {
  STATUS1_NLOAD = 1 << 0,          // Bit 0  When this bit is set to 1, it indicates that at least one phase entered no load condition determined by the total active power and apparent power. The phase is indicated in Bits[2:0] (NLPHASE[x]) in the PHNOLOAD register (see Table 41.)
  STATUS1_FNLOAD = 1 << 1,         // Bit 1  When this bit is set to 1, it indicates that at least one phase entered no load condition based on fundamental active and reactive powers. The phase is indicated in Bits[5:3] (FNLPHASE[x]) in the PHNOLOAD register (see Table 41).
  STATUS1_VANLOAD = 1 << 2,        // Bit 2  When this bit is set to 1, it indicates that at least one phase entered no load condition based on apparent power. The phase is indicated in Bits[8:6] (VANLPHASE[x]) in the PHNOLOAD register (see Table 41).
  STATUS1_ZXTOVA = 1 << 3,         // Bit 3  When this bit is set to 1, it indicates a zero crossing on Phase A voltage is missing.
  STATUS1_ZXTOVB = 1 << 4,         // Bit 4  When this bit is set to 1, it indicates a zero crossing on Phase B voltage is missing.
  STATUS1_ZXTOVC = 1 << 5,         // Bit 5  When this bit is set to 1, it indicates a zero crossing on Phase C voltage is missing.
  STATUS1_ZXTOIA = 1 << 6,         // Bit 6  When this bit is set to 1, it indicates a zero crossing on Phase A current is missing.
  STATUS1_ZXTOIB = 1 << 7,         // Bit 7  When this bit is set to 1, it indicates a zero crossing on Phase B current is missing.
  STATUS1_ZXTOIC = 1 << 8,         // Bit 8  When this bit is set to 1, it indicates a zero crossing on Phase C current is missing.
  STATUS1_ZXVA = 1 << 9,           // Bit 9  When this bit is set to 1, it indicates a zero crossing has been detected on Phase A voltage.
  STATUS1_ZXVB = 1 << 10,          // Bit 10 When this bit is set to 1, it indicates a zero crossing has been detected on Phase B voltage.
  STATUS1_ZXVC = 1 << 11,          // Bit 11 When this bit is set to 1, it indicates a zero crossing has been detected on Phase C voltage.
  STATUS1_ZXIA = 1 << 12,          // Bit 12 When this bit is set to 1, it indicates a zero crossing has been detected on Phase A current.
  STATUS1_ZXIB = 1 << 13,          // Bit 13 When this bit is set to 1, it indicates a zero crossing has been detected on Phase B current.
  STATUS1_ZXIC = 1 << 14,          // Bit 14 When this bit is set to 1, it indicates a zero crossing has been detected on Phase C current.
  STATUS1_RSTDONE = 1 << 15,       // Bit 15 In case of a software reset command, Bit 7 (SWRST) is set to 1 in the CONFIG register, or a transition from PSM1, PSM2, or PSM3 to PSM0, or a hardware reset, this bit is set to 1 at the end of the transition process and after all registers change value to default. The IRQ1 pin goes low to signal this moment because this interrupt cannot be disabled.
  STATUS1_SAG = 1 << 16,           // Bit 16 When this bit is set to 1, it indicates one of phase voltages entered or exited a sag state. The phase is indicated by Bits[14:12] (VSPHASE[x]) in the PHSTATUS register (see Table 40).
  STATUS1_OI = 1 << 17,            // Bit 17 When this bit is set to 1, it indicates an overcurrent event has occurred on one of the phases indicated by Bits[5:3] (OIPHASE[x]) in the PHSTATUS register (see Table 40).
  STATUS1_OV = 1 << 18,            // Bit 18 When this bit is set to 1, it indicates an overvoltage event has occurred on one of the phases indicated by Bits[11:9] (OVPHASE[x]) in the PHSTATUS register (see Table 40).
  STATUS1_SEQERR = 1 << 19,        // Bit 19 When this bit is set to 1, it indicates a negative-to-positive zero crossing on Phase A voltage was not followed by a negative-to-positive zero crossing on Phase B voltage but by a negative-to-positive zero crossing on Phase C voltage.
  STATUS1_MISMTCH = 1 << 20,       // Bit 20 When this bit is set to 1, it indicates ISUMLVLINWVISUM >− , where ISUMLVL is indicated in the ISUMLVL register.
  STATUS1_RESERVED1 = 1 << 21,     // Bit 21 Reserved. This bit is always set to 1.
  STATUS1_RESERVED2 = 1 << 22,     // Bit 22 Reserved. This bit is always set to 0.
  STATUS1_PKI = 1 << 23            // Bit 23 When this bit is set to 1, it indicates that the period used to detect the peak value in the current channel has ended. The IPEAK register contains the peak value and the phase where the peak has been detected (see Table 34).
};

// Page 96-97 Table 38. MASK0 Register (Address 0xE50A)
enum Mask0Register {
  MASK0_AEHF = 1 << 0,             // Bit 0  When this bit is set to 1, it enables an interrupt when Bit 30 of any one of the total active energy registers (AWATTHR, BWATTHR, or CWATTHR) changes.
  MASK0_FAEHF = 1 << 1,            // Bit 1  When this bit is set to 1, it enables an interrupt when Bit 30 of any one of the fundamental active energy registers (AFWATTHR, BFWATTHR, or CFWATTHR) changes.
  MASK0_RESERVED = 1 << 2,         // Bit 2  Reserved. This bit does not manage any functionality.
  MASK0_FREHF = 1 << 3,            // Bit 3  When this bit is set to 1, it enables an interrupt when Bit 30 of any one of the fundamental reactive energy registers (AFVARHR, BFVARHR, or CFVARHR) changes.
  MASK0_VAEHF = 1 << 4,            // Bit 4  When this bit is set to 1, it enables an interrupt when Bit 30 of any one of the apparent energy registers (AVAHR, BVAHR, or CVAHR) changes.
  MASK0_LENERGY = 1 << 5,          // Bit 5  When this bit is set to 1, in line energy accumulation mode, it enables an interrupt at the end of an integration over an integer number of half line cycles set in the LINECYC register.
  MASK0_REVAPA = 1 << 6,           // Bit 6  When this bit is set to 1, it enables an interrupt when the Phase A active power identified by Bit 6 (REVAPSEL) in the ACCMODE register (total or fundamental) changes sign.
  MASK0_REVAPB = 1 << 7,           // Bit 7  When this bit is set to 1, it enables an interrupt when the Phase B active power identified by Bit 6 (REVAPSEL) in the ACCMODE register (total or fundamental) changes sign.
  MASK0_REVAPC = 1 << 8,           // Bit 8  When this bit is set to 1, it enables an interrupt when the Phase C active power identified by Bit 6 (REVAPSEL) in the ACCMODE register (total or fundamental) changes sign.
  MASK0_REVPSUM1 = 1 << 9,         // Bit 9  When this bit is set to 1, it enables an interrupt when the sum of all phase powers in the CF1 data path changes sign.
  MASK0_REVFRPA = 1 << 10,         // Bit 10 When this bit is set to 1, it enables an interrupt when the Phase A fundamental reactive power changes sign.
  MASK0_REVFRPB = 1 << 11,         // Bit 11 When this bit is set to 1, it enables an interrupt when the Phase B fundamental reactive power changes sign.
  MASK0_REVFRPC = 1 << 12,         // Bit 12 When this bit is set to 1, it enables an interrupt when the Phase C fundamental reactive power changes sign.
  MASK0_REVPSUM2 = 1 << 13,        // Bit 13 When this bit is set to 1, it enables an interrupt when the sum of all phase powers in the CF2 data path changes sign.
  MASK0_CF1 = 1 << 14,             // Bit 14 When this bit is set to 1, it enables an interrupt when a high-to-low transition occurs at the CF1 pin, that is an active low pulse is generated. The interrupt can be enabled even if the CF1 output is disabled by setting Bit 9 (CF1DIS) to 1 in the CFMODE register. The type of power used at the CF1 pin is determined by Bits[2:0] (CF1SEL[2:0]) in the CFMODE register (see Table 44).
  MASK0_CF2 = 1 << 15,             // Bit 15 When this bit is set to 1, it enables an interrupt when a high-to-low transition occurs at CF2 pin, that is an active low pulse is generated. The interrupt may be enabled even if the CF2 output is disabled by setting Bit 10 (CF2DIS) to 1 in the CFMODE register. The type of power used at the CF2 pin is determined by Bits[5:3] (CF2SEL[2:0]) in the CFMODE register (see Table 44).
  MASK0_CF3 = 1 << 16,             // Bit 16 When this bit is set to 1, it enables an interrupt when a high to low transition occurs at CF3 pin, that is an active low pulse is generated. The interrupt may be enabled even if the CF3 output is disabled by setting Bit 11 (CF3DIS) to 1 in the CFMODE register. The type of power used at the CF3 pin is determined by Bits[8:6] (CF3SEL[2:0]) in the CFMODE register (see Table 44).
  MASK0_DREADY = 1 << 17,          // Bit 17 When this bit is set to 1, it enables an interrupt when all periodical (at 8 kHz rate) DSP computations finish.
  MASK0_REVPSUM3 = 1 << 18,        // Bit 18 When this bit is set to 1, it enables an interrupt when the sum of all phase powers in the CF3 data path changes sign.
  MASK0_HREADY = 1 << 19           // Bit 19 When this bit is set to 1, it enables an interrupt when the harmonic block output registers have been updated. If Bit 0 (HRCFG) in HCONFIG register is cleared to 0, the interrupt is triggered every time the harmonic block output registers are updated at a rate identified by Bits [7:5] (HRATE) in HCONFIG register starting HSTIME (Bits [4:3] in HCONFIG register) after the harmonic block setup. If Bit HRCFG is set to 1, the interrupt is triggered every time the harmonic block output registers are updated at a rate identified by Bits [7:5] (HRATE) in HCONFIG register starting immediately after the harmonic block setup.
};

// Page 97-98 Table 39. MASK1 Register (Address 0xE50B)
enum Mask1Register {
  MASK1_NLOAD = 1 << 0,            // Bit 0  When this bit is set to 1, it enables an interrupt when at least one phase enters no load condition determined by the total active power and VNOM based apparent power.
  MASK1_FNLOAD = 1 << 1,           // Bit 1  When this bit is set to 1, it enables an interrupt when at least one phase enters no load condition based on fundamental active and reactive powers.
  MASK1_VANLOAD = 1 << 2,          // Bit 2  When this bit is set to 1, it enables an interrupt when at least one phase enters no load condition based on apparent power.
  MASK1_ZXTOVA = 1 << 3,           // Bit 3  When this bit is set to 1, it enables an interrupt when a zero crossing on Phase A voltage is missing.
  MASK1_ZXTOVB = 1 << 4,           // Bit 4  When this bit is set to 1, it enables an interrupt when a zero crossing on Phase B voltage is missing.
  MASK1_ZXTOVC = 1 << 5,           // Bit 5  When this bit is set to 1, it enables an interrupt when a zero crossing on Phase C voltage is missing.
  MASK1_ZXTOIA = 1 << 6,           // Bit 6  When this bit is set to 1, it enables an interrupt when a zero crossing on Phase A current is missing.
  MASK1_ZXTOIB = 1 << 7,           // Bit 7  When this bit is set to 1, it enables an interrupt when a zero crossing on Phase B current is missing.
  MASK1_ZXTOIC = 1 << 8,           // Bit 8  When this bit is set to 1, it enables an interrupt when a zero crossing on Phase C current is missing.
  MASK1_ZXVA = 1 << 9,             // Bit 9  When this bit is set to 1, it enables an interrupt when a zero crossing is detected on Phase A voltage.
  MASK1_ZXVB = 1 << 10,            // Bit 10 When this bit is set to 1, it enables an interrupt when a zero crossing is detected on Phase B voltage.
  MASK1_ZXVC = 1 << 11,            // Bit 11 When this bit is set to 1, it enables an interrupt when a zero crossing is detected on Phase C voltage.
  MASK1_ZXIA = 1 << 12,            // Bit 12 When this bit is set to 1, it enables an interrupt when a zero crossing is detected on Phase A current.
  MASK1_ZXIB = 1 << 13,            // Bit 13 When this bit is set to 1, it enables an interrupt when a zero crossing is detected on Phase B current.
  MASK1_ZXIC = 1 << 14,            // Bit 14 When this bit is set to 1, it enables an interrupt when a zero crossing is detected on Phase C current.
  MASK1_RSTDONE = 1 << 15,         // Bit 15 In case of a software reset command, Bit 7 (SWRST) is set to 1 in the CONFIG register, or a transition from PSM1, PSM2, or PSM3 to PSM0, or a hardware reset, this bit is set to 1 at the end of the transition process and after all registers change value to default. The IRQ1 pin goes low to signal this moment because this interrupt cannot be disabled.
  MASK1_SAG = 1 << 16,             // Bit 16 When this bit is set to 1, it enables an interrupt when one of the phase voltages entered or exited a sag state. The phase is indicated by Bits[14:12] (VSPHASE[x]) in the PHSTATUS register (see Table 40).
  MASK1_OI = 1 << 17,              // Bit 17 When this bit is set to 1, it enables an interrupt when an overcurrent event occurs on one of the phases indicated by Bits[5:3] (OIPHASE[x]) in the PHSTATUS register (see Table 40).
  MASK1_OV = 1 << 18,              // Bit 18 When this bit is set to 1, it enables an interrupt when an overvoltage event occurs on one of the phases indicated by Bits[11:9] (OVPHASE[x]) in the PHSTATUS register (see Table 40).
  MASK1_SEQERR = 1 << 19,          // Bit 19 When this bit is set to 1, it enables an interrupt when a negative-to-positive zero crossing on Phase A voltage is not followed by a negative-to-positive zero crossing on Phase B voltage, but by a negative-to-positive zero crossing on Phase C voltage.
  MASK1_MISMTCH = 1 << 20,         // Bit 20 When this bit is set to 1, it enables an interrupt when ISUMLVLINWVISUM >− is greater than the value indicated in ISUMLVL register.
  MASK1_RESERVED1 = 1 << 21,       // Bit 21 Reserved. This bit does not manage any functionality.
  MASK1_RESERVED2 = 1 << 22,       // Bit 22 Reserved. This bit does not manage any functionality.
  MASK1_PKI = 1 << 23,             // Bit 23 When this bit is set to 1, it enables an interrupt when the period used to detect the peak value in the current channel has ended.
  MASK1_PKV = 1 << 24,             // Bit 24 When this bit is set to 1, it enables an interrupt when the period used to detect the peak value in the voltage channel has ended.
  MASK1_CRC = 1 << 25              // Bit 25 When this bit is set to 1, it enables an interrupt when the latest checksum value is different from the checksum value computed when Run register was set to 1.
};

// Page 99 Table 42. COMPMODE Register (Address 0xE60E)
enum CompmodeRegister {
  COMPMODE_TERMSEL1_0 = 1 << 0,            // Bit 0  Setting all TERMSEL1[2:0] to 1 signifies the sum of all three phases is included in the CF1 output. Phase A is included in the CF1 outputs calculations.
  COMPMODE_TERMSEL1_1 = 1 << 1,            // Bit 1  Phase B is included in the CF1 outputs calculations.
  COMPMODE_TERMSEL1_2 = 1 << 2,            // Bit 2  Phase C is included in the CF1 outputs calculations.
  COMPMODE_TERMSEL1 = (1 << 0) | (1 << 1) | (1 << 2),
  COMPMODE_TERMSEL2_0 = 1 << 3,            // Bit 3  Setting all TERMSEL2[2:0] to 1 signifies the sum of all three phases is included in the CF2 output. Phase A is included in the CF2 outputs calculations.
  COMPMODE_TERMSEL2_1 = 1 << 4,            // Bit 4  Phase B is included in the CF2 outputs calculations.
  COMPMODE_TERMSEL2_2 = 1 << 5,            // Bit 5  Phase C is included in the CF2 outputs calculations.
  COMPMODE_TERMSEL2 = (1 << 3) | (1 << 4) | (1 << 5),
  COMPMODE_TERMSEL3_0 = 1 << 6,            // Bit 6  Setting all TERMSEL3[2:0] to 1 signifies the sum of all three phases is included in the CF3 output. Phase A is included in the CF3 outputs calculations.
  COMPMODE_TERMSEL3_1 = 1 << 7,            // Bit 7  Phase B is included in the CF3 outputs calculations.
  COMPMODE_TERMSEL3_2 = 1 << 8,            // Bit 8  Phase C is included in the CF3 outputs calculations.
  COMPMODE_TERMSEL3 = (1 << 6) | (1 << 7) | (1 << 8),
  COMPMODE_ANGLESEL_00 = 0,                // Bits 9-10 00: the angles between phase voltages and phase currents are measured.
  COMPMODE_ANGLESEL_01 = 1 << 9,           // Bits 9-10 01: the angles between phase voltages are measured.
  COMPMODE_ANGLESEL_10 = 1 << 10,          // Bits 9-10 10: the angles between phase currents are measured.
  COMPMODE_ANGLESEL_11 = 1 << 9 | 1 << 10, // Bits 9-10 11: no angles are measured.
  COMPMODE_VNOMAEN = 1 << 11,              // Bit 11 When this bit is 0, the apparent power on Phase A is computed regularly. When this bit is 1, the apparent power on Phase A is computed using the VNOM register instead of regular measured rms phase voltage. The applied Phase A voltage input is ignored, and all Phase A rms voltage instances are replaced by the value in the VNOM register.
  COMPMODE_VNOMBEN = 1 << 12,              // Bit 12 When this bit is 0, the apparent power on Phase B is computed regularly. When this bit is 1, the apparent power on Phase B is computed using the VNOM register instead of regular measured rms phase voltage. The applied Phase B voltage input is ignored, and all Phase B rms voltage instances are replaced by the value in the VNOM register.
  COMPMODE_VNOMCEN = 1 << 13,              // Bit 13 When this bit is 0, the apparent power on Phase C is computed regularly. When this bit is 1, the apparent power on Phase C is computed using the VNOM register instead of regular measured rms phase voltage. The applied Phase C voltage input is ignored, and all Phase C rms voltage instances are replaced by the value in the VNOM register.
  COMPMODE_SELFREQ = 1 << 14,              // Bit 14 When the ADE7880 is connected to networks with fundamental frequencies between 45 Hz and 55 Hz, clear this bit to 0 (default value). When the ADE7880 is connected to networks with fundamental frequencies between 55 Hz and 66 Hz, set this bit to 1.
  COMPMODE_PFMODE = 1 << 15,               // Bit 15 When this bit is 0, power factor calculation uses instantaneous values of various phase powers used in its expression. When this bit is 1, power factor calculation uses phase energies values calculated using line cycle accumulation mode. Bits LWATT and LVA in LCYCMODE register must be enabled for the power factors to be computed correctly. The update rate of the power factor measurement in t
};

// Page 104-105 Table 51. LCYCMODE Register (Address 0xE702)
enum LcycmodeRegister {
  LCYCMODE_LWATT = 1 << 0,         // Bit 0  0: the watt-hour accumulation registers (AWATTHR, BWATTHR, CWATTHR, AFWATTHR, BFWATTHR, and CFWATTHR) are placed in regular accumulation mode.
                                   //        1: the watt-hour accumulation registers (AWATTHR, BWATTHR, CWATTHR, AFWATTHR, BFWATTHR, and CFWATTHR) are placed into line cycle accumulation mode.
  LCYCMODE_LVAR = 1 << 1,          // Bit 1  0: the VAR-hour accumulation registers (AVARHR, BVARHR, CVARHR, AFVARHR, BFVARHR, and CFVARHR) are placed in regular accumulation mode.
                                   //        1: the VAR-hour accumulation registers (AVARHR, BVARHR, CVARHR, AFVARHR, BFVARHR, and CFVARHR) are placed into line cycle accumulation mode.
  LCYCMODE_LVA = 1 << 2,           // Bit 2  0: the VA-hour accumulation registers (AVAHR, BVAHR, and CVAHR) are placed in regular accumulation mode.
                                   //        1: the VA-hour accumulation registers (AVAHR, BVAHR, and CVAHR) are placed into line-cycle accumulation mode.
  LCYCMODE_ZXSEL_0 = 1 << 3,       // Bit 3  0: Phase A is not selected for zero-crossings counts in the line cycle accumulation mode.
                                   //        1: Phase A is selected for zero-crossings counts in the line cycle accumulation mode. If more than one phase is selected for zero-crossing detection, the accumulation time is shortened accordingly.
  LCYCMODE_ZXSEL_1 = 1 << 4,       // Bit 4  0: Phase B is not selected for zero-crossings counts in the line cycle accumulation mode.
                                   //        1: Phase B is selected for zero-crossings counts in the line cycle accumulation mode.
  LCYCMODE_ZXSEL_2 = 1 << 5,       // Bit 5  0: Phase C is not selected for zero-crossings counts in the line cycle accumulation mode.
                                   //        1: Phase C is selected for zero-crossings counts in the line cycle accumulation mode.
  LCYCMODE_RSTREAD = 1 << 6,       // Bit 6  0: read-with-reset of all energy registers is disabled. Clear this bit to 0 when Bits[2:0] (LWATT, LVAR, and LVA) are set to 1.
                                   //        1: enables read-with-reset of all xWATTHR, xVARHR, xVAHR, xFWATTHR, and xFVARHR registers. This means a read of those registers resets them to 0.
  LCYCMODE_PFMODE = 1 << 7         // Bit 7  0: power factor calculation uses instantaneous values of various phase powers used in its expression.
                                   //        1: power factor calculation uses phase energies values calculated using line cycle accumulation mode. Bits LWATT and LVA in LCYCMODE register must be enabled for the power factors to be computed correctly. The update rate of the power factor measurement in t
};

// Page 106 Table 56. CONFIG2 Register (Address 0xEC01)
enum Config2Register {
  CONFIG2_EXTREFEN = 1 << 0,       // Bit 0  When this bit is 0, it signifies that the internal voltage reference is used in the ADCs.
                                   //        When this bit is 1, an external reference is connected to the Pin 17 REF IN/OUT.
  CONFIG2_I2C_LOCK = 1 << 1,       // Bit 1  When this bit is 0, the SS/HSA pin can be toggled three times to activate the SPI port. If I2 C is the active serial port, this bit must be set to 1 to lock it in. From this moment on, toggling of the SS/HSA pin and an eventual switch into using the SPI port is no longer possible. If SPI is the active serial port, any write to CONFIG2 register locks the port. From this moment on, a switch into using I2 C port is no longer possible. Once locked, the serial port choice is maintained when the ADE7880 changes PSMx power modes.
};

} // namespace ade7880
} // namespace esphome
